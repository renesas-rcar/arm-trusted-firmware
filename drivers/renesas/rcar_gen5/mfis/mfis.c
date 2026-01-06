/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/******************************************************************************/
/* Header file                                                                */
/******************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>
#include "mfis.h"
#include "mfis_register.h"

/******************************************************************************/
/* Variable                                                                   */
/******************************************************************************/
static const uint32_t mfis_mfislckr_table[] = {
	MFIS_MFISLCKR0, /* MFIS_RPC           */
	MFIS_MFISLCKR1, /* Reserved           */
	MFIS_MFISLCKR2, /* MFIS_UFS           */
	MFIS_MFISLCKR3, /* MFIS_OVERLAP_CHECK */
	MFIS_MFISLCKR4, /* MFIS_MEASUREMENT   */
	MFIS_MFISLCKR5  /* MFIS_TARGET_HSCIF  */
};

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
void rcar_mfis_init(void)
{
	uint32_t register_value;

	/* Write Protection Control Register */
	/* Enable write protection setting */
	register_value = (MFISWPCNTR_CODEVALUE_SET | MFISWPCNTR_WPD_SET);
	mmio_write_32(MFIS_MFISWPCNTR, register_value);

}

void rcar_mfis_lock(mfis_target_t target)
{
	uintptr_t lock_reg;
	uint32_t val;
	uint32_t i;

	lock_reg = mfis_mfislckr_table[target];

	/* Wait until acquired (with timeout to avoid hang) */
	for (i = 0; i < MFIS_LOCK_TIMEOUT; i++) {
		val = mmio_read_32(lock_reg);
		if ((val & MFISLCKR_LCK_MASK) == MFISLCKR_LCK_NOT_ACQUIRED_CHECK) {
			return;
		}
	}
}


void rcar_mfis_unlock(mfis_target_t target)
{
	uint32_t register_value_mfis_mfiswacntr;
	uint32_t register_value_mfis_mfislckr;

	register_value_mfis_mfiswacntr = MFISWACNTR_CODEVALUE_SET;
	register_value_mfis_mfiswacntr |= (mfis_mfislckr_table[target]
					   & MFISWACNTR_REGISTERADDRESS_MASK);

	register_value_mfis_mfislckr = mmio_read_32(mfis_mfislckr_table[target]);
	register_value_mfis_mfislckr &= (~(MFISLCKR_LCK_MASK));
	register_value_mfis_mfislckr |= MFISLCKR_LCK_RELEASE_SET;

	/* Write Access Control Register */
	/* MFISLCKR[j] Register address setting */
	mmio_write_32(MFIS_MFISWACNTR, register_value_mfis_mfiswacntr);

	/* MFIS Lock Register [j] (MFISLCKR[j]) */
	mmio_write_32(mfis_mfislckr_table[target], register_value_mfis_mfislckr);

}
