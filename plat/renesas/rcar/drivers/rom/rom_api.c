/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <mmio.h>
#include "rcar_def.h"
#include "rom_api.h"


typedef uint32_t (*ROM_SECURE_BOOT_API)(uint32_t *pKeyCert,
					uint32_t *pContentCert,
					ROM_FuncReadFlash pFuncReadFlash);
typedef uint32_t (*ROM_GETLCS_API)(uint32_t *pLcs);

static uint32_t get_table_index(void);


#define OLD_API_TABLE1	(0U)	/* H3 Ver.1.0/Ver.1.1 */
#define OLD_API_TABLE2	(1U)	/* H3 Ver.2.0 */
#define OLD_API_TABLE3	(2U)	/* M3 Ver.1.0 */
#define NEW_API_TABLE	(3U)	/* H3 Ver.3.0, M3 Ver.1.1 or later, M3N, E3, */
				/* V3M WS2.0 */
#define NEW_API_TABLE2	(4U)	/* V3M WS1.0 */
#define NEW_API_TABLE3	(5U)	/* V3H WS1.0 */
#define API_TABLE_MAX	(6U)	/* table max */



uint32_t ROM_SecureBootAPI( uint32_t *pKeyCert,
			   uint32_t *pContentCert,
			   ROM_FuncReadFlash pFuncReadFlash)
{
	/* Secure Boot API address table */
	static const uintptr_t ROM_SecureBootAPI_table[API_TABLE_MAX] = {
		0xEB10DD64U,	/* H3 Ver.1.0/Ver.1.1 */
		0xEB116ED4U,	/* H3 Ver.2.0 */
		0xEB1102FCU,	/* M3 Ver.1.0 */
		0xEB100180U,	/* H3 Ver.3.0, M3 Ver.1.1 or later, M3N, E3, */
				/* V3M WS2.0 */
		0xEB110128U,	/* V3M WS1.0 */
		0xEB101960U,	/* V3H WS1.0 */
	};

	ROM_SECURE_BOOT_API func;
	uint32_t index;

	index = get_table_index();
	func = (ROM_SECURE_BOOT_API)ROM_SecureBootAPI_table[index];

	return func(pKeyCert, pContentCert, pFuncReadFlash);
}


uint32_t ROM_GetLcs(uint32_t *pLcs)
{
#if RCAR_LSI == RCAR_V3H
	*pLcs = 0xff;
	return 0;
#else
	/* Get LCS stete API address table */
	static const uintptr_t ROM_GetLcs_table[API_TABLE_MAX] = {
		0xEB10DFE0U,	/* H3 Ver.1.0/Ver.1.1 */
		0xEB117150U,	/* H3 Ver.2.0 */
		0xEB110578U,	/* M3 Ver.1.0 */
		0xEB10018CU,	/* H3 Ver.3.0, M3 Ver.1.1 or later, M3N, E3, */
				/* V3M WS2.0 */
		0xEB1103A4U,	/* V3M WS1.0 */
		0xEB101940U,	/* V3H WS1.0 */
	};

	ROM_GETLCS_API func;
	uint32_t index;

	index = get_table_index();
	func = (ROM_GETLCS_API)ROM_GetLcs_table[index];

	return func(pLcs);
#endif
}


static uint32_t get_table_index(void)
{
	uint32_t product;
	uint32_t cut_ver;
	uint32_t index;

	product = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
	cut_ver = mmio_read_32(RCAR_PRR) & RCAR_CUT_MASK;

	switch (product) {
	case RCAR_PRODUCT_H3:
		if (cut_ver == RCAR_CUT_VER10) {
			index = OLD_API_TABLE1;	/* H3 Ver.1.0 */
		} else if (cut_ver == RCAR_CUT_VER11) {
			index = OLD_API_TABLE1;	/* H3 Ver.1.1 */
		} else if (cut_ver == RCAR_CUT_VER20) {
			index = OLD_API_TABLE2;	/* H3 Ver.2.0 */
		} else {
			index = NEW_API_TABLE;	/* Later than H3 Ver.2.0 */
		}
		break;
	case RCAR_PRODUCT_M3:
		if (cut_ver == RCAR_CUT_VER10) {
			index = OLD_API_TABLE3;	/* M3 Ver.1.0 */
		} else {
			index = NEW_API_TABLE;	/* M3 Ver.1.1 or later */
		}
		break;
	case RCAR_PRODUCT_V3M:
		if (cut_ver == RCAR_CUT_VER10) {
			index = NEW_API_TABLE2;	/* V3M WS1.0 */
		} else {
			index = NEW_API_TABLE;	/* V3M WS2.0 or later */
		}
		break;
	case RCAR_PRODUCT_V3H:
		if (cut_ver == RCAR_CUT_VER10) {
			index = NEW_API_TABLE3;	/* V3H WS1.0 */
		} else {
			index = NEW_API_TABLE3;	/* V3H WS2.0 or later */
		}
		break;
	default:
		index = NEW_API_TABLE;		/* M3N and E3 */
		break;
	}

	return index;
}
