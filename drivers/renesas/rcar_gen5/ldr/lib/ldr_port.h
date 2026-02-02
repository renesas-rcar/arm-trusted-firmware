/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Environment hooks of the LDR producer libraries (ldr_dlt_protocol/,
 * ldr_ring_buffer/), which otherwise need only <stddef.h>, <stdint.h> and
 * <stdbool.h>. Locking, cache maintenance, timestamps and level mapping are
 * left to the caller (../ldr.c).
 */

#ifndef LDR_PORT_H
#define LDR_PORT_H

#include <stddef.h>
#include <stdint.h>

#include <cdefs.h>

/* No padding, or the given alignment; sizes are checked at build time. */
#define LDR_PACKED			__packed __aligned(1)
#define LDR_ALIGNED_N(n)		__aligned(n)

/* Full memory barrier: the consumer runs on another core. */
#define LDR_BARRIER()			__sync_synchronize()

/* Byte-wise copy and fill: no libc, and nothing that can fault or block. */
#define LDR_MEMCPY(dest, src, size)					\
	do {								\
		for (size_t ldr_i_ = 0; ldr_i_ < (size); ++ldr_i_) {	\
			((uint8_t *)(dest))[ldr_i_] =			\
				((const uint8_t *)(src))[ldr_i_];	\
		}							\
	} while (0)

#define LDR_MEMSET(dest, value, size)					\
	do {								\
		for (size_t ldr_i_ = 0; ldr_i_ < (size); ++ldr_i_) {	\
			((uint8_t *)(dest))[ldr_i_] = (uint8_t)(value);	\
		}							\
	} while (0)

/* Compile-time assertion. */
#define LDR_CASSERT(cond, name)						\
	typedef char name[(cond) ? 1 : -1]

#endif /* LDR_PORT_H */
