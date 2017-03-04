/*
 * Copyright (c) 2013-2015, ARM Limited and Contributors. All rights reserved.
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
#include <asm_macros.S>

	.globl	console_init
	.globl	console_putc
	.globl	console_uninit
	.globl	console_core_init
	.globl	console_core_putc
	.globl	console_core_getc

	.extern	rcar_log_init
	.extern	rcar_set_log_data

	/* -----------------------------------------------
	 * int console_core_init(unsigned long base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the log area. This
	 * function will be accessed by console_init and
	 * crash reporting.
	 * Return 1 on SUCCESS, 0 on error
	 * In: x0 - Not used
	 *     w1 - Not used
	 *     w2 - Not used
	 * -----------------------------------------------
	 */
func console_core_init
	b	rcar_log_init
endfunc console_core_init
func  console_init
	b	console_core_init
endfunc console_init

	/* --------------------------------------------------------
	 * int console_core_putc(int c, unsigned long base_addr)
	 * Function to output a character over the log area.
	 * Return 1 on SUCCESS, 0 on error
	 * In : w0 - Not used
	 *      x1 - Not used
	 * --------------------------------------------------------
	 */
func console_core_putc
	b	rcar_set_log_data
endfunc console_core_putc
func console_putc
	b	console_core_putc
endfunc console_putc

	/* ---------------------------------------------
	 * int console_core_getc(unsigned long base_addr)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 on error.
	 * In : x0 - console base address
	 * Clobber list : x0, x1
	 * ---------------------------------------------
	 */
func console_core_getc
	ret
endfunc console_core_getc

	/* -----------------------------------------------
	 * void console_uninit(void)
	 * Function to finish the use of console driver.
	 * -----------------------------------------------
	 */
func console_uninit
	ret
endfunc console_uninit
