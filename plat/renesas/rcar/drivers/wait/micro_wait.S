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
#include "micro_wait.h"


#define	CPG_BASE		(0xE6150000)
#define CPG_SMSTPCR1		(0x0134)
#define CPG_CPGWPR		(0x0900)

#define MSTPCR1_TMU1		(1 << 24)

#define TMU3_BASE		(0xE6FC0000)
#define TMU_TSTR		(0x0004)
#define TMU_TCOR		(0x0008)
#define TMU_TCNT		(0x000C)
#define TMU_TCR			(0x0010)

#define TSTR1_TMU3		(1 << 0)

#define MIDR_CA57		(0x0D07 << MIDR_PN_SHIFT)
#define MIDR_CA53		(0x0D03 << MIDR_PN_SHIFT)


	.globl	micro_wait
#if (TMU3_MEASUREMENT == 1)
	.globl	init_TMU3
	.globl	start_TMU3
	.globl	snapshot_TCNT3
	.globl	stop_TMU3
#endif /* TMU3_MEASUREMENT */

	.align	6	/* Aligned with the cache line */

func micro_wait
	cbz	x0, micro_wait_e
	mrs	x1, midr_el1
	and	x1, x1, #MIDR_PN_MASK << MIDR_PN_SHIFT
	mov	w2, #MIDR_CA53
	cmp	w1, w2
	b.eq	micro_wait_ca53
	b	micro_wait_ca57
micro_wait_e:
	ret
endfunc micro_wait


func micro_wait_ca57	/* Cortex-A57 1.5GHz */
ca57_loop_1:
	mov	x1, #185
ca57_loop_2:
	subs	x1, x1, #1
	b.ne	ca57_loop_2
	subs	x0, x0, #1
	b.ne	ca57_loop_1
	ret
endfunc micro_wait_ca57


func micro_wait_ca53	/* Cortex-A53 1.2GHz */
ca53_loop_1:
	mov	x1, #134
ca53_loop_2:
	subs	x1, x1, #1
	b.ne	ca53_loop_2
	subs	x0, x0, #1
	b.ne	ca53_loop_1
	ret
endfunc micro_wait_ca53


/* Time measurement function using the TMU3. */
#if (TMU3_MEASUREMENT == 1)
func init_TMU3
	ldr	x0, =CPG_BASE
	ldr	w1, [x0, #CPG_SMSTPCR1]
	and	w1, w1, #~MSTPCR1_TMU1
	mvn	w2, w1
	str	w2, [x0, #CPG_CPGWPR]
	str	w1, [x0, #CPG_SMSTPCR1]
	dsb	sy
	ret
endfunc init_TMU3


func start_TMU3
	ldr	x0, =TMU3_BASE
	mov	w1, #0xFFFFFFFF
	str	w1, [x0, TMU_TCNT]

	ldr	x0, =TMU3_BASE
	ldrb	w1, [x0, TMU_TSTR]
	orr	w1, w1, #TSTR1_TMU3
	strb	w1, [x0, TMU_TSTR]
	ret
endfunc start_TMU3


func snapshot_TCNT3
	ldr	x0, =TMU3_BASE
	ldr	w0, [x0, TMU_TCNT]
	ret
endfunc snapshot_TCNT3


func stop_TMU3
	ldr	x0, =TMU3_BASE
	ldrb	w1, [x0, TMU_TSTR]
	and	w1, w1, #~TSTR1_TMU3
	strb	w1, [x0, TMU_TSTR]
	ret
endfunc stop_TMU3
#endif /* TMU3_MEASUREMENT */
