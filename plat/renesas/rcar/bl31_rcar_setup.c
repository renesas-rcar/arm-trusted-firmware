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

#include <arch.h>
#include <arch_helpers.h>
#include <arm_gic.h>
#include <assert.h>
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

#define	CHECK_MD13_MD14		(0x6000U)
#define	FREQ_10M		(0x2000U)
#define	FREQ_12_5M		(0x4000U)
#define	FREQ_8_33M		(0x0000U)
#define	FREQ_16_66M		(0x6000U)

/*******************************************************************************
 * Declarations of linker defined symbols which will help us find the layout
 * of trusted SRAM
 ******************************************************************************/
extern unsigned long __RO_START__;
extern unsigned long __RO_END__;
extern unsigned long __BL31_END__;

#if USE_COHERENT_MEM
extern unsigned long __COHERENT_RAM_START__;
extern unsigned long __COHERENT_RAM_END__;
#endif

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

#if RESET_TO_BL31
static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;
#else
/*******************************************************************************
 * Reference to structure which holds the arguments that have been passed to
 * BL31 from BL2.
 ******************************************************************************/
static bl31_params_t *bl2_to_bl31_params;
#endif

/* Array of secure interrupts to be configured by the gic driver */
const unsigned int irq_sec_array[] = {
	ARM_IRQ_SEC_PHY_TIMER,		/* 29		*/
	ARM_IRQ_SEC_SGI_0,		/* 8		*/
	ARM_IRQ_SEC_SGI_1,		/* 9		*/
	ARM_IRQ_SEC_SGI_2,		/* 10		*/
	ARM_IRQ_SEC_SGI_3,		/* 11		*/
	ARM_IRQ_SEC_SGI_4,		/* 12		*/
	ARM_IRQ_SEC_SGI_5,		/* 13		*/
	ARM_IRQ_SEC_SGI_6,		/* 14		*/
	ARM_IRQ_SEC_SGI_7,		/* 15		*/
	ARM_IRQ_SEC_RPC,		/* 70		*/
	ARM_IRQ_SEC_TIMER,		/* 166		*/
	ARM_IRQ_SEC_TIMER_UP,		/* 171		*/
	ARM_IRQ_SEC_WDT,		/* 173		*/
	ARM_IRQ_SEC_CRYPT,		/* 102		*/
	ARM_IRQ_SEC_CRYPT_SecPKA,	/* 97		*/
	ARM_IRQ_SEC_CRYPT_PubPKA	/* 98		*/
};

uint32_t rcar_boot_kind_flag __attribute__((section("data")));

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for the
 * security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type. A NULL pointer is returned
 * if the image does not exist.
 ******************************************************************************/
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	assert(sec_state_is_valid(type));

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
void bl31_early_platform_setup(bl31_params_t *from_bl2,
		void *plat_params_from_bl2)
{
	uint32_t chk_data;
	uint32_t freq_data;
	uint32_t product_cut = mmio_read_32((uintptr_t)RCAR_PRR)
				& (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);

	/*
	 * Set frequency data to CNTFID0
	 */
	chk_data = mmio_read_32((uintptr_t)RCAR_MODEMR) & CHECK_MD13_MD14;
	switch (chk_data) {
	case FREQ_8_33M:
		freq_data = 8333300U; /* 8.33MHz	*/
		break;
	case FREQ_10M:
		freq_data = 10000000U; /* 10MHz	*/
		break;
	case FREQ_12_5M:
		freq_data = 12500000U; /* 12.5MHz	*/
		break;
	case FREQ_16_66M:
		freq_data = 16666600U; /* 16.66MHz	*/
		if (product_cut == (RCAR_PRODUCT_H3 | RCAR_CUT_ES10)) {
			freq_data = freq_data >> 1;
		}
		break;
	default:
		freq_data = 0U;
		break;
	}
	mmio_write_32(RCAR_CNTC_BASE + CNTFID_OFF, freq_data);

	/* Initialize the log area to provide early debug support */
	console_init(1U, 0U, 0U);

	NOTICE("BL3-1 : Rev.%s\n",version_of_renesas);

	/* Check params passed from BL2 should not be NULL,
	 * We are not checking plat_params_from_bl2 as NULL as we are not
	 * using it on RCAR
	 */
	assert(from_bl2 != NULL);
	assert(from_bl2->h.type == PARAM_BL31);
	assert(from_bl2->h.version >= VERSION_1);

	bl2_to_bl31_params = from_bl2;

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

/*******************************************************************************
 * Initialize the gic, configure the CLCD and zero out variables needed by the
 * secondaries to boot up correctly.
 ******************************************************************************/
void bl31_platform_setup(void)
{

	/* Initialize the gic cpu and distributor interfaces */
	arm_gic_init(RCAR_GICC_BASE, RCAR_GICD_BASE, RCAR_GICR_BASE,
			irq_sec_array, ARRAY_SIZE(irq_sec_array));
	arm_gic_setup();

	/* Enable and initialize the System level generic timer */
	mmio_write_32(RCAR_CNTC_BASE + CNTCR_OFF, CNTCR_FCREQ(0) | CNTCR_EN);

	/* Intialize the power controller */
	rcar_pwrc_setup();

	/* Topologies are best known to the platform. */
	rcar_setup_topology();
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

}
