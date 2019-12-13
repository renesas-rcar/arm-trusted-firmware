/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <auth_mod.h>
#include <bl_common.h>
#include <debug.h>
#include <errno.h>
#include <firmware_image_package.h>
#include <io_driver.h>
#include <io_storage.h>
#include <platform.h>
#include <platform_def.h>
#include <stdint.h>
#include <string.h>
#include <uuid.h>
#include <mmio.h>
#include <arch_helpers.h>
#include "io_rcar.h"
#include "io_common.h"
#include "io_private.h"

typedef struct {
	const int32_t	name;
	const uint32_t	offset;
	const uint32_t	attr;
} plat_rcar_name_offset_t;

typedef struct {
	/* Put file_pos above the struct to allow {0} on static init.
	 * It is a workaround for a known bug in GCC
	 * http://gcc.gnu.org/bugzilla/show_bug.cgi?id=53119
	 */
	uint32_t	file_pos;
	uint32_t	is_noload;
	uintptr_t	offset_address;
	uint32_t	size;
	uintptr_t	dest_address;
	uintptr_t	partition;	/* for eMMC */
					/* RCAR_EMMC_PARTITION_BOOT_0 */
					/* RCAR_EMMC_PARTITION_BOOT_1 */
					/* RCAR_EMMC_PARTITION_USER   */
} file_state_t;

#define RCAR_GET_FLASH_ADR(a,b)		((uint32_t)((0x40000U*(a)) + (b)))
#define RCAR_ATTR_SET_CALCADDR(a)	((a) & 0xF)			/* lower 4bit use */
#define RCAR_ATTR_SET_ISNOLOAD(a)	(((a) & 0x1) << 16U)		/* 16bit use */
#define RCAR_ATTR_SET_CERTOFF(a)	(((a) & 0xF) << 8U)		/* 11-8bit use */
#define RCAR_ATTR_SET_ALL(a,b,c)	((uint32_t)(RCAR_ATTR_SET_CALCADDR(a) |\
					RCAR_ATTR_SET_ISNOLOAD(b) | RCAR_ATTR_SET_CERTOFF(c)))

#define RCAR_ATTR_GET_CALCADDR(a)	((a) & 0xFU)			/* lower 4bit use */
#define RCAR_ATTR_GET_ISNOLOAD(a)	(((a) >> 16) & 0x1U)		/* 16bit use */
#define RCAR_ATTR_GET_CERTOFF(a)	(((a) >> 8) & 0xFU)		/* 11-8bit use */

#define RCAR_MAX_BL3X_IMAGE		(8U)
#define RCAR_SECTOR6_CERT_OFFSET	(0x400U)
#define RCAR_SDRAM_CERT_ADDRESS		(0x43F00000U)
#define RCAR_CERT_SIZE			(0x800U)	/* cert field size*/
#define RCAR_CERT_INFO_SIZE_OFFSET	(0x264U)	/* byte address set : must 4byte alignment */
#define RCAR_CERT_INFO_DST_OFFSET	(0x154U)	/* byte address set : must 4byte alignment */
#define RCAR_CERT_INFO_SIZE_OFFSET1	(0x364U)
#define RCAR_CERT_INFO_DST_OFFSET1	(0x1D4U)
#define RCAR_CERT_INFO_SIZE_OFFSET2	(0x464U)
#define RCAR_CERT_INFO_DST_OFFSET2	(0x254U)
#define RCAR_CERT_LOAD			(1U)

#define RCAR_FLASH_CERT_HEADER	RCAR_GET_FLASH_ADR(6U, 0U)
#define RCAR_EMMC_CERT_HEADER	(0x00030000U)

#define RCAR_COUNT_LOAD_BL33		(2U)
#define RCAR_COUNT_LOAD_BL33X		(3U)

static const plat_rcar_name_offset_t name_offset[] = {		/* calc addr, no load, cert offset */
	{BL31_IMAGE_ID,		0U,				RCAR_ATTR_SET_ALL(0,0,0)},
	/* BL3-2 is optional in the platform */
	{BL32_IMAGE_ID,				0U,		RCAR_ATTR_SET_ALL(1,0,1)},
	{BL33_IMAGE_ID,				0U,		RCAR_ATTR_SET_ALL(2,0,2)},
	{BL332_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(3,0,3)},
	{BL333_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(4,0,4)},
	{BL334_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(5,0,5)},
	{BL335_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(6,0,6)},
	{BL336_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(7,0,7)},
	{BL337_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(8,0,8)},
	{BL338_IMAGE_ID,			0U,		RCAR_ATTR_SET_ALL(9,0,9)},
};
#if TRUSTED_BOARD_BOOT
static const plat_rcar_name_offset_t cert_offset[] = {
	/* Certificates */
	{TRUSTED_KEY_CERT_ID,			0U,		RCAR_ATTR_SET_ALL(0,1,0)},
	{SOC_FW_KEY_CERT_ID,			0U,		RCAR_ATTR_SET_ALL(0,1,0)},
	{TRUSTED_OS_FW_KEY_CERT_ID,		0U,		RCAR_ATTR_SET_ALL(0,1,0)},
	{NON_TRUSTED_FW_KEY_CERT_ID,		0U,		RCAR_ATTR_SET_ALL(0,1,0)},
	{SOC_FW_CONTENT_CERT_ID,		0U,		RCAR_ATTR_SET_ALL(0,1,0)},
	{TRUSTED_OS_FW_CONTENT_CERT_ID,		0U,		RCAR_ATTR_SET_ALL(0,1,1)},
	{NON_TRUSTED_FW_CONTENT_CERT_ID,	0U,		RCAR_ATTR_SET_ALL(0,1,2)},
	{BL332_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,3)},
	{BL333_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,4)},
	{BL334_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,5)},
	{BL335_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,6)},
	{BL336_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,7)},
	{BL337_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,8)},
	{BL338_CERT_ID,				0U,		RCAR_ATTR_SET_ALL(0,1,9)},
};
#endif /* TRUSTED_BOARD_BOOT */


static file_state_t current_file = {0};
static uintptr_t backend_dev_handle;
static uintptr_t backend_image_spec;
static uint64_t rcar_image_header[RCAR_MAX_BL3X_IMAGE + 2U] = {0U};
static uint64_t rcar_image_header_prttn[RCAR_MAX_BL3X_IMAGE + 2U] = {0U};
static uint64_t rcar_image_number = {0U};
static uint32_t	rcar_cert_load = {0U};


/* prototype */
static io_type_t device_type_rcar(void);
static int32_t rcar_dev_open(const uintptr_t dev_spec, io_dev_info_t **dev_info);
static int32_t rcar_file_open(io_dev_info_t *dev_info, const uintptr_t spec,
			  io_entity_t *entity);
static int32_t rcar_file_len(io_entity_t *entity, size_t *length);
static int32_t rcar_file_read(io_entity_t *entity, uintptr_t buffer, size_t length,
			  size_t *length_read);
static int32_t rcar_file_close(io_entity_t *entity);
static int32_t rcar_dev_close(io_dev_info_t *dev_info);
static int32_t file_to_offset(const int32_t file, uintptr_t *offset,
	uint32_t *cert_addr, uint32_t *is_noload, uintptr_t *partition);
static int32_t load_bl33x(void);
static int32_t check_load_area(uintptr_t dst, uintptr_t len);

/* Identify the device type as a virtual driver */
static io_type_t device_type_rcar(void)
{
	return IO_TYPE_FIRMWARE_IMAGE_PACKAGE;
}


static const io_dev_connector_t rcar_dev_connector = {
	.dev_open = &rcar_dev_open
};


static const io_dev_funcs_t rcar_dev_funcs = {
	.type = &device_type_rcar,
	.open = &rcar_file_open,
	.seek = NULL,
	.size = &rcar_file_len,
	.read = &rcar_file_read,
	.write = NULL,
	.close = &rcar_file_close,
	.dev_init = &rcar_dev_init,
	.dev_close = &rcar_dev_close,
};


/* No state associated with this device so structure can be const */
static const io_dev_info_t rcar_dev_info = {
	.funcs = &rcar_dev_funcs,
	.info = (uintptr_t)0
};


/* Open a connection to the FIP device */
static int32_t rcar_dev_open(const uintptr_t dev_spec __attribute__((unused)),
			 io_dev_info_t **dev_info)
{
	*dev_info = (io_dev_info_t *)&rcar_dev_info; /* cast away const */

	return IO_SUCCESS;
}

int32_t file_to_cert(const int32_t filename, uint32_t *cert_addr)
{
	int32_t i;
	int32_t status = -EINVAL;

	for (i = 0; i < (int32_t)ARRAY_SIZE(cert_offset); i++) {
		if (filename == cert_offset[i].name) {
			*cert_addr = RCAR_CERT_SIZE;
			*cert_addr *= (uint32_t)RCAR_ATTR_GET_CERTOFF(cert_offset[i].attr);
			*cert_addr += RCAR_SDRAM_CERT_ADDRESS;
			status = 0;
			break;
		}
	}
	return status;
}

static int32_t file_to_offset(const int32_t file, uintptr_t *offset,
	uint32_t *cert_addr, uint32_t *is_noload, uintptr_t *partition)
{
	int32_t i;
	int32_t status = -EINVAL;
	uint32_t is_calc_addr;

	for (i = 0; i < (int32_t)ARRAY_SIZE(name_offset); i++) {
		if (file == name_offset[i].name) {
			is_calc_addr = RCAR_ATTR_GET_CALCADDR(name_offset[i].attr);
			if ((rcar_image_number + 2U) >= is_calc_addr)
			{
				*offset = rcar_image_header[is_calc_addr];
				*cert_addr = RCAR_CERT_SIZE;
				*cert_addr *= RCAR_ATTR_GET_CERTOFF(name_offset[i].attr);
				*cert_addr += RCAR_SDRAM_CERT_ADDRESS;
				*is_noload = RCAR_ATTR_GET_ISNOLOAD(name_offset[i].attr);
				*partition = rcar_image_header_prttn[is_calc_addr];
				status = IO_SUCCESS;
				break;
			}
		}
	}

	if (IO_SUCCESS != status) {
		for (i = 0; i < (int32_t)ARRAY_SIZE(cert_offset); i++) {
			if (file == cert_offset[i].name) {
				*offset = 0U;
				*cert_addr = 0U;
				*is_noload = RCAR_ATTR_GET_ISNOLOAD(cert_offset[i].attr);
				*partition = 0U;
				status = IO_SUCCESS;
				break;
			}
		}
	}
	return status;
}

#define RCAR_BOOT_KEY_CERT_NEW	(0xE6300F00U)
#define	RCAR_CERT_MAGIC_NUM	(0xE291F358U)
void get_info_from_cert(uint64_t cert_addr, uint32_t *size, uintptr_t *dest_addr)
{
	uint32_t	val;
	uint32_t	magicNumber;
	uint32_t	certInfo1;
	uint32_t	certInfo2;
	uintptr_t	pSize;
	uintptr_t	pDestH;
	uintptr_t	pDestL;

	cert_addr &= 0xFFFFFFFFU;		/* need? */

	magicNumber = mmio_read_32((uintptr_t)RCAR_BOOT_KEY_CERT_NEW);
	val = mmio_read_32((uintptr_t)RCAR_BOOT_KEY_CERT_NEW+0xCU);
	certInfo1 = (val >> 18) & 0x3U;
	val = mmio_read_32((uintptr_t)cert_addr+0xCU);
	certInfo2 = (val >> 21) & 0x3U;
	if (RCAR_CERT_MAGIC_NUM == magicNumber) {
		if (0x1U != certInfo1) {
			ERROR("BL2: Cert is invalid.\n");
			*size = 0x0U;
			*dest_addr = 0x0U;
		} else {
			if (0x2U == certInfo2) {
				pSize = cert_addr + RCAR_CERT_INFO_SIZE_OFFSET2;
				*size = mmio_read_32(pSize) * 4U;
				pDestL = cert_addr + RCAR_CERT_INFO_DST_OFFSET2;
				pDestH = pDestL + 4U;
				*dest_addr =
				((uintptr_t)mmio_read_32(pDestH) << 32) +
				((uintptr_t)mmio_read_32(pDestL));
			} else if (0x1U == certInfo2) {
				pSize = cert_addr + RCAR_CERT_INFO_SIZE_OFFSET1;
				*size = mmio_read_32(pSize) * 4U;
				pDestL = cert_addr + RCAR_CERT_INFO_DST_OFFSET1;
				pDestH = pDestL + 4U;
				*dest_addr =
				((uintptr_t)mmio_read_32(pDestH) << 32) +
				((uintptr_t)mmio_read_32(pDestL));
			} else if (0x0U == certInfo2) {
				pSize = cert_addr + RCAR_CERT_INFO_SIZE_OFFSET;
				*size = mmio_read_32(pSize) * 4U;
				pDestL = cert_addr + RCAR_CERT_INFO_DST_OFFSET;
				pDestH = pDestL + 4U;
				*dest_addr =
				((uintptr_t)mmio_read_32(pDestH) << 32) +
				((uintptr_t)mmio_read_32(pDestL));
 			} else /* if (0x3U == certInfo2) */ {
				ERROR("BL2: Cert is invalid.\n");
				*size = 0x0U;
				*dest_addr = 0x0U;
 			}
		}
	} else {
		pSize = cert_addr + RCAR_CERT_INFO_SIZE_OFFSET;
		*size = mmio_read_32(pSize) * 4U;
		pDestL = cert_addr + RCAR_CERT_INFO_DST_OFFSET;
		pDestH = pDestL + 4U;
		*dest_addr =
		((uintptr_t)mmio_read_32(pDestH) << 32) +
		((uintptr_t)mmio_read_32(pDestL));
	}
}

static int32_t load_bl33x(void)
{
	int32_t result = IO_SUCCESS;
	uint32_t loop = 1U;	/* start is BL332 */
	uintptr_t file_offset;
	uint32_t noload;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uintptr_t dest_addr;
	uintptr_t emmc_prttn;
	size_t bytes_read;
	uintptr_t backend_handle;
	const int32_t load_names[] = {
		BL33_IMAGE_ID,
		BL332_IMAGE_ID,
		BL333_IMAGE_ID,
		BL334_IMAGE_ID,
		BL335_IMAGE_ID,
		BL336_IMAGE_ID,
		BL337_IMAGE_ID,
		BL338_IMAGE_ID
	};
	static uint32_t load_bl33x_firsttime_flag = 0U;

	for (; (loop < rcar_image_number) &&
		(0U == load_bl33x_firsttime_flag); loop++) {

		if (IO_SUCCESS != result){
			break;
		}

		result = file_to_offset(load_names[loop], &file_offset,
			&cert_addr, &noload, &emmc_prttn);
		if (IO_SUCCESS != result) {
			WARN("load_bl33x: failed to get offset\n");
			result = IO_FAIL;
		} else {

			get_info_from_cert((uint64_t) cert_addr, &l_image_size,
				&dest_addr);

			/* set eMMC partition */
			((io_drv_spec_t *)backend_image_spec)->partition =
				(uint32_t)emmc_prttn;	/* needs 32-bits only */

			/* Open the backend, attempt to access the blob image */
			result = io_open(backend_dev_handle, backend_image_spec,
				&backend_handle);
			if (result != IO_SUCCESS) {
				WARN("Failed to open FIP (%i)\n", result);
				result = IO_FAIL;
			} else {
				result = io_seek(backend_handle, IO_SEEK_SET,
					(ssize_t) file_offset);
				if (IO_SUCCESS != result) {
					WARN("load_bl33x: failed to seek\n");
					result = IO_FAIL;
				}
			}

			if (IO_SUCCESS == result) {

                                result = check_load_area(dest_addr,
                                	(uintptr_t)l_image_size);
                                if (result != IO_SUCCESS) {
                                	WARN("load_bl33x: check load area\n");
                                        result = IO_FAIL;
                                }
			}

			if (IO_SUCCESS == result) {

				result = io_read(backend_handle,
					dest_addr,
					(size_t)l_image_size,
					(size_t *)&bytes_read);
				if (IO_SUCCESS != result) {
					WARN("load_bl33x: failed to read\n");
					result = IO_FAIL;
				}
#if TRUSTED_BOARD_BOOT
				else {
					/* Authenticate it */
					result = (int32_t)auth_mod_verify_img(
						(unsigned int)load_names[loop],
						(void *)dest_addr,
						(unsigned int)l_image_size);
					if (0 != result) {
						(void)memset(
						(void *)dest_addr,
						0x00,
						(size_t)l_image_size);
						result = IO_FAIL;
					}
				}
#endif /* TRUSTED_BOARD_BOOT */
			}
			/* Close the backend. */
			(void) io_close(backend_handle);
		}
	}

	load_bl33x_firsttime_flag = 1U;

	return result;
}

static int32_t check_load_area(uintptr_t dst, uintptr_t len)
{
	int32_t result = IO_SUCCESS;
	uintptr_t dram_start;
	uintptr_t dram_end;
	uintptr_t prot_start;
	uintptr_t prot_end;

	/* set parameter */
	if ((dst + len - 1U) <= UINT32_MAX) {
		/* set legacy range param */
		dram_start = DRAM1_BASE;
		dram_end   = (DRAM1_BASE + DRAM1_SIZE);
		prot_start = (uintptr_t)DRAM_PROTECTED_BASE;
		prot_end   =
		(uintptr_t)(DRAM_PROTECTED_BASE + DRAM_PROTECTED_SIZE);

	} else {
		/* set 40 bit range param */
		dram_start = (uintptr_t)DRAM_40BIT_BASE;
		dram_end   =
		(uintptr_t)(DRAM_40BIT_BASE + DRAM_40BIT_SIZE);
		prot_start = (uintptr_t)DRAM_40BIT_PROTECTED_BASE;
		prot_end   =
		(uintptr_t)(DRAM_40BIT_PROTECTED_BASE + DRAM_PROTECTED_SIZE);
	}

	/* check destination range */
	if ((dram_start <= dst) && ((dst + len) <= dram_end)) {
		/* load image is within SDRAM protected area */
		if ((prot_start <= dst) &&  (dst < prot_end)) {
			ERROR("BL2: dst address is on the protected area.\n");
			result = IO_FAIL;
		}
		if ((dst < prot_start) && (prot_start < (dst + len))) {
			ERROR("BL2: loaded data is on the protected area.\n");
			result = IO_FAIL;
		}
	} else {
		ERROR("BL2: loaded data is outside the loadable area.\n");
		result = IO_FAIL;
	}

	if (result == IO_FAIL) {
		ERROR("BL2: Out of range : dst=0x%lx len=0x%lx\n", dst, len);
	}
	return result;
}

/* Do some basic package checks. */
int32_t rcar_dev_init(io_dev_info_t *dev_info, const uintptr_t init_params)
{
	int32_t result;
	uint32_t image_name = (uint32_t)init_params;
	uintptr_t backend_handle;
	size_t bytes_read;
	ssize_t offset;
	uint32_t loop;
	static uint64_t rcar_image_header_tmp[64]
			__aligned(FLASH_TRANS_SIZE_UNIT) = {0U};

	/* Obtain a reference to the image by querying the platform layer */
	/* get rcar flash memory address... (certain BL2, BL31, BL32, BL33... max 64MB:RPC LBSC address) */
	result = plat_get_drv_source(image_name, &backend_dev_handle,
				       &backend_image_spec);
	if (result != IO_SUCCESS) {
		WARN("Failed to obtain reference to image '%d' (%i)\n",
			image_name, result);
		result = IO_FAIL;
	} else {

		if (RCAR_CERT_LOAD == rcar_cert_load) {
			/* not load certificate file after 1st initialize */
			result = IO_SUCCESS;
		} else {

			/* Attempt to access the RCAR image */
			/* open memory driver (certain SYS-DMAC2 initialize) */
			result = io_open(backend_dev_handle, backend_image_spec,
					&backend_handle);
			if (result != IO_SUCCESS) {
				WARN("Failed to access image '%d' (%i)\n",
						image_name, result);
				result = IO_FAIL;
			}

			/* get start address list */
			/* [0] address num */
			/* [1] BL33-1 image address */
			/* [2] BL33-2 image address */
			/* [3] BL33-3 image address */
			/* [4] BL33-4 image address */
			/* [5] BL33-5 image address */
			/* [6] BL33-6 image address */
			/* [7] BL33-7 image address */
			/* [8] BL33-8 image address */
			if (IO_SUCCESS == result) {
				if ( image_name == EMMC_DEV_ID) {
					offset =
					(ssize_t)RCAR_EMMC_CERT_HEADER;
				} else {
					offset =
					(ssize_t)RCAR_FLASH_CERT_HEADER;
				}
				result = io_seek(backend_handle,
						IO_SEEK_SET, offset);
				if (result != IO_SUCCESS) {
					WARN("Firmware Image Package header "\
						"failed to seek\n");
					result = IO_FAIL;
					(void) io_close(backend_handle);
				}
			}
			if (IO_SUCCESS == result) {
#if RCAR_BL2_DCACHE == 1
				inv_dcache_range(
					(uint64_t)rcar_image_header_tmp,
					sizeof(rcar_image_header_tmp));
#endif /* RCAR_BL2_DCACHE == 1 */
				result = io_read(backend_handle,
					(uintptr_t) &rcar_image_header_tmp,
					sizeof(rcar_image_header_tmp),
					&bytes_read);
				if (result == IO_SUCCESS) {
					/* 32 bits array to 64 bits array */
					rcar_image_number =
						rcar_image_header_tmp[0U];
					for (loop = 0U; loop <
						(rcar_image_number + 2U);
								loop++) {
						rcar_image_header[loop] =
						rcar_image_header_tmp
							[loop * 2U + 1U];
						rcar_image_header_prttn[loop] =
						rcar_image_header_tmp
							[loop * 2U + 2U];
					}
					result = IO_SUCCESS;
					if ((rcar_image_number == 0U) ||
						(rcar_image_number >
							RCAR_MAX_BL3X_IMAGE)) {
						WARN("Firmware Image Package "\
							"header check failed.\n");
						result = IO_FAIL;
						(void) io_close(backend_handle);
					} else {
						VERBOSE("header looks OK.\n");
					}
				}
			}
			/* load cert file */
			if (IO_SUCCESS == result) {
				result = io_seek(backend_handle, IO_SEEK_SET,
						(offset
						+ (ssize_t)
						RCAR_SECTOR6_CERT_OFFSET));
				if (result != IO_SUCCESS) {
					WARN("Firmware Image Package "\
						"header failed to seek\n");
					result = IO_FAIL;
					(void) io_close(backend_handle);
				}
			}
			if (IO_SUCCESS == result) {
#if RCAR_BL2_DCACHE == 1
				inv_dcache_range(RCAR_SDRAM_CERT_ADDRESS,
						(RCAR_CERT_SIZE * (2U +
						rcar_image_number)));
#endif /* RCAR_BL2_DCACHE == 1 */
				result = io_read(backend_handle,
						(uintptr_t)
						RCAR_SDRAM_CERT_ADDRESS,
						(size_t) (RCAR_CERT_SIZE *
						(2U +
						(size_t) rcar_image_number)),
						&bytes_read);
				if (result != IO_SUCCESS) {
					WARN("cert file read error.\n");
					result = IO_FAIL;
				} else {

					rcar_cert_load = RCAR_CERT_LOAD;
				}
				(void) io_close(backend_handle);
			}
		}
	}

	return result;
}

/* Close a connection to the FIP device */
static int32_t rcar_dev_close(io_dev_info_t *dev_info)
{
	/* TODO: Consider tracking open files and cleaning them up here */

	/* Clear the backend. */
	backend_dev_handle = (uintptr_t)0;
	backend_image_spec = (uintptr_t)0;

	return IO_SUCCESS;
}


/* Open a file for access from package. */
static int32_t rcar_file_open(io_dev_info_t *dev_info, const uintptr_t spec,
			 io_entity_t *entity)
{
	int32_t result;
	uintptr_t file_offset;
	uint32_t noload;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uintptr_t dest_addr;
	uintptr_t emmc_prttn;
	const io_drv_spec_t *file_spec = (io_drv_spec_t *)spec;

	/* Can only have one file open at a time for the moment. We need to
	 * track state like file cursor position. We know the header lives at
	 * offset zero, so this entry should never be zero for an active file.
	 * When the system supports dynamic memory allocation we can allow more
	 * than one open file at a time if needed.
	 */
	if (current_file.offset_address != 0U) {
		WARN("rcar_file_open : Only one open file at a time.\n");
		result = IO_RESOURCES_EXHAUSTED;
	} else {

		/* get file offset(but BL33 image id not get) */
		result = file_to_offset(file_spec->offset,
			&file_offset, &cert_addr, &noload, &emmc_prttn);
		if (result != IO_SUCCESS) {
			WARN("Failed to open file name %ld (%i)\n",
				file_spec->offset, result);
			result = IO_FAIL;
		} else {
			if (0U != noload) {
				current_file.offset_address = 1U;
				current_file.dest_address = 0U;
				current_file.size = 1U;
				current_file.file_pos = 0U;
				current_file.is_noload = noload;
				current_file.partition = 0U;
				entity->info = (uintptr_t) &current_file;

			} else {

				/* get image size and destination address */
				get_info_from_cert((uint64_t) cert_addr,
					&l_image_size, &dest_addr);

				current_file.offset_address = file_offset;
				current_file.dest_address = dest_addr;
				current_file.size = l_image_size;
				current_file.file_pos = 0U;
				current_file.is_noload = noload;
				current_file.partition = emmc_prttn;
				entity->info = (uintptr_t) &current_file;
			}
		}
	}

	return result;
}


/* Return the size of a file in package */
static int32_t rcar_file_len(io_entity_t *entity, size_t *length)
{
	*length =  ((file_state_t *)entity->info)->size;

	return IO_SUCCESS;
}


/* Read data from a file in package */
static int32_t rcar_file_read(io_entity_t *entity, uintptr_t buffer, size_t length,
			  size_t *length_read)
{
	int32_t result;
	file_state_t *fp;
	ssize_t file_offset;
	size_t bytes_read;
	uintptr_t backend_handle;
#ifdef SPD_NONE
	static uint32_t load_bl33x_counter = 1U;
#else
	static uint32_t load_bl33x_counter = 0U;
#endif /* SPD_NONE */

	/* cert image no load */
	if ( 0U != current_file.is_noload ) {
		result = IO_SUCCESS;
		*length_read = length;
	} else {
		fp = (file_state_t *) entity->info;
		/* set eMMC partition */
		((io_drv_spec_t *)backend_image_spec)->partition =
			(uint32_t)fp->partition;/* needs 32-bits only */


		/* Open the backend, attempt to access the blob image */
		result = io_open(backend_dev_handle, backend_image_spec,
			&backend_handle);
		if (result != IO_SUCCESS) {
			WARN("Failed to open FIP (%i)\n", result);
			result = IO_FAIL;
		} else {

			/* Seek to the position in the FIP where the payload lives */
			file_offset = (ssize_t) fp->offset_address
				+ (ssize_t) fp->file_pos;
			result = io_seek(backend_handle, IO_SEEK_SET,
				file_offset);
			if (result != IO_SUCCESS) {
				WARN("rcar_file_read: failed to seek\n");
				result = IO_FAIL;
			}

			if (load_bl33x_counter == RCAR_COUNT_LOAD_BL33) {
				/* Loading target is BL33 */
				result = check_load_area(
					buffer, (uintptr_t)length);
				if (result != IO_SUCCESS) {
					WARN("rcar_file_read: load area err\n");
					result = IO_FAIL;
				}
			}

			if (result == IO_SUCCESS) {
				result = io_read(backend_handle, buffer, length,
					&bytes_read);
				if (result != IO_SUCCESS) {
					/* We cannot read our data. Fail. */
					WARN("Failed to read payload (%i)\n",
						result);
					result = IO_FAIL;
				} else {
					/* Set caller length and new file position. */
					*length_read = bytes_read;
					fp->file_pos += (uint32_t) bytes_read;
				}
			}
			/* Close the backend. */
			(void) io_close(backend_handle);

			if (result == IO_SUCCESS) {
				load_bl33x_counter += 1U;
				if (load_bl33x_counter == RCAR_COUNT_LOAD_BL33X) {
					result = load_bl33x();
				}
			}
		}
	}

	return result;
}


/* Close a file in package */
static int32_t rcar_file_close(io_entity_t *entity)
{
	/* Clear our current file pointer.
	 * If we had malloc() we would free() here.
	 */
	if (current_file.offset_address != 0U) {
		(void)memset(&current_file, 0, sizeof(current_file));
	}

	/* Clear the Entity info. */
	entity->info = 0U;

	return IO_SUCCESS;
}

/* Exported functions */

/* Register the Firmware Image Package driver with the IO abstraction */
int32_t register_io_dev_rcar(const io_dev_connector_t **dev_con)
{
	int32_t result;

	result = io_register_device(&rcar_dev_info);
	if (result == IO_SUCCESS) {
		*dev_con = &rcar_dev_connector;
	}

	return result;
}
