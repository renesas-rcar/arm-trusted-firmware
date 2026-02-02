/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR DLT wire format, copied from the LDR DLT protocol library
 * (ldr_protocol_types_internal.h, ldr_protocol_usr_msg.h, r_ldr_types.h),
 * whose headers need the OS-side LDR client API. Keep in sync with it.
 */

#ifndef LDR_DLT_WIRE_H
#define LDR_DLT_WIRE_H

#include <stdint.h>

#include "ldr_port.h"

#define LDR_ID_SIZE			4U

/* User message type of a log record. */
#define LDR_USER_MESSAGE_LOG		1U

/* Host (little-endian) to big-endian. */
#define LDR_SWAP_16(x)							\
	((uint16_t)((((x) >> 8) & 0xffU) | (((x) << 8) & 0xff00U)))
#define LDR_SWAP_32(x)							\
	((uint32_t)((((x) >> 24) & 0xffU) | (((x) << 8) & 0xff0000U) |	\
		    (((x) >> 8) & 0xff00U) | (((x) << 24) & 0xff000000U)))
#define LDR_HTOBE_16(x)			LDR_SWAP_16(x)
#define LDR_HTOBE_32(x)			LDR_SWAP_32(x)

/* Standard header type (htyp) flags. */
#define LDR_HTYP_UEH			0x01	/* Extended header present */
#define LDR_HTYP_WEID			0x04	/* ECU ID present */
#define LDR_HTYP_WSID			0x08	/* Session ID present */
#define LDR_HTYP_WTMS			0x10	/* Timestamp present */
#define LDR_HTYP_PROTOCOL_VERSION1	(1 << 5)

/* Message type and message info (msin) field. */
#define LDR_TYPE_LOG			0x00
#define LDR_MSIN_VERB			0x01
#define LDR_MSIN_MTIN			0xf0
#define LDR_MSIN_MSTP_SHIFT		1
#define LDR_MSIN_MTIN_SHIFT		4

/* Payload argument type info: ASCII string. */
#define LDR_TYPE_INFO_STRG		0x00000200U
#define LDR_SCOD_ASCII			0x00000000U

/* Wire structures, packed: the layout is the sum of the field sizes. */
typedef struct {
	char pattern[LDR_ID_SIZE];	/* "DUH\x01" */
	uint32_t message;		/* LDR_USER_MESSAGE_*, host order */
} LDR_PACKED UserProtocolHeader;

typedef struct {
	uint8_t htyp;
	uint8_t mcnt;
	int16_t len;			/* Big-endian */
} LDR_PACKED StandardHeader;

typedef struct {
	uint8_t ecu[LDR_ID_SIZE];
	uint32_t seid;			/* Big-endian */
	uint32_t tmsp;			/* Big-endian, 100 us units */
} LDR_PACKED StandardHeaderExtra;

typedef struct {
	uint8_t msin;
	uint8_t noar;
	char apid[LDR_ID_SIZE];
	char ctid[LDR_ID_SIZE];
} LDR_PACKED ExtendedHeader;

#endif /* LDR_DLT_WIRE_H */
