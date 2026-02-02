/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LDR_PRIVATE_H
#define LDR_PRIVATE_H

/*
 * Text area of one record: level tag, text and NUL. It sizes the static
 * record buffer; the daemon rejects records over 2048 bytes.
 */
#define LDR_MSG_TEXT_MAX		256U

/* Length of the level tag ("E:", "N:", ...) from ldr_dlt_level_prefix(). */
#define LDR_LEVEL_TAG_LEN		2U

/* Longest text of one record: 253 characters. */
#define LDR_MSG_TEXT_CHARS_MAX		\
	(LDR_MSG_TEXT_MAX - LDR_LEVEL_TAG_LEN - 1U)

#endif /* LDR_PRIVATE_H */
