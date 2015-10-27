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

#include <stddef.h>

#include <assert.h>
#include <auth.h>
#include <debug.h>
#include <platform.h>
#include <platform_def.h>
#include <mmio.h>

#include "rcarboot.h"

#if MASTER_BOOT_CPU == RCAR_BOOT_CA5X
#define RCAR_BOOT_KEY_CERT	(0xE6300C00U)
#elif MASTER_BOOT_CPU == RCAR_BOOT_CR7
#define RCAR_BOOT_KEY_CERT	(0xE6300400U)
#endif
#define RCAR_SBROM_API		(0xeb10dd64U)
#define	RST_BASE		(0xE6160000U)
#define	RST_MODEMR		(RST_BASE + 0x0060U)
#define LIFEC_CC_LCS		(0xE6110028U)		/* cc_lcs  Life cycle state read */
#define	MFISSOFTMDR		(0xE6260600U)		/* SOFTMD register */

static SECURE_BOOT_API	sbrom_SecureBootAPI;

static int32_t rcarboot_mod_verify(uint32_t id, uintptr_t obj, size_t len);
static int32_t rcarboot_mod_init(void);
#if !RCAR_SECURE_BOOT
static int32_t local_verify(uint32_t a, uint32_t b, void *c);
#endif

/*
 * Object verification function
 *
 * The id parameter will indicate the expected format of the object
 * (certificate, image, etc).
 *
 * Return: 0 = success, Otherwise = error
 */
static int32_t rcarboot_mod_verify(uint32_t id, uintptr_t obj, size_t len)
{
	int32_t ret = 0;
	uint32_t cert_addr;

	switch (id) {
#if IMAGE_BL2
	case (uint32_t)AUTH_TRUSTED_KEY_CERT:
	case (uint32_t)AUTH_BL31_KEY_CERT:
	case (uint32_t)AUTH_BL32_KEY_CERT:
	case (uint32_t)AUTH_BL33_KEY_CERT:
	case (uint32_t)AUTH_BL31_IMG_CERT:
	case (uint32_t)AUTH_BL32_IMG_CERT:
	case (uint32_t)AUTH_BL33_IMG_CERT:
		/* no check */
		break;
	case (uint32_t)AUTH_BL31_IMG:
		ret = file_to_cert((const int8_t *)BL31_CERT_NAME, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
		}
		break;
	case (uint32_t)AUTH_BL32_IMG:
		ret = file_to_cert((const int8_t *)BL32_CERT_NAME, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
		}
		break;
	case (uint32_t)AUTH_BL33_IMG:
		ret = file_to_cert((const int8_t *)BL33_CERT_NAME, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
		}
		break;
#endif /* IMAGE_BL2 */
	default:
		ret = -1;
		break;
	}

	return ret;
}

static int32_t local_verify(uint32_t a, uint32_t b, void *c)
{
	return 0;
}

/*
 * Module initialization function
 *
 * Return: 0 = success, Otherwise = error
 */
static int32_t rcarboot_mod_init(void)
{
#if RCAR_SECURE_BOOT
	uint32_t lcs = (mmio_read_32(LIFEC_CC_LCS) & 0x000000F0U) >> 4;
	uint32_t md = (mmio_read_32(RST_MODEMR) & 0x00000020U) >> 5;
	uint32_t softmd = (mmio_read_32(MFISSOFTMDR) & 0x00000001U);

	/* default is Secure boot */
	sbrom_SecureBootAPI = (SECURE_BOOT_API)RCAR_SBROM_API;

	if (lcs == 0x5U) { /* LCS=Secure */
		if (softmd == 0x1U) {
			sbrom_SecureBootAPI = &local_verify; /* LCS=Secure + Normal boot (temp setting) */
		}  /* else is LCS=Secure + Secure boot */
	} else if (md != 0U) {
		sbrom_SecureBootAPI = &local_verify; /* MD5=1 => LCS=CM/DM + Normal boot */
	} /* else is MD5=0 => LCS=CM/DM + Secure boot */

	if (sbrom_SecureBootAPI == &local_verify) {
		NOTICE("BL2: Normal boot\n");
	} else {
		NOTICE("BL2: Secure boot\n");
	}
#else
	sbrom_SecureBootAPI = &local_verify;
	NOTICE("BL2: Normal boot\n");
#endif
	return 0;
}

const auth_mod_t auth_mod = {
	.name = "RCarSecureBoot",
	.init = &rcarboot_mod_init,
	.verify = &rcarboot_mod_verify
};
