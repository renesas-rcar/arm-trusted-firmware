/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdint.h>
#include <iic_dvfs.h>
#include "board.h"

static uint8_t board_id = BOARD_ID_UNKNOWN;

void board_id_init()
{
	uint8_t id = BOARD_ID_UNKNOWN;
	int ret;

	/* board detection and validation had been done already */
	if (board_id != BOARD_ID_UNKNOWN)
		return;

#if PMIC_ON_BOARD && !defined(BOARD_ID)
	/* Board ID detection from EEPROM */
	ret = rcar_iic_dvfs_recieve(PMIC_EEPROM_SLAVE_ADDR,
				PMIC_EEPROM_BOARD_ID_REG_ADDR, &id);
	if (ret != 0)
		id = BOARD_ID_UNKNOWN;
#endif

#ifdef BOARD_ID
	id = BOARD_ID;
#endif
	if (id == BOARD_ID_UNKNOWN)
		id = BOARD_ID_DEFAULT;

	switch (id >> BOARD_ID_NAME_SHIFT) {
	case BOARD_SALVATOR_X:
	case BOARD_KRIEK:
	case BOARD_STARTER_KIT:
	case BOARD_SALVATOR_XS:
		break;
	default:
		id = BOARD_ID_DEFAULT;
		break;
	}

	board_id = id;
}

static const char *board_name[] = {
	[BOARD_SALVATOR_X]	= "Salvator-X",
	[BOARD_KRIEK]		= "Kriek",
	[BOARD_STARTER_KIT]	= "Starter Kit",
	[BOARD_SALVATOR_XS]	= "Salvator-XS"
};

int32_t get_board_id()
{
	return board_id;
}

const char *get_board_name()
{
	return board_name[board_id];
}

/* board rev ID is not linearly mapped to revision string
 *   0 : rev1.0
 *   1 : rev1.1
 *   ...
 *
 * rev ID translation is done via lookup table as below
 *
 *  7      4 3      0
 * +--------+--------+
 * | major  | minor  |
 * +--------+--------+
 */
static const uint8_t board_rev[] = {
	/* 0     1      2      3 */
	0x10U, 0x11U, 0xFFU, 0xFFU,
	/* 4     5      6      7 */
	0xFFU, 0xFFU, 0xFFU, 0xFFU
};

int32_t get_board_rev()
{
	int32_t rev = board_id & BOARD_ID_REV_MASK;

	return board_rev[rev];
}

int32_t get_board_rev_major()
{
	int32_t rev = board_id & BOARD_ID_REV_MASK;

	return board_rev[rev] >> 4;
}

int32_t get_board_rev_minor()
{
	int32_t rev = board_id & BOARD_ID_REV_MASK;

	return board_rev[rev] & 0xFU;
}
