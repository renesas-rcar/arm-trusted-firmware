/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
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
#include <assert.h>
#include <bl_common.h>
#include <console.h>
#include <platform.h>
#include <platform_def.h>
#include <string.h>
#include <arm_gic.h>
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


/* CPG write protect registers */
/*#define	CPG_CPGWPR		(CPG_BASE + 0x900U)*/
/*#define	CPG_CPGWPCR		(CPG_BASE + 0x904U)*/
#define	CPGWPR_PASSWORD		(0x5A5AFFFFU)
#define	CPGWPCR_PASSWORD	(0xA5A50000U)

/* CA57 Debug Resource control registers */
#define	CPG_CA57DBGRCR		(CPG_BASE + 0x2180U)
#define	DBGCPUPREN		((uint32_t)1U << 19U)
#define	CPG_PLL0CR		(CPG_BASE + 0x00D8U)
#define	CPG_PLL2CR		(CPG_BASE + 0x002CU)
#define	CPG_PLL4CR		(CPG_BASE + 0x01F4U)

/* RST Registers */
#define	RST_BASE		(0xE6160000U)
#define	RST_WDTRSTCR		(RST_BASE + 0x0054U)
#define	WDTRSTCR_PASSWORD	(0xA55A0000U)
#define	WDTRSTCR_RWDT_RSTMSK	((uint32_t)1U << 0U)

/* IPMMUregisters */
#define IPMMU_MM_BASE		(0xE67B0000U)	/* IPMMU-MM */
#define IPMMUMM_SYSCTRL		(IPMMU_MM_BASE + 0x0500U)
#define IPMMUMM_SYSAUX		(IPMMU_MM_BASE + 0x0504U)

/* MIDR */
#define MIDR_CA57		(0x0D07U << MIDR_PN_SHIFT)
#define MIDR_CA53		(0x0D03U << MIDR_PN_SHIFT)

/* MaskROM API */
typedef uint32_t(*ROM_GETLCS_API)(uint32_t *pLcs);
#if RCAR_LSI == RCAR_H3
#define ROM_GETLCS_API_ADDR	((ROM_GETLCS_API)0xEB10DFE0U)
#elif RCAR_LSI == RCAR_M3
#define ROM_GETLCS_API_ADDR	((ROM_GETLCS_API)0xEB110578U)
#endif
#define LCS_CM			(0x0U)
#define LCS_DM			(0x1U)
#define LCS_SD			(0x3U)
#define LCS_SE			(0x5U)
#define LCS_FA			(0x7U)

/* R-Car Gen3 product check */
#if RCAR_LSI == RCAR_H3
#define TARGET_PRODUCT		RCAR_PRODUCT_H3
#define TARGET_NAME		"R-Car H3"
#elif RCAR_LSI == RCAR_M3
#define TARGET_PRODUCT		RCAR_PRODUCT_M3
#define TARGET_NAME		"R-Car M3"
#endif

/* Instruction Cache Invalidate All to PoU */
DEFINE_SYSOP_TYPE_FUNC(ic, iallu)	/* ICIALLU instruction macro */

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
#if 0
#if DEBUG
	bl31_ep_info->args.arg1 = RCAR_BL31_PLAT_PARAM_VAL;
#endif
#else
	bl31_ep_info->args.arg1 = 0x00000000;	/* nomal boot/cold boot */
#endif
	return bl31_ep_info;
}

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
#define	LOSSY_ST_ADDR1			0x0 /* Undefined */
#define	LOSSY_END_ADDR1			0x0 /* Undefined */
#define	LOSSY_FMT1				LOSSY_FMT_ARGB8888
#define	LOSSY_ENA_DIS1			LOSSY_DISABLE

/* Settings of Entry 2 */
#define	LOSSY_ST_ADDR2			0x0 /* Undefined */
#define	LOSSY_END_ADDR2			0x0 /* Undefined */
#define	LOSSY_FMT2				LOSSY_FMT_YUV422INTLV
#define	LOSSY_ENA_DIS2			LOSSY_DISABLE

typedef struct bl2_lossy_info {
	uint32_t magic;
	uint32_t a0;
	uint32_t b0;
} bl2_lossy_info_t;

void bl2_lossy_setting(uint32_t no, uint32_t start_addr, uint32_t end_addr,
	uint32_t format, uint32_t enable)
{
	uint32_t reg;
	bl2_lossy_info_t *bl2_lossy_info;

	/* Setting of the start address and format */
	reg = format | (start_addr >> 20);
	mmio_write_32(AXI_DCMPAREACRA0 + 0x8 * no, reg);

	/* Setting of the end address */
	mmio_write_32(AXI_DCMPAREACRB0 + 0x8 * no,
		(end_addr >> 20));

	/* Enable or Disable of Lossy setting */
	mmio_write_32(AXI_DCMPAREACRA0 + 0x8 * no, (reg | enable));

	bl2_lossy_info = (bl2_lossy_info_t *)(LOSSY_PARAMS_BASE);
	bl2_lossy_info += no;
	bl2_lossy_info->magic = 0x12345678;
	bl2_lossy_info->a0 = mmio_read_32(AXI_DCMPAREACRA0 + 0x8 * no);
	bl2_lossy_info->b0 = mmio_read_32(AXI_DCMPAREACRB0 + 0x8 * no);

	NOTICE("     Entry %d: DCMPAREACRAx:0x%x DCMPAREACRBx:0x%x\n", no,
		mmio_read_32(AXI_DCMPAREACRA0 + 0x8 * no),
		mmio_read_32(AXI_DCMPAREACRB0 + 0x8 * no));
}

/*******************************************************************************
 * BL1 has passed the extents of the trusted SRAM that should be visible to BL2
 * in x0. This memory layout is sitting at the base of the free trusted SRAM.
 * Copy it to a safe loaction before its reclaimed by later BL2 functionality.
 ******************************************************************************/
void bl2_early_platform_setup(meminfo_t *mem_layout)
{
	const unsigned int irq_sec_array[] = {
		ARM_IRQ_SEC_WDT                /* 173          */
	};
	const ROM_GETLCS_API	ROM_GetLcs = ROM_GETLCS_API_ADDR;
	uint32_t reg;
	uint32_t lcs;
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
	const char *unknown         = "unknown";

#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
	/* initialize Pin Function */
	pfc_init();
#endif

	/* Initialize the console to provide early debug support */
	(void)console_init(0U, 0U, 0U);

	/* GIC initialize		*/
	arm_gic_init(RCAR_GICC_BASE, RCAR_GICD_BASE, RCAR_GICR_BASE
			,irq_sec_array, ARRAY_SIZE(irq_sec_array));
	/* GIC setup			*/
	arm_gic_setup();

	/* Enable FIQ interrupt		*/
	enable_fiq();

	/* System WDT initialize	*/
	bl2_swdt_init();

	bl2_avs_init();		/*  Initialize AVS Settings */
	
	bl2_avs_setting();	/*  Proceed with separated AVS processing */
	
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
	NOTICE("BL2: R-Car Gen3 Initial Program Loader(%s) Rev.%s\n", str, version_of_renesas);

	bl2_avs_setting();	/*  Proceed with separated AVS processing */
	
	/* R-Car Gen3 product display & check */
	reg = mmio_read_32(RCAR_PRR);
	switch (reg & RCAR_PRODUCT_MASK) {
	case RCAR_PRODUCT_H3:
		str = product_h3;
		break;
	case RCAR_PRODUCT_M3:
		str = product_m3;
		break;
	default:
		str = unknown;
		break;
	}
	NOTICE("BL2: PRR is R-Car %s ES%d.%d\n", str,
		((reg & RCAR_MAJOR_MASK) >> RCAR_MAJOR_SHIFT) + RCAR_MAJOR_OFFSET,
		 (reg & RCAR_MINOR_MASK));
	if((reg & RCAR_PRODUCT_MASK) != TARGET_PRODUCT) {
		ERROR("BL2: This IPL has been built for the %s.\n", 
								TARGET_NAME);
		ERROR("BL2: Please write the correct IPL to flash memory.\n");
		panic();
	}

	bl2_avs_setting();	/*  Proceed with separated AVS processing */

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
	NOTICE("BL2: LCM state is %s\n", str);

	bl2_avs_setting();	/*  Proceed with separated AVS processing */
	
	/* Setup the BL2 memory layout */
	bl2_tzram_layout = *mem_layout;

#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
	/* Initialize SDRAM */
	InitDram();

	bl2_avs_setting();	/*  Proceed with separated AVS processing */
#endif

	/* Initialize RPC */
	initRPC();

	bl2_avs_setting();	/*  Proceed with separated AVS processing */

	/* Initialize DMA */
	initDMA();

#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
	bl2_avs_setting();	/*  Proceed with separated AVS processing */

	/* Initialize secure configuration */
	bl2_secure_setting();
#endif

	bl2_avs_setting();	/*  Proceed with separated AVS processing */

	/* Initialize CPG configuration */
	bl2_cpg_init();

#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
	bl2_avs_setting();	/*  Proceed with separated AVS processing */

	/* initialize QoS configration */
	qos_init();
#endif

	bl2_avs_end();		/* End of AVS Settings */

	/* unmask the detection of RWDT overflow */
	reg = mmio_read_32(RST_WDTRSTCR);
	reg &= ~WDTRSTCR_RWDT_RSTMSK;
	reg |= WDTRSTCR_PASSWORD;
	mmio_write_32(RST_WDTRSTCR, reg);

	/* Release CPG write protect */
	mmio_write_32(CPG_CPGWPR, CPGWPR_PASSWORD);
	mmio_write_32(CPG_CPGWPCR, CPGWPCR_PASSWORD);

	/* CA57 debug resource control */
	mmio_write_32(CPG_CA57DBGRCR,
			DBGCPUPREN | mmio_read_32(CPG_CA57DBGRCR));

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

	NOTICE("BL2: Lossy Decomp areas\n");
	/* Lossy setting : entry 0 */
	bl2_lossy_setting(0, LOSSY_ST_ADDR0, LOSSY_END_ADDR0,
		LOSSY_FMT0, LOSSY_ENA_DIS0);

	/* Lossy setting : entry 1 */
	bl2_lossy_setting(1, LOSSY_ST_ADDR1, LOSSY_END_ADDR1,
		LOSSY_FMT1, LOSSY_ENA_DIS1);

	/* Lossy setting : entry 2 */
	bl2_lossy_setting(2, LOSSY_ST_ADDR2, LOSSY_END_ADDR2,
		LOSSY_FMT2, LOSSY_ENA_DIS2);

	/* Initialise the IO layer and register platform IO devices */
	rcar_io_setup();
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

	/* IPMMU-MM setting for linux */
	mmio_write_32(IPMMUMM_SYSCTRL, 0xC0000000U);
	mmio_write_32(IPMMUMM_SYSAUX, 0x01000000U);
}

/* Flush the TF params and the TF plat params */
void bl2_plat_flush_bl31_params(void)
{
#if 0
	flush_dcache_range((unsigned long)PARAMS_BASE, \
				sizeof(bl2_to_bl31_params_mem_t));
#endif
	uint32_t val;

	/* disable the System WDT, FIQ and GIC	*/
	bl2_swdt_release();

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


/*******************************************************************************
 * Populate the extents of memory available for loading BL33
 ******************************************************************************/
void bl2_plat_get_bl33_meminfo(meminfo_t *bl33_meminfo)
{
	bl33_meminfo->total_base = DRAM1_NS_BASE;
	bl33_meminfo->total_size = DRAM1_NS_SIZE;
	bl33_meminfo->free_base = DRAM1_NS_BASE;
	bl33_meminfo->free_size = DRAM1_NS_SIZE;
}
