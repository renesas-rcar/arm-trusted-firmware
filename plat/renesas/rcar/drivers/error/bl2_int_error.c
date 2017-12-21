/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
