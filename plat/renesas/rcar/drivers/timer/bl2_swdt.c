/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <mmio.h>
#include <gicv2.h>
#include <debug.h>
#include "bl2_swdt.h"
#include "rcar_def.h"

#define RST_BASE		(0xE6160000U)
#define RST_WDTRSTCR		(RST_BASE + 0x0054U)
#define SWDT_BASE		(0xE6030000U)
#define SWDT_WTCNT		(SWDT_BASE + 0x0000U)
#define SWDT_WTCSRA		(SWDT_BASE + 0x0004U)
#define SWDT_WTCSRB		(SWDT_BASE + 0x0008U)
#define SWDT_GICD_BASE		(0xF1010000U)
#define SWDT_GICC_BASE		(0xF1020000U)
#define SWDT_GICD_CTLR		(SWDT_GICD_BASE + 0x0000U)
#define SWDT_GICD_IGROUPR	(SWDT_GICD_BASE + 0x0080U)
#define SWDT_GICD_ISPRIORITYR	(SWDT_GICD_BASE + 0x0400U)
#define SWDT_GICC_CTLR		(SWDT_GICC_BASE + 0x0000U)
#define SWDT_GICC_PMR		(SWDT_GICC_BASE + 0x0004U)
#define SWDT_GICD_ITARGETSR	(SWDT_GICD_BASE + 0x0800U)
#define IGROUPR_NUM		(16U)
#define ISPRIORITY_NUM		(128U)
#define ITARGET_MASK		((uint32_t)0x03U)

#define WDTRSTCR_UPPER_BYTE	(0xA55A0000U)
#define WTCSRA_UPPER_BYTE	(0xA5A5A500U)
#define WTCSRB_UPPER_BYTE	(0xA5A5A500U)
#define WTCNT_UPPER_BYTE	(0x5A5A0000U)
#define WTCNT_RESET_VALUE	(0xF488U)
#define WTCSRA_BIT_CKS		(0x0007U)
#define WTCSRB_BIT_CKS		(0x003FU)
#define SWDT_RSTMSK		((uint32_t)1U << 1U)
#define WTCSRA_WOVFE		((uint32_t)1U << 3U)
#define WTCSRA_WRFLG		((uint32_t)1U << 5U)
#define SWDT_ENABLE		((uint32_t)1U << 7U)

#define WDTRSTCR_MASK_ALL	(0x0000FFFFU)
#define WTCSRA_MASK_ALL		(0x000000FFU)
#define WTCNT_INIT_DATA		(WTCNT_UPPER_BYTE + WTCNT_RESET_VALUE)
#define WTCSRA_INIT_DATA	(WTCSRA_UPPER_BYTE + 0x0FU)
#define WTCSRB_INIT_DATA	(WTCSRB_UPPER_BYTE + 0x21U)

#define WTCNT_COUNT_8p13k		(0x10000U - 40687U)
#define WTCNT_COUNT_8p13k_H3ES1p0	(0x10000U - 20343U)
#define WTCNT_COUNT_8p22k		(0x10000U - 41115U)
#define WTCNT_COUNT_7p81k		(0x10000U - 39062U)
#define WTCSRA_CKS_DIV16		(0x00000002U)

static void bl2_swdt_disable(void);

void bl2_swdt_init(void)
{
	uint32_t sr;
	uint32_t rmsk;
#if (RCAR_LSI != RCAR_E3)
	uint32_t product_cut = mmio_read_32((uintptr_t)RCAR_PRR)
				& (RCAR_PRODUCT_MASK | RCAR_CUT_MASK);
	uint32_t chk_data = mmio_read_32((uintptr_t)RCAR_MODEMR)
							& CHECK_MD13_MD14;
#endif /* RCAR_LSI != RCAR_E3 */

	if ((mmio_read_32(SWDT_WTCSRA) & SWDT_ENABLE) != 0U) {
		/* Stop SWDT	*/
		mmio_write_32(SWDT_WTCSRA,WTCSRA_UPPER_BYTE);
	}

	/* clock is OSC/16 and overflow interrupt is enabled	*/
	mmio_write_32(SWDT_WTCSRA,(WTCSRA_UPPER_BYTE | WTCSRA_WOVFE
							 | WTCSRA_CKS_DIV16));

	/* Set the overflow counter				*/
	/* The System Watchdog timer is a single-channel timer  */
	/* that uses the OSCCLK as an input clock and can be    */
	/* used as a watchdog timer.                            */
#if (RCAR_LSI == RCAR_E3)
	/* OSCCLK=125kHz count=39062, set 0x5A5A676A	*/
	mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE |
		WTCNT_COUNT_7p81k));
#else /* RCAR_LSI == RCAR_E3 */
	switch (chk_data) {
	case MD14_MD13_TYPE_0:	/* MD13=0 and MD14=0		*/
	case MD14_MD13_TYPE_2:	/* MD13=0 and MD14=1		*/
		/* OSCCLK=130.2kHz count=40687, set 0x5A5A6111	*/
		mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE |
			WTCNT_COUNT_8p13k));
		break;
	case MD14_MD13_TYPE_1:	/* MD13=1 and MD14=0		*/
		/* OSCCLK=131.57kHz count=41115, set 0x5A5A5F65	*/
		mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE |
			WTCNT_COUNT_8p22k));
		break;
	case MD14_MD13_TYPE_3:	/* MD13=1 and MD14=1		*/
		/* OSCCLK=130.2kHz				*/
		if (product_cut==(RCAR_PRODUCT_H3 | RCAR_CUT_ES10)) {
			/* R-car H3 ES1.0			*/
			/* count=20343, set 0x5A5AB089		*/
			mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE |
				WTCNT_COUNT_8p13k_H3ES1p0));
		} else {
			/* count=40687, set 0x5A5A6111		*/
			mmio_write_32(SWDT_WTCNT,(WTCNT_UPPER_BYTE |
				WTCNT_COUNT_8p13k));
		}
		break;
	default:
		/* Error					*/
		ERROR("BL2: MODEMR ERROR value=%x\n", chk_data);
		panic();
		break;
	}
#endif /* RCAR_LSI == RCAR_E3 */

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
	uintptr_t p_gicd_ctlr = (uintptr_t)SWDT_GICD_CTLR;
	uintptr_t p_igroupr = (uintptr_t)SWDT_GICD_IGROUPR;
	uintptr_t p_ispriorityr = (uintptr_t)SWDT_GICD_ISPRIORITYR;
	uintptr_t p_gicc_ctlr = (uintptr_t)SWDT_GICC_CTLR;
	uintptr_t p_pmr = (uintptr_t)SWDT_GICC_PMR;
	uintptr_t p_itargetsr = (uintptr_t)(SWDT_GICD_ITARGETSR
				+ (ARM_IRQ_SEC_WDT & (uint32_t)(~ITARGET_MASK)));
	uint32_t i;

	write_daifset(DAIF_FIQ_BIT);
        bl2_swdt_disable();
        gicv2_cpuif_disable();
	for (i=0U; i<IGROUPR_NUM; i++) {
		mmio_write_32((p_igroupr + (uint32_t)(i * 4U)), 0U);
	}
	for (i=0U; i<ISPRIORITY_NUM; i++) {
		mmio_write_32((p_ispriorityr + (uint32_t)(i * 4U)), 0U);
	}
	mmio_write_32(p_itargetsr, 0U);
	mmio_write_32(p_gicd_ctlr, 0U);
	mmio_write_32(p_gicc_ctlr, 0U);
	mmio_write_32(p_pmr, 0U);
}

void bl2_swdt_exec(uint64_t addr)
{
	/* Clear the interrupt request	*/
	gicv2_end_of_interrupt((uint32_t)ARM_IRQ_SEC_WDT);
        bl2_swdt_release();
	ERROR("\n");
	ERROR("BL2: System WDT overflow, occured address is 0x%x\n"
						, (uint32_t)addr);
	/* Endless loop		*/
	panic();
}
