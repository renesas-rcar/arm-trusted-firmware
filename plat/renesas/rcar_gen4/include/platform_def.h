/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H

#ifndef __ASSEMBLER__
#include <stdlib.h>
#endif

#include <arch.h>

#include "rcar_def.h"

/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT          "elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH            aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Size of cacheable stacks */
#define PLATFORM_STACK_SIZE	U(0x400)

/*
 * R-Car S4 Cortex-A55
 * L1:I/32KB per core, D/32KB per core, L2:0B L3:256KB per cluster
 */
#define PLATFORM_CLUSTER_COUNT		U(4)
#define PLATFORM_CLUSTER0_CORE_COUNT	U(2)
#define PLATFORM_CLUSTER1_CORE_COUNT	U(2)
#define PLATFORM_CLUSTER2_CORE_COUNT	U(2)
#define PLATFORM_CLUSTER3_CORE_COUNT	U(2)
#define PLATFORM_CORE_COUNT		(PLATFORM_CLUSTER3_CORE_COUNT + \
					 PLATFORM_CLUSTER2_CORE_COUNT + \
					 PLATFORM_CLUSTER1_CORE_COUNT + \
					 PLATFORM_CLUSTER0_CORE_COUNT)
#define PLATFORM_MAX_CPUS_PER_CLUSTER	U(2)

#define PLAT_MAX_PWR_LVL		MPIDR_AFFLVL2
#define PLAT_NUM_PWR_DOMAINS		(PLATFORM_CORE_COUNT + \
					 PLATFORM_CLUSTER_COUNT + 1)

#define PLAT_MAX_RET_STATE		U(1)
#define PLAT_MAX_OFF_STATE		U(2)


/*
 ******************************************************************************
 * BL31 specific defines.
 ******************************************************************************
 * Put BL3-1 at the top of the Trusted SRAM. BL31_BASE is calculated using the
 * current BL3-1 debug size plus a little space for growth.
 */
#define BL31_BASE		(RCAR_TRUSTED_SRAM_BASE)
#define BL31_LIMIT		(RCAR_TRUSTED_SRAM_BASE + \
				 RCAR_TRUSTED_SRAM_SIZE)


/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define MAX_XLAT_TABLES		U(8)

#define PLAT_PHY_ADDR_SPACE_SIZE	(ULL(1) << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE	(ULL(1) << 32)

#define MAX_MMAP_REGIONS	(RCAR_MMAP_ENTRIES + RCAR_BL_REGIONS)

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two mailboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT   (6)
#define CACHE_WRITEBACK_GRANULE (1 << CACHE_WRITEBACK_SHIFT)

/*******************************************************************************
 * Size of the per-cpu data in bytes that should be reserved in the generic
 * per-cpu data structure for the RCAR port.
 ******************************************************************************/
#if !USE_COHERENT_MEM
#define PLAT_PCPU_DATA_SIZE	(4)
#endif

#endif /* PLATFORM_DEF_H */
