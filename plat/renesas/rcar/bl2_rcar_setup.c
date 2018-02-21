/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <bl_common.h>
#include <bl1.h>
#include <console.h>
#include <platform.h>
#include <platform_def.h>
#include <string.h>
#include <plat_arm.h>
#include "rcar_def.h"
#include "rcar_private.h"
#include "io_common.h"
#include "pfc_init.h"
#include "rpc_driver.h"
#include "dma_driver.h"
#include "bl2_secure_setting.h"
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"
#include <debug.h>
#include <mmio.h>
#include "boot_init_dram.h"
#include "qos_init.h"
#include "rcar_version.h"
#include "bl2_swdt.h"
#include "avs_driver.h"
#include "scif.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_def.h"
#include "rom_api.h"
#include "board.h"


/* CPG write protect registers */
/*#define	CPG_CPGWPR		(CPG_BASE + 0x900U)*/
/*#define	CPG_CPGWPCR		(CPG_BASE + 0x904U)*/
#define	CPGWPR_PASSWORD		(0x5A5AFFFFU)
#define	CPGWPCR_PASSWORD	(0xA5A50000U)

/* CA5x Debug Resource control registers */
#define	CPG_CA57DBGRCR		(CPG_BASE + 0x2180U)
#define	CPG_CA53DBGRCR		(CPG_BASE + 0x1180U)
#define	DBGCPUPREN		((uint32_t)1U << 19U)
#define	CPG_PLL0CR		(CPG_BASE + 0x00D8U)
#define	CPG_PLL2CR		(CPG_BASE + 0x002CU)
#define	CPG_PLL4CR		(CPG_BASE + 0x01F4U)

/* RST Registers */
#define	RST_BASE		(0xE6160000U)
#define	RST_WDTRSTCR		(RST_BASE + 0x0054U)
#define	WDTRSTCR_PASSWORD	(0xA55A0000U)
#define	WDTRSTCR_RWDT_RSTMSK	((uint32_t)1U << 0U)

/* MFIS Registers */
#define	MFISWPCNTR_PASSWORD	(0xACCE0000U)
#define	MFISWPCNTR		(0xE6260900U)

/* IPMMU registers */
#define IPMMU_MM_BASE		(0xE67B0000U)	/* IPMMU-MM */
#define IPMMUMM_IMSCTLR		(IPMMU_MM_BASE + 0x0500U)
#define IPMMUMM_IMAUXCTLR	(IPMMU_MM_BASE + 0x0504U)
#define IPMMUMM_IMSCTLR_ENABLE		(0xC0000000U)
#define IPMMUMM_IMAUXCTLR_NMERGE40_BIT	(0x01000000U)

#define IMSCTLR_DISCACHE	(0xE0000000U)

#define IPMMU_VI0_BASE		(0xFEBD0000U)	/* IPMMU-VI0 */
#define IPMMUVI0_IMSCTLR	(IPMMU_VI0_BASE + 0x0500U)

#define IPMMU_VI1_BASE		(0xFEBE0000U)	/* IPMMU-VI1 */
#define IPMMUVI1_IMSCTLR	(IPMMU_VI1_BASE + 0x0500U)

#define IPMMU_PV0_BASE		(0xFD800000U)	/* IPMMU-PV0 */
#define IPMMUPV0_IMSCTLR	(IPMMU_PV0_BASE + 0x0500U)

#define IPMMU_PV2_BASE		(0xFD960000U)	/* IPMMU-PV2 */
#define IPMMUPV2_IMSCTLR	(IPMMU_PV2_BASE + 0x0500U)

#define IPMMU_PV3_BASE		(0xFD970000U)	/* IPMMU-PV3 */
#define IPMMUPV3_IMSCTLR	(IPMMU_PV3_BASE + 0x0500U)

#define IPMMU_HC_BASE		(0xE6570000U)	/* IPMMU-HC */
#define IPMMUHC_IMSCTLR		(IPMMU_HC_BASE + 0x0500U)

#define IPMMU_RT_BASE		(0xFFC80000U)	/* IPMMU-RT */
#define IPMMURT_IMSCTLR		(IPMMU_RT_BASE + 0x0500U)

#define IPMMU_MP_BASE		(0xEC670000U)	/* IPMMU-MP */
#define IPMMUMP_IMSCTLR		(IPMMU_MP_BASE + 0x0500U)

#define IPMMU_DS0_BASE		(0xE6740000U)	/* IPMMU-DS0 */
#define IPMMUDS0_IMSCTLR	(IPMMU_DS0_BASE + 0x0500U)

#define IPMMU_DS1_BASE		(0xE7740000U)	/* IPMMU-DS1 */
#define IPMMUDS1_IMSCTLR	(IPMMU_DS1_BASE + 0x0500U)

/* ARMREG registers */
#define	P_ARMREG_SEC_CTRL	(0xE62711F0U)
#define	P_ARMREG_SEC_CTRL_PROT	(0x00000001U)

/* MIDR */
#define MIDR_CA57		(0x0D07U << MIDR_PN_SHIFT)
#define MIDR_CA53		(0x0D03U << MIDR_PN_SHIFT)

/* R-Car Gen3 product check */
#if RCAR_LSI == RCAR_H3
#define TARGET_PRODUCT		RCAR_PRODUCT_H3
#define TARGET_NAME		"R-Car H3"
#elif RCAR_LSI == RCAR_M3
#define TARGET_PRODUCT		RCAR_PRODUCT_M3
#define TARGET_NAME		"R-Car M3"
#elif RCAR_LSI == RCAR_M3N
#define TARGET_PRODUCT		RCAR_PRODUCT_M3N
#define TARGET_NAME		"R-Car M3N"
#elif RCAR_LSI == RCAR_E3
#define TARGET_PRODUCT		RCAR_PRODUCT_E3
#define TARGET_NAME		"R-Car E3"
#endif

/* for SuspendToRAM */
#define	GPIO_BASE		(0xE6050000U)
#define	GPIO_INDT1		(GPIO_BASE + 0x100CU)
#define GPIO_INDT6		(GPIO_BASE + 0x540CU)

#if (RCAR_LSI == RCAR_E3)
#define GPIO_INDT		(GPIO_INDT6)
#define GPIO_BKUP_TRG		((uint32_t)1U<<13)
#else  /* (RCAR_LSI == RCAR_E3) */
#define GPIO_INDT		(GPIO_INDT1)
#define GPIO_BKUP_TRG		((uint32_t)1U<<8)
#endif /* (RCAR_LSI == RCAR_E3) */

static uint32_t isDdrBackupMode(void);

/*******************************************************************************
 * Declarations of linker defined symbols which will help us find the layout
 * of trusted SRAM
 ******************************************************************************/
extern unsigned long __RO_START__;
extern unsigned long __RO_END__;

#if USE_COHERENT_MEM
extern unsigned long __COHERENT_RAM_START__;
extern unsigned long __COHERENT_RAM_END__;
#endif

/*
 * The next 2 constants identify the extents of the code & RO data region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __RO_START__ and __RO_END__ linker symbols refer to page-aligned addresses.
 */
#define BL2_RO_BASE (unsigned long)(&__RO_START__)
#define BL2_RO_LIMIT (unsigned long)(&__RO_END__)

#if USE_COHERENT_MEM
/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols refer to
 * page-aligned addresses.
 */
#define BL2_COHERENT_RAM_BASE (unsigned long)(&__COHERENT_RAM_START__)
#define BL2_COHERENT_RAM_LIMIT (unsigned long)(&__COHERENT_RAM_END__)
#endif

/* Data structure which holds the extents of the trusted SRAM for BL2 */
static meminfo_t bl2_tzram_layout
__attribute__ ((aligned(PLATFORM_CACHE_LINE_SIZE)));

/* Assert that BL3-1 parameters fit in shared memory */
CASSERT((PARAMS_BASE + sizeof(bl2_to_bl31_params_mem_t)) <
	(RCAR_SHARED_MEM_BASE + RCAR_SHARED_MEM_SIZE),
	assert_bl31_params_do_not_fit_in_shared_memory);

/*******************************************************************************
 * Reference to structures which holds the arguments which need to be passed
 * to BL31
 ******************************************************************************/
static bl31_params_t *bl2_to_bl31_params;
static entry_point_info_t *bl31_ep_info;

meminfo_t *bl2_plat_sec_mem_layout(void)
{
	return &bl2_tzram_layout;
}

/*******************************************************************************
 * This function assigns a pointer to the memory that the platform has kept
 * aside to pass platform specific and trusted firmware related information
 * to BL31. This memory is allocated by allocating memory to
 * bl2_to_bl31_params_mem_t structure which is a superset of all the
 * structure whose information is passed to BL31
 * NOTE: This function should be called only once and should be done
 * before generating params to BL31
 ******************************************************************************/
bl31_params_t *bl2_plat_get_bl31_params(void)
{
	bl2_to_bl31_params_mem_t *bl31_params_mem;

	/*
	 * Allocate the memory for all the arguments that needs to
	 * be passed to BL31
	 */
	bl31_params_mem = (bl2_to_bl31_params_mem_t *)PARAMS_BASE;
	memset((void *)PARAMS_BASE, 0, sizeof(bl2_to_bl31_params_mem_t));

	/* Assign memory for TF related information */
	bl2_to_bl31_params = &bl31_params_mem->bl31_params;
	SET_PARAM_HEAD(bl2_to_bl31_params, PARAM_BL31, VERSION_1, 0);

	/* Fill BL31 related information */
	bl31_ep_info = &bl31_params_mem->bl31_ep_info;
	bl2_to_bl31_params->bl31_image_info = &bl31_params_mem->bl31_image_info;
	SET_PARAM_HEAD(bl2_to_bl31_params->bl31_image_info, PARAM_IMAGE_BINARY,
						VERSION_1, 0);

	/* Fill BL32 related information if it exists */
#ifdef BL32_BASE
	if (BL32_BASE) {
		bl2_to_bl31_params->bl32_ep_info =
					&bl31_params_mem->bl32_ep_info;
		SET_PARAM_HEAD(bl2_to_bl31_params->bl32_ep_info,
					PARAM_EP, VERSION_1, 0);
		bl2_to_bl31_params->bl32_image_info =
					&bl31_params_mem->bl32_image_info;
		SET_PARAM_HEAD(bl2_to_bl31_params->bl32_image_info,
					PARAM_IMAGE_BINARY,
					VERSION_1, 0);
	}
#endif

	/* Fill BL33 related information */
	bl2_to_bl31_params->bl33_ep_info = &bl31_params_mem->bl33_ep_info;
	SET_PARAM_HEAD(bl2_to_bl31_params->bl33_ep_info,
					PARAM_EP, VERSION_1, 0);
	bl2_to_bl31_params->bl33_image_info = &bl31_params_mem->bl33_image_info;
	SET_PARAM_HEAD(bl2_to_bl31_params->bl33_image_info, PARAM_IMAGE_BINARY,
					VERSION_1, 0);

	return bl2_to_bl31_params;
}


/*******************************************************************************
 * This function returns a pointer to the shared memory that the platform
 * has kept to point to entry point information of BL31 to BL2
 ******************************************************************************/
struct entry_point_info *bl2_plat_get_bl31_ep_info(void)
{
	if (isDdrBackupMode() != 0U) {

		NOTICE("BL2: Skip loading images. (SuspendToRAM)\n");

		bl31_ep_info->args.arg0 = (unsigned long)bl2_plat_get_bl31_params();
		bl31_ep_info->args.arg1 = 0x00000001UL;	/* warm boot */
		bl31_ep_info->pc = (uintptr_t)BL31_BASE;
		SET_SECURITY_STATE(bl31_ep_info->h.attr, SECURE);
		bl31_ep_info->spsr = (uint32_t)SPSR_64(MODE_EL3, MODE_SP_ELX,
						DISABLE_ALL_EXCEPTIONS);

		/* Flush the params to be passed to memory */
		bl2_plat_flush_bl31_params();

		/* Flush of all buffered data and desables the SCIF. */
		(void)console_flush();

		/*
		 * Run BL3-1 via an SMC to BL1.
		 * Need to jumps entrypoint of Suspend to RAM at SMC handler.
		 */
		smc((unsigned long)BL1_SMC_RUN_IMAGE, (unsigned long)bl31_ep_info,
			0UL, 0UL, 0UL, 0UL, 0UL, 0UL);

		/* Jump to BL31 (Not come back here) */
	}

	bl31_ep_info->args.arg1 = 0x00000000UL;	/* cold boot */
	return bl31_ep_info;
}

#if (RCAR_LOSSY_ENABLE == 1)
/* Settings for Lossy Decompression */
#define LOSSY_PARAMS_BASE 		(0x47FD7000U)

#define	AXI_DCMPAREACRA0		(0xE6784100U)
#define	AXI_DCMPAREACRB0		(0xE6784104U)

#define LOSSY_ENABLE			(0x80000000U)
#define LOSSY_DISABLE			(0x00000000U)

#define LOSSY_FMT_YUVPLANAR		(0x00000000U)
#define LOSSY_FMT_YUV422INTLV	(0x20000000U)
#define LOSSY_FMT_ARGB8888		(0x40000000U)

/* Settings of Entry 0 */
#define	LOSSY_ST_ADDR0			(0x54000000U)
#define	LOSSY_END_ADDR0			(0x57000000U)
#define	LOSSY_FMT0				LOSSY_FMT_YUVPLANAR
#define	LOSSY_ENA_DIS0			LOSSY_ENABLE

/* Settings of Entry 1 */
#define	LOSSY_ST_ADDR1			0x0U /* Undefined */
#define	LOSSY_END_ADDR1			0x0U /* Undefined */
#define	LOSSY_FMT1				LOSSY_FMT_ARGB8888
#define	LOSSY_ENA_DIS1			LOSSY_DISABLE

/* Settings of Entry 2 */
#define	LOSSY_ST_ADDR2			0x0U /* Undefined */
#define	LOSSY_END_ADDR2			0x0U /* Undefined */
#define	LOSSY_FMT2				LOSSY_FMT_YUV422INTLV
#define	LOSSY_ENA_DIS2			LOSSY_DISABLE

static void bl2_lossy_setting(uint32_t no, uint64_t start_addr,
	uint64_t end_addr, uint32_t format, uint32_t enable);

typedef struct bl2_lossy_info {
	uint32_t magic;
	uint32_t a0;
	uint32_t b0;
} bl2_lossy_info_t;

static void bl2_lossy_setting(uint32_t no, uint64_t start_addr,
	uint64_t end_addr, uint32_t format, uint32_t enable)
{
	uint32_t reg;
	bl2_lossy_info_t info;

	/* Setting of the start address and format */
	reg = (uint32_t)(format | (start_addr >> 20U));
	mmio_write_32(AXI_DCMPAREACRA0 + (0x8U * (uintptr_t)no), reg);

	/* Setting of the end address */
	mmio_write_32(AXI_DCMPAREACRB0 + (0x8U * (uintptr_t)no),
		(uint32_t)(end_addr >> 20U));

	/* Enable or Disable of Lossy setting */
	mmio_write_32(AXI_DCMPAREACRA0 + (0x8U * (uintptr_t)no), (reg | enable));

	info.magic = 0x12345678U;
	info.a0 = mmio_read_32(AXI_DCMPAREACRA0 + (0x8U * (uintptr_t)no));
	info.b0 = mmio_read_32(AXI_DCMPAREACRB0 + (0x8U * (uintptr_t)no));

	mmio_write_32(LOSSY_PARAMS_BASE +
		(sizeof(bl2_lossy_info_t) * (uintptr_t)no), info.magic);
	mmio_write_32(LOSSY_PARAMS_BASE +
		(sizeof(bl2_lossy_info_t) * (uintptr_t)no) + 0x4U, info.a0);
	mmio_write_32(LOSSY_PARAMS_BASE +
		(sizeof(bl2_lossy_info_t) * (uintptr_t)no) + 0x8U, info.b0);

	NOTICE("     Entry %d: DCMPAREACRAx:0x%x DCMPAREACRBx:0x%x\n", no,
		mmio_read_32(AXI_DCMPAREACRA0 + (0x8U * (uintptr_t)no)),
		mmio_read_32(AXI_DCMPAREACRB0 + (0x8U * (uintptr_t)no)));
}
#endif /* #if (RCAR_LOSSY_ENABLE == 1) */

/*******************************************************************************
 * BL1 has passed the extents of the trusted SRAM that should be visible to BL2
 * in x0. This memory layout is sitting at the base of the free trusted SRAM.
 * Copy it to a safe loaction before its reclaimed by later BL2 functionality.
 ******************************************************************************/
void bl2_early_platform_setup(meminfo_t *mem_layout)
{
	uint32_t reg;
	uint32_t lcs;
	uint32_t modemr;
	uint32_t modemr_boot_dev;
	uint32_t modemr_sscg;
	int32_t ret;
	uint32_t board_type;
	uint32_t board_rev;
	uint32_t prr_val;
	char msg[128];
	const char *str;
	const char *cpu_ca57        = "CA57";
	const char *cpu_ca53        = "CA53";
	const char *product_h3      = "H3";
	const char *product_m3      = "M3";
	const char *product_m3n     = "M3N";
	const char *product_e3      = "E3";
	const char *lcs_cm          = "CM";
	const char *lcs_dm          = "DM";
	const char *lcs_sd          = "SD";
	const char *lcs_secure      = "SE";
	const char *lcs_fa          = "FA";
#if RCAR_LSI == RCAR_E3
	const char *boot_hyper160   = "HyperFlash(150MHz)";
#else /* RCAR_LSI == RCAR_E3 */
	const char *boot_hyper160   = "HyperFlash(160MHz)";
#endif /* RCAR_LSI == RCAR_E3 */
	const char *boot_hyper80    = "HyperFlash(80MHz)";
	const char *boot_qspi40     = "QSPI Flash(40MHz)";
	const char *boot_qspi80     = "QSPI Flash(80MHz)";
	const char *boot_emmc25x1   = "eMMC(25MHz x1)";
	const char *boot_emmc50x8   = "eMMC(50MHz x8)";
	const char *sscg_off        = "PLL1 nonSSCG Clock select";
	const char *sscg_on         = "PLL1 SSCG Clock select";
	const char *unknown         = "unknown";

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr_boot_dev = modemr & MODEMR_BOOT_DEV_MASK;
	modemr &= MODEMR_BOOT_CPU_MASK;

	/* Initialize CPG configuration */
	bl2_cpg_init();

	if((modemr == MODEMR_BOOT_CPU_CA57) ||
	   (modemr == MODEMR_BOOT_CPU_CA53)) {
		/* initialize Pin Function */
		pfc_init();

		/* Initialize the console to provide early debug support */
		(void)console_init(0U, 0U, 0U);
	}

	/* Initialize the GIC driver, cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();

	/* System WDT initialize	*/
	bl2_swdt_init();

	/* Enable FIQ interrupt		*/
	write_daifclr(DAIF_FIQ_BIT);

	/* boot message */
	reg = (uint32_t)read_midr();
	switch (reg & ((uint32_t)MIDR_PN_MASK << MIDR_PN_SHIFT)) {
	case MIDR_CA57:
		str = cpu_ca57;
		break;
	case MIDR_CA53:
		str = cpu_ca53;
		break;
	default:
		str = unknown;
		break;
	}
	(void)sprintf(msg, "BL2: R-Car Gen3 Initial Program Loader(%s) Rev.%s\n"
						, str, version_of_renesas);
	NOTICE("%s", msg);

	/* R-Car Gen3 product display & check */
	reg = mmio_read_32(RCAR_PRR);
	prr_val = reg;
	switch (reg & RCAR_PRODUCT_MASK) {
	case RCAR_PRODUCT_H3:
		str = product_h3;
		break;
	case RCAR_PRODUCT_M3:
		str = product_m3;
		/* M3 Ver1.1 */
		if(RCAR_PRODUCT_M3_CUT11 ==
			(reg & (RCAR_PRODUCT_MASK | RCAR_CUT_MASK))) {
			prr_val = RCAR_CUT_ES11;
		}
		break;
	case RCAR_PRODUCT_M3N:
		str = product_m3n;
		break;
	case RCAR_PRODUCT_E3:
		str = product_e3;
		break;
	default:
		str = unknown;
		break;
	}
	(void)sprintf(msg, "BL2: PRR is R-Car %s Ver%d.%d\n", str,
		((prr_val & RCAR_MAJOR_MASK) >> RCAR_MAJOR_SHIFT)
		 + RCAR_MAJOR_OFFSET, (prr_val & RCAR_MINOR_MASK));
	NOTICE("%s", msg);

	/* R-Car Gen3 PLL1 clock select display (E3 only) */
	reg = mmio_read_32(RCAR_PRR);
	if ((reg & RCAR_PRODUCT_MASK) == RCAR_PRODUCT_E3) {
		modemr_sscg = mmio_read_32(RCAR_MODEMR);
		if ((modemr_sscg & RCAR_SSCG_MASK) == RCAR_SSCG_ENABLE) {
			str = sscg_on;
		} else {
			str = sscg_off;
		}
		(void)sprintf(msg, "BL2: %s\n", str);
		NOTICE("%s", msg);
	}

	/* Board ID detection */
	(void)get_board_type(&board_type, &board_rev);
	
	switch (board_type) {
	case BOARD_SALVATOR_X:
	case BOARD_KRIEK:
	case BOARD_STARTER_KIT:
	case BOARD_SALVATOR_XS:
	case BOARD_EBISU:
	case BOARD_STARTER_KIT_PRE:
		/* Do nothing. */
		break;
	default:
		board_type = BOARD_UNKNOWN;
		break;
	}
	
	if ((board_type == BOARD_UNKNOWN) || (board_rev == BOARD_REV_UNKNOWN)) {
		(void)sprintf(msg, "BL2: Board is %s Rev---\n",
			GET_BOARD_NAME(board_type));
	} else {
		(void)sprintf(msg, "BL2: Board is %s Rev%d.%d\n",
			GET_BOARD_NAME(board_type), GET_BOARD_MAJOR(board_rev),
			GET_BOARD_MINOR(board_rev));
	}
	NOTICE("%s", msg);

#if RCAR_LSI != RCAR_AUTO
	if((reg & RCAR_PRODUCT_MASK) != TARGET_PRODUCT) {
		ERROR("BL2: This IPL has been built for the %s.\n",
								TARGET_NAME);
		ERROR("BL2: Please write the correct IPL to flash memory.\n");
		panic();
	}
#endif /* RCAR_LSI != RCAR_AUTO */

	/* Initialize AVS Settings */
	bl2_avs_init();

	/* Proceed with separated AVS processing */
	bl2_avs_setting();

	switch (modemr_boot_dev) {
	case MODEMR_BOOT_DEV_HYPERFLASH160:
		str = boot_hyper160;
		break;
	case MODEMR_BOOT_DEV_HYPERFLASH80:
		str = boot_hyper80;
		break;
	case MODEMR_BOOT_DEV_QSPI_FLASH40:
		str = boot_qspi40;
		break;
	case MODEMR_BOOT_DEV_QSPI_FLASH80:
		str = boot_qspi80;
		break;
	case MODEMR_BOOT_DEV_EMMC_25X1:
		str = boot_emmc25x1;
		break;
	case MODEMR_BOOT_DEV_EMMC_50X8:
		str = boot_emmc50x8;
		break;
	default:
		str = unknown;
		break;
	}
	(void)sprintf(msg, "BL2: Boot device is %s\n", str);
	NOTICE("%s", msg);

	/* Proceed with separated AVS processing */
	bl2_avs_setting();

	reg = ROM_GetLcs(&lcs);
	if (reg == 0U) {
		switch (lcs) {
		case LCS_CM:
			str = lcs_cm;
			break;
		case LCS_DM:
			str = lcs_dm;
			break;
		case LCS_SD:
			str = lcs_sd;
			break;
		case LCS_SE:
			str = lcs_secure;
			break;
		case LCS_FA:
			str = lcs_fa;
			break;
		default:
			str = unknown;
			break;
		}
	} else {
		str = unknown;
	}
	(void)sprintf(msg, "BL2: LCM state is %s\n", str);
	NOTICE("%s", msg);

	/* End of AVS Settings */
	bl2_avs_end();

	/* Save BKUP_TRG for SuspendToRAM */
	(void)isDdrBackupMode();

	/* Setup the BL2 memory layout */
	bl2_tzram_layout = *mem_layout;

	if((modemr == MODEMR_BOOT_CPU_CA57) ||
	   (modemr == MODEMR_BOOT_CPU_CA53)) {
		/* Initialize SDRAM */
		ret = InitDram();
		if (ret != 0) {
			NOTICE("BL2: Failed to DRAM initialize (%d).\n", ret);
			/* Infinite loop */
			panic();
		}

		/* initialize QoS configration */
		qos_init();
	}

	if ((modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_25X1) ||
	    (modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_50X8)) {
		/* Initialize eMMC */
		if (emmc_init() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC driver initialize.\n");
			/* Infinite loop */
			panic();
		}

		/* Card power on */
		(void)emmc_memcard_power(EMMC_POWER_ON);

		/* Card mount */
		if (emmc_mount() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC mount operation.\n");
			/* Infinite loop */
			panic();
		}
	} else {
		/* Initialize RPC */
		initRPC();

		/* Initialize DMA */
		initDMA();
	}

	/* unmask the detection of RWDT overflow */
	reg = mmio_read_32(RST_WDTRSTCR);
	reg &= ~WDTRSTCR_RWDT_RSTMSK;
	reg |= WDTRSTCR_PASSWORD;
	mmio_write_32(RST_WDTRSTCR, reg);

	/* Release CPG write protect */
	mmio_write_32(CPG_CPGWPR, CPGWPR_PASSWORD);
	mmio_write_32(CPG_CPGWPCR, CPGWPCR_PASSWORD);

	/* CA5x debug resource control */
	reg = mmio_read_32(RCAR_PRR);
	if ((reg & RCAR_CPU_MASK_CA57) == RCAR_CPU_HAVE_CA57 ) {
		mmio_write_32(CPG_CA57DBGRCR,
				DBGCPUPREN | mmio_read_32(CPG_CA57DBGRCR));
	}
	if ((reg & RCAR_CPU_MASK_CA53) == RCAR_CPU_HAVE_CA53 ) {
		mmio_write_32(CPG_CA53DBGRCR,
				DBGCPUPREN | mmio_read_32(CPG_CA53DBGRCR));
	}

	/* STA restriction check for R-Car H3 WS1.0 */
	reg = mmio_read_32(RCAR_PRR) & (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	if (reg  == RCAR_PRODUCT_H3_CUT10) {
		/* PLL0, PLL2, PLL4 setting */
		reg = mmio_read_32(CPG_PLL2CR);
		reg &= ~((uint32_t)1U << 5U);	/* bit5 = 0 */
		mmio_write_32(CPG_PLL2CR, reg);

		reg = mmio_read_32(CPG_PLL4CR);
		reg &= ~((uint32_t)1U << 5U);	/* bit5 = 0 */
		mmio_write_32(CPG_PLL4CR, reg);

		reg = mmio_read_32(CPG_PLL0CR);
		reg &= ~((uint32_t)1U << 12U);	/* bit12 = 0 */
		mmio_write_32(CPG_PLL0CR, reg);
	}

#if (RCAR_LOSSY_ENABLE == 1)
	NOTICE("BL2: Lossy Decomp areas\n");
	/* Lossy setting : entry 0 */
	bl2_lossy_setting(0U, LOSSY_ST_ADDR0, LOSSY_END_ADDR0,
		LOSSY_FMT0, LOSSY_ENA_DIS0);

	/* Lossy setting : entry 1 */
	bl2_lossy_setting(1U, LOSSY_ST_ADDR1, LOSSY_END_ADDR1,
		LOSSY_FMT1, LOSSY_ENA_DIS1);

	/* Lossy setting : entry 2 */
	bl2_lossy_setting(2U, LOSSY_ST_ADDR2, LOSSY_END_ADDR2,
		LOSSY_FMT2, LOSSY_ENA_DIS2);
#endif /* #if (RCAR_LOSSY_ENABLE == 1) */

	/* Initialise the IO layer and register platform IO devices */
	if((modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_25X1) ||
	   (modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_50X8)) {
		rcar_io_emmc_setup();
	} else {
		rcar_io_setup();
	}
}

/*******************************************************************************
 * Get DDR Backup Mode from GPIO
 *  BKUP_TRG(IO port A8, GPIO GP-0-8): LOW=Cold boot, HIGH=Warm boot
 * return: uint8_t
 *  0: DDR is not backup mode.
 *  1: DDR is backup mode.
 ******************************************************************************/
static uint32_t isDdrBackupMode(void)
{
#if RCAR_SYSTEM_SUSPEND
	static uint32_t backupTriggerOnce = 1U;
	static uint32_t backupTrigger = 0U;
	if (backupTriggerOnce == 1U) {
		backupTriggerOnce = 0U;
		/* Read and return BKUP_TRG(IO port B8, GPIO GP-1-8) */
		if ((mmio_read_32((uintptr_t)GPIO_INDT) & GPIO_BKUP_TRG) != 0U) {
			backupTrigger = 1U;
		}
	}
	return backupTrigger;
#else	/* RCAR_SYSTEM_SUSPEND */
	return 0U;	/* Cold boot only */
#endif	/* RCAR_SYSTEM_SUSPEND */
}

/*******************************************************************************
 * Perform platform specific setup. For now just initialize the memory location
 * to use for passing arguments to BL31.
 ******************************************************************************/
void bl2_platform_setup(void)
{
	/*
	 * Do initial security configuration to allow DRAM/device access. On
	 * Base RCAR only DRAM security is programmable (via TrustZone), but
	 * other platforms might have more programmable security devices
	 * present.
	 */
}

/* Flush the TF params and the TF plat params */
void bl2_plat_flush_bl31_params(void)
{
	uint32_t val;
	uint32_t modemr;
	uint32_t modemr_boot_dev;
	uint32_t lcs;

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr_boot_dev = modemr & MODEMR_BOOT_DEV_MASK;
	if((modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_25X1) ||
	   (modemr_boot_dev == MODEMR_BOOT_DEV_EMMC_50X8)) {
		(void)emmc_terminate();
	}

	if((modemr & MODEMR_BOOT_CPU_MASK) != MODEMR_BOOT_CPU_CR7) {
		/* Initialize secure configuration */
		bl2_secure_setting();
	}

	val = mmio_read_32(RCAR_PRR);
	if ((RCAR_PRODUCT_M3 == (val & RCAR_PRODUCT_MASK)) ||
		((RCAR_PRODUCT_H3 == (val & RCAR_PRODUCT_MASK)) &&
			(RCAR_CUT_ES20 > (val & RCAR_CUT_MASK)))) {
		/* No need to disable MFIS write protection */
		;
	} else {
		/* Disable MFIS write protection */
		mmio_write_32(MFISWPCNTR, MFISWPCNTR_PASSWORD | 0x1U);
	}

	modemr &= MODEMR_BOOT_CPU_MASK;
	if((modemr == MODEMR_BOOT_CPU_CA57) ||
	   (modemr == MODEMR_BOOT_CPU_CA53)) {
		val &= (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
		if (val == RCAR_PRODUCT_H3_CUT20) {
			/* Disable TLB function in each IPMMU cache */
			mmio_write_32(IPMMUVI0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUVI1_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUPV0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUPV2_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUPV3_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUHC_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMURT_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUMP_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS1_IMSCTLR, IMSCTLR_DISCACHE);
		} else if ((val == (RCAR_PRODUCT_M3N | RCAR_CUT_ES10)) ||
			   (val == (RCAR_PRODUCT_M3N | RCAR_CUT_ES11))) {
			/* Disable TLB function in each IPMMU cache */
			mmio_write_32(IPMMUVI0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUPV0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUHC_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMURT_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUMP_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS1_IMSCTLR, IMSCTLR_DISCACHE);
		} else if (val == (RCAR_PRODUCT_E3 | RCAR_CUT_ES10)) {
			/* Disable TLB function in each IPMMU cache */
			mmio_write_32(IPMMUVI0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUPV0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUHC_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMURT_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUMP_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS0_IMSCTLR, IMSCTLR_DISCACHE);
			mmio_write_32(IPMMUDS1_IMSCTLR, IMSCTLR_DISCACHE);
		}
	}

	/* IPMMU-MM setting for linux */
	mmio_write_32(IPMMUMM_IMSCTLR, IPMMUMM_IMSCTLR_ENABLE);
	mmio_write_32(IPMMUMM_IMAUXCTLR, IPMMUMM_IMAUXCTLR_NMERGE40_BIT);

	val = ROM_GetLcs(&lcs);
	if (val != 0U) {
		ERROR("BL2: Failed to get the LCS. (%d)\n", val);
		panic();
	}
	if (lcs == LCS_SE) {
		/* ARMREG setting */
		mmio_write_32(P_ARMREG_SEC_CTRL, 
			mmio_read_32(P_ARMREG_SEC_CTRL) & 
				~((uint32_t)P_ARMREG_SEC_CTRL_PROT));
	}

	/* disable the System WDT, FIQ and GIC	*/
	bl2_swdt_release();

	/* Initialize the System Module stop registers */
	bl2_system_cpg_init();

#if RCAR_BL2_DCACHE == 1
	/* Disable data cache (clean and invalidate) */
	val = (uint32_t)read_sctlr_el1();
	val &= ~((uint32_t)(SCTLR_C_BIT | SCTLR_M_BIT));
	write_sctlr_el1((uint64_t)val);
	dcsw_op_all(DCCISW);
	tlbivmalle1();
#endif /* RCAR_BL2_DCACHE == 1 */

	/* Disable instruction cache */
	val = (uint32_t)read_sctlr_el1();
	val &= ~((uint32_t)SCTLR_I_BIT);
	write_sctlr_el1((uint64_t)val);
	isb();

	/* Invalidate instruction cache */
	iciallu();
	dsb();
	isb();
}


/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only intializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl2_plat_arch_setup(void)
{
#if RCAR_BL2_DCACHE == 1
	NOTICE("BL2: D-Cache enable\n");
	rcar_configure_mmu_el1(BL2_BASE,
			      (DEVICE_RCAR_BASE2 - BL2_BASE),
			      BL2_RO_BASE,
			      BL2_RO_LIMIT
#if USE_COHERENT_MEM
			      , BL2_COHERENT_RAM_BASE,
			      BL2_COHERENT_RAM_LIMIT
#endif
			      );
#endif /* RCAR_BL2_DCACHE == 1 */
}

/*******************************************************************************
 * Before calling this function BL31 is loaded in memory and its entrypoint
 * is set by load_image. This is a placeholder for the platform to change
 * the entrypoint of BL31 and set SPSR and security state.
 * On RCAR we are only setting the security state, entrypoint
 ******************************************************************************/
void bl2_plat_set_bl31_ep_info(image_info_t *bl31_image_info,
					entry_point_info_t *bl31_ep_info)
{
	SET_SECURITY_STATE(bl31_ep_info->h.attr, SECURE);
	bl31_ep_info->spsr = SPSR_64(MODE_EL3, MODE_SP_ELX,
					DISABLE_ALL_EXCEPTIONS);
}


/*******************************************************************************
 * Before calling this function BL32 is loaded in memory and its entrypoint
 * is set by load_image. This is a placeholder for the platform to change
 * the entrypoint of BL32 and set SPSR and security state.
 * On RCAR we are only setting the security state, entrypoint
 ******************************************************************************/
void bl2_plat_set_bl32_ep_info(image_info_t *bl32_image_info,
					entry_point_info_t *bl32_ep_info)
{
	SET_SECURITY_STATE(bl32_ep_info->h.attr, SECURE);
	bl32_ep_info->spsr = rcar_get_spsr_for_bl32_entry();
}

/*******************************************************************************
 * Before calling this function BL33 is loaded in memory and its entrypoint
 * is set by load_image. This is a placeholder for the platform to change
 * the entrypoint of BL33 and set SPSR and security state.
 * On RCAR we are only setting the security state, entrypoint
 ******************************************************************************/
void bl2_plat_set_bl33_ep_info(image_info_t *image,
					entry_point_info_t *bl33_ep_info)
{
	SET_SECURITY_STATE(bl33_ep_info->h.attr, NON_SECURE);
	bl33_ep_info->spsr = rcar_get_spsr_for_bl33_entry();
#ifdef RCAR_BL33_ARG0
	bl33_ep_info->args.arg0 = RCAR_BL33_ARG0;
#endif
}


/*******************************************************************************
 * Populate the extents of memory available for loading BL32
 ******************************************************************************/
#ifdef BL32_BASE
void bl2_plat_get_bl32_meminfo(meminfo_t *bl32_meminfo)
{
	/*
	 * Populate the extents of memory available for loading BL32.
	 */
	bl32_meminfo->total_base = BL32_BASE;
	bl32_meminfo->free_base = BL32_BASE;
	bl32_meminfo->total_size = BL32_BASE;
	bl32_meminfo->free_size = BL32_LIMIT - BL32_BASE;
}
#endif

/*******************************************************************************
 * Populate the extents of memory available for loading BL33
 ******************************************************************************/
void bl2_plat_get_bl33_meminfo(meminfo_t *bl33_meminfo)
{
	/* Non-secure target programs loading area limit is 40-bits address. */
	bl33_meminfo->total_base = DRAM1_NS_BASE;
	bl33_meminfo->total_size = DRAM_LIMIT - DRAM1_NS_BASE;
	bl33_meminfo->free_base = AARCH64_SPACE_BASE;
	bl33_meminfo->free_size = AARCH64_SPACE_SIZE;
}

void bl2_init_generic_timer(void)
{
#if RCAR_LSI == RCAR_E3
	uint32_t reg_cntfid = EXTAL_EBISU;
#else /* RCAR_LSI == RCAR_E3 */
	uint32_t reg;
	uint32_t modemr;
	uint32_t modemr_pll;
	uint32_t pll_table[] =
		{	EXTAL_MD14_MD13_TYPE_0,		/* MD14/MD13 : 0b00 */
			EXTAL_MD14_MD13_TYPE_1,		/* MD14/MD13 : 0b01 */
			EXTAL_MD14_MD13_TYPE_2,		/* MD14/MD13 : 0b10 */
			EXTAL_MD14_MD13_TYPE_3};	/* MD14/MD13 : 0b11 */
	uint32_t reg_cntfid;
	uint32_t board_type;
	uint32_t board_rev;

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr_pll = (modemr & MODEMR_BOOT_PLL_MASK);

	/* Set frequency data in CNTFID0 */
	reg_cntfid = pll_table[modemr_pll >> MODEMR_BOOT_PLL_SHIFT];
	reg = mmio_read_32(RCAR_PRR) & (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	switch (modemr_pll) {
	case MD14_MD13_TYPE_0:
		(void)get_board_type(&board_type, &board_rev);
		if (BOARD_SALVATOR_XS == board_type) {
			reg_cntfid = EXTAL_SALVATOR_XS;
		}
		break;
	case MD14_MD13_TYPE_3:
		if (RCAR_PRODUCT_H3_CUT10 == reg) {
			reg_cntfid = reg_cntfid >> 1U;
		}
		break;
	default:
		/* none */
		break;
	}
#endif /* RCAR_LSI == RCAR_E3 */
	/* Update memory mapped and register based freqency */
	write_cntfrq_el0((u_register_t )reg_cntfid);
	mmio_write_32(ARM_SYS_CNTCTL_BASE + (uintptr_t)CNTFID_OFF, reg_cntfid);
	/* Enable counter */
	mmio_setbits_32(RCAR_CNTC_BASE + (uintptr_t)CNTCR_OFF,
		(uint32_t)CNTCR_EN);
}


