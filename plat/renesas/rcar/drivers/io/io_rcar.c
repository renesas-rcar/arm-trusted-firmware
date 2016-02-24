/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
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

#include <assert.h>
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
#include "io_rcar.h"


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
	uint32_t	offset_address;
	uint32_t	size;
	uint32_t	dest_address;
} file_state_t;

#define RCAR_GET_FLASH_ADR(a,b)		(uint32_t)((0x40000U*(a)) + (b))
#define RCAR_ATTR_SET_CALCADDR(a)	((a) & 0xF)			/* lower 4bit use */
#define RCAR_ATTR_SET_ISNOLOAD(a)	(((a) & 0x1) << 16U)		/* 16bit use */
#define RCAR_ATTR_SET_CERTOFF(a)	(((a) & 0xF) << 8U)		/* 11-8bit use */
#define RCAR_ATTR_SET_ALL(a,b,c)	(uint32_t)(RCAR_ATTR_SET_CALCADDR(a) |\
					RCAR_ATTR_SET_ISNOLOAD(b) | RCAR_ATTR_SET_CERTOFF(c))

#define RCAR_ATTR_GET_CALCADDR(a)	((a) & 0xFU)			/* lower 4bit use */
#define RCAR_ATTR_GET_ISNOLOAD(a)	(((a) >> 16) & 0x1U)		/* 16bit use */
#define RCAR_ATTR_GET_CERTOFF(a)	(((a) >> 8) & 0xFU)		/* 11-8bit use */

#define RCAR_MAX_BL3X_IMAGE		(8U)
#define RCAR_SECTOR6_CERT_OFFSET	(0x400U)
#define RCAR_SDRAM_CERT_ADDRESS		(0x43F00000U)
#define RCAR_CERT_SIZE			(0x400U)
#define RCAR_CERT_INFO_SIZE_OFFSET	(0x264U)	/* byte address set : must 4byte alignment */
#define RCAR_CERT_INFO_DST_OFFSET	(0x154U)	/* byte address set : must 4byte alignment */

#define RCAR_CERT_LOAD			(1U)

static const plat_rcar_name_offset_t name_offset[] = {		/* calc addr, no load, cert offset */
	{BL31_IMAGE_ID,		RCAR_GET_FLASH_ADR(7U,0x000U),	RCAR_ATTR_SET_ALL(0,0,0)},
	/* BL3-2 is optional in the platform */
	{BL32_IMAGE_ID,		RCAR_GET_FLASH_ADR(8U,0x000U),	RCAR_ATTR_SET_ALL(0,0,1)},
	{BL33_IMAGE_ID,		0U,				RCAR_ATTR_SET_ALL(1,0,2)},
	{BL332_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(2,0,3)},
	{BL333_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(3,0,4)},
	{BL334_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(4,0,5)},
	{BL335_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(5,0,6)},
	{BL336_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(6,0,7)},
	{BL337_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(7,0,8)},
	{BL338_IMAGE_ID,	0U,				RCAR_ATTR_SET_ALL(8,0,9)},
};
#if TRUSTED_BOARD_BOOT
static const plat_rcar_name_offset_t cert_offset[] = {
	/* Certificates */
	{TRUSTED_KEY_CERT_ID,	0U,				RCAR_ATTR_SET_ALL(0,1,0)},
	{BL31_KEY_CERT_ID,	0U,				RCAR_ATTR_SET_ALL(0,1,0)},
	{BL32_KEY_CERT_ID,	0U,				RCAR_ATTR_SET_ALL(0,1,0)},
	{BL33_KEY_CERT_ID,	0U,				RCAR_ATTR_SET_ALL(0,1,0)},
	{BL31_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,0)},
	{BL32_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,1)},
	{BL33_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,2)},
	{BL332_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,3)},
	{BL333_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,4)},
	{BL334_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,5)},
	{BL335_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,6)},
	{BL336_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,7)},
	{BL337_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,8)},
	{BL338_CERT_ID,		0U,				RCAR_ATTR_SET_ALL(0,1,9)},
};
#endif /* TRUSTED_BOARD_BOOT */


static file_state_t current_file = {0};
static uintptr_t backend_dev_handle;
static uintptr_t backend_image_spec;
static uint32_t rcar_image_header[RCAR_MAX_BL3X_IMAGE + 1U] = {0U};
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
static int32_t rcar_dev_init(io_dev_info_t *dev_info, const uintptr_t init_params);
static int32_t rcar_dev_close(io_dev_info_t *dev_info);
static int32_t file_to_offset(const int32_t file, uint32_t *offset, uint32_t *cert_addr, uint32_t *is_noload);
static int32_t load_bl33x(uintptr_t handle);


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
	assert(dev_info != NULL);
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

static int32_t file_to_offset(const int32_t file, uint32_t *offset, uint32_t *cert_addr, uint32_t *is_noload)
{
	int32_t i;
	int32_t status = -EINVAL;
	uint32_t is_calc_addr;
	
	assert(offset != NULL);
	assert(cert_addr != NULL);
	assert(is_noload != NULL);

	for (i = 0; i < (int32_t)ARRAY_SIZE(name_offset); i++) {
		if (file == name_offset[i].name) {
			is_calc_addr = RCAR_ATTR_GET_CALCADDR(name_offset[i].attr);
			if (rcar_image_header[0] >= is_calc_addr)
			{
				if (0U != is_calc_addr)
				{
					*offset = rcar_image_header[is_calc_addr];
				} else {
					*offset = name_offset[i].offset;
				}
				*cert_addr = RCAR_CERT_SIZE;
				*cert_addr *= RCAR_ATTR_GET_CERTOFF(name_offset[i].attr);
				*cert_addr += RCAR_SDRAM_CERT_ADDRESS;
				*is_noload = RCAR_ATTR_GET_ISNOLOAD(name_offset[i].attr);
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
				status = IO_SUCCESS;
				break;
			}
		}
	}
	return status;
}

void get_info_from_cert(uint64_t cert_addr, uint32_t *size, uint32_t *dest_addr)
{
	assert(size != NULL);
	assert(dest_addr != NULL);
	
	cert_addr &= 0xFFFFFFFFU;		/* need? */

	*size = *((uint32_t *)(cert_addr + RCAR_CERT_INFO_SIZE_OFFSET)) * 4U;
	*dest_addr = *((uint32_t *)(cert_addr + RCAR_CERT_INFO_DST_OFFSET));
}

static int32_t load_bl33x(uintptr_t handle)
{
	int32_t result = IO_SUCCESS;
	uint32_t loop = 1U;	/* start is BL332 */
	uint32_t file_offset;
	uint32_t noload;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uint32_t dest_addr;
	size_t bytes_read;
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

	for (; loop < rcar_image_header[0]; loop++) {

		if (IO_SUCCESS != result){
			break;
		}

		result = file_to_offset(load_names[loop], &file_offset,
			&cert_addr, &noload);
		if (IO_SUCCESS != result) {
			WARN("load_bl33x: failed to get offset\n");
			result = IO_FAIL;
		} else {

			get_info_from_cert((uint64_t) cert_addr, &l_image_size,
				&dest_addr);

				result = io_seek(handle, IO_SEEK_SET,
					(ssize_t) file_offset);
				if (IO_SUCCESS != result) {
					WARN("load_bl33x: failed to seek\n");
					result = IO_FAIL;
				}
			}

		if (IO_SUCCESS == result) {

			result = io_read(handle, (uintptr_t)dest_addr, (size_t)l_image_size,
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
					(void *)((uintptr_t)dest_addr),
					(unsigned int)l_image_size);
				if (0 != result) {
					memset((void *)((uintptr_t)dest_addr),
						0x00,
						(size_t)l_image_size);
					result = IO_FAIL;
				}
			}
#endif /* TRUSTED_BOARD_BOOT */
		}
	}

	return result;
}

/* Do some basic package checks. */
static int32_t rcar_dev_init(io_dev_info_t *dev_info, const uintptr_t init_params)
{
	int32_t result;
	int32_t image_name = (int32_t)init_params;
	uintptr_t backend_handle;
	size_t bytes_read;

	/* Obtain a reference to the image by querying the platform layer */
	/* get rcar flash memory address... (certain BL2, BL31, BL32, BL33... max 64MB:RPC LBSC address) */
	/* sakata check image number */
	result = plat_get_image_source(image_name, &backend_dev_handle,
				       &backend_image_spec);
	if (result != IO_SUCCESS) {
		WARN("Failed to obtain reference to image '%d' (%i)\n",
			image_name, result);
		result = IO_FAIL;
	} else {

		if (RCAR_CERT_LOAD == rcar_cert_load) {
			/* not load certificate file after 1at initialize */
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
				result = io_seek(backend_handle, IO_SEEK_SET,
					(ssize_t) RCAR_GET_FLASH_ADR(6U, 0U));
				if (result != IO_SUCCESS) {
					WARN("Firmware Image Package header "\
						"failed to seek\n");
					result = IO_FAIL;
					(void) io_close(backend_handle);
				}
			}
			if (IO_SUCCESS == result) {
				result = io_read(backend_handle,
					(uintptr_t) &rcar_image_header,
					sizeof(rcar_image_header), &bytes_read);
				if (result == IO_SUCCESS) {
					if ((rcar_image_header[0] == 0U)
						|| (rcar_image_header[0] > 8U)) {
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
					(ssize_t) (RCAR_GET_FLASH_ADR(6U, 0U)
						+ RCAR_SECTOR6_CERT_OFFSET));
				if (result != IO_SUCCESS) {
					WARN("Firmware Image Package "\
						"header failed to seek\n");
					result = IO_FAIL;
					(void) io_close(backend_handle);
				}
			}
			if (IO_SUCCESS == result) {
				result = io_read(backend_handle,
						(uintptr_t) RCAR_SDRAM_CERT_ADDRESS,
						(size_t) (RCAR_CERT_SIZE *
						 (2U + (size_t) rcar_image_header[0])),
						&bytes_read);
				if (result != IO_SUCCESS) {
					WARN("cert file road error.\n");
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
	uint32_t file_offset;
	uint32_t noload;
	uint32_t cert_addr;
	uint32_t l_image_size;
	uint32_t dest_addr;
	const io_block_spec_t *file_spec = (io_block_spec_t *)spec;

	assert(file_spec != NULL);
	assert(entity != NULL);

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
			&file_offset, &cert_addr, &noload);
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
				entity->info = (uintptr_t) &current_file;
			}
		}
	}

	return result;
}


/* Return the size of a file in package */
static int32_t rcar_file_len(io_entity_t *entity, size_t *length)
{
	assert(entity != NULL);
	assert(length != NULL);

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

	assert(entity != NULL);
	assert(buffer != (uintptr_t)NULL);
	assert(length_read != NULL);
	assert(entity->info != (uintptr_t)NULL);

	/* cert image no load */
	if ( 0U != current_file.is_noload ) {
		result = IO_SUCCESS;
		*length_read = length;
	} else {

		/* Open the backend, attempt to access the blob image */
		result = io_open(backend_dev_handle, backend_image_spec,
			&backend_handle);
		if (result != IO_SUCCESS) {
			WARN("Failed to open FIP (%i)\n", result);
			result = IO_FAIL;
		} else {

			fp = (file_state_t *) entity->info;

			/* Seek to the position in the FIP where the payload lives */
			file_offset = (ssize_t) fp->offset_address
				+ (ssize_t) fp->file_pos;
			result = io_seek(backend_handle, IO_SEEK_SET,
				file_offset);
			if (result != IO_SUCCESS) {
				WARN("rcar_file_read: failed to seek\n");
				result = IO_FAIL;
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

			if ((result == IO_SUCCESS) && (buffer == (uintptr_t)NS_IMAGE_OFFSET)) {
				result = load_bl33x(backend_handle);
			}
			/* Close the backend. */
			(void) io_close(backend_handle);
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
	assert(entity != NULL);
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
	assert(dev_con != NULL);

	result = io_register_device(&rcar_dev_info);
	if (result == IO_SUCCESS) {
		*dev_con = &rcar_dev_connector;
	}

	return result;
}
