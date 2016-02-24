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

#include <bl_common.h>
#include <arm_gic.h>
#include <mmio.h>
#include <debug.h>
#include "bl2_int_error.h"
#include "../../rcar_def.h"

#define INT_ID_MASK	(0x000003FFU)
#define SWDT_ERROR_ID	(1024U)
#define SWDT_ERROR_TYPE	(16U)
#define SWDT_CHAR_MAX	(13U)

void bl2_interrupt_error_id(uint32_t wrong_id)
{
	if (wrong_id >= SWDT_ERROR_ID) {
		ERROR("\n");
		ERROR("BL2: bl2_interrupt_error_id error, invalid ID = %d\n"
			 ,wrong_id);
		/* Endless loop		*/
		panic();
	}
	/* Clear the interrupt request	*/
	arm_gic_end_of_interrupt((uint32_t)wrong_id);
	bl2_swdt_release();
	ERROR("\n");
	ERROR("BL2: This interrupt is Invalid, receive interrupt ID = %d\n"
		 ,wrong_id);
	/* Endless loop		*/
	panic();
}

void bl2_interrupt_error_type(uint32_t wrong_type)
{
	const uint8_t interrupt_ex[SWDT_ERROR_TYPE][SWDT_CHAR_MAX] = {
		"SYNC SP EL0",
		"IRQ SP EL0",
		"FIQ SP EL0",
		"SERR SP EL0",
		"SYNC SP ELx",
		"IRQ SP ELx",
		"FIQ SP ELx",
		"SERR SP ELx",
		"SYNC AARCH64",
		"IRQ AARCH64",
		"FIQ AARCH64",
		"SERR AARCH64",
		"SYNC AARCH32",
		"IRQ AARCH32",
		"FIQ AARCH32",
		"SERR AARCH32"
	};
	uint32_t interrupt_id;

	if (wrong_type >= SWDT_ERROR_TYPE) {
		/* Clear the interrupt request	*/
		(void)arm_gic_acknowledge_interrupt();
		ERROR("\n");
		ERROR("BL2: bl2_interrupt_error_type error, invalid type = %d\n"
			,wrong_type);
		/* endless loop		*/
		panic();
	}
	interrupt_id = arm_gic_acknowledge_interrupt() & INT_ID_MASK;
	bl2_swdt_release();

	ERROR("\n");
	ERROR("BL2: This interrupt is not FIQ, interrupt type = %s, ID = %d\n"
			,&interrupt_ex[wrong_type][0], interrupt_id);
	/* Endless loop		*/
	panic();
}
