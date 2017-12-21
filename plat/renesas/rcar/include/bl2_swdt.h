/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BL2_SWDT_H__
#define BL2_SWDT_H__

void bl2_swdt_init(void);

void bl2_swdt_enable(void);

void bl2_swdt_release(void);

void bl2_swdt_exec(uint64_t addr);

extern void gicd_set_icenabler(uintptr_t base, unsigned int id);

#endif /* BL2_SWDT_H__ */
