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

#include <arch_helpers.h>
#include <runtime_svc.h>
#include <bl_common.h>
#include <gicv2.h>
#include <mmio.h>
#include <debug.h>
#include "bl2_int_error.h"
#include "rcar_def.h"

#define SWDT_ERROR_ID	(1024U)
#define SWDT_ERROR_TYPE	(16U)
#define SWDT_CHAR_MAX	(13U)

void bl2_interrupt_error_id(uint32_t int_id)
{
	ERROR("\n");
	if (int_id >= SWDT_ERROR_ID) {
		ERROR("BL2: Unhandled exception occurred.\n");
		ERROR("     Exception type = FIQ_SP_ELX\n");
		/* Endless loop		*/
		panic();
	}
	/* Clear the interrupt request	*/
	gicv2_end_of_interrupt((uint32_t)int_id);
	bl2_swdt_release();
	ERROR("BL2: Unhandled exception occurred.\n");
	ERROR("     Exception type = FIQ_SP_ELX\n");
	ERROR("     SPSR_EL1 = 0x%x\n",
		(uint32_t)(read_spsr_el1() & 0x0FFFFFFFFU));
	ERROR("     ELR_EL1  = 0x%x\n",
		(uint32_t)(read_elr_el1()  & 0x0FFFFFFFFU));
	ERROR("     ESR_EL1  = 0x%x\n",
		(uint32_t)(read_esr_el1()  & 0x0FFFFFFFFU));
	ERROR("     FAR_EL1  = 0x%x\n",
		(uint32_t)(read_far_el1()  & 0x0FFFFFFFFU));
	ERROR("\n");
	/* Endless loop		*/
	panic();
}

void bl2_interrupt_error_type(uint32_t ex_type)
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
	char msg[128];

	/* Clear the interrupt request	*/
	if (ex_type >= SWDT_ERROR_TYPE) {
		ERROR("\n");
		ERROR("BL2: Unhandled exception occurred.\n");
		ERROR("     Exception type = Unknown (%d)\n", ex_type);
	} else {
		bl2_swdt_release();
		ERROR("\n");
		ERROR("BL2: Unhandled exception occurred.\n");
		(void)sprintf(msg, "     Exception type = %s\n",
				&interrupt_ex[ex_type][0]);
		ERROR("%s", msg);
		switch(ex_type)
		{
		case SYNC_EXCEPTION_SP_ELX:
			ERROR("     SPSR_EL1 = 0x%x\n",
				(uint32_t)(read_spsr_el1() & 0x0FFFFFFFFU));
			ERROR("     ELR_EL1  = 0x%x\n",
				(uint32_t)(read_elr_el1()  & 0x0FFFFFFFFU));
			ERROR("     ESR_EL1  = 0x%x\n",
				(uint32_t)(read_esr_el1()  & 0x0FFFFFFFFU));
			ERROR("     FAR_EL1  = 0x%x\n",
				(uint32_t)(read_far_el1()  & 0x0FFFFFFFFU));
			break;
		case IRQ_SP_ELX:
			ERROR("     SPSR_EL1 = 0x%x\n",
				(uint32_t)(read_spsr_el1() & 0x0FFFFFFFFU));
			ERROR("     ELR_EL1  = 0x%x\n",
				(uint32_t)(read_elr_el1()  & 0x0FFFFFFFFU));
			ERROR("     IAR_EL1  = 0x%x\n",
				gicv2_acknowledge_interrupt());
			break;
		case FIQ_SP_ELX:
			ERROR("     SPSR_EL1 = 0x%x\n",
				(uint32_t)(read_spsr_el1() & 0x0FFFFFFFFU));
			ERROR("     ELR_EL1  = 0x%x\n",
				(uint32_t)(read_elr_el1()  & 0x0FFFFFFFFU));
			ERROR("     IAR_EL1  = 0x%x\n",
				gicv2_acknowledge_interrupt());
			break;
		case SERROR_SP_ELX:
			ERROR("     SPSR_EL1 = 0x%x\n",
				(uint32_t)(read_spsr_el1() & 0x0FFFFFFFFU));
			ERROR("     ELR_EL1  = 0x%x\n",
				(uint32_t)(read_elr_el1()  & 0x0FFFFFFFFU));
			ERROR("     ESR_EL1  = 0x%x\n",
				(uint32_t)(read_esr_el1()  & 0x0FFFFFFFFU));
			ERROR("     FAR_EL1  = 0x%x\n",
				(uint32_t)(read_far_el1()  & 0x0FFFFFFFFU));
			break;
		default:
			break;
		}
	}
	ERROR("\n");
	/* Endless loop		*/
	panic();
}
