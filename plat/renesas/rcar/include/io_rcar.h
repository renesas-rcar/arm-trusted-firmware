/*
 * Copyright (c) 2014 ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_RCAR_H__
#define IO_RCAR_H__

int32_t register_io_dev_rcar(const io_dev_connector_t **dev_con);
int32_t file_to_cert(const int32_t filename, uint32_t *cert_addr);
void get_info_from_cert(uint64_t cert_addr, uint32_t *size, uintptr_t *dest_addr);
int32_t rcar_dev_init(io_dev_info_t *dev_info, const uintptr_t init_params);

#endif /* IO_FIP_H__ */
