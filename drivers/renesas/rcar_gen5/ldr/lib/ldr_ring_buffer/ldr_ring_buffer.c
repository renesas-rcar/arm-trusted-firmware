/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * LDR ring buffer, producer side, adapted from the LDR middleware with the
 * same memory layout. The consumer (LDR daemon) owns 'head'; the producer
 * owns 'tail' and 'fold_point'. Not locked: callers with several producers
 * serialize themselves (ldr.c uses a spinlock).
 */

#include <stdint.h>

#include "ldr_ring_buffer.h"

/* Both sides cast the same bytes: check the layout at build time. */
LDR_CASSERT(sizeof(RingBufferHeader) == 32U, assert_ring_buffer_header_size);
LDR_CASSERT(sizeof(LdrQueueHeader) == 16U, assert_ring_queue_header_size);

/* Round up to the 16-byte slot alignment. */
#define LDR_ALIGN_UP_16(x)		(((x) + 15U) & ~15U)
#define LDR_ALIGN_PTR_UP_16(p)						\
	((__typeof__(p))(((uintptr_t)(p) + 15U) & ~(uintptr_t)15U))

/* Record slots start immediately after the ring header. */
static uint8_t *ldr_mem_mmgr_get_queue_entry(uint8_t *mem_ptr, uint32_t head)
{
	return mem_ptr + sizeof(RingBufferHeader) + head;
}

static void ldr_ring_buf_header_init(volatile RingBufferHeader *rb,
				     uint32_t size)
{
	rb->magic_num = LDR_RING_BUFFER_MAGIC_NUMBER;
	rb->buffer_size = size - (uint32_t)sizeof(RingBufferHeader);
	rb->size = 0;
	rb->status = LDR_RING_BUFFER_CREATED;
	rb->head = 0;
	rb->tail = 0;
	rb->cnt = 0;
	rb->fold_point = 0;

	LDR_MEMSET((char *)rb + sizeof(RingBufferHeader), '\0',
		   rb->buffer_size);
}

/*
 * Fill one slot: queue header, barrier, payload, barrier. The consumer tests
 * 'exists' before it reads the payload.
 */
static void ldr_ring_buf_write_entry(LdrQueueHeader *queue,
				     const uint8_t *data, uint32_t size)
{
	uint8_t *data_ptr;
	uint8_t *data_ptr_aligned;

	queue->size = (uint16_t)size;
	queue->exists = true;

	LDR_BARRIER();

	data_ptr = (uint8_t *)queue + sizeof(LdrQueueHeader);
	data_ptr_aligned = LDR_ALIGN_PTR_UP_16(data_ptr);

	LDR_MEMCPY(data_ptr_aligned, data, size);

	LDR_BARRIER();
}

ldr_errno_t ldr_ring_buf_mmgr_init(uint8_t *ptr, uint32_t size)
{
	ldr_errno_t ret = LDR_RB_OK;
	volatile RingBufferHeader *ring_buf_head = (RingBufferHeader *)ptr;

	/* A formatted ring is left untouched, so no side wipes live data. */
	if (ring_buf_head->magic_num == LDR_RING_BUFFER_MAGIC_NUMBER) {
		ret = LDR_RB_ALREADY_INITIALIZED;
	} else {
		ldr_ring_buf_header_init(ring_buf_head, size);
	}

	return ret;
}

ldr_errno_t ldr_ring_buf_mmgr_enqueue(uint8_t *rb_ptr, const uint8_t *data,
				      uint32_t size)
{
	volatile RingBufferHeader *ring_buf_head = (RingBufferHeader *)rb_ptr;
	uint32_t queue_size;
	uint32_t head;
	uint32_t tail;
	uint8_t *queue_addr;

	/*
	 * Kept from the middleware, which passes the record size here. Not
	 * reached from BL31: rcar_ldr_init() formats the ring first.
	 */
	if (ring_buf_head->magic_num != LDR_RING_BUFFER_MAGIC_NUMBER) {
		(void)ldr_ring_buf_mmgr_init(rb_ptr, size);
	}

	queue_size = LDR_ALIGN_UP_16((uint32_t)sizeof(LdrQueueHeader) + size);

	LDR_BARRIER();

	head = ring_buf_head->head;
	tail = ring_buf_head->tail;

	if ((ring_buf_head->buffer_size - tail) < queue_size) {
		/*
		 * Fold to offset 0 only into space the consumer has read, and
		 * not while it still has to catch up with an earlier fold.
		 */
		if ((head > tail) || (head <= queue_size)) {
			return LDR_RB_SIZE_OVERFLOW;
		}

		queue_addr = ldr_mem_mmgr_get_queue_entry(rb_ptr, 0U);
		ldr_ring_buf_write_entry((LdrQueueHeader *)queue_addr, data,
					 size);

		/* fold_point: where the data before the fold ends. */
		ring_buf_head->tail = queue_size;
		ring_buf_head->fold_point = tail;
	} else {
		/* After a fold, stop before the oldest unread record. */
		if ((head > tail) && ((tail + queue_size) >= head)) {
			return LDR_RB_SIZE_OVERFLOW;
		}

		queue_addr = ldr_mem_mmgr_get_queue_entry(rb_ptr, tail);
		queue_addr = LDR_ALIGN_PTR_UP_16(queue_addr);

		ldr_ring_buf_write_entry((LdrQueueHeader *)queue_addr, data,
					 size);

		ring_buf_head->tail = tail + queue_size;
	}

	return LDR_RB_OK;
}
