/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <gic_common.h>
#include <gicv2.h>
#include <bl_common.h>
#include <cci.h>
#include <debug.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <xlat_tables_v2.h>
#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_version.h"
#if (IMAGE_BL2)
#include <io_driver.h>
#include "io_rcar.h"
#endif /* IMAGE_BL2 */

const uint8_t version_of_renesas[VERSION_OF_RENESAS_MAXLEN]
	__attribute__((__section__(".version"))) = VERSION_OF_RENESAS;

#define MAP_SHARED_RAM	MAP_REGION_FLAT(RCAR_SHARED_MEM_BASE,		\
					RCAR_SHARED_MEM_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_FLASH0	MAP_REGION_FLAT(FLASH0_BASE,			\
					FLASH0_SIZE,			\
					MT_MEMORY | MT_RO | MT_SECURE)

#define MAP_DRAM1_NS	MAP_REGION_FLAT(DRAM1_NS_BASE,			\
					DRAM1_NS_SIZE,			\
					MT_MEMORY | MT_RW | MT_NS)

#ifdef BL32_BASE
#define MAP_BL32_MEM	MAP_REGION_FLAT(BL32_BASE,			\
					BL32_LIMIT - BL32_BASE,		\
					MT_MEMORY | MT_RW | MT_SECURE)
#endif

#define MAP_DEVICE_RCAR	MAP_REGION_FLAT(DEVICE_RCAR_BASE,		\
					DEVICE_RCAR_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DEVICE_RCAR2	MAP_REGION_FLAT(DEVICE_RCAR_BASE2,	\
					DEVICE_RCAR_SIZE2,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_SRAM_STACK	MAP_REGION_FLAT(DEVICE_SRAM_STACK_BASE,		\
					DEVICE_SRAM_STACK_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_ATFW_CRASH  MAP_REGION_FLAT(RCAR_BL31_CRASH_BASE,            \
                                       RCAR_BL31_CRASH_SIZE,            \
                                       MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_ATFW_LOG	MAP_REGION_FLAT(RCAR_BL31_LOG_BASE,		\
					RCAR_BL31_LOG_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#if IMAGE_BL2
#define MAP_DRAM0	MAP_REGION_FLAT(DRAM1_BASE,			\
					DRAM1_SIZE,			\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_REG0	MAP_REGION_FLAT(DEVICE_RCAR_BASE,		\
					DEVICE_RCAR_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_RAM0	MAP_REGION_FLAT(RCAR_SYSRAM_BASE,		\
					RCAR_SYSRAM_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_REG1	MAP_REGION_FLAT(REG1_BASE,			\
					REG1_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_ROM		MAP_REGION_FLAT(ROM0_BASE,			\
					ROM0_SIZE,			\
					MT_MEMORY | MT_RO | MT_SECURE)

#define MAP_REG2	MAP_REGION_FLAT(REG2_BASE,			\
					REG2_SIZE,			\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_DRAM1	MAP_REGION_FLAT(DRAM_40BIT_BASE,		\
					DRAM_40BIT_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)
#endif /* IMAGE_BL2 */

/*
 * Table of regions for various BL stages to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * configure_mmu_elx() will give the available subset of that,
 */
#if IMAGE_BL1
static const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_FLASH0,
	MAP_DEVICE0,
	MAP_DEVICE1,
	{	0}
};
#endif
#if IMAGE_BL2
static const mmap_region_t rcar_mmap[] = {
	MAP_FLASH0,	/*   0x08000000 -   0x0BFFFFFF  RPC area            */
	MAP_DRAM0,	/*   0x40000000 -   0xBFFFFFFF  DRAM area(Legacy)   */
	MAP_REG0,	/*   0xE6000000 -   0xE62FFFFF  SoC register area   */
	MAP_RAM0,	/*   0xE6300000 -   0xE6303FFF  System RAM area     */
	MAP_REG1,	/*   0xE6400000 -   0xEAFFFFFF  SoC register area   */
	MAP_ROM,	/*   0xEB100000 -   0xEB127FFF  boot ROM area       */
	MAP_REG2,	/*   0xEC000000 -   0xFFFFFFFF  SoC register area   */
	MAP_DRAM1,	/* 0x0400000000 - 0x07FFFFFFFF  DRAM area(4GB over) */
	{	0}
};
#endif
#if IMAGE_BL31
static const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_ATFW_CRASH,
	MAP_ATFW_LOG,
	MAP_DEVICE_RCAR,
	MAP_DEVICE_RCAR2,
	MAP_SRAM_STACK,
	{	0}
};
#endif
#if IMAGE_BL32
static const mmap_region_t rcar_mmap[] = {
	MAP_DEVICE0,
	MAP_DEVICE1,
	{	0}
};
#endif

CASSERT(ARRAY_SIZE(rcar_mmap) + RCAR_BL_REGIONS \
		<= MAX_MMAP_REGIONS, assert_max_mmap_regions);

/*******************************************************************************
 * Macro generating the code for the function setting up the pagetables as per
 * the platform memory map & initialize the mmu, for the given exception level
 ******************************************************************************/
#if USE_COHERENT_MEM
#define DEFINE_CONFIGURE_MMU_EL(_el)				\
	void rcar_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit,		\
				   unsigned long coh_start,		\
				   unsigned long coh_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add_region(coh_start, coh_start,			\
				coh_limit - coh_start,			\
				MT_DEVICE | MT_RW | MT_SECURE);		\
		mmap_add(rcar_mmap);					\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#else
#define DEFINE_CONFIGURE_MMU_EL(_el)				\
	void rcar_configure_mmu_el##_el(unsigned long total_base,	\
				   unsigned long total_size,		\
				   unsigned long ro_start,		\
				   unsigned long ro_limit)		\
	{								\
		mmap_add_region(total_base, total_base,			\
				total_size,				\
				MT_MEMORY | MT_RW | MT_SECURE);		\
		mmap_add_region(ro_start, ro_start,			\
				ro_limit - ro_start,			\
				MT_MEMORY | MT_RO | MT_SECURE);		\
		mmap_add(rcar_mmap);					\
		init_xlat_tables();					\
									\
		enable_mmu_el##_el(0);					\
	}
#endif

/* Define EL1 and EL3 variants of the function initialising the MMU */
DEFINE_CONFIGURE_MMU_EL(3)

#if (IMAGE_BL2)
uintptr_t plat_get_bl31_bl32_image_entrypoint(int32_t content_cert_id)
{
	int32_t ret;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uintptr_t dest_addr;
	ret = file_to_cert(content_cert_id, &cert_addr);
	if (0 == ret) {
		get_info_from_cert((uint64_t) cert_addr, &l_image_size, &dest_addr);
	} else {
		ERROR("%s : cert file load error", __func__);
		panic();
	}
	return dest_addr;
}
#endif

uintptr_t plat_get_ns_image_entrypoint(void)
{
#if (IMAGE_BL2)
	int32_t ret;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uintptr_t dest_addr;
	ret = file_to_cert(NON_TRUSTED_FW_CONTENT_CERT_ID, &cert_addr);
	if (0 == ret) {
		get_info_from_cert((uint64_t) cert_addr, &l_image_size, &dest_addr);
	} else {
		ERROR("%s : cert file load error", __func__);
		dest_addr = NS_IMAGE_OFFSET;
	}
	return dest_addr;
#else
	return NS_IMAGE_OFFSET;
#endif
}

unsigned int plat_get_syscnt_freq2(void)
{
	unsigned int counter_base_frequency;

	/* Read the frequency from Frequency modes table */
	counter_base_frequency = (unsigned int)mmio_read_32(ARM_SYS_CNTCTL_BASE
							 + (uint32_t)CNTFID_OFF);

	/* The first entry of the frequency modes table must not be 0 */
	if (counter_base_frequency == 0U) {
		panic();
	}

	return counter_base_frequency;
}

/* Map of CCI masters with the slave interfaces they are connected */
static int cci_map[] = {
	CCI500_CLUSTER0_SL_IFACE_IX_FOR_M3,
	CCI500_CLUSTER1_SL_IFACE_IX_FOR_M3
};

void rcar_cci_init(void)
{
	/*
	 * Initialize CCI-500 driver
	 */
	uint32_t product = mmio_read_32((uintptr_t)RCAR_PRR)
			& (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	if((RCAR_PRODUCT_H3_CUT10 == product) || (RCAR_PRODUCT_H3_CUT11 == product)) {
		cci_map[0U] = CCI500_CLUSTER0_SL_IFACE_IX;
		cci_map[1U] = CCI500_CLUSTER1_SL_IFACE_IX;
	}
	cci_init(RCAR_CCI_BASE, cci_map, ARRAY_SIZE(cci_map));
}

void rcar_cci_enable(void)
{
	cci_enable_snoop_dvm_reqs((unsigned int)MPIDR_AFFLVL1_VAL(read_mpidr()));
}

void rcar_cci_disable(void)
{
	cci_disable_snoop_dvm_reqs((unsigned int)MPIDR_AFFLVL1_VAL(read_mpidr()));
}

/*******************************************************************************
 * Gets SPSR for BL32 entry
 ******************************************************************************/
uint32_t rcar_get_spsr_for_bl32_entry(void)
{
	/*
	 * The Secure Payload Dispatcher service is responsible for
	 * setting the SPSR prior to entry into the BL32 image.
	 */
	return 0U;
}

/*******************************************************************************
 * Gets SPSR for BL33 entry
 ******************************************************************************/
#define	BL33_EL1	0
#define	BL33_EL2	1

uint32_t rcar_get_spsr_for_bl33_entry(void)
{
#if (RCAR_BL33_EXECUTION_EL == BL33_EL1)
	return (uint32_t)SPSR_64(MODE_EL1, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
#elif (RCAR_BL33_EXECUTION_EL == BL33_EL2)
	return (uint32_t)SPSR_64(MODE_EL2, MODE_SP_ELX, DISABLE_ALL_EXCEPTIONS);
#else
#error
#endif
}

/* Array of secure interrupts to be configured by the gic driver */
#if IMAGE_BL2
#define PLATFORM_G1S_PROPS(grp)						\
	INTR_PROP_DESC(ARM_IRQ_SEC_WDT, GIC_HIGHEST_SEC_PRIORITY,	\
					   grp, GIC_INTR_CFG_LEVEL)
#endif
#if IMAGE_BL31
#define PLATFORM_G1S_PROPS(grp)							\
	INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_EDGE),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_RPC, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_TIMER, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_TIMER_UP, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_WDT, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_CRYPT, GIC_HIGHEST_SEC_PRIORITY,		\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_CRYPT_SecPKA, GIC_HIGHEST_SEC_PRIORITY,	\
					   grp, GIC_INTR_CFG_LEVEL),		\
	INTR_PROP_DESC(ARM_IRQ_SEC_CRYPT_PubPKA, GIC_HIGHEST_SEC_PRIORITY,	\
					   grp, GIC_INTR_CFG_LEVEL)
#endif

#define PLATFORM_G0_PROPS(grp)

static const interrupt_prop_t rcar_interrupt_props[] = {
	PLATFORM_G1S_PROPS(GICV2_INTR_GROUP0),
	PLATFORM_G0_PROPS(GICV2_INTR_GROUP0)
};

static const struct gicv2_driver_data plat_gicv2_driver_data = {
	.gicd_base = RCAR_GICD_BASE,
	.gicc_base = RCAR_GICC_BASE,
	.interrupt_props = rcar_interrupt_props,
	.interrupt_props_num = ARRAY_SIZE(rcar_interrupt_props),
};

/******************************************************************************
 * ARM common helper to initialize the GICv2 only driver.
 *****************************************************************************/
void plat_arm_gic_driver_init(void)
{
	gicv2_driver_init(&plat_gicv2_driver_data);
}

void plat_arm_gic_init(void)
{
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
}
