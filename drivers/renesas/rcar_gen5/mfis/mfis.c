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
	/*
	 * Disable MFIS write protection (WPD=1).
	 *
	 * MFISWPCNTR and MFISWACNTR are single shared registers covering
	 * ALL channels and ALL software agents (IPL, BL31, SCP, Linux,
	 * FreeRTOS).  There is no cross-agent mutex protecting the
	 * three-step sequence: MFISWPCNTR→MFISWACNTR→MFISLCKRj.
	 *
	 * The previous implementation enabled write protection (WPD=0)
	 * which forced every unlock through the shared MFISWACNTR write-gate.
	 * This created two problems:
	 *   1. BL31 re-locked protection that FreeRTOS had already unlocked,
	 *      breaking RPMsg CR-CA communication.
	 *   2. Concurrent MFISWACNTR writes from different agents/channels
	 *      race: the last writer wins, silently corrupting the unlock of
	 *      the other channel.
	 *
	 * Fix: disable write protection once at init (WPD=1).  All agents
	 * can then write MFISLCKRj directly without going through MFISWACNTR,
	 * eliminating the shared-register race.  No agent should re-enable
	 * protection (WPD=0) after this point.
	 */
	mmio_write_32(MFIS_MFISWPCNTR,
		      MFISWPCNTR_CODEVALUE_SET | MFISWPCNTR_WPD_UNLOCK);
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
	uint32_t register_value_mfis_mfislckr;

	register_value_mfis_mfislckr = mmio_read_32(mfis_mfislckr_table[target]);
	register_value_mfis_mfislckr &= (~(MFISLCKR_LCK_MASK));
	register_value_mfis_mfislckr |= MFISLCKR_LCK_RELEASE_SET;

	/*
	 * Write protection is disabled (WPD=1, set in rcar_mfis_init()).
	 * Write MFISLCKRj directly — no MFISWACNTR step required.
	 *
	 * MFISWACNTR must NOT be used here: it is a single shared register
	 * with no cross-agent mutex.  Concurrent writes from different agents
	 * or channels overwrite each other's address field, causing the wrong
	 * channel to be unlocked.
	 */
	mmio_write_32(mfis_mfislckr_table[target], register_value_mfis_mfislckr);
}
