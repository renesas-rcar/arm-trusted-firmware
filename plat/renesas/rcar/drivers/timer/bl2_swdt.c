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
#include <mmio.h>
#include <arm_gic.h>
#include <gic_v2.h>
#include <debug.h>
#include "bl2_swdt.h"
#include "../../rcar_def.h"

#define RST_BASE		(0xE6160000U)
#define RST_WDTRSTCR		(RST_BASE + 0x0054U)
#define SWDT_BASE		(0xE6030000U)
#define SWDT_WTCNT		(SWDT_BASE + 0x0000U)
#define SWDT_WTCSRA		(SWDT_BASE + 0x0004U)
#define SWDT_WTCSRB		(SWDT_BASE + 0x0008U)

#define WDTRSTCR_UPPER_BYTE	(0xA55A0000U)
#define WTCSRA_UPPER_BYTE	(0xA5A5A500U)
#define WTCSRB_UPPER_BYTE	(0xA5A5A500U)
#define WTCNT_UPPER_BYTE	(0x5A5A0000U)
#define WTCNT_RESET_VALUE	(0xF488U)
#define WTCSRA_BIT_CKS		(0x0007U)
#define WTCSRB_BIT_CKS		(0x003FU)
#define SWDT_RSTMSK		((uint32_t)1U << 1)
#define WTCSRA_WOVFE		((uint32_t)1U << 3)
#define WTCSRA_WRFLG		((uint32_t)1U << 5)
#define SWDT_ENABLE		((uint32_t)1U << 7)

#define WDTRSTCR_MASK_ALL	(0x0000FFFFU)
#define WTCSRA_MASK_ALL		(0x000000FFU)
#define WTCNT_INIT_DATA		(WTCNT_UPPER_BYTE + WTCNT_RESET_VALUE)
#define WTCSRA_INIT_DATA	(WTCSRA_UPPER_BYTE + 0x0FU)
#define WTCSRB_INIT_DATA	(WTCSRB_UPPER_BYTE + 0x21U)

#define WTCNT_COUNT_8p13k		(0x10000U - 40687U)
#define WTCNT_COUNT_8p13k_H3ES1p0	(0x10000U - 20343U)
#define WTCNT_COUNT_8p22k		(0x10000U - 41115U)
#define WTCSRA_CKS_DIV16		(0x00000002U)

#define CHECK_MD13_MD14			(0x6000U)
#define FREQ_10p0M			(0x2000U)
#define FREQ_12p5M			(0x4000U)
#define FREQ_8p33M			(0x0000U)
#define FREQ_16p66M			(0x6000U)

static void bl2_swdt_disable(void);

void bl2_swdt_init(void)
{
	uint32_t sr;
	uint32_t rmsk;
	uint32_t product_cut = mmio_read_32((uintptr_t)RCAR_PRR)
				& (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	uint32_t chk_data = mmio_read_32((uintptr_t)RCAR_MODEMR)
							& CHECK_MD13_MD14;

	if ((mmio_read_32(SWDT_WTCSRA) & SWDT_ENABLE) != 0U) {
		/* Stop SWDT	*/
		mmio_write_32(SWDT_WTCSRA,WTCSRA_UPPER_BYTE);
	}

	/* clock is OSC/16 and overflow interrupt is enabled	*/
	mmio_write_32(SWDT_WTCSRA,(WTCSRA_UPPER_BYTE | WTCSRA_WOVFE
							 | WTCSRA_CKS_DIV16));

	/* Set the overflow counter				*/
	switch (chk_data) {
	case FREQ_8p33M:	/* MD13=0 and MD14=0		*/
	case FREQ_12p5M:	/* MD13=0 and MD14=1		*/
		/* OSCCLK=130.2kHz count=40687, set 0x5A5A6111	*/
		mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE | WTCNT_COUNT_8p13k));
		break;
	case FREQ_10p0M:	/* MD13=1 and MD14=0		*/
		/* OSCCLK=131.57kHz count=41115, set 0x5A5A5F65	*/
		mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE | WTCNT_COUNT_8p22k));
		break;
	case FREQ_16p66M:	/* MD13=1 and MD14=1		*/
		/* OSCCLK=130.2kHz				*/
		if (product_cut==(RCAR_PRODUCT_H3 | RCAR_CUT_ES10)) {
			/* R-car H3 ES1.0			*/
			/* count=20343, set 0x5A5AB089		*/
			mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE | WTCNT_COUNT_8p13k_H3ES1p0));
		} else {
			/* count=40687, set 0x5A5A6111		*/
			mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE | WTCNT_COUNT_8p13k));
		}
		break;
	default:
		/* Error					*/
		ERROR("BL2: MODEMR ERROR value=%x\n", chk_data);
		panic();
		break;
	}

	rmsk = mmio_read_32(RST_WDTRSTCR) & WDTRSTCR_MASK_ALL;
	mmio_write_32(RST_WDTRSTCR,(WDTRSTCR_UPPER_BYTE
					 | (rmsk | SWDT_RSTMSK)));
	while ((mmio_read_8(SWDT_WTCSRA) & WTCSRA_WRFLG) != 0U) {
		/* Wait until the WTCNT is reflected		*/
		;
	}
	sr = mmio_read_32(SWDT_WTCSRA) & WTCSRA_MASK_ALL;
	/* Start the System WatchDog Timer			*/
	mmio_write_32(SWDT_WTCSRA,(WTCSRA_UPPER_BYTE | sr | SWDT_ENABLE));
}

static void bl2_swdt_disable(void)
{
	uint32_t rmsk;
	uintptr_t base = (uintptr_t)RCAR_GICD_BASE;
	uint32_t id = (uint32_t)ARM_IRQ_SEC_WDT;

	/* Initialize the HW initial data, but SWDT is not moved	*/
	rmsk = mmio_read_32(RST_WDTRSTCR) & WDTRSTCR_MASK_ALL;
	mmio_write_32(RST_WDTRSTCR,(WDTRSTCR_UPPER_BYTE
					 | (rmsk | SWDT_RSTMSK)));
	mmio_write_32(SWDT_WTCNT,WTCNT_INIT_DATA);
	mmio_write_32(SWDT_WTCSRA,WTCSRA_INIT_DATA);
	mmio_write_32(SWDT_WTCSRB,WTCSRB_INIT_DATA);
	/* Set the interrupt clear enable register			*/
	gicd_set_icenabler(base, id);
}

void bl2_swdt_release(void)
{
        bl2_swdt_disable();
        disable_fiq();
        arm_gic_cpuif_deactivate();
}

void bl2_swdt_exec(uint64_t addr)
{
	/* Clear the interrupt request	*/
	arm_gic_end_of_interrupt((uint32_t)ARM_IRQ_SEC_WDT);
        bl2_swdt_release();
	ERROR("\n");
	ERROR("BL2: System WDT overflow, occured address is 0x%x\n"
						, (uint32_t)addr);
	/* Endless loop		*/
	panic();
}
