/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR ring buffer, producer side. RingBufferHeader and LdrQueueHeader are
 * shared with the LDR daemon and must match the LDR middleware.
 */

#ifndef LDR_RING_BUFFER_H
#define LDR_RING_BUFFER_H

#include <stdbool.h>
#include <stdint.h>

#include "ldr_port.h"

#define LDR_RING_BUFFER_MAGIC_NUMBER	0x5a26
#define LDR_ALIGNE_LENGTH		16
#define LDR_ALIGNED			LDR_ALIGNED_N(LDR_ALIGNE_LENGTH)

typedef int8_t ldr_errno_t;

typedef enum {
	LDR_RB_OK = 0,
	LDR_RB_ERROR = -1,
	LDR_RB_SIZE_OVERFLOW = -2,
	LDR_RB_ACCESS_ERROR = -3,
	LDR_RB_FULL_BUFFER = -4,
	LDR_RB_ALREADY_INITIALIZED = -5,
	LDR_RB_QUEUE_ERROR = -6
} LdrRbErrorCode;

typedef enum {
	LDR_RING_BUFFER_OK = 0,
	LDR_RING_BUFFER_CREATED = 1,
	LDR_RING_BUFFER_EXITED = 2,
	LDR_RING_BUFFER_EDITING = 3,
	LDR_RING_BUFFER_WAITING = 4,
	LDR_RING_BUFFER_HEARTBEAT = 5,
} LDR_RING_BUFFER_STATUS;

typedef struct {
	uint16_t size;
	bool exists;
} LDR_ALIGNED LdrQueueHeader;

typedef struct {
	uint32_t magic_num;
	uint32_t buffer_size;
	uint32_t size;		/* Used size */
	uint8_t status;
	uint32_t head;		/* Consumer offset of the next record */
	uint32_t tail;		/* Producer offset of the next free slot */
	int32_t cnt;
	uint32_t fold_point;	/* End of the data before the last fold */
} LDR_ALIGNED RingBufferHeader;

ldr_errno_t ldr_ring_buf_mmgr_init(uint8_t *ptr, uint32_t size);
ldr_errno_t ldr_ring_buf_mmgr_enqueue(uint8_t *rb_ptr, const uint8_t *data,
				      uint32_t size);

#endif /* LDR_RING_BUFFER_H */
