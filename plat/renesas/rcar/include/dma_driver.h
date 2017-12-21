/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DMA_DRIVER_H__
#define DMA_DRIVER_H__

#include <stdint.h>

void initDMA(void);
void execDMA(uintptr_t dst, uint32_t src, uint32_t len);

#endif	/* DMA_DRIVER_H__ */
