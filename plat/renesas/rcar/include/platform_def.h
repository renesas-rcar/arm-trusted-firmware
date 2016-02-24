/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
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

#ifndef RCAR_PLATFORM_DEF_H__
#define RCAR_PLATFORM_DEF_H__

#include <arch.h>
#ifndef __ASSEMBLY__
#include <stdlib.h>
#endif
#include "../rcar_def.h"


/*******************************************************************************
 * Platform binary types for linking
 ******************************************************************************/
#define PLATFORM_LINKER_FORMAT          "elf64-littleaarch64"
#define PLATFORM_LINKER_ARCH            aarch64

/*******************************************************************************
 * Generic platform constants
 ******************************************************************************/

/* Align the storing version code for BL2 and BL31 */
#define	ALIGN_VERSION		(0x400)

/* Size of cacheable stacks */
#if IMAGE_BL1
#if TRUSTED_BOARD_BOOT
#define PLATFORM_STACK_SIZE 0x1000
#else
#define PLATFORM_STACK_SIZE 0x440
#endif
#elif IMAGE_BL2
#if TRUSTED_BOARD_BOOT
#define PLATFORM_STACK_SIZE 0x1000
#else
#define PLATFORM_STACK_SIZE 0x400
#endif
#elif IMAGE_BL31
#define PLATFORM_STACK_SIZE 0x400
#elif IMAGE_BL32
#define PLATFORM_STACK_SIZE 0x440
#endif

#define FIRMWARE_WELCOME_STR		"Booting Trusted Firmware\n"

#define BL332_IMAGE_ID			(BL33_CERT_ID + 1U)
#define BL333_IMAGE_ID			(BL33_CERT_ID + 2U)
#define BL334_IMAGE_ID			(BL33_CERT_ID + 3U)
#define BL335_IMAGE_ID			(BL33_CERT_ID + 4U)
#define BL336_IMAGE_ID			(BL33_CERT_ID + 5U)
#define BL337_IMAGE_ID			(BL33_CERT_ID + 6U)
#define BL338_IMAGE_ID			(BL33_CERT_ID + 7U)

#define BL332_KEY_CERT_ID		(BL33_CERT_ID + 8U)
#define BL333_KEY_CERT_ID		(BL33_CERT_ID + 9U)
#define BL334_KEY_CERT_ID		(BL33_CERT_ID + 10U)
#define BL335_KEY_CERT_ID		(BL33_CERT_ID + 11U)
#define BL336_KEY_CERT_ID		(BL33_CERT_ID + 12U)
#define BL337_KEY_CERT_ID		(BL33_CERT_ID + 13U)
#define BL338_KEY_CERT_ID		(BL33_CERT_ID + 14U)

#define BL332_CERT_ID			(BL33_CERT_ID + 15U)
#define BL333_CERT_ID			(BL33_CERT_ID + 16U)
#define BL334_CERT_ID			(BL33_CERT_ID + 17U)
#define BL335_CERT_ID			(BL33_CERT_ID + 18U)
#define BL336_CERT_ID			(BL33_CERT_ID + 19U)
#define BL337_CERT_ID			(BL33_CERT_ID + 20U)
#define BL338_CERT_ID			(BL33_CERT_ID + 21U)

/*
 * R-Car H3 Cortex-A57
 * L1:I/48KB(16KBx3way) D/32KB(16KBx2way) L2:2MB(128KBx16way)
 *          Cortex-A53
 * L1:I/32KB(16KBx2way) D/32KB(8KBx4way) L2:512KB(32KBx16way)
 */
#define PLATFORM_CACHE_LINE_SIZE	128
#define PLATFORM_CLUSTER_COUNT		2ull
#define PLATFORM_CLUSTER0_CORE_COUNT	4
#define PLATFORM_CLUSTER1_CORE_COUNT	4
#define PLATFORM_CORE_COUNT		(PLATFORM_CLUSTER1_CORE_COUNT + \
						PLATFORM_CLUSTER0_CORE_COUNT)
#define PLATFORM_MAX_CPUS_PER_CLUSTER	4
#define PLATFORM_NUM_AFFS		(PLATFORM_CLUSTER_COUNT + \
					 PLATFORM_CORE_COUNT)
#define PLATFORM_MAX_AFFLVL		MPIDR_AFFLVL1
#define MAX_IO_DEVICES			3
#define MAX_IO_HANDLES			4

/*******************************************************************************
 * BL1 specific defines.
 * BL1 RW data is relocated from ROM to RAM at runtime so we need 2 sets of
 * addresses.
 ******************************************************************************/
#define BL1_RO_BASE			RCAR_TRUSTED_ROM_BASE
#define BL1_RO_LIMIT			(RCAR_TRUSTED_ROM_BASE \
					+ RCAR_TRUSTED_ROM_SIZE)
/*
 * Put BL1 RW at the top of the Trusted SRAM. BL1_RW_BASE is calculated using
 * the current BL1 RW debug size plus a little space for growth.
 */
#if TRUSTED_BOARD_BOOT
#define BL1_RW_BASE			(RCAR_TRUSTED_SRAM_BASE \
					+ RCAR_TRUSTED_SRAM_SIZE - 0x8000)
#else
#define BL1_RW_BASE			(RCAR_TRUSTED_SRAM_BASE \
					+ RCAR_TRUSTED_SRAM_SIZE - 0x6000)
#endif
#define BL1_RW_LIMIT			(RCAR_TRUSTED_SRAM_BASE \
					+ RCAR_TRUSTED_SRAM_SIZE)

/*******************************************************************************
 * BL2 specific defines.
 ******************************************************************************/
/*
 * Put BL2 just below BL3-1. BL2_BASE is calculated using the current BL2 debug
 * size plus a little space for growth.
 */
#define	RCAR_SECRAM_BASE		(0xE6300000)
#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
#define BL2_BASE			(0xE6302000)
#define BL2_LIMIT			(0xE632E800)
#elif RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CR7
#define BL2_BASE			(0x43F20000)
#define BL2_LIMIT			(0x43F4C800)
#endif

/*******************************************************************************
 * BL31 specific defines.
 ******************************************************************************/
/*
 * Put BL3-1 at the top of the Trusted SRAM. BL31_BASE is calculated using the
 * current BL3-1 debug size plus a little space for growth.
 */
#define BL31_BASE			(RCAR_TRUSTED_SRAM_BASE)
#define BL31_LIMIT			(RCAR_TRUSTED_SRAM_BASE \
					+ RCAR_TRUSTED_SRAM_SIZE)
#define	RCAR_BL31_LOG_BASE		(0x44040000)
#define	RCAR_BL31_SDRAM_BTM		(RCAR_BL31_LOG_BASE + 0x14000)
#define	RCAR_BL31_LOG_SIZE		(RCAR_BL31_SDRAM_BTM \
					- RCAR_BL31_LOG_BASE)

/*******************************************************************************
 * BL32 specific defines.
 ******************************************************************************/

# define BL32_BASE			(0x44100000)
# define BL32_LIMIT			(BL32_BASE + 0x100000)
/*
 * ID of the secure physical generic timer interrupt used by the TSP.
 */


/*******************************************************************************
 * Platform specific page table and MMU setup constants
 ******************************************************************************/
#define ADDR_SPACE_SIZE			(1ull << 32)

#if IMAGE_BL1
# define MAX_XLAT_TABLES		2
#elif IMAGE_BL2
# define MAX_XLAT_TABLES		3
#elif IMAGE_BL31
# define MAX_XLAT_TABLES		4
#elif IMAGE_BL32
#  define MAX_XLAT_TABLES		3
#endif

#define MAX_MMAP_REGIONS		(RCAR_MMAP_ENTRIES + RCAR_BL_REGIONS)

/*******************************************************************************
 * Declarations and constants to access the mailboxes safely. Each mailbox is
 * aligned on the biggest cache line size in the platform. This is known only
 * to the platform as it might have a combination of integrated and external
 * caches. Such alignment ensures that two maiboxes do not sit on the same cache
 * line at any cache level. They could belong to different cpus/clusters &
 * get written while being protected by different locks causing corruption of
 * a valid mailbox address.
 ******************************************************************************/
#define CACHE_WRITEBACK_SHIFT   6
#define CACHE_WRITEBACK_GRANULE (1 << CACHE_WRITEBACK_SHIFT)

#if !USE_COHERENT_MEM
/*******************************************************************************
 * Size of the per-cpu data in bytes that should be reserved in the generic
 * per-cpu data structure for the RCAR port.
 ******************************************************************************/
#define PLAT_PCPU_DATA_SIZE	2
#endif

#endif /* RCAR_PLATFORM_DEF_H__ */
