/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>

.global rcar_bl31_asm_switch_stack_pointer


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
