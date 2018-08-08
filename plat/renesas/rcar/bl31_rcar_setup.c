/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <bl_common.h>
#include <bl31.h>
#include <console.h>
#include <mmio.h>
#include <platform.h>
#include <stddef.h>
#include <debug.h>
#include "drivers/pwrc/rcar_pwrc.h"
#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_version.h"

/*
 * The next 3 constants identify the extents of the code, RO data region and the
 * limit of the BL3-1 image.  These addresses are used by the MMU setup code and
 * therefore they must be page-aligned.  It is the responsibility of the linker
 * script to ensure that __RO_START__, __RO_END__ & __BL31_END__ linker symbols
 * refer to page-aligned addresses.
 */
#define BL31_RO_BASE (uint64_t)(&__RO_START__)
#define BL31_RO_LIMIT (uint64_t)(&__RO_END__)
#define BL31_END (uint64_t)(&__BL31_END__)

#if USE_COHERENT_MEM
/*
 * The next 2 constants identify the extents of the coherent memory region.
 * These addresses are used by the MMU setup code and therefore they must be
 * page-aligned.  It is the responsibility of the linker script to ensure that
 * __COHERENT_RAM_START__ and __COHERENT_RAM_END__ linker symbols
 * refer to page-aligned addresses.
 */
#define BL31_COHERENT_RAM_BASE (uint64_t)(&__COHERENT_RAM_START__)
#define BL31_COHERENT_RAM_LIMIT (uint64_t)(&__COHERENT_RAM_END__)
#endif

/*******************************************************************************
 * Reference to structure which holds the arguments that have been passed to
 * BL31 from BL2.
 ******************************************************************************/
static bl31_params_t *bl2_to_bl31_params;

uint32_t rcar_boot_kind_flag __attribute__((section("data")));

static uint64_t rcar_boot_mpidr;

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for the
 * security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type. A NULL pointer is returned
 * if the image does not exist.
 ******************************************************************************/
struct entry_point_info *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	next_image_info =
			(type == NON_SECURE) ?
					bl2_to_bl31_params->bl33_ep_info :
					bl2_to_bl31_params->bl32_ep_info;

	/* None of the images on this platform can have 0x0 as the entrypoint */
	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}

/*******************************************************************************
 * Perform any BL31 specific platform actions. Here is an opportunity to copy
 * parameters passed by the calling EL (S-EL1 in BL2) before they
 * are lost (potentially). This needs to be done before the MMU is initialized
 * so that the memory layout can be used while creating page tables. On the RCAR
 * we know that BL2 has populated the parameters. So we just use
 * the reference passed in 'from_bl2' instead of copying. The 'data' parameter
 * is not used since all the information is contained in 'from_bl2'. Also, BL2
 * has flushed this information to memory, so we are guaranteed to pick up good
 * data
 ******************************************************************************/
void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
		u_register_t arg2, u_register_t arg3)
{
	uint32_t cluster_type;
	bl31_params_t *from_bl2    = (bl31_params_t *)arg0;
	uintptr_t soc_fw_config    = (uintptr_t)arg1;
	uintptr_t hw_config        = (uintptr_t)arg2;
	/* arg3 is not used in RCAR_BL31. */

	/* Initialize the log area to provide early debug support */
	console_init(1U, 0U, 0U);

	NOTICE("BL3-1 : Rev.%s\n",version_of_renesas);

	/* Check params passed from BL2 should not be NULL,
	 */
	if ((NULL == from_bl2) || ((uint8_t)PARAM_BL31 != from_bl2->h.type) ||
	    ((uint8_t)VERSION_1 > from_bl2->h.version)) {
		panic();
	}

	/* Dynamic Config is not supported for LOAD_IMAGE_V1 */
	if ((0U != soc_fw_config) || (0U != hw_config)) {
		panic();
	}

	bl2_to_bl31_params = from_bl2;

	cluster_type = rcar_bl31_get_cluster();
	if (RCAR_CLUSTER_A53A57 == cluster_type) {
		/*
		 * Initialize CCI for this cluster during cold boot.
		 * No need for locks as no other CPU is active.
		 */
		rcar_cci_init();
		/*
		 * Enable CCI coherency for the primary CPU's cluster
		 * RCAR PSCI code will enable coherency for other clusters.
		 */
		rcar_cci_enable();
	}

}


/*******************************************************************************
 * Initialize the gic, configure the CLCD and zero out variables needed by the
 * secondaries to boot up correctly.
 ******************************************************************************/
void bl31_platform_setup(void)
{

	/* Initialize the GIC driver, cpu and distributor interfaces */
	plat_arm_gic_driver_init();
	plat_arm_gic_init();

	/* Enable and initialize the System level generic timer */
	mmio_write_32(RCAR_CNTC_BASE + CNTCR_OFF, CNTCR_FCREQ(U(0)) | CNTCR_EN);

	/* Intialize the power controller */
	rcar_pwrc_setup();

	/* Topologies are best known to the platform. */
	rcar_setup_topology();

	/* Get the mpidr for boot cpu */
	rcar_boot_mpidr = read_mpidr_el1() & 0x0000ffffU;
}

/*******************************************************************************
 * Perform the very early platform specific architectural setup here. At the
 * moment this is only intializes the mmu in a quick and dirty way.
 ******************************************************************************/
void bl31_plat_arch_setup(void)
{
	rcar_configure_mmu_el3(BL31_RO_BASE, (BL31_END - BL31_RO_BASE),
			BL31_RO_BASE,
			BL31_RO_LIMIT
#if USE_COHERENT_MEM
			, BL31_COHERENT_RAM_BASE,
			BL31_COHERENT_RAM_LIMIT
#endif
	);

	rcar_bl31_code_copy_to_system_ram(RCAR_DYNAMIC_REGION_NOT_EXIST);
}

/*******************************************************************************
 * Instead of svc_migrate_info in RCAR.
 * returned PSCI_TOS_NOT_UP_MIG_CAP and boot MPIDR.
 ******************************************************************************/
uint32_t bl31_plat_cpu_migrate_info(u_register_t *resident_cpu)
{
	*resident_cpu = rcar_boot_mpidr;
	return (uint32_t)PSCI_TOS_NOT_UP_MIG_CAP;
}

/*******************************************************************************
 * There check whether CPU_OFF is OK or not.
 ******************************************************************************/
int32_t bl31_plat_denied_cpu_off_chk(void)
{
	int32_t rc = PSCI_E_SUCCESS;
	uint64_t tmp_mpidr;

	tmp_mpidr = read_mpidr_el1() & 0x0000ffffU;

	if (tmp_mpidr == rcar_boot_mpidr) {
		rc = PSCI_E_DENIED;
	}
	return rc;
}

/*******************************************************************************
 * Check boot_mpidr(CPU0).
 * If the CPU is the same as CPU0, return RCAR_MPIDRCHK_BOOTCPU,
 * otherwise it returns RCAR_MPIDRCHK_NOT_BOOTCPU.
 ******************************************************************************/
uint32_t bl31_plat_boot_mpidr_chk(void)
{
	uint32_t rc = RCAR_MPIDRCHK_NOT_BOOTCPU;
	uint64_t tmp_mpidr;

	tmp_mpidr = read_mpidr_el1() & 0x0000ffffU;

	if (tmp_mpidr == rcar_boot_mpidr) {
		rc = RCAR_MPIDRCHK_BOOTCPU;
	}
	return rc;
}

/*******************************************************************************
 * Perform the runtime platform specific setup here.
 ******************************************************************************/
void bl31_plat_runtime_setup(void)
{
#if (RCAR_DISABLE_NONSECURE_RPC_ACCESS == 0)
	/* Enable non-secure access to the RPC HyperFlash region. */
	mmio_write_32(0xee2000b8, 0x155);
	mmio_write_32(0xee200000, mmio_read_32(0xee200000) & 0x7fffffff);
#endif
	/*
	 * Finish the use of console driver in BL31 so that any runtime logs
	 * from BL31 will be suppressed.
	 */
	console_uninit();
}
