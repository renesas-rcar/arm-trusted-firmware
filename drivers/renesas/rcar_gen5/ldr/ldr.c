/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * BL31 producer for the LDR (Logging Data Recorder) daemon.
 *
 * Each console line becomes one DLT record in a ring buffer in Non-secure
 * DRAM, which the daemon maps and drains. lib/ builds the records and the
 * ring; this file adds the lock, timestamp, publication and identity.
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch_helpers.h>
#include <lib/cassert.h>
#include <lib/spinlock.h>
#include <lib/utils_def.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include <plat/common/platform.h>
#include <platform_def.h>

#include "rcar_def.h"
#include "ldr.h"
#include "ldr_private.h"
#include "lib/ldr_dlt_protocol/ldr_dlt_protocol.h"
#include "lib/ldr_ring_buffer/ldr_ring_buffer.h"

/* Region layout (rcar_def.h). Page alignment also aligns the ring slots. */
CASSERT((BL31_LDR_BASE % PAGE_SIZE) == 0U, assert_ldr_base_page_aligned);
CASSERT((BL31_LDR_SEND_SIZE % PAGE_SIZE) == 0U, assert_ldr_send_page_aligned);
CASSERT((BL31_LDR_SEND_SIZE + BL31_LDR_RECV_SIZE) == BL31_LDR_SIZE,
	assert_ldr_split_fills_region);

/* platform.mk prints the region from its own copy of these values. */
CASSERT(RCAR_LDR_MK_BASE == BL31_LDR_BASE, assert_mk_ldr_base);
CASSERT(RCAR_LDR_MK_SEND_SIZE == BL31_LDR_SEND_SIZE, assert_mk_ldr_send_size);
CASSERT(RCAR_LDR_MK_RECV_SIZE == BL31_LDR_RECV_SIZE, assert_mk_ldr_recv_size);

CASSERT(RCAR_CNTC_EXTAL != 0U, assert_ldr_counter_freq);

/* Application and context ID shown by the daemon and viewers. */
#define LDR_BL31_APID			"BL31"
#define LDR_BL31_CTID			"BL31"

/* Placeholder: the daemon replaces the ECU ID with its own. */
#define LDR_BL31_ECUID			"ECU1"

/*
 * Session ID = 31000 + core position: the daemon prints it in decimal, so
 * the last two digits are the core. 0 is shown as a control message.
 */
#define LDR_BL31_SEID_BASE		U(31000)

CASSERT(((LDR_BL31_SEID_BASE % 100U) == 0U) && (PLATFORM_CORE_COUNT <= 100U),
	assert_ldr_seid_core_digits);

/* One record: headers plus the text area, well below the daemon's limit. */
#define LDR_MSG_BUF_SIZE		(LDR_DLT_TEXT_OFFSET + LDR_MSG_TEXT_MAX)

CASSERT(LDR_MSG_BUF_SIZE < 2000U, assert_record_fits_daemon_limits);

struct ldr_state {
	bool initialized;	/* Ring formatted and ready */
	uint8_t msg_counter;	/* DLT message counter, wraps at 255 */
	uint32_t dropped_count;	/* Records dropped because the ring was full */
};

static struct ldr_state ldr_state;

/* Serializes ring writes and ldr_msg_buf across cores. */
static spinlock_t ldr_lock;

/* Assembles one record; static to keep it off the small EL3 stack. */
static uint8_t ldr_msg_buf[LDR_MSG_BUF_SIZE];

/*
 * DLT timestamp in 100 us units. The frequency is RCAR_CNTC_EXTAL, not
 * CNTFRQ_EL0: a secondary core programs CNTFRQ_EL0 only after
 * pwr_domain_on_finish(), which may log. Divide first to avoid overflow.
 */
static uint32_t ldr_get_timestamp_100us(void)
{
	uint64_t counter = read_cntpct_el0();
	uint64_t freq = (uint64_t)RCAR_CNTC_EXTAL;
	uint64_t ticks;

	ticks = ((counter / freq) * 10000ULL) +
		(((counter % freq) * 10000ULL) / freq);

	return (uint32_t)ticks;
}

/*
 * The region is mapped non-cacheable (MAP_LDR_REGION), so the slot and 'tail'
 * go straight to memory; only wait for the stores to complete. No cache
 * maintenance here: cleaning the header line would also write back the
 * daemon's 'head' as BL31 last read it, and a stale 'head' makes the daemon
 * read the same record twice.
 */
static void ldr_publish(void)
{
	dsb();
}

void rcar_ldr_init(void)
{
	volatile RingBufferHeader *ring = (RingBufferHeader *)BL31_LDR_BASE;

	if (ldr_state.initialized) {
		return;
	}

	/* LDR_MSG_TEXT_CHARS_MAX assumes this tag length. */
	assert(strlen(ldr_dlt_level_prefix(LDR_LEVEL_INFO)) ==
	       LDR_LEVEL_TAG_LEN);

	/* Reformat a ring left by a build with another ring size. */
	if ((ring->magic_num == (uint32_t)LDR_RING_BUFFER_MAGIC_NUMBER) &&
	    (ring->buffer_size != ((uint32_t)BL31_LDR_SEND_SIZE -
				   (uint32_t)sizeof(RingBufferHeader)))) {
		ring->magic_num = 0U;
	}

	/* No-op if the ring is already formatted (warm reset or daemon). */
	(void)ldr_ring_buf_mmgr_init((uint8_t *)BL31_LDR_BASE,
				     (uint32_t)BL31_LDR_SEND_SIZE);

	ldr_state.msg_counter = 0U;
	ldr_state.dropped_count = 0U;

	/* Once, at init: drop any line of the ring cached before this mapping. */
	flush_dcache_range((uintptr_t)BL31_LDR_BASE,
			   (size_t)BL31_LDR_SEND_SIZE);
	dsb();
	isb();

	ldr_state.initialized = true;
}

bool rcar_ldr_is_ready(void)
{
	return ldr_state.initialized;
}

/* Identity and metadata of a record. Caller holds ldr_lock. */
static void ldr_fill_params(LdrDltLogParams *params, int level)
{
	params->ecuid = LDR_BL31_ECUID;
	params->apid = LDR_BL31_APID;
	params->ctid = LDR_BL31_CTID;
	params->seid = LDR_BL31_SEID_BASE + (uint32_t)plat_my_core_pos();
	params->tmsp = ldr_get_timestamp_100us();
	params->mcnt = ldr_state.msg_counter;
	params->level = level;
}

/* Push the record in ldr_msg_buf into the ring. Caller holds ldr_lock. */
static void ldr_enqueue_locked(size_t total)
{
	ldr_errno_t ret;

	ret = ldr_ring_buf_mmgr_enqueue((uint8_t *)BL31_LDR_BASE, ldr_msg_buf,
					(uint32_t)total);
	if (ret == (ldr_errno_t)LDR_RB_SIZE_OVERFLOW) {
		/* No room in front of unread records: drop and count. */
		ldr_state.dropped_count++;
	} else {
		ldr_publish();
	}
}

void rcar_ldr_record_text(int level, const char *text, size_t len)
{
	LdrDltLogParams params;
	size_t total;

	if (!ldr_state.initialized || (text == NULL) || (len == 0U)) {
		return;
	}

	spin_lock(&ldr_lock);

	ldr_fill_params(&params, level);
	total = ldr_dlt_log_build(ldr_msg_buf, sizeof(ldr_msg_buf), &params,
				  text, len);
	if (total != 0U) {
		ldr_state.msg_counter++;
		ldr_enqueue_locked(total);
	}

	spin_unlock(&ldr_lock);
}
