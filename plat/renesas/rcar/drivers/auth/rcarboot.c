/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>

#include <arch_helpers.h>
#include <auth_mod.h>
#include <debug.h>
#include <platform.h>
#include <platform_def.h>
#include <mmio.h>

#include "rcarboot.h"
#include "rom_api.h"

#define	RCAR_IMAGE_ID_MAX	(10)
#define	RCAR_CERT_MAGIC_NUM	(0xE291F358U)
#define RCAR_BOOT_KEY_CERT	(0xE6300C00U)
#define RCAR_BOOT_KEY_CERT_NEW	(0xE6300F00U)
#define	RST_BASE		(0xE6160000U)
#define	RST_MODEMR		(RST_BASE + 0x0060U)
#define	MFISOFTMDR		(0xE6260600U)		/* SOFTMD register */
#define	MODEMR_MD5_MASK		(0x00000020U)
#define	MODEMR_MD5_SHIFT	(5U)
#define	SOFTMD_BOOTMODE_MASK	(0x00000001U)
#define	SOFTMD_NORMALBOOT	(0x1U)

static SECURE_BOOT_API	sbrom_SecureBootAPI;

static int32_t local_verify(uint32_t a, uint32_t b, void *c);

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
	uint32_t cert_addr = 0U;
	int32_t index = 0;
	static const struct img_to_cert_t {
		uint32_t	image_id;
		int32_t		cert_id;
		const char	*image_name;
	} img_to_cert[RCAR_IMAGE_ID_MAX] = {
		{
			(uint32_t)BL31_IMAGE_ID,
			(int32_t)SOC_FW_CONTENT_CERT_ID,
			"BL31"
		},
		{
			(uint32_t)BL32_IMAGE_ID,
			(int32_t)TRUSTED_OS_FW_CONTENT_CERT_ID,
			"BL32"
		},
		{
			(uint32_t)BL33_IMAGE_ID,
			(int32_t)NON_TRUSTED_FW_CONTENT_CERT_ID,
			"BL33"
		},
		{
			(uint32_t)BL332_IMAGE_ID,
			(int32_t)BL332_CERT_ID,
			"BL332"
		},
		{
			(uint32_t)BL333_IMAGE_ID,
			(int32_t)BL333_CERT_ID,
			"BL333"
		},
		{
			(uint32_t)BL334_IMAGE_ID,
			(int32_t)BL334_CERT_ID,
			"BL334"
		},
		{
			(uint32_t)BL335_IMAGE_ID,
			(int32_t)BL335_CERT_ID,
			"BL335"
		},
		{
			(uint32_t)BL336_IMAGE_ID,
			(int32_t)BL336_CERT_ID,
			"BL336"
		},
		{
			(uint32_t)BL337_IMAGE_ID,
			(int32_t)BL337_CERT_ID,
			"BL337"
		},
		{
			(uint32_t)BL338_IMAGE_ID,
			(int32_t)BL338_CERT_ID,
			"BL338"
		},
	};

	switch (img_id) {
#if IMAGE_BL2
	case (uint32_t)TRUSTED_KEY_CERT_ID:
	case (uint32_t)SOC_FW_KEY_CERT_ID:
	case (uint32_t)TRUSTED_OS_FW_KEY_CERT_ID:
	case (uint32_t)NON_TRUSTED_FW_KEY_CERT_ID:
	case (uint32_t)BL332_KEY_CERT_ID:
	case (uint32_t)BL333_KEY_CERT_ID:
	case (uint32_t)BL334_KEY_CERT_ID:
	case (uint32_t)BL335_KEY_CERT_ID:
	case (uint32_t)BL336_KEY_CERT_ID:
	case (uint32_t)BL337_KEY_CERT_ID:
	case (uint32_t)BL338_KEY_CERT_ID:
	case (uint32_t)SOC_FW_CONTENT_CERT_ID:
	case (uint32_t)TRUSTED_OS_FW_CONTENT_CERT_ID:
	case (uint32_t)NON_TRUSTED_FW_CONTENT_CERT_ID:
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
	case (uint32_t)BL32_IMAGE_ID:
	case (uint32_t)BL33_IMAGE_ID:
	case (uint32_t)BL332_IMAGE_ID:
	case (uint32_t)BL333_IMAGE_ID:
	case (uint32_t)BL334_IMAGE_ID:
	case (uint32_t)BL335_IMAGE_ID:
	case (uint32_t)BL336_IMAGE_ID:
	case (uint32_t)BL337_IMAGE_ID:
	case (uint32_t)BL338_IMAGE_ID:
		{
			for (index = 0; index < RCAR_IMAGE_ID_MAX; index++) {
				if (img_id == img_to_cert[index].image_id) {
					ret = file_to_cert(
						img_to_cert[index].cert_id,
						&cert_addr);
					break;
				}
			}
			if (RCAR_IMAGE_ID_MAX == index) {
				ERROR("Verification Failed!!! (%s)\n",
					"Unknown img_id !");
				break;
			}
			if (0 == ret) {
#if RCAR_BL2_DCACHE == 1	/* clean and disable D-Cache */
				write_sctlr_el3(read_sctlr_el3() & ~SCTLR_C_BIT);
				dcsw_op_all(DCCISW);
#endif /* RCAR_BL2_DCACHE == 1 */
				if (RCAR_CERT_MAGIC_NUM ==
					mmio_read_32((uint64_t)RCAR_BOOT_KEY_CERT_NEW)) {
					ret = sbrom_SecureBootAPI(
						RCAR_BOOT_KEY_CERT_NEW,
						cert_addr, NULL);
				} else {
					ret = sbrom_SecureBootAPI(
						RCAR_BOOT_KEY_CERT,
						cert_addr, NULL);
				}
#if RCAR_BL2_DCACHE == 1	/* enable D-Cache */
				write_sctlr_el3(read_sctlr_el3() | SCTLR_C_BIT);
#endif /* RCAR_BL2_DCACHE == 1 */
				if (0 != ret) {
					ERROR(
					"Verification Failed!!! 0x%x -> %s\n",
						ret,
						img_to_cert[index].image_name);
				}
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
	uint32_t lcs;
	uint32_t md = (mmio_read_32(RST_MODEMR) & MODEMR_MD5_MASK)
							>> MODEMR_MD5_SHIFT;
	uint32_t softmd = (mmio_read_32(MFISOFTMDR) & SOFTMD_BOOTMODE_MASK);
	uint32_t ret;

	/* default is Secure boot */
	sbrom_SecureBootAPI = (SECURE_BOOT_API)&ROM_SecureBootAPI;

	ret = ROM_GetLcs(&lcs);
	if (ret != 0U) {
		ERROR("BL2: Failed to get the LCS. (%d)\n", ret);
		panic();
	}
	if (lcs == LCS_SE) {
		/* LCS=Secure */
		if (softmd == SOFTMD_NORMALBOOT) {
			/* LCS=Secure + Normal boot (temp setting) */
			sbrom_SecureBootAPI = &local_verify;
		}
		/* else is LCS=Secure + Secure boot */
	} else if (lcs == LCS_SD) {
		/* LCS=SD */
		sbrom_SecureBootAPI = &local_verify;
	} else {
		if (md != 0U) {
			/* MD5=1 => LCS=CM/DM/SD/RMA + Normal boot */
			sbrom_SecureBootAPI = &local_verify;
		}
		/* else is MD5=0 => LCS=CM/DM/SD/RMA + Secure boot */
	}

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
