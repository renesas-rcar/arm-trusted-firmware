/*
 * Copyright (c) 2014 ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_COMMON_H__
#define IO_COMMON_H__

/* io driver specification - used to refer to data on a device supporting */
typedef struct io_drv_spec {
	size_t offset;
	size_t length;
	uint32_t partition;
} io_drv_spec_t;

extern void rcar_io_setup (void);
extern void rcar_io_emmc_setup (void);
extern int32_t plat_get_drv_source(uint32_t io_drv_id,
			uintptr_t *dev_handle,
			uintptr_t *image_spec);


#endif /* IO_COMMON_H__ */
