/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
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

#define FUSE_DUMMY5	(0xE60603E8U)	/* Fuse dummy5 */ 
#define	M3WSMASK	(0x1C000000U)	/* Dummy5[28:26] */
#define	M3WS100		(0x00000000U)	/* M3 WS1.0  */
#define	M3WS105		(0x04000000U)	/* M3 WS1.05 */
#define	M3WS106		(0x08000000U)	/* M3 WS1.06 */


#define OLD_API_TABLE1	(0U)		/* H3 WS1.0/WS1.1 */
#define OLD_API_TABLE2	(1U)		/* H3 WS2.0 */
#define OLD_API_TABLE3	(2U)		/* M3 WS1.0/1.05 */
#define NEW_API_TABLE	(3U)		/* M3 WS1.06 or later, M3N, E3 */
#define API_TABLE_MAX	(4U)		/* table max */



uint32_t ROM_SecureBootAPI( uint32_t *pKeyCert,
			   uint32_t *pContentCert,
			   ROM_FuncReadFlash pFuncReadFlash)
{
	/* Secure Boot API address table */
	static const uintptr_t ROM_SecureBootAPI_table[API_TABLE_MAX] = {
		0xEB10DD64U,		/* H3 WS1.0/WS1.1 */
		0xEB116ED4U,		/* H3 WS2.0 */
		0xEB1102FCU,		/* M3 WS1.0/1.05 */
		0xEB100180U		/* M3 WS1.06 or later, M3N, E3 */
	};

	ROM_SECURE_BOOT_API func;
	uint32_t index;

	index = get_table_index();
	func = (ROM_SECURE_BOOT_API)ROM_SecureBootAPI_table[index];

	return func(pKeyCert, pContentCert, pFuncReadFlash);
}


uint32_t ROM_GetLcs(uint32_t *pLcs)
{
	/* Get LCS stete API address table */
	static const uintptr_t ROM_GetLcs_table[API_TABLE_MAX] = {
		0xEB10DFE0U,		/* H3 WS1.0/WS1.1 */
		0xEB117150U,		/* H3 WS2.0 */
		0xEB110578U,		/* M3 WS1.0/1.05 */
		0xEB10018CU		/* M3 WS1.06 or later, M3N, E3 */
	};

	ROM_GETLCS_API func;
	uint32_t index;

	index = get_table_index();
	func = (ROM_GETLCS_API)ROM_GetLcs_table[index];

	return func(pLcs);
}


static uint32_t get_table_index(void)
{
	uint32_t product;
	uint32_t cut_ver;
	uint32_t dummy5;
	uint32_t index;

	product = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
	cut_ver = mmio_read_32(RCAR_PRR) & RCAR_CUT_MASK;

	switch (product) {
	case RCAR_PRODUCT_H3:
		if (cut_ver == RCAR_CUT_ES10) {
			index = OLD_API_TABLE1;	/* H3 WS1.0 */
		} else if (cut_ver == RCAR_CUT_ES11) {
			index = OLD_API_TABLE1;	/* H3 WS1.1 */
		} else if (cut_ver == RCAR_CUT_ES20) {
			index = OLD_API_TABLE2;	/* H3 WS2.0 */
		} else {
			index = NEW_API_TABLE;	/* Later than H3 WS2.0 */
		}
		break;
	case RCAR_PRODUCT_M3:
		if (cut_ver == RCAR_CUT_ES10) {
			dummy5 = mmio_read_32(FUSE_DUMMY5) & M3WSMASK;
			if (dummy5 == M3WS100) {
				index = OLD_API_TABLE3;	/* M3 WS1.0 */
			}else if (dummy5 == M3WS105) {
				index = OLD_API_TABLE3;	/* M3 WS1.05 */
			} else {
				index = NEW_API_TABLE;	/* M3 WS1.06 */
			}
		} else {
			index = NEW_API_TABLE;	/* M3 WS1.1 or later */
		}
		break;
	default:
		index = NEW_API_TABLE;		/* assume that M3N and E3 */
		break;
	}

	return index;
}
