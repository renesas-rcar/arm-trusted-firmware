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
#include <auth_mod.h>
#include <debug.h>
#include <platform.h>
#include <platform_def.h>
#include <mmio.h>

#include "rcarboot.h"

#if RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CA5X
#define RCAR_BOOT_KEY_CERT	(0xE6300C00U)
#elif RCAR_MASTER_BOOT_CPU == RCAR_BOOT_CR7
#define RCAR_BOOT_KEY_CERT	(0xE6300400U)
#endif
#if RCAR_LSI == RCAR_H3
#define RCAR_SBROM_API		(0xeb10dd64U)
#elif RCAR_LSI == RCAR_M3
#define RCAR_SBROM_API		(0xeb1102fcU)
#endif
#define	RST_BASE		(0xE6160000U)
#define	RST_MODEMR		(RST_BASE + 0x0060U)
#define LIFEC_CC_LCS		(0xE6110028U)		/* cc_lcs  Life cycle state read */
#define	MFISSOFTMDR		(0xE6260600U)		/* SOFTMD register */

static SECURE_BOOT_API	sbrom_SecureBootAPI;

#if !RCAR_SECURE_BOOT
static int32_t local_verify(uint32_t a, uint32_t b, void *c);
#endif

/*
 * Return the parent id in the output parameter '*parent_id'
 *
 * Return value:
 *   0 = Image has parent, 1 = Image has no parent or parent is authenticated
 */
int auth_mod_get_parent_id(unsigned int img_id, unsigned int *parent_id)
{
	return 1;
}

/*
 * Authenticate a certificate/image
 *
 * Return: 0 = success, Otherwise = error
 */
int auth_mod_verify_img(unsigned int img_id, void *img_ptr,
			unsigned int img_len)
{
	int32_t ret = 0;
	uint32_t cert_addr;

	switch (img_id) {
#if IMAGE_BL2
	case (uint32_t)TRUSTED_KEY_CERT_ID:
	case (uint32_t)BL31_KEY_CERT_ID:
	case (uint32_t)BL32_KEY_CERT_ID:
	case (uint32_t)BL33_KEY_CERT_ID:
	case (uint32_t)BL332_KEY_CERT_ID:
	case (uint32_t)BL333_KEY_CERT_ID:
	case (uint32_t)BL334_KEY_CERT_ID:
	case (uint32_t)BL335_KEY_CERT_ID:
	case (uint32_t)BL336_KEY_CERT_ID:
	case (uint32_t)BL337_KEY_CERT_ID:
	case (uint32_t)BL338_KEY_CERT_ID:
	case (uint32_t)BL31_CERT_ID:
	case (uint32_t)BL32_CERT_ID:
	case (uint32_t)BL33_CERT_ID:
	case (uint32_t)BL332_CERT_ID:
	case (uint32_t)BL333_CERT_ID:
	case (uint32_t)BL334_CERT_ID:
	case (uint32_t)BL335_CERT_ID:
	case (uint32_t)BL336_CERT_ID:
	case (uint32_t)BL337_CERT_ID:
	case (uint32_t)BL338_CERT_ID:
		/* no check */
		break;
	case (uint32_t)BL31_IMAGE_ID:
		ret = file_to_cert(BL31_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL31\n", ret);
			}
		}
		break;
	case (uint32_t)BL32_IMAGE_ID:
		ret = file_to_cert(BL32_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL32\n", ret);
			}
		}
		break;
	case (uint32_t)BL33_IMAGE_ID:
		ret = file_to_cert(BL33_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL33\n", ret);
			}
		}
		break;
	case (uint32_t)BL332_IMAGE_ID:
		ret = file_to_cert(BL332_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL332\n", ret);
			} 
		}
		break;
	case (uint32_t)BL333_IMAGE_ID:
		ret = file_to_cert(BL333_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL333\n", ret);
			} 
		}
		break;
	case (uint32_t)BL334_IMAGE_ID:
		ret = file_to_cert(BL334_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL334\n", ret);
			} 
		}
		break;
	case (uint32_t)BL335_IMAGE_ID:
		ret = file_to_cert(BL335_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL335\n", ret);
			} 
		}
		break;
	case (uint32_t)BL336_IMAGE_ID:
		ret = file_to_cert(BL336_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL336\n", ret);
			} 
		}
		break;
	case (uint32_t)BL337_IMAGE_ID:
		ret = file_to_cert(BL337_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL337\n", ret);
			} 
		}
		break;
	case (uint32_t)BL338_IMAGE_ID:
		ret = file_to_cert(BL338_CERT_ID, &cert_addr);
		if (0 == ret) {
			ret = sbrom_SecureBootAPI(RCAR_BOOT_KEY_CERT, cert_addr, NULL);
			if (0 != ret) {
				ERROR("Verification Failed!!! 0x%x -> BL338\n", ret);
			} 
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
 * Initialize the different modules in the authentication framework
 */
void auth_mod_init(void)
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
}
