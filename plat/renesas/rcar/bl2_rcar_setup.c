/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
#include "ddr/boot_init_dram.h"
#include "qos/qos_init.h"
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
#define IPMMUMM_SYSCTRL		(IPMMU_MM_BASE + 0x0500U)
#define IPMMUMM_SYSAUX		(IPMMU_MM_BASE + 0x0504U)

#define IPMMU_VI0_BASE		(0xFEBD0000U)	/* IPMMU-VI0 */
#define IPMMUVI0_SYSCTRL	(IPMMU_VI0_BASE + 0x0500U)

#define IPMMU_VI1_BASE		(0xFEBE0000U)	/* IPMMU-VI1 */
#define IPMMUVI1_SYSCTRL	(IPMMU_VI1_BASE + 0x0500U)

#define IPMMU_PV0_BASE		(0xFD800000U)	/* IPMMU-PV0 */
#define IPMMUPV0_SYSCTRL	(IPMMU_PV0_BASE + 0x0500U)

#define IPMMU_PV2_BASE		(0xFD960000U)	/* IPMMU-PV2 */
#define IPMMUPV2_SYSCTRL	(IPMMU_PV2_BASE + 0x0500U)

#define IPMMU_PV3_BASE		(0xFD970000U)	/* IPMMU-PV3 */
#define IPMMUPV3_SYSCTRL	(IPMMU_PV3_BASE + 0x0500U)

#define IPMMU_HC_BASE		(0xE6570000U)	/* IPMMU-HC */
#define IPMMUHC_SYSCTRL		(IPMMU_HC_BASE + 0x0500U)

#define IPMMU_RT_BASE		(0xFFC80000U)	/* IPMMU-RT */
#define IPMMURT_SYSCTRL		(IPMMU_RT_BASE + 0x0500U)

#define IPMMU_MP_BASE		(0xEC670000U)	/* IPMMU-MP */
#define IPMMUMP_SYSCTRL		(IPMMU_MP_BASE + 0x0500U)

#define IPMMU_DS0_BASE		(0xE6740000U)	/* IPMMU-DS0 */
#define IPMMUDS0_SYSCTRL	(IPMMU_DS0_BASE + 0x0500U)

#define IPMMU_DS1_BASE		(0xE7740000U)	/* IPMMU-DS1 */
#define IPMMUDS1_SYSCTRL	(IPMMU_DS1_BASE + 0x0500U)

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
#endif

/* for SuspendToRAM */
#define	GPIO_BASE	(0xE6050000U)
#define	GPIO_INDT1	(GPIO_BASE + 0x100CU)
#define	BIT8		((uint32_t)1U<<8)
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
	int32_t ret;
	uint32_t prr_val;
	char msg[128];
	const char *str;
	const char *cpu_ca57        = "CA57";
	const char *cpu_ca53        = "CA53";
	const char *product_h3      = "H3";
	const char *product_m3      = "M3";
	const char *lcs_cm          = "CM";
	const char *lcs_dm          = "DM";
	const char *lcs_sd          = "SD";
	const char *lcs_secure      = "SE";
	const char *lcs_fa          = "FA";
	const char *boot_hyper160   = "HyperFlash(160MHz)";
	const char *boot_hyper80    = "HyperFlash(80MHz)";
	const char *boot_qspi40     = "QSPI Flash(40MHz)";
	const char *boot_qspi80     = "QSPI Flash(80MHz)";
	const char *boot_emmc25x1   = "eMMC(25MHz x1)";
	const char *boot_emmc50x8   = "eMMC(50MHz x8)";
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

	/* Initialize AVS Settings */
	bl2_avs_init();

	/* Proceed with separated AVS processing */
	bl2_avs_setting();

	/* boot message */
	reg = (uint32_t)read_midr();
	switch (reg & (uint32_t)(MIDR_PN_MASK << MIDR_PN_SHIFT)) {
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

	/* Proceed with separated AVS processing */
	bl2_avs_setting();

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
			prr_val = 0x00000001U;
		}
		break;
	default:
		str = unknown;
		break;
	}
	(void)sprintf(msg, "BL2: PRR is R-Car %s Ver%d.%d\n", str,
		((prr_val & RCAR_MAJOR_MASK) >> RCAR_MAJOR_SHIFT)
		 + RCAR_MAJOR_OFFSET, (prr_val & RCAR_MINOR_MASK));
	NOTICE("%s", msg);

	board_id_init();

	(void)sprintf(msg, "BL2: Board %s Rev%d.%d\n",
			get_board_name(),
			get_board_rev_major(),
			get_board_rev_minor());
	NOTICE("%s", msg);

#if RCAR_LSI != RCAR_AUTO
	if((reg & RCAR_PRODUCT_MASK) != TARGET_PRODUCT) {
		ERROR("BL2: This IPL has been built for the %s.\n",
								TARGET_NAME);
		ERROR("BL2: Please write the correct IPL to flash memory.\n");
		panic();
	}
#endif /* RCAR_LSI != RCAR_AUTO */

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

	/* Proceed with separated AVS processing */
	bl2_avs_setting();

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
	mmio_write_32(CPG_CA57DBGRCR,
			DBGCPUPREN | mmio_read_32(CPG_CA57DBGRCR));
	mmio_write_32(CPG_CA53DBGRCR,
			DBGCPUPREN | mmio_read_32(CPG_CA53DBGRCR));

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
#if (PMIC_ON_BOARD==1)
	static uint32_t backupTriggerOnce = 1U;
	static uint32_t backupTrigger = 0U;
	if (backupTriggerOnce == 1U) {
		backupTriggerOnce = 0U;
		/* Read and return BKUP_TRG(IO port B8, GPIO GP-1-8) */
		if ((mmio_read_32((uintptr_t)GPIO_INDT1) & BIT8) != 0U) {
			backupTrigger = 1U;
		}
	}
	return backupTrigger;
#else	/* (PMIC_ON_BOARD==1) */
	return 0U;	/* Cold boot only */
#endif	/* (PMIC_ON_BOARD==1) */
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
			mmio_write_32(IPMMUVI0_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUVI1_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUPV0_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUPV2_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUPV3_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUHC_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMURT_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUMP_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUDS0_SYSCTRL, 0xE0000000U);
			mmio_write_32(IPMMUDS1_SYSCTRL, 0xE0000000U);
		}
	}

	/* IPMMU-MM setting for linux */
	mmio_write_32(IPMMUMM_SYSCTRL, 0xC0000000U);
	mmio_write_32(IPMMUMM_SYSAUX, 0x01000000U);

	/* disable the System WDT, FIQ and GIC	*/
	bl2_swdt_release();

	/* Finalize a console of provide early debug support */
	console_finalize();

	/* Initialize the System Module stop registers */
	bl2_system_cpg_init();

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
#if 0
	rcar_configure_mmu_el1(bl2_tzram_layout.total_base,
			      bl2_tzram_layout.total_size,
			      BL2_RO_BASE,
			      BL2_RO_LIMIT
#if USE_COHERENT_MEM
			      , BL2_COHERENT_RAM_BASE,
			      BL2_COHERENT_RAM_LIMIT
#endif
			      );
#endif
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
	uint32_t reg;
	uint32_t modemr;
	uint32_t modemr_pll;
	uint32_t pll_table[] = { 8333300U,	/* MD14/MD13 : 0b00 */
				10000000U,	/* MD14/MD13 : 0b01 */
				12500000U,	/* MD14/MD13 : 0b10 */
				16666600U};	/* MD14/MD13 : 0b11 */
	uint32_t reg_cntfid;

	/* board id initialization before bl2_early_platform_setup */
	board_id_init();

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr_pll = (modemr & MODEMR_BOOT_PLL_MASK);

	/* Set frequency data in CNTFID0 */
	reg_cntfid = pll_table[modemr_pll >> MODEMR_BOOT_PLL_SHIFT];
	reg = mmio_read_32(RCAR_PRR) & (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	switch (modemr_pll) {
	case MD14_MD13_TYPE_0:
		if (board_is(BOARD_SALVATOR_XS))
			reg_cntfid = 8320000U;
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
	/* Update memory mapped and register based freqency */
	write_cntfrq_el0((u_register_t )reg_cntfid);
	mmio_write_32(ARM_SYS_CNTCTL_BASE + (uintptr_t)CNTFID_OFF, reg_cntfid);
	/* Enable counter */
	mmio_setbits_32(RCAR_CNTC_BASE + (uintptr_t)CNTCR_OFF,
		(uint32_t)CNTCR_EN);
}

