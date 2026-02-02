/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR DLT log record assembly, producer side. Record layout:
 *
 *   off  size  field
 *   ---------------------------------------------------------------
 *     0     8  UserProtocolHeader   "DUH\x01" + message type
 *     8     4  StandardHeader       htyp, mcnt, len (big-endian)
 *    12    12  StandardHeaderExtra  ecu, seid, tmsp (big-endian)
 *    24    10  ExtendedHeader       msin, noar, apid, ctid
 *    34     4  payload arg type info      (host byte order)
 *    38     2  payload arg length         (host byte order)
 *    40     n  text, NUL-terminated
 *
 * StandardHeader.len counts the whole record, the 8-byte LDR envelope
 * included. The payload argument header is in host byte order, as in the
 * LDR middleware.
 */

#include "ldr_dlt_protocol.h"
#include "ldr_dlt_wire.h"

#include "ldr_port.h"

/* Fixed header bytes ahead of the payload: 8 + 4 + 12 + 10 = 34. */
#define LDR_HDR_TOTAL			(sizeof(UserProtocolHeader) +	\
					 sizeof(StandardHeader) +	\
					 sizeof(StandardHeaderExtra) +	\
					 sizeof(ExtendedHeader))

/* Payload argument header: 4-byte type info + 2-byte length. */
#define LDR_PAYLOAD_HDR			(sizeof(uint32_t) + sizeof(uint16_t))

/* htyp: version 1, ECU id + session id + timestamp present, extended header. */
#define LDR_DLT_HTYP			(LDR_HTYP_PROTOCOL_VERSION1 |	\
					 LDR_HTYP_WEID |		\
					 LDR_HTYP_WSID |		\
					 LDR_HTYP_WTMS |		\
					 LDR_HTYP_UEH)

/* Both sides cast the same bytes: check the layout at build time. */
LDR_CASSERT(sizeof(UserProtocolHeader) == 8U, assert_user_protocol_header_size);
LDR_CASSERT(sizeof(StandardHeader) == 4U, assert_standard_header_size);
LDR_CASSERT(sizeof(StandardHeaderExtra) == 12U,
	    assert_standard_header_extra_size);
LDR_CASSERT(sizeof(ExtendedHeader) == 10U, assert_extended_header_size);
LDR_CASSERT(LDR_DLT_TEXT_OFFSET == (LDR_HDR_TOTAL + LDR_PAYLOAD_HDR),
	    assert_text_offset_matches_headers);

/*
 * Copy a 4-character DLT id, padding short strings with NULs and ignoring
 * anything past the fourth character. A NULL id becomes all NULs.
 */
static void ldr_dlt_copy_id(uint8_t *dest, const char *id)
{
	size_t i;

	for (i = 0U; i < LDR_ID_SIZE; i++) {
		if ((id != NULL) && (id[i] != '\0')) {
			dest[i] = (uint8_t)id[i];
		} else {
			/* Pad every remaining byte: the buffer is reused. */
			dest[i] = 0U;
			id = NULL;
		}
	}
}

/* Local strlen: this module carries no libc dependency. */
static size_t ldr_dlt_strlen(const char *s)
{
	size_t n = 0U;

	while (s[n] != '\0') {
		n++;
	}

	return n;
}

uint8_t ldr_dlt_level_to_mtin(int level)
{
	uint8_t mtin;

	switch (level) {
	case LDR_LEVEL_ERROR:
		mtin = LDR_DLT_MTIN_ERROR;
		break;
	case LDR_LEVEL_NOTICE:
		/* DLT has no NOTICE level; INFO is the closest equivalent. */
		mtin = LDR_DLT_MTIN_INFO;
		break;
	case LDR_LEVEL_WARNING:
		mtin = LDR_DLT_MTIN_WARN;
		break;
	case LDR_LEVEL_INFO:
		mtin = LDR_DLT_MTIN_INFO;
		break;
	case LDR_LEVEL_VERBOSE:
		mtin = LDR_DLT_MTIN_VERBOSE;
		break;
	default:
		mtin = LDR_DLT_MTIN_DEBUG;
		break;
	}

	return mtin;
}

const char *ldr_dlt_level_prefix(int level)
{
	const char *prefix;

	switch (level) {
	case LDR_LEVEL_ERROR:
		prefix = "E:";
		break;
	case LDR_LEVEL_NOTICE:
		prefix = "N:";
		break;
	case LDR_LEVEL_WARNING:
		prefix = "W:";
		break;
	case LDR_LEVEL_INFO:
		prefix = "I:";
		break;
	case LDR_LEVEL_VERBOSE:
		prefix = "V:";
		break;
	default:
		prefix = "?:";
		break;
	}

	return prefix;
}

int ldr_dlt_log_start(LdrDltMsg *msg, uint8_t *buf, size_t buf_size,
		      const LdrDltLogParams *params)
{
	UserProtocolHeader *usr_hdr;
	StandardHeader *std_hdr;
	StandardHeaderExtra *extra_hdr;
	ExtendedHeader *ext_hdr;
	uint32_t type_info = LDR_TYPE_INFO_STRG | LDR_SCOD_ASCII;
	const char *prefix;
	size_t prefix_len;
	size_t text_max;
	uint8_t msin;

	if ((msg == NULL) || (buf == NULL) || (params == NULL)) {
		return -1;
	}

	if (buf_size <= LDR_DLT_TEXT_OFFSET) {
		return -1;
	}

	text_max = buf_size - LDR_DLT_TEXT_OFFSET;

	prefix = ldr_dlt_level_prefix(params->level);
	prefix_len = ldr_dlt_strlen(prefix);

	/* Need room for the prefix plus at least one byte of message. */
	if (text_max <= prefix_len) {
		return -1;
	}

	/* LDR transport envelope: "DUH\x01" plus the message type tag. */
	usr_hdr = (UserProtocolHeader *)buf;
	usr_hdr->pattern[0] = 'D';
	usr_hdr->pattern[1] = 'U';
	usr_hdr->pattern[2] = 'H';
	usr_hdr->pattern[3] = 1;
	usr_hdr->message = LDR_USER_MESSAGE_LOG;

	/* Standard header. 'len' is patched by ldr_dlt_log_finish(). */
	std_hdr = (StandardHeader *)(buf + sizeof(UserProtocolHeader));
	std_hdr->htyp = (uint8_t)LDR_DLT_HTYP;
	std_hdr->mcnt = params->mcnt;
	std_hdr->len = 0;

	/* ECU id, session id and timestamp, all big-endian on the wire. */
	extra_hdr = (StandardHeaderExtra *)(buf + sizeof(UserProtocolHeader) +
					    sizeof(StandardHeader));
	ldr_dlt_copy_id(extra_hdr->ecu, params->ecuid);
	extra_hdr->seid = LDR_HTOBE_32(params->seid);
	extra_hdr->tmsp = LDR_HTOBE_32(params->tmsp);

	/* Extended header: verbose mode, log type, mapped level, apid/ctid. */
	ext_hdr = (ExtendedHeader *)(buf + sizeof(UserProtocolHeader) +
				     sizeof(StandardHeader) +
				     sizeof(StandardHeaderExtra));
	msin = (uint8_t)(((uint32_t)LDR_TYPE_LOG << LDR_MSIN_MSTP_SHIFT) |
			 (((uint32_t)ldr_dlt_level_to_mtin(params->level) <<
			   LDR_MSIN_MTIN_SHIFT) & LDR_MSIN_MTIN));
	msin |= LDR_MSIN_VERB;
	ext_hdr->msin = msin;
	ext_hdr->noar = 1U;
	ldr_dlt_copy_id((uint8_t *)ext_hdr->apid, params->apid);
	ldr_dlt_copy_id((uint8_t *)ext_hdr->ctid, params->ctid);

	/* Payload argument type info; its length is patched in finish(). */
	LDR_MEMCPY(buf + LDR_HDR_TOTAL, &type_info, sizeof(type_info));

	/* Level tag first, so raw memory dumps stay readable. */
	LDR_MEMCPY(buf + LDR_DLT_TEXT_OFFSET, prefix, prefix_len);

	msg->buf = buf;
	msg->buf_size = buf_size;
	msg->text = (char *)(buf + LDR_DLT_TEXT_OFFSET + prefix_len);
	msg->text_capacity = text_max - prefix_len;
	msg->text_used = prefix_len;

	return 0;
}

size_t ldr_dlt_log_finish(LdrDltMsg *msg, size_t appended_len)
{
	StandardHeader *std_hdr;
	size_t text_max;
	uint16_t text_len;
	size_t total;

	if ((msg == NULL) || (msg->buf == NULL)) {
		return 0U;
	}

	text_max = msg->buf_size - LDR_DLT_TEXT_OFFSET;

	/*
	 * A length at or past the capacity means the text was cut and fills
	 * the buffer. Otherwise count the NUL, which the argument length
	 * includes.
	 */
	if (appended_len >= msg->text_capacity) {
		text_len = (uint16_t)text_max;
	} else {
		text_len = (uint16_t)(msg->text_used + appended_len + 1U);
	}

	total = LDR_DLT_TEXT_OFFSET + (size_t)text_len;

	/* Payload argument length, host byte order (see the file header). */
	LDR_MEMCPY(msg->buf + LDR_HDR_TOTAL + sizeof(uint32_t), &text_len,
		   sizeof(text_len));

	/* Whole-record length, big-endian, envelope included. */
	std_hdr = (StandardHeader *)(msg->buf + sizeof(UserProtocolHeader));
	std_hdr->len = (int16_t)LDR_HTOBE_16((uint16_t)total);

	return total;
}

size_t ldr_dlt_log_build(uint8_t *buf, size_t buf_size,
			 const LdrDltLogParams *params,
			 const char *text, size_t text_len)
{
	LdrDltMsg msg;
	size_t copy_len;

	if (text == NULL) {
		return 0U;
	}

	if (ldr_dlt_log_start(&msg, buf, buf_size, params) != 0) {
		return 0U;
	}

	/* Leave room for the NUL that log_finish() accounts for. */
	copy_len = text_len;
	if (copy_len >= msg.text_capacity) {
		copy_len = msg.text_capacity - 1U;
	}

	LDR_MEMCPY(msg.text, text, copy_len);
	msg.text[copy_len] = '\0';

	return ldr_dlt_log_finish(&msg, copy_len);
}
