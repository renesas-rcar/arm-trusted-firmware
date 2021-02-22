/*
 * Copyright (c) 2013-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <bl_common.h>
#include <el3_common_macros.S>
#include <pmf_asm_macros.S>
#include <runtime_instr.h>
#include <xlat_mmu_helpers.h>
#include "bl31_entrypoint.h"

	.globl	bl31_entrypoint
	.globl	bl31_warm_entrypoint
	.globl	secondly_boot

	/* -----------------------------------------------------
	 * bl31_entrypoint() is the cold boot entrypoint,
	 * executed only by the primary cpu.
	 * -----------------------------------------------------
	 */

func bl31_entrypoint
#if !RESET_TO_BL31

	ldr	x4, =0x4403E200
	ldr	x5, =0x0000000000300103
	str	x5, [x4]
	ldr	x4, =0x4403E208
	ldr	x5, =0x000000004403E230
	str	x5, [x4]
	ldr	x4, =0x4403E210
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E218
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E220
	ldr	x5, =0x000000004403E278
	str	x5, [x4]
	ldr	x4, =0x4403E228
	ldr	x5, =0x000000004403E260
	str	x5, [x4]
	ldr	x4, =0x4403E230
	ldr	x5, =0x0000000000180102
	str	x5, [x4]
	ldr	x4, =0x4403E238
	ldr	x5, =0x0000000044000000
	str	x5, [x4]
	ldr	x4, =0x4403E240
	ldr	x5, =0x0000000000010000
	str	x5, [x4]
	ldr	x4, =0x4403E248
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E250
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E258
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E260
	ldr	x5, =0x0000000000180102
	str	x5, [x4]
	ldr	x4, =0x4403E268
	ldr	x5, =0x0000000050000000
	str	x5, [x4]
	ldr	x4, =0x4403E270
	ldr	x5, =0x0000000000100000
	str	x5, [x4]
	ldr	x4, =0x4403E278
	ldr	x5, =0x0000000100580101
	str	x5, [x4]
	ldr	x4, =0x4403E280
	ldr	x5, =0x0000000050000000
	str	x5, [x4]
	ldr	x4, =0x4403E288
	ldr	x5, =0x00000000000003C5
	str	x5, [x4]
	ldr	x4, =0x4403E290
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E298
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2A0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2A8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2B0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2B8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2C0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2C8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2D0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2D8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2E0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2E8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2F0
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E2F8
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E300
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E308
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E310
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E318
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E320
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E328
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E330
	ldr	x5, =0x0000000044000000
	str	x5, [x4]
	ldr	x4, =0x4403E338
	ldr	x5, =0x00000000000003CD
	str	x5, [x4]
	ldr	x4, =0x4403E340
	ldr	x5, =0x000000004403E200
	str	x5, [x4]
	ldr	x4, =0x4403E348
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E350
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E358
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E360
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E368
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E370
	ldr	x5, =0x0000000000000000
	str	x5, [x4]
	ldr	x4, =0x4403E378
	ldr	x5, =0x0000000000000000
	str	x5, [x4]

	ldr	x0, =0x4403E200
	mov	x1, #0x0
	mov	x2, #0x0
	mov	x3, #0x0

secondly_boot:
	/* ---------------------------------------------------------------
	 * Stash the previous bootloader arguments x0 - x3 for later use.
	 * ---------------------------------------------------------------
	 */
	mov	x20, x0
	mov	x21, x1
	mov	x22, x2
	mov	x23, x3

	/* ---------------------------------------------------------------------
	 * For !RESET_TO_BL31 systems, only the primary CPU ever reaches
	 * bl31_entrypoint() during the cold boot flow, so the cold/warm boot
	 * and primary/secondary CPU logic should not be executed in this case.
	 *
	 * Also, assume that the previous bootloader has already initialised the
	 * SCTLR_EL3, including the endianness, and has initialised the memory.
	 * ---------------------------------------------------------------------
	 */
#if PLAT_rcar
	el3_entrypoint_common					\
		_init_sctlr=0					\
		_warm_boot_mailbox=1				\
		_secondary_cold_boot=1				\
		_init_memory=1					\
		_init_c_runtime=1				\
		_exception_vectors=runtime_exceptions
#else
	el3_entrypoint_common					\
		_init_sctlr=0					\
		_warm_boot_mailbox=0				\
		_secondary_cold_boot=0				\
		_init_memory=0					\
		_init_c_runtime=1				\
		_exception_vectors=runtime_exceptions
#endif
#else
	/* ---------------------------------------------------------------------
	 * For RESET_TO_BL31 systems which have a programmable reset address,
	 * bl31_entrypoint() is executed only on the cold boot path so we can
	 * skip the warm boot mailbox mechanism.
	 * ---------------------------------------------------------------------
	 */
	el3_entrypoint_common					\
		_init_sctlr=1					\
		_warm_boot_mailbox=!PROGRAMMABLE_RESET_ADDRESS	\
		_secondary_cold_boot=!COLD_BOOT_SINGLE_CPU	\
		_init_memory=1					\
		_init_c_runtime=1				\
		_exception_vectors=runtime_exceptions

	/* ---------------------------------------------------------------------
	 * For RESET_TO_BL31 systems, BL31 is the first bootloader to run so
	 * there's no argument to relay from a previous bootloader. Zero the
	 * arguments passed to the platform layer to reflect that.
	 * ---------------------------------------------------------------------
	 */
	mov	x20, 0
	mov	x21, 0
	mov	x22, 0
	mov	x23, 0
#endif /* RESET_TO_BL31 */
	/* ---------------------------------------------
	 * Perform platform specific early arch. setup
	 * ---------------------------------------------
	 */
	mov	x0, x20
	mov	x1, x21
	mov	x2, x22
	mov	x3, x23
	bl	bl31_early_platform_setup2
	bl	bl31_plat_arch_setup

	/* ---------------------------------------------
	 * Jump to main function.
	 * ---------------------------------------------
	 */
	bl	bl31_main

	/* -------------------------------------------------------------
	 * Clean the .data & .bss sections to main memory. This ensures
	 * that any global data which was initialised by the primary CPU
	 * is visible to secondary CPUs before they enable their data
	 * caches and participate in coherency.
	 * -------------------------------------------------------------
	 */
	adr	x0, __DATA_START__
	adr	x1, __DATA_END__
	sub	x1, x1, x0
	bl	clean_dcache_range

	adr	x0, __BSS_START__
	adr	x1, __BSS_END__
	sub	x1, x1, x0
	bl	clean_dcache_range

	b	el3_exit
endfunc bl31_entrypoint

	/* --------------------------------------------------------------------
	 * This CPU has been physically powered up. It is either resuming from
	 * suspend or has simply been turned on. In both cases, call the BL31
	 * warmboot entrypoint
	 * --------------------------------------------------------------------
	 */
func bl31_warm_entrypoint
#if ENABLE_RUNTIME_INSTRUMENTATION

	/*
	 * This timestamp update happens with cache off.  The next
	 * timestamp collection will need to do cache maintenance prior
	 * to timestamp update.
	 */
	pmf_calc_timestamp_addr rt_instr_svc RT_INSTR_EXIT_HW_LOW_PWR
	mrs	x1, cntpct_el0
	str	x1, [x0]
#endif

	/*
	 * On the warm boot path, most of the EL3 initialisations performed by
	 * 'el3_entrypoint_common' must be skipped:
	 *
	 *  - Only when the platform bypasses the BL1/BL31 entrypoint by
	 *    programming the reset address do we need to initialise SCTLR_EL3.
	 *    In other cases, we assume this has been taken care by the
	 *    entrypoint code.
	 *
	 *  - No need to determine the type of boot, we know it is a warm boot.
	 *
	 *  - Do not try to distinguish between primary and secondary CPUs, this
	 *    notion only exists for a cold boot.
	 *
	 *  - No need to initialise the memory or the C runtime environment,
	 *    it has been done once and for all on the cold boot path.
	 */
	el3_entrypoint_common					\
		_init_sctlr=PROGRAMMABLE_RESET_ADDRESS		\
		_warm_boot_mailbox=0				\
		_secondary_cold_boot=0				\
		_init_memory=0					\
		_init_c_runtime=0				\
		_exception_vectors=runtime_exceptions

	/*
	 * We're about to enable MMU and participate in PSCI state coordination.
	 *
	 * The PSCI implementation invokes platform routines that enable CPUs to
	 * participate in coherency. On a system where CPUs are not
	 * cache-coherent without appropriate platform specific programming,
	 * having caches enabled until such time might lead to coherency issues
	 * (resulting from stale data getting speculatively fetched, among
	 * others). Therefore we keep data caches disabled even after enabling
	 * the MMU for such platforms.
	 *
	 * On systems with hardware-assisted coherency, or on single cluster
	 * platforms, such platform specific programming is not required to
	 * enter coherency (as CPUs already are); and there's no reason to have
	 * caches disabled either.
	 */
	mov	x0, #DISABLE_DCACHE
	bl	bl31_plat_enable_mmu

#if HW_ASSISTED_COHERENCY || WARMBOOT_ENABLE_DCACHE_EARLY
	mrs	x0, sctlr_el3
	orr	x0, x0, #SCTLR_C_BIT
	msr	sctlr_el3, x0
	isb
#endif

	bl	psci_warmboot_entrypoint

#if ENABLE_RUNTIME_INSTRUMENTATION
	pmf_calc_timestamp_addr rt_instr_svc RT_INSTR_EXIT_PSCI
	mov	x19, x0

	/*
	 * Invalidate before updating timestamp to ensure previous timestamp
	 * updates on the same cache line with caches disabled are properly
	 * seen by the same core. Without the cache invalidate, the core might
	 * write into a stale cache line.
	 */
	mov	x1, #PMF_TS_SIZE
	mov	x20, x30
	bl	inv_dcache_range
	mov	x30, x20

	mrs	x0, cntpct_el0
	str	x0, [x19]
#endif
	b	el3_exit
endfunc bl31_warm_entrypoint
