/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOARD_H_
#define BOARD_H_


/************************************************************************
 * Board type
 ************************************************************************/
#define BOARD_SALVATOR_X		(0x00U)
#define BOARD_KRIEK			(0x01U)
#define BOARD_STARTER_KIT		(0x02U)
#define BOARD_SALVATOR_XS		(0x04U)
#define BOARD_STARTER_KIT_PRE		(0x0BU)
#define BOARD_UNKNOWN			(BOARD_STARTER_KIT_PRE+1U)

#define BOARD_REV_UNKNOWN		(0xFFU)

/************************************************************************
 * Board name
 ************************************************************************/
extern const char *g_board_tbl[];

/************************************************************************
 * Revisions are expressed in 8 bits.
 *  The upper 4 bits are major version.
 *  The lower 4 bits are minor version.
 ************************************************************************/
#define GET_BOARD_MAJOR(a)	((uint32_t)(a) >> 4U)
#define GET_BOARD_MINOR(a)	((uint32_t)(a) & 0xFU)

#define GET_BOARD_NAME(a)	(g_board_tbl[(a)])

int32_t get_board_type(uint32_t *type, uint32_t *rev);

#endif /* BOARD_H_ */
