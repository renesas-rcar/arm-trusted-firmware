/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
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
