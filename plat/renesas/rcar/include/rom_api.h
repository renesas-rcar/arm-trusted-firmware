/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ROM_API_H__
#define ROM_API_H__

#include <stdint.h>

#define SBROM_OK				(0x00000000U)
#define SBROM_ILLEGAL_INPUT_PARAM_ERR		(0x0B000001U)
#define SBROM_ILLEGAL_OEM_HASH_VALUE_ERR	(0x0B000008U)
#define SBROM_ILLEGAL_LCS_FOR_OPERATION_ERR	(0x0B000010U)
#define SBROM_HASH_NOT_PROGRAMMED_ERR		(0x0B000100U)
#define SBROM_PUB_KEY_HASH_VALIDATION_FAILURE	(0xF1000006U)
#define SBROM_RSA_SIG_VERIFICATION_FAILED	(0xF1000007U)

#define LCS_CM					(0x0U)	/* CM */
#define LCS_DM					(0x1U)	/* DM */
#define LCS_SD					(0x3U)	/* SD */
#define LCS_SE					(0x5U)	/* SE */
#define LCS_FA					(0x7U)	/* FA */

typedef uint32_t (*ROM_FuncReadFlash)(uint64_t srcAddr,
				      uint8_t *pDest, 
				      uint32_t sizeToRead);

uint32_t ROM_SecureBootAPI(uint32_t *pKeyCert, 
			   uint32_t *pContentCert,
			   ROM_FuncReadFlash pFuncReadFlash);
uint32_t ROM_GetLcs(uint32_t *pLcs);


#endif /* ROM_API_H__ */
