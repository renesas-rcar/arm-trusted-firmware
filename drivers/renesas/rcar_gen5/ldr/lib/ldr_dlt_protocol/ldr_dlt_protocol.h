/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR DLT log record assembly, producer side. Builds one record into a
 * caller buffer, in the format the LDR daemon expects from a "shared DRAM"
 * producer. Free-standing: see ../ldr_port.h.
 */

#ifndef LDR_DLT_PROTOCOL_H
#define LDR_DLT_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

/* Header bytes before the text: 8 + 4 + 12 + 10 + 6 (checked in the .c). */
#define LDR_DLT_TEXT_OFFSET		40U

/* DLT message type info (MTIN) of log messages. */
#define LDR_DLT_MTIN_ERROR		2U
#define LDR_DLT_MTIN_WARN		3U
#define LDR_DLT_MTIN_INFO		4U
#define LDR_DLT_MTIN_DEBUG		5U
#define LDR_DLT_MTIN_VERBOSE		6U

/*
 * Log levels, with the values of TF-A's LOG_LEVEL_*. NOTICE and INFO map to
 * the same DLT type but keep different text tags.
 */
typedef enum {
	LDR_LEVEL_ERROR		= 10,
	LDR_LEVEL_NOTICE	= 20,
	LDR_LEVEL_WARNING	= 30,
	LDR_LEVEL_INFO		= 40,
	LDR_LEVEL_VERBOSE	= 50,
} LdrLogLevel;

/* Identity and metadata of one record, supplied by the caller. */
typedef struct {
	/* 4-character IDs, NUL-padded; the daemon replaces ecuid. */
	const char *ecuid;
	const char *apid;
	const char *ctid;
	uint32_t seid;		/* Session ID, must not be 0 */
	uint32_t tmsp;		/* Timestamp in 100 us units */
	uint8_t mcnt;		/* Message counter, wraps at 255 */
	int level;		/* LdrLogLevel */
} LdrDltLogParams;

/*
 * Build cursor set up by ldr_dlt_log_start(). The caller writes at most
 * 'text_capacity' bytes, NUL included, at 'text', then calls
 * ldr_dlt_log_finish().
 */
typedef struct {
	uint8_t *buf;		/* Record buffer */
	size_t buf_size;
	char *text;		/* Where the message text goes */
	size_t text_capacity;	/* Room at 'text' */
	size_t text_used;	/* Level tag already written */
} LdrDltMsg;

/* DLT type of a level; an unknown level gives LDR_DLT_MTIN_DEBUG. */
uint8_t ldr_dlt_level_to_mtin(int level);

/* Text tag of a level ("E:", "N:", ..., "?:" if unknown). Never NULL. */
const char *ldr_dlt_level_prefix(int level);

/*
 * Write the headers and the level tag into 'buf' and set up 'msg'. Returns
 * 0, or -1 on a NULL argument or a buffer with no room for text.
 */
int ldr_dlt_log_start(LdrDltMsg *msg, uint8_t *buf, size_t buf_size,
		      const LdrDltLogParams *params);

/*
 * Complete the record. 'appended_len' follows vsnprintf(): a value of at
 * least msg->text_capacity means the text was cut. Returns the record size,
 * or 0 if 'msg' was not set up.
 */
size_t ldr_dlt_log_finish(LdrDltMsg *msg, size_t appended_len);

/*
 * Start, copy 'text_len' bytes of 'text' (cut to fit) and finish, in one
 * call. Returns the record size, or 0 on bad input.
 */
size_t ldr_dlt_log_build(uint8_t *buf, size_t buf_size,
			 const LdrDltLogParams *params,
			 const char *text, size_t text_len);

#endif /* LDR_DLT_PROTOCOL_H */
