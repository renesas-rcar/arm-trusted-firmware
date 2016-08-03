/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
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
#include "rcar_def.h"

.global rcar_bl31_asm_switch_stack_pointer
.global	rcar_bl31_save_generic_timer
.global	rcar_bl31_restore_generic_timer

#define	OFFSET_SP_X9_X10	(0x00)
#define	OFFSET_CNTP_TVAL_EL0	(0x10)
#define	OFFSET_CNTKCTL_EL1	(0x20)
#define	OFFSET_CNTHP_TVAL_EL2	(0x30)
#define	OFFSET_CNTHCTL_EL2	(0x40)

/* X0 : Jump address */
/* X1 : Stack address */
/* X2 : arg */
/* X3 : Stack address temporary */
func rcar_bl31_asm_switch_stack_pointer

	/* lr to stack */
	stp	x29, x30, [sp,#-16]

	/* change stack pointer */
	mov	x3, sp
	mov	sp, x1

	/* save stack pointer */
	sub	sp, sp, #16
	stp	x0, x3, [sp]

	/* data synchronization barrier */
	dsb	sy

	/* jump to code */
	mov	x1, x0
	mov	x0, x2
	blr	x1

	/* load stack pointer */
	ldp 	x0, x2, [sp,#0]

	/* change stack pointer */
	mov	sp, x2

	/* return */
	ldp	x29, x30, [sp,#-16]
	ret
endfunc rcar_bl31_asm_switch_stack_pointer

/* X0 : Stack pointer base addrtess */
func rcar_bl31_save_generic_timer

	stp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	/* save cntp_tval_el0 and cntv_tval_el0 */
	mrs	x9, cntp_tval_el0
	mrs	x10, cntv_tval_el0
	stp	x9, x10, [x0, #OFFSET_CNTP_TVAL_EL0]

	/* save cntvct_el0 and cntv_cval_el0 */
	mrs	x9, cntkctl_el1
	mrs	x10, cntv_cval_el0
	stp	x9, x10, [x0, #OFFSET_CNTKCTL_EL1]

	/* save cnthp_tval_el2 and cntps_tval_el1 */
	mrs	x9, cnthp_tval_el2
	mrs	x10, cntps_tval_el1
	stp	x9, x10, [x0, #OFFSET_CNTHP_TVAL_EL2]

	/* save cnthctl_el2 and CNTFID0 */
	mov_imm	x9, (RCAR_CNTC_BASE_A + CNTFID_OFF)
	ldr	x10, [x9]
	mrs	x9, cnthctl_el2
	stp	x9, x10, [x0, #OFFSET_CNTHCTL_EL2]

	ldp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	ret
endfunc rcar_bl31_save_generic_timer

/* X0 : Stack pointer base addrtess */
func rcar_bl31_restore_generic_timer

	stp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	/* restore cnthctl_el2 and CNTFID0 */
	ldp	x9, x10, [x0, #OFFSET_CNTHCTL_EL2]
	msr	cnthctl_el2, x9
	mov_imm	x9, (RCAR_CNTC_BASE_A + CNTFID_OFF)
	str	x10, [x9]

	/* restore cnthp_tval_el2 and cntps_tval_el1 */
	ldp	x9, x10, [x0, #OFFSET_CNTHP_TVAL_EL2]
	msr	cnthp_tval_el2, x9
	msr	cntps_tval_el1, x10

	/* restore cntkctl_el1 and cntv_cval_el0 */
	ldp	x9, x10, [x0, #OFFSET_CNTKCTL_EL1]
	msr	cntkctl_el1, x9
	msr	cntv_cval_el0, x10

	/* restore cntp_tval_el0 and cntv_tval_el0 */
	ldp	x9, x10, [x0, #OFFSET_CNTP_TVAL_EL0]
	msr	cntp_tval_el0, x9
	msr	cntv_tval_el0, x10

	ldp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	ret
endfunc rcar_bl31_restore_generic_timer
