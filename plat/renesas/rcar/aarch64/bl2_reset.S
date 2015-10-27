/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
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
#include <asm_macros.S>
#include <bl_common.h>
#include <runtime_svc.h>
#include "platform_def.h"


#define	RST_BASE		(0xE6160000)
#define	RST_CA53BAR2		(0x0034)
#define	RST_CA57BAR2		(0x0024)
#define	RST_CA53CPU0BARH	(0x0080)
#define	RST_CA53CPU0BARL	(0x0084)
#define	RST_CA53CPU1BARH	(0x0090)
#define	RST_CA53CPU1BARL	(0x0094)
#define	RST_CA53CPU2BARH	(0x00A0)
#define	RST_CA53CPU2BARL	(0x00A4)
#define	RST_CA53CPU3BARH	(0x00B0)
#define	RST_CA53CPU3BARL	(0x00B4)
#define	RST_CA57CPU0BARH	(0x00C0)
#define	RST_CA57CPU0BARL	(0x00C4)
#define	RST_CA57CPU1BARH	(0x00D0)
#define	RST_CA57CPU1BARL	(0x00D4)
#define	RST_CA57CPU2BARH	(0x00E0)
#define	RST_CA57CPU2BARL	(0x00E4)
#define	RST_CA57CPU3BARH	(0x00F0)
#define	RST_CA57CPU3BARL	(0x00F4)

#define	RST_BAR2_MASK		(0xFFFFFC00)
#define	RST_BAR2_BAREN		(0x00000010)
#define	RST_BAR2_VLD		(0x00000001)
#define	RST_BAR2_SHIFT		(18-10)
#define RST_CPUnBARH_SHIFT	(32)
#define	RST_CPUnBARH_MASK	(0x000000FF)
#define	RST_CPUnBARL_MASK	(0xFFFFFFFC)


	.global	bl2_reset
	.global	bl2_secondly_reset
	.global	bl2_init_secondly_reset

func bl2_reset
    mov x0,  #0
    mov x1,  #0
    mov x2,  #0
    mov x3,  #0
    mov x4,  #0
    mov x5,  #0
    mov x6,  #0
    mov x7,  #0
    mov x8,  #0
    mov x9,  #0
    mov x10, #0
    mov x11, #0
    mov x12, #0
    mov x13, #0
    mov x14, #0
    mov x15, #0
    mov x16, #0
    mov x17, #0
    mov x18, #0
    mov x19, #0
    mov x20, #0
    mov x21, #0
    mov x22, #0
    mov x23, #0
    mov x24, #0
    mov x25, #0
    mov x26, #0
    mov x27, #0
    mov x28, #0
    mov x29, #0
    mov x30, #0

	/* ---------------------------------------------
	 * Set the CPU endianness before doing anything
	 * that might involve memory reads or writes.
	 * ---------------------------------------------
	 */
	mrs	x0, sctlr_el3
	bic	x0, x0, #SCTLR_EE_BIT
	msr	sctlr_el3, x0
	isb

	/* ---------------------------------------------
	 * Set the exception vector to something sane.
	 * ---------------------------------------------
	 */
	adr	x0, bl2_exceptions
	msr	vbar_el3, x0
	isb

	/* ---------------------------------------------------------------------
	 * The initial state of the Architectural feature trap register
	 * (CPTR_EL3) is unknown and it must be set to a known state. All
	 * feature traps are disabled. Some bits in this register are marked as
	 * Reserved and should not be modified.
	 *
	 * CPTR_EL3.TCPAC: This causes a direct access to the CPACR_EL1 from EL1
	 *  or the CPTR_EL2 from EL2 to trap to EL3 unless it is trapped at EL2.
	 * CPTR_EL3.TTA: This causes access to the Trace functionality to trap
	 *  to EL3 when executed from EL0, EL1, EL2, or EL3. If system register
	 *  access to trace functionality is not supported, this bit is RES0.
	 * CPTR_EL3.TFP: This causes instructions that access the registers
	 *  associated with Floating Point and Advanced SIMD execution to trap
	 *  to EL3 when executed from any exception level, unless trapped to EL1
	 *  or EL2.
	 * ---------------------------------------------------------------------
	 */
	mrs	x0, cptr_el3
	bic	w0, w0, #TCPAC_BIT
	bic	w0, w0, #TTA_BIT
	bic	w0, w0, #TFP_BIT
	msr	cptr_el3, x0

	/* --------------------------------------------
	 * Allocate a stack whose memory will be marked
	 * as Normal-IS-WBWA when the MMU is enabled.
	 * There is no risk of reading stale stack
	 * memory after enabling the MMU as only the
	 * primary cpu is running at the moment.
	 * --------------------------------------------
	 */
	mrs	x0, mpidr_el1
	bl	platform_set_stack

	mov	x0, #(SCR_RES1_BITS | SCR_RW_BIT)
	msr	scr_el3, x0

	bl	bl2_init_secondly_reset

	ldr	x0, =bl2_entrypoint
	msr	elr_el3, x0
	mov	x1, #(DISABLE_ALL_EXCEPTIONS << SPSR_DAIF_SHIFT) | (MODE_RW_64 << MODE_RW_SHIFT) | (MODE_EL1 << MODE_EL_SHIFT) | (MODE_SP_ELX << MODE_SP_SHIFT)
	msr	spsr_el3, x1

	adr	x1, BL2_MEM_INFO
	mov	x0, #RUN_IMAGE

	eret
endfunc bl2_reset


	.align	3
BL2_MEM_INFO:					/* meminfo_t */
	.quad	BL31_BASE			/* total_base */
	.quad	(BL31_LIMIT - BL31_BASE)	/* total_size */
	.quad	BL31_BASE			/* free_base */
	.quad	(BL31_LIMIT - BL31_BASE)	/* free_size */


	/* ---------------------------------------------
	 * initialize secondly boot
	 * ---------------------------------------------
	 */
func bl2_init_secondly_reset
	adr	x0, bl2_secondly_reset
	ldr	x1, =RCAR_SECRAM_BASE
	mov	x2, #0				/* Number of copied bytes */
	mov	x4, #(BL2_SECONDLY_RESET - bl2_secondly_reset)
1:
	ldr	w3, [x0, x2]
	str	w3, [x1, x2]			/* copy secondly boot code */
	add	x2, x2, #4
	cmp	x4, x2
	b.gt	1b

	/* initialize Boot Address Registers */
	ldr	x0, =RST_BASE

	ldr	w1, =(RCAR_SECRAM_BASE & RST_CPUnBARL_MASK)
	str	w1, [x0, #RST_CA53CPU0BARL]
	str	wzr, [x0, #RST_CA53CPU0BARH]
	str	w1, [x0, #RST_CA53CPU1BARL]
	str	wzr, [x0, #RST_CA53CPU1BARH]
	str	w1, [x0, #RST_CA53CPU2BARL]
	str	wzr, [x0, #RST_CA53CPU2BARH]
	str	w1, [x0, #RST_CA53CPU3BARL]
	str	wzr, [x0, #RST_CA53CPU3BARH]
	str	w1, [x0, #RST_CA57CPU0BARL]
	str	wzr, [x0, #RST_CA57CPU0BARH]
	str	w1, [x0, #RST_CA57CPU1BARL]
	str	wzr, [x0, #RST_CA57CPU1BARH]
	str	w1, [x0, #RST_CA57CPU2BARL]
	str	wzr, [x0, #RST_CA57CPU2BARH]
	str	w1, [x0, #RST_CA57CPU3BARL]
	str	wzr, [x0, #RST_CA57CPU3BARH]

	ret
endfunc bl2_init_secondly_reset


	/* ---------------------------------------------
	 * secondly boot
	 * ---------------------------------------------
	 */
func bl2_secondly_reset
	mrs	x0, sctlr_el3
	bic	x0, x0, #SCTLR_EE_BIT
	msr	sctlr_el3, x0
	isb

	mrs	x0, cptr_el3
	bic	w0, w0, #TCPAC_BIT
	bic	w0, w0, #TTA_BIT
	bic	w0, w0, #TFP_BIT
	msr	cptr_el3, x0

	mov_imm	x0, PARAMS_BASE			/* pointer to bl31_ep_info */
	mov_imm	x2, BL31_BASE
	mov x1, #0x00000001			/* warm boot */
	br	x2				/* jump to BL31 */
	nop
	nop
	nop
BL2_SECONDLY_RESET:
endfunc bl2_secondly_reset



	.section	.vectors, "ax"; .align 11

	/* -----------------------------------------------------
	 * Very simple stackless exception handlers used by BL1.
	 * -----------------------------------------------------
	 */
	.align	7
bl2_exceptions:
	/* -----------------------------------------------------
	 * Current EL with SP0 : 0x0 - 0x200
	 * -----------------------------------------------------
	 */
SynchronousExceptionSP0:
	mov	x0, #SYNC_EXCEPTION_SP_EL0
	bl	plat_report_exception
	b	SynchronousExceptionSP0
	check_vector_size SynchronousExceptionSP0

	.align	7
IrqSP0:
	mov	x0, #IRQ_SP_EL0
	bl	plat_report_exception
	b	IrqSP0
	check_vector_size IrqSP0

	.align	7
FiqSP0:
	mov	x0, #FIQ_SP_EL0
	bl	plat_report_exception
	b	FiqSP0
	check_vector_size FiqSP0

	.align	7
SErrorSP0:
	mov	x0, #SERROR_SP_EL0
	bl	plat_report_exception
	b	SErrorSP0
	check_vector_size SErrorSP0

	/* -----------------------------------------------------
	 * Current EL with SPx: 0x200 - 0x400
	 * -----------------------------------------------------
	 */
	.align	7
SynchronousExceptionSPx:
	mov	x0, #SYNC_EXCEPTION_SP_ELX
	bl	plat_report_exception
	b	SynchronousExceptionSPx
	check_vector_size SynchronousExceptionSPx

	.align	7
IrqSPx:
	mov	x0, #IRQ_SP_ELX
	bl	plat_report_exception
	b	IrqSPx
	check_vector_size IrqSPx

	.align	7
FiqSPx:
	mov	x0, #FIQ_SP_ELX
	bl	plat_report_exception
	b	FiqSPx
	check_vector_size FiqSPx

	.align	7
SErrorSPx:
	mov	x0, #SERROR_SP_ELX
	bl	plat_report_exception
	b	SErrorSPx
	check_vector_size SErrorSPx

	/* -----------------------------------------------------
	 * Lower EL using AArch64 : 0x400 - 0x600
	 * -----------------------------------------------------
	 */
	.align	7
SynchronousExceptionA64:
	/* Enable the SError interrupt */
	msr	daifclr, #DAIF_ABT_BIT

	/* ------------------------------------------------
	 * Only a single SMC exception from BL2 to ask
	 * BL1 to pass EL3 control to BL31 is expected
	 * here.
	 * It expects X0 with RUN_IMAGE SMC function id
	 * X1 with address of a entry_point_info_t structure
	 * describing the BL3-1 entrypoint
	 * ------------------------------------------------
	 */
	mov	x19, x0
	mov	x20, x1

	mrs	x0, esr_el3
	ubfx	x1, x0, #ESR_EC_SHIFT, #ESR_EC_LENGTH
	cmp	x1, #EC_AARCH64_SMC
	b.ne	panic

	mov	x0, #RUN_IMAGE
	cmp	x19, x0
	b.ne	panic

	mov	x0, x20
	ldp	x0, x1, [x20, #ENTRY_POINT_INFO_PC_OFFSET]
	msr	elr_el3, x0
	msr	spsr_el3, x1
	ubfx	x0, x1, #MODE_EL_SHIFT, #2
	cmp	x0, #MODE_EL3
	b.ne	panic

	ldp	x6, x7, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x30)]
	ldp	x4, x5, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x20)]
	ldp	x2, x3, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x10)]
	ldp	x0, x1, [x20, #(ENTRY_POINT_INFO_ARGS_OFFSET + 0x0)]
	eret

panic:
	mov	x0, #SYNC_EXCEPTION_AARCH64
	bl	plat_report_exception

	wfi
	b	panic
	check_vector_size SynchronousExceptionA64


	.align	7
IrqA64:
	mov	x0, #IRQ_AARCH64
	bl	plat_report_exception
	b	IrqA64
	check_vector_size IrqA64

	.align	7
FiqA64:
	mov	x0, #FIQ_AARCH64
	bl	plat_report_exception
	b	FiqA64
	check_vector_size FiqA64

	.align	7
SErrorA64:
	mov	x0, #SERROR_AARCH64
	bl	plat_report_exception
	b   	SErrorA64
	check_vector_size SErrorA64

	/* -----------------------------------------------------
	 * Lower EL using AArch32 : 0x600 - 0x800
	 * -----------------------------------------------------
	 */
	.align	7
SynchronousExceptionA32:
	mov	x0, #SYNC_EXCEPTION_AARCH32
	bl	plat_report_exception
	b	SynchronousExceptionA32
	check_vector_size SynchronousExceptionA32

	.align	7
IrqA32:
	mov	x0, #IRQ_AARCH32
	bl	plat_report_exception
	b	IrqA32
	check_vector_size IrqA32

	.align	7
FiqA32:
	mov	x0, #FIQ_AARCH32
	bl	plat_report_exception
	b	FiqA32
	check_vector_size FiqA32

	.align	7
SErrorA32:
	mov	x0, #SERROR_AARCH32
	bl	plat_report_exception
	b	SErrorA32
	check_vector_size SErrorA32
