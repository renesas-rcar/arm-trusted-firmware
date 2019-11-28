/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <iic_dvfs.h>
#include "board.h"


/************************************************************************
 * Defines
 ************************************************************************/
#ifndef BOARD_DEFAULT
#if (RCAR_LSI == RCAR_E3)
#define BOARD_DEFAULT		(BOARD_EBISU << BOARD_CODE_SHIFT)
#else  /* (RCAR_LSI == RCAR_E3) */
#define BOARD_DEFAULT		(BOARD_SALVATOR_X << BOARD_CODE_SHIFT)
#endif /* (RCAR_LSI == RCAR_E3) */
#endif

#define SLAVE_ADDR_EEPROM	(0x50U)
#define	REG_ADDR_BOARD_ID	(0x70U)

#define BOARD_CODE_MASK		(0xF8U)
#define BOARD_REV_MASK		(0x07U)
#define BOARD_CODE_SHIFT	(3U)

#define BOARD_ID_UNKNOWN	(0xFFU)


/************************************************************************
 * Global variables
 ************************************************************************/
const char *g_board_tbl[] = {
	[BOARD_SALVATOR_X]	= "Salvator-X",
	[BOARD_KRIEK]		= "Kriek",
	[BOARD_STARTER_KIT]	= "Starter Kit",
	[BOARD_SALVATOR_XS]	= "Salvator-XS",
	[BOARD_EBISU]		= "Ebisu",
	[BOARD_STARTER_KIT_PRE]	= "Starter Kit",
	[BOARD_EBISU_4D]	= "Ebisu-4D",
	[BOARD_UNKNOWN]		= "unknown"
};


int32_t get_board_type(uint32_t *type, uint32_t *rev)
{
	int32_t ret = 0;
	uint8_t read_rev;
	static uint8_t g_board_id = BOARD_ID_UNKNOWN;
	const uint8_t board_tbl[][8U] = {
		[BOARD_SALVATOR_X]	= {0x10U, 0x11U, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_KRIEK]		= {0x10U, 0xFFU, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_STARTER_KIT]	= {0x10U, 0x30U, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_SALVATOR_XS]	= {0x10U, 0xFFU, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_EBISU]		= {0x10U, 0xFFU, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_STARTER_KIT_PRE]	= {0x10U, 0x10U, 0x20U, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
		[BOARD_EBISU_4D]	= {0x10U, 0xFFU, 0xFFU, 0xFFU,
					   0xFFU, 0xFFU, 0xFFU, 0xFFU},
	};

	if (BOARD_ID_UNKNOWN == g_board_id) {
#if PMIC_ROHM_BD9571
		/* Board ID detection from EEPROM */
		ret = rcar_iic_dvfs_recieve(SLAVE_ADDR_EEPROM,
			REG_ADDR_BOARD_ID, &g_board_id);
		if (0 != ret) {
			g_board_id = BOARD_ID_UNKNOWN;
		} else if (BOARD_ID_UNKNOWN == g_board_id) {
			/* Can't recognize the board */
			g_board_id = BOARD_DEFAULT;
		} else {
			/* none */
		}
#else	/* PMIC_ROHM_BD9571 */
		g_board_id = BOARD_DEFAULT;
#endif	/* PMIC_ROHM_BD9571 */
	}

	*type = ((uint32_t)g_board_id & BOARD_CODE_MASK) >> BOARD_CODE_SHIFT;
	if (*type < (sizeof(board_tbl) / sizeof(board_tbl[0]))) {
		read_rev = (uint8_t)(g_board_id & BOARD_REV_MASK);
		*rev = board_tbl[*type][read_rev];
	} else {
		/* If there is no revision information, set Rev0.0. */
		*rev = 0x00U;
	}

	return ret;
}
