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
#include <debug.h>
#include <io_driver.h>
#include <io_storage.h>
#include <io_semihosting.h>
#include <platform_def.h>
#include <semihosting.h>	/* For FOPEN_MODE_... */
#include <string.h>
#include "io_common.h"
#include "io_rcar.h"
#include "io_memdrv.h"

/* IO devices */
static uintptr_t rcar_dev_handle;
static uintptr_t memdrv_dev_handle;

static const io_block_spec_t rcar_block_spec = {
	.offset = FLASH0_BASE,
	.length = FLASH0_SIZE
};

static const io_file_spec_t bl2_file_spec = {
	.path = BL2_IMAGE_NAME,
};

static const io_file_spec_t bl31_file_spec = {
	.path = BL31_IMAGE_NAME,
};

static const io_file_spec_t bl32_file_spec = {
	.path = BL32_IMAGE_NAME,
};

static const io_file_spec_t bl33_file_spec = {
	.path = BL33_IMAGE_NAME,
};

static const io_file_spec_t bl332_file_spec = {
	.path = BL332_IMAGE_NAME,
};

static const io_file_spec_t bl333_file_spec = {
	.path = BL333_IMAGE_NAME,
};

static const io_file_spec_t bl334_file_spec = {
	.path = BL334_IMAGE_NAME,
};

static const io_file_spec_t bl335_file_spec = {
	.path = BL335_IMAGE_NAME,
};

static const io_file_spec_t bl336_file_spec = {
	.path = BL336_IMAGE_NAME,
};

static const io_file_spec_t bl337_file_spec = {
	.path = BL337_IMAGE_NAME,
};

static const io_file_spec_t bl338_file_spec = {
	.path = BL338_IMAGE_NAME,
};


#if TRUSTED_BOARD_BOOT
static const io_file_spec_t trusted_key_cert_file_spec = {
	.path = TRUSTED_KEY_CERT_NAME,
};

static const io_file_spec_t bl31_key_cert_file_spec = {
	.path = BL31_KEY_CERT_NAME,
};

static const io_file_spec_t bl32_key_cert_file_spec = {
	.path = BL32_KEY_CERT_NAME,
};

static const io_file_spec_t bl33_key_cert_file_spec = {
	.path = BL33_KEY_CERT_NAME,
};

static const io_file_spec_t bl31_cert_file_spec = {
	.path = BL31_CERT_NAME,
};

static const io_file_spec_t bl32_cert_file_spec = {
	.path = BL32_CERT_NAME,
};

static const io_file_spec_t bl33_cert_file_spec = {
	.path = BL33_CERT_NAME,
};
#endif /* TRUSTED_BOARD_BOOT */

static int32_t open_rcar(const uintptr_t spec);
static int32_t open_memmap(const uintptr_t spec);

struct plat_io_policy {
	int8_t *image_name;
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy policies[] = {
	{
		(int8_t *)RCAR_IMAGE_NAME,
		&memdrv_dev_handle,
		(uintptr_t)&rcar_block_spec,
		&open_memmap
	}, {
		(int8_t *)BL2_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl2_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL31_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl31_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL32_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl32_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL33_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl33_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL332_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl332_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL333_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl333_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL334_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl334_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL335_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl335_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL336_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl336_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL337_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl337_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL338_IMAGE_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl338_file_spec,
		&open_rcar
	}, {
#if TRUSTED_BOARD_BOOT
		(int8_t *)TRUSTED_KEY_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&trusted_key_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL31_KEY_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl31_key_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL32_KEY_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl32_key_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL33_KEY_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl33_key_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL31_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl31_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL32_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl32_cert_file_spec,
		&open_rcar
	}, {
		(int8_t *)BL33_CERT_NAME,
		&rcar_dev_handle,
		(uintptr_t)&bl33_cert_file_spec,
		&open_rcar
	}, {
#endif /* TRUSTED_BOARD_BOOT */
		0, 0, 0
	}
};


static int32_t open_rcar(const uintptr_t spec)
{
	int32_t result;

	/* See if a Firmware Image Package is available */
	result = io_dev_init(rcar_dev_handle, (uintptr_t)RCAR_IMAGE_NAME);
	if (result == IO_SUCCESS) {
		VERBOSE("Using RCar File Manager\n");
	}
	return result;
}


static int32_t open_memmap(const uintptr_t spec)
{
	int32_t result;
	uintptr_t local_image_handle;

	result = io_dev_init(memdrv_dev_handle, 0U);
	if (result == IO_SUCCESS) {
		result = io_open(memdrv_dev_handle, spec, &local_image_handle);
		if (result == IO_SUCCESS) {
			VERBOSE("Using Memdrv IO\n");
			(void)io_close(local_image_handle);
		}
	}
	return result;
}



void rcar_io_setup (void)
{
	int32_t io_result;
	const io_dev_connector_t *rcar_dev_con;
	const io_dev_connector_t *memmap_dev_con;

	/* Register the IO devices on this platform */
	io_result = register_io_dev_rcar(&rcar_dev_con);
	assert(io_result == IO_SUCCESS);

	io_result = register_io_dev_memdrv(&memmap_dev_con);
	assert(io_result == IO_SUCCESS);

	/* Open connections to devices and cache the handles */
	io_result = io_dev_open(rcar_dev_con, 0U, &rcar_dev_handle);
	assert(io_result == IO_SUCCESS);

	io_result = io_dev_open(memmap_dev_con, 0U,
				&memdrv_dev_handle);
	assert(io_result == IO_SUCCESS);

	/* Ignore improbable errors in release builds */
	(void)io_result;
}


/* Return an IO device handle and specification which can be used to access
 * an image. Use this to enforce platform load policy */
int plat_get_image_source(const char *image_name, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	int32_t result = IO_FAIL;
	const struct plat_io_policy *policy;
	int32_t loop = 0;

	if ((image_name != NULL) && (dev_handle != NULL) &&
	    (image_spec != NULL)) {
		policy = &policies[0];
		while (policy->image_name != NULL) {
			if (strcmp((const char *)policy->image_name, image_name) == 0) {
				result = policy->check(policy->image_spec);
				if (result == IO_SUCCESS) {
					*image_spec = policy->image_spec;
					*dev_handle = *(policy->dev_handle);
					break;
				}
			}
			loop++;
			policy = &policies[loop];
		}
	} else {
		result = IO_FAIL;
	}
	return result;
}
