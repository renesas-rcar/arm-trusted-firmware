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
#ifndef _RCAR_BOARD_H_
#define _RCAR_BOARD_H_

/* board ID register in the EEPROM (e.g. BR24T01FVM-W)
 *
 *   7        3   2    0
 * +------------+--------+
 * |  name ID   | rev ID |
 * +------------+--------+
 */
#define PMIC_EEPROM_SLAVE_ADDR		(0x50U)
#define	PMIC_EEPROM_BOARD_ID_REG_ADDR	(0x70U)

/* board ID bit fields definition */
#define BOARD_ID_NAME_MASK	(0xF8U)
#define BOARD_ID_NAME_SHIFT 	(3U)
#define BOARD_ID_REV_MASK	(0x07U)

/* board name ID */
#define BOARD_SALVATOR_X	(0x00U)
#define BOARD_KRIEK		(0x01U)
#define BOARD_STARTER_KIT	(0x02U) /* i.e. ULCB */
#define BOARD_SALVATOR_XS	(0x04U)
#define BOARD_UNKNOWN		(0x1FU)

/* If board ID is not specified during compiling, or board detection fails,
 * then fallback to Salvator-X Rev1.0  */
#define BOARD_ID_DEFAULT 	(BOARD_SALVATOR_X << BOARD_ID_NAME_SHIFT)
#define BOARD_ID_UNKNOWN	(0xFFU)

#define board_is(board) ((get_board_id() >> BOARD_ID_NAME_SHIFT) == board)

void board_id_init();
int32_t get_board_id();
const char *get_board_name();
int32_t get_board_name_id();
int32_t get_board_rev_major();
int32_t get_board_rev_minor();

#endif /* _RCAR_BOARD_H_ */
