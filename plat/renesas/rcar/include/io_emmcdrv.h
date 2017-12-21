/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_EMMCDRV_H__
#define IO_EMMCDRV_H__

struct io_dev_connector;

int32_t register_io_dev_emmcdrv(const io_dev_connector_t **dev_con);

#endif /* IO_EMMCDRV_H__ */
