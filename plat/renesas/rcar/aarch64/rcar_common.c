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
#include <bl_common.h>
#include <cci.h>
#include <debug.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <xlat_tables.h>
#include "../rcar_def.h"
#include "../rcar_private.h"
#include "../rcar_version.h"

const uint8_t version_of_renesas[] __attribute__((__section__(".version"))) = VERSION_OF_RENESAS;

#define MAP_SHARED_RAM	MAP_REGION_FLAT(RCAR_SHARED_MEM_BASE,		\
					RCAR_SHARED_MEM_SIZE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_FLASH0	MAP_REGION_FLAT(FLASH0_BASE,			\
					FLASH0_SIZE,			\
					MT_MEMORY | MT_RO | MT_SECURE)

#define MAP_DRAM1_NS	MAP_REGION_FLAT(DRAM1_NS_BASE,			\
					DRAM1_NS_SIZE,			\
					MT_MEMORY | MT_RW | MT_NS)

#define MAP_BL32_MEM	MAP_REGION_FLAT(BL32_BASE,			\
					BL32_LIMIT - BL32_BASE,		\
					MT_MEMORY | MT_RW | MT_SECURE)

#define MAP_DEVICE_RCAR	MAP_REGION_FLAT(DEVICE_RCAR_BASE,		\
					DEVICE_RCAR_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

#define MAP_ATFW_LOG	MAP_REGION_FLAT(RCAR_BL31_LOG_BASE,		\
					RCAR_BL31_LOG_SIZE,		\
					MT_DEVICE | MT_RW | MT_SECURE)

/*
 * Table of regions for various BL stages to map using the MMU.
 * This doesn't include TZRAM as the 'mem_layout' argument passed to
 * configure_mmu_elx() will give the available subset of that,
 */
#if IMAGE_BL1
const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_FLASH0,
	MAP_DEVICE0,
	MAP_DEVICE1,
	{	0}
};
#endif
#if IMAGE_BL2
const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_FLASH0,
	MAP_DRAM1_NS,
	MAP_BL32_MEM,
	MAP_DEVICE_RCAR,
	{	0}
};
#endif
#if IMAGE_BL31
const mmap_region_t rcar_mmap[] = {
	MAP_SHARED_RAM,
	MAP_ATFW_LOG,
	MAP_DEVICE_RCAR,
	{	0}
};
#endif
#if IMAGE_BL32
const mmap_region_t rcar_mmap[] = {
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
DEFINE_CONFIGURE_MMU_EL(1)
DEFINE_CONFIGURE_MMU_EL(3)

#if (IMAGE_BL2)
extern int32_t file_to_cert(const int32_t filename, uint32_t *cert_addr);
extern void get_info_from_cert(uint64_t cert_addr, uint32_t *size, uint32_t *dest_addr);
#endif

unsigned long plat_get_ns_image_entrypoint(void)
{
#if (IMAGE_BL2)
	int32_t ret;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uint32_t dest_addr;
	ret = file_to_cert(BL33_CERT_ID, &cert_addr);
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

uint64_t plat_get_syscnt_freq(void)
{
	uint64_t counter_base_frequency;

	/* Read the frequency from Frequency modes table */
	counter_base_frequency = mmio_read_32(ARM_SYS_CNTCTL_BASE + (uint32_t)CNTFID_OFF);

	/* The first entry of the frequency modes table must not be 0 */
	if (counter_base_frequency == 0U) {
		panic();
	}

	return counter_base_frequency;
}

/* Map of CCI masters with the slave interfaces they are connected */
static int cci_map[] = {
	CCI500_CLUSTER0_SL_IFACE_IX,
	CCI500_CLUSTER1_SL_IFACE_IX
};

void rcar_cci_init(void)
{
	/*
	 * Initialize CCI-500 driver
	 */
	uint32_t product = mmio_read_32((uintptr_t)RCAR_PRR)
			& RCAR_PRODUCT_MASK;
	if (RCAR_PRODUCT_M3 == product) {
		cci_map[0U] = CCI500_CLUSTER0_SL_IFACE_IX_FOR_M3;
		cci_map[1U] = CCI500_CLUSTER1_SL_IFACE_IX_FOR_M3;
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
