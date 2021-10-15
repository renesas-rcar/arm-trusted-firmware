/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <platform_def.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include "../drivers/arm/gic/v3/gicv3_private.h"
#include <bl31/interrupt_mgmt.h>
#include <common/interrupt_props.h>
#include <drivers/arm/gicv3.h>
#include <plat_helpers.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>

#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_version.h"

const uint8_t version_of_renesas[VERSION_OF_RENESAS_MAXLEN]
		__attribute__ ((__section__("ro"))) = VERSION_OF_RENESAS;

#define RCAR_DCACHE		MT_MEMORY

#define MAP_SHARED_RAM		MAP_REGION_FLAT(RCAR_SHARED_MEM_BASE,	\
					RCAR_SHARED_MEM_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_DEVICE_RCAR		MAP_REGION_FLAT(DEVICE_RCAR_BASE,	\
					DEVICE_RCAR_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)


#define MAP_ATFW_CRASH		MAP_REGION_FLAT(RCAR_BL31_CRASH_BASE,	\
					RCAR_BL31_CRASH_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)



static const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_ATFW_CRASH,
	MAP_DEVICE_RCAR,
	{0}
};

CASSERT((ARRAY_SIZE(rcar_mmap) + RCAR_BL_REGIONS)
	<= MAX_MMAP_REGIONS, assert_max_mmap_regions);

/*
 * Macro generating the code for the function setting up the pagetables as per
 * the platform memory map & initialize the mmu, for the given exception level
 */
#if USE_COHERENT_MEM
void rcar_configure_mmu_el3(uintptr_t total_base,
			    size_t total_size,
			    uintptr_t ro_start,
			    uintptr_t ro_limit,
			    uintptr_t coh_start,
			    uintptr_t coh_limit)
{
	mmap_add_region(total_base, total_base, total_size,
			RCAR_DCACHE | MT_RW | MT_SECURE);
	mmap_add_region(ro_start, ro_start, ro_limit - ro_start,
			RCAR_DCACHE | MT_RO | MT_SECURE);
	mmap_add_region(coh_start, coh_start, coh_limit - coh_start,
			MT_DEVICE | MT_RW | MT_SECURE);
	mmap_add(rcar_mmap);

	init_xlat_tables();
	enable_mmu_el3(0);
}
#else
void rcar_configure_mmu_el3(uintptr_t total_base,
			    size_t total_size,
			    uintptr_t ro_start,
			    uintptr_t ro_limit)
{
	mmap_add_region(total_base, total_base, total_size,
			RCAR_DCACHE | MT_RW | MT_SECURE);
	mmap_add_region(ro_start, ro_start, ro_limit - ro_start,
			RCAR_DCACHE | MT_RO | MT_SECURE);
	mmap_add(rcar_mmap);

	init_xlat_tables();
	enable_mmu_el3(0);
}
#endif

unsigned int plat_get_syscnt_freq2(void)
{
	unsigned int freq;

	freq = mmio_read_32(ARM_SYS_CNTCTL_BASE + CNTFID_OFF);
	if (freq == 0U) {
		panic();
	}

	return freq;
}

void plat_rcar_gic_init(void)
{
	gicv3_distif_init();
	gicv3_rdistif_init(plat_my_core_pos());
	gicv3_cpuif_enable(plat_my_core_pos());
}


/*
 * Define a list of Group 1 Secure and Group 0 interrupt properties as per GICv3
 * terminology. On a GICv2 system or mode, the lists will be merged and treated
 * as Group 0 interrupts.
 */
#define PLATFORM_G1S_PROPS(grp) \
	INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_LEVEL), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE), \
	INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, (grp), \
			GIC_INTR_CFG_EDGE)

static const interrupt_prop_t interrupt_props[] = {
	PLATFORM_G1S_PROPS(INTR_GROUP1S)
};

static unsigned int rcar_mpidr_to_core_pos(u_register_t mpidr)
{
	return plat_renesas_calc_core_pos(mpidr);
}

void plat_rcar_gic_driver_init(void)
{
	static uintptr_t rdistif_base_addrs[PLATFORM_CORE_COUNT];
	static gicv3_driver_data_t rcar_gicv3_data = {
		.gicd_base = RCAR_GICD_BASE,
		.gicr_base = RCAR_GICR_BASE,
		.rdistif_num = PLATFORM_CORE_COUNT,
		.rdistif_base_addrs = rdistif_base_addrs,
		.mpidr_to_core_pos = rcar_mpidr_to_core_pos,
		.interrupt_props = interrupt_props,
		.interrupt_props_num = (uint32_t) ARRAY_SIZE(interrupt_props),
	};

	gicv3_driver_init(&rcar_gicv3_data);
}
