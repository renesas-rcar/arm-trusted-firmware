/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include "rcar_def.h"

.global rcar_bl31_asm_switch_stack_pointer
.global	rcar_bl31_save_generic_timer
.global	rcar_bl31_restore_generic_timer

#define	OFFSET_SP_X9_X10	(0x00)
#define	OFFSET_CNTFID0		(0x10)
#define	OFFSET_CNTPCT_EL0	(0x18)
#define	OFFSET_TIMER_COUNT	(0x20)

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

	/* stop the generic timer */
	mov_imm	x10, (RCAR_CNTC_BASE + CNTCR_OFF)
	mov	x9, #0
	str	w9, [x10]

	/* save CNTFID0 and cntpct_el0 */
	mov_imm	x10, (RCAR_CNTC_BASE + CNTFID_OFF)
	ldr	x9, [x10]
	mrs	x10, cntpct_el0
	stp	x9, x10, [x0, #OFFSET_CNTFID0]

	ldp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	ret
endfunc rcar_bl31_save_generic_timer

/* X0 : Stack pointer base addrtess */
func rcar_bl31_restore_generic_timer

	stp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	/* restore CNTFID0 and cntpct_el0 */
	ldr	x10, [x0, #OFFSET_CNTFID0]
	mov_imm	x9, (RCAR_CNTC_BASE + CNTFID_OFF)
	str	x10, [x9]
	ldp	x9, x10, [x0, #OFFSET_CNTPCT_EL0]
	add	x9, x9, x10
	str	x9, [x0, #OFFSET_TIMER_COUNT]

	ldp	x9, x10, [x0, #OFFSET_SP_X9_X10]

	ret
endfunc rcar_bl31_restore_generic_timer
