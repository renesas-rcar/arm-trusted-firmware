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
#include <scif.h>

	.globl	console_init
	.globl	console_putc
	.globl	console_core_init
	.globl	console_core_putc
	.globl	console_getc

	/*
	 *  The console base is in the data section and not in .bss
	 *  even though it is zero-init. In particular, this allows
	 *  the console functions to start using this variable before
	 *  the runtime memory is initialized for images which do not
	 *  need to copy the .data section from ROM to RAM.
	 */
	/* -----------------------------------------------
	 * int console_init(unsigned long base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. It saves
	 * the console base to the data section.
	 * In: x0 - console base address
	 *     w1 - Uart clock in Hz
	 *     w2 - Baud rate
	 * out: return 1 on success.
	 * Clobber list : x1 - x3
	 * -----------------------------------------------
	 */
func console_init
	b	console_core_init
endfunc console_init

	/* -----------------------------------------------
	 * int console_core_init(unsigned long base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. This
	 * function will be accessed by console_init and
	 * crash reporting.
	 * In: x0 - console base address
	 *     w1 - Uart clock in Hz
	 *     w2 - Baud rate
	 * Out: return 1 on success
	 * Clobber list : x1, x2
	 * -----------------------------------------------
	 */
func console_core_init
	ldr	x0, =CPG_BASE
	ldr	w1, [x0, #CPG_SMSTPCR3]
	and	w1, w1, #~MSTP310		/* MSTP310=0 */
	mvn	w2, w1
	str	w2, [x0, #CPG_CPGWPR]
	str	w1, [x0, #CPG_SMSTPCR3]

	ldr	x0, =SCIF2_BASE
	mov	w1, #0
	strh	w1, [x0, #SCIF_SCSMRIR]
	/* Clear bits TE and RE in SCSCR to 0 */
	mov	w1, #(SCSCR_TE_DIS + SCSCR_RE_DIS)	/* TE=0,RE=0 */
	strh	w1, [x0, #SCIF_SCSCR]
	/* Set bits TFRST and RFRST in SCFCR to 1 */
	ldrh	w1, [x0, #SCIF_SCFCR]
	orr	w1, w1, #(SCFCR_TFRST_EN + SCFCR_RFRS_EN)	/* TFRESET=1,RFRESET=1 */
	strh	w1, [x0, #SCIF_SCFCR]
	/* Read flags of ER, DR, BRK, and RDF in SCFSR and those of TO and ORER in SCLSR, then clear them to 0 */
	mov	w1, #SCFSR_INIT_DATA
	strh	w1, [x0, #SCIF_SCFSR]
	mov	w1, #0
	strh	w1, [x0, #SCIF_SCLSR]
	/* Set bits CKE[1:0] in SCSCR */
	ldrh	w1, [x0, #SCIF_SCSCR]
	and	w1, w1, #~SCSCR_CKE_MASK
	mov	w2, #SCSCR_CKE_INT_CLK		/* CKE=00 */
	orr	w1, w1, w2
	strh	w1, [x0, #SCIF_SCSCR]
	/* Set data transfer format in SCSMR */
	mov	w1, #SCSMR_INIT_DATA
	strh	w1, [x0, #SCIF_SCSMR]		/* CA=0,CHR=0,PE=0,STOP=0,CKS=P */
	/* Set value in SCBRR */
#if SCIF_CLK == SCIF_INTERNAL_CLK
	ldr	x1, =PRR
	ldr	w1, [x1]
	and	w1, w1, #PRR_PRODUCT_MASK
	mov	w2, #PRR_PRODUCT_H3_ES_1_0
	cmp	w1, w2
	beq	3f
	mov	w1, #SCBRR_115200BPS	/* 115200bps */
	b	2f
3:
	mov	w1, #SCBRR_230400BPS	/* H3 ES1.0 sets clock to doubling */
2:
	strb	w1, [x0, SCIF_SCBRR]
#else
	mov	w1, #DL_INIT_DATA
	strh	w1, [x0, #SCIF_DL]
	mov	w1, #CKS_INIT_DATA		/* CKS=0,XIN=0 */
	strh	w1, [x0, #SCIF_CKS]
#endif
	/* 1-bit interval elapsed */
	mov	w1, #100			/* 100 times */
1:
	subs	w1, w1, #1
	cbnz	w1, 1b
	/* Set bits RTRG[1:0], TTRG[1:0], and MCE in SCFCR, and clear bits FRST and RFRST to 0 */
	mov	w1, #SCFCR_INIT_DATA
	strh	w1, [x0, #SCIF_SCFCR]
	/* Set bits TE and RE in SCSCR to 1 */
	ldrh	w1, [x0, #SCIF_SCSCR]
	orr	w1, w1, #(SCSCR_TE_EN + SCSCR_RE_EN)	/* TE=1,RE=1 */
	strh	w1, [x0, #SCIF_SCSCR]
	mov	x1, #1

	ret
endfunc console_core_init

	/* ---------------------------------------------
	 * int console_putc(int c)
	 * Function to output a character over the
	 * console. It returns the character printed on
	 * success or -1 on error.
	 * In : x0 - character to be printed
	 * Out : return -1 on error else return character.
	 * Clobber list : x1, x2
	 * ---------------------------------------------
	 */
func console_putc
	b	console_core_putc
endfunc console_putc

	/* --------------------------------------------------------
	 * int console_core_putc(int c, unsigned int base_addr)
	 * Function to output a character over the console. It
	 * returns the character printed on success or -1 on error.
	 * In : w0 - character to be printed
	 *      x1 - console base address
	 * Out : return -1 on error else return character.
	 * Clobber list : x2
	 * --------------------------------------------------------
	 */
func console_core_putc
	ldr	x1, =SCIF2_BASE
	cmp	w0, #0xA
	/* Prepend '\r' to '\n' */
	bne	2f
1:
	/* Check if the transmit FIFO is full */
	ldrh	w2, [x1, #SCIF_SCFDR]
	ubfx	w2, w2, #8, #5
	cmp	w2, #16
	bcs	1b
	mov	w2, #0x0D
	strb	w2, [x1, #SCIF_SCFTDR]
2:
	/* Check if the transmit FIFO is full */
	ldrh	w2, [x1, #SCIF_SCFDR]
	ubfx	w2, w2, #8, #5
	cmp	w2, #16
	bcs	2b
	strb	w0, [x1, #SCIF_SCFTDR]

	ret
endfunc console_core_putc

	/* ---------------------------------------------
	 * int console_getc(void)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 on error.
	 * Clobber list : x0, x1
	 * ---------------------------------------------
	 */
func console_getc
	mov	w0, #-1			/* no support */
	ret
endfunc console_getc
