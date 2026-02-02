/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR console. It is registered next to the serial console, so it receives
 * all BL31 console output: log macros, printf() and puts(). Characters are
 * collected per core and written as one LDR record per line.
 */

#include <stddef.h>
#include <string.h>

#include <drivers/console.h>
#include <lib/utils_def.h>
#include <plat/common/platform.h>
#include <platform_def.h>

#include "ldr.h"
#include "ldr_private.h"
#include "lib/ldr_dlt_protocol/ldr_dlt_protocol.h"

/* Text of one record; a longer line is split across records. */
#define LDR_CONSOLE_LINE_MAX		LDR_MSG_TEXT_CHARS_MAX

/* Level of a line without a tf_log() prefix, e.g. from printf(). */
#define LDR_CONSOLE_DEFAULT_LEVEL	LDR_LEVEL_INFO

/* Per core: console output is not serialized across cores. */
static char ldr_line[PLATFORM_CORE_COUNT][LDR_CONSOLE_LINE_MAX];
static size_t ldr_line_len[PLATFORM_CORE_COUNT];

/*
 * tf_log() prefixes (plat_prefix_str[] in plat/common/plat_log_common.c).
 * They give the level of the record and are stripped from its text.
 */
struct ldr_console_prefix {
	const char *text;
	size_t len;
	int level;
};

#define LDR_CONSOLE_PREFIX(str, lvl)	{ (str), sizeof(str) - 1U, (lvl) }

static const struct ldr_console_prefix ldr_console_prefixes[] = {
	LDR_CONSOLE_PREFIX("ERROR:   ", LDR_LEVEL_ERROR),
	LDR_CONSOLE_PREFIX("NOTICE:  ", LDR_LEVEL_NOTICE),
	LDR_CONSOLE_PREFIX("WARNING: ", LDR_LEVEL_WARNING),
	LDR_CONSOLE_PREFIX("INFO:    ", LDR_LEVEL_INFO),
	LDR_CONSOLE_PREFIX("VERBOSE: ", LDR_LEVEL_VERBOSE),
};

static void ldr_console_flush_line(unsigned int idx)
{
	const char *text = ldr_line[idx];
	size_t len = ldr_line_len[idx];
	int level = LDR_CONSOLE_DEFAULT_LEVEL;
	size_t i;

	ldr_line_len[idx] = 0U;

	/* Tolerate CRLF. */
	if ((len > 0U) && (text[len - 1U] == '\r')) {
		len--;
	}

	for (i = 0U; i < ARRAY_SIZE(ldr_console_prefixes); i++) {
		const struct ldr_console_prefix *p = &ldr_console_prefixes[i];

		if ((len >= p->len) && (memcmp(text, p->text, p->len) == 0)) {
			level = p->level;
			text += p->len;
			len -= p->len;
			break;
		}
	}

	if (len == 0U) {
		return;
	}

	rcar_ldr_record_text(level, text, len);
}

/*
 * Return c, not 0: console_putc() returns the lowest result of all
 * consoles, and the serial console returns c.
 */
static int console_rcar_ldr_putc(int c, console_t *console)
{
	unsigned int idx;

	(void)console;

	if (!rcar_ldr_is_ready()) {
		return c;
	}

	idx = plat_my_core_pos();
	if (idx >= PLATFORM_CORE_COUNT) {
		return c;
	}

	if (c == '\n') {
		ldr_console_flush_line(idx);
	} else {
		ldr_line[idx][ldr_line_len[idx]] = (char)c;
		ldr_line_len[idx]++;

		/* Full buffer: write it and continue in the next record. */
		if (ldr_line_len[idx] >= LDR_CONSOLE_LINE_MAX) {
			ldr_console_flush_line(idx);
		}
	}

	return c;
}

static void console_rcar_ldr_flush(console_t *console)
{
	unsigned int idx;

	(void)console;

	if (!rcar_ldr_is_ready()) {
		return;
	}

	idx = plat_my_core_pos();
	if (idx < PLATFORM_CORE_COUNT) {
		ldr_console_flush_line(idx);
	}
}

/*
 * No CONSOLE_FLAG_TRANSLATE_CRLF: records must not contain '\r'.
 * No CONSOLE_FLAG_CRASH: a crash with ldr_lock held would deadlock.
 */
static console_t ldr_console = {
	.next = NULL,
	.flags = CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME,
	.putc = console_rcar_ldr_putc,
	.flush = console_rcar_ldr_flush,
	.base = 0U,
};

void console_rcar_ldr_register(void)
{
	(void)console_register(&ldr_console);
}
