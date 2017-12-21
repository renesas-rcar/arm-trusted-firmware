/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_RCARBOOT_H_
#define RCAR_RCARBOOT_H_

/* Define */
typedef int32_t (*SECURE_BOOT_API)(uint32_t a, uint32_t b, void *c);

/* Prototype */
extern int32_t file_to_cert(const int32_t filename, uint32_t *cert_addr);

#endif /* RCAR_RCARBOOT_H_ */
