/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
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

#include <debug.h>
#include <stdarg.h>
#include <stdint.h>

#include	<arch_helpers.h>
#include	<platform_def.h>
#include	<bakery_lock.h>
#include	"rcar_def.h"
#include	"rcar_private.h"
#include	"rcar_printf.h"

#define INDEX_TIMER_COUNT	(4U)

extern RCAR_INSTANTIATE_LOCK

typedef struct log_head
{
	uint8_t head[4];
	uint32_t index;
	uint32_t size;
	uint8_t res[4];
} loghead_t;

typedef struct log_map
{
	loghead_t header;
	uint8_t log_data[RCAR_BL31_LOG_MAX];
	uint8_t res_data[RCAR_LOG_RES_SIZE];
} logmap_t;

void rcar_set_log_time(void)
{
	uint64_t now_time;
	uint64_t freq;
	uint64_t second;
	uint64_t micro_sec;
	uint64_t t_log[2][15];
	int32_t i;
	int32_t start_counter;

	now_time = (uint64_t)read_cntpct_el0();
#ifndef IMAGE_BL2
	now_time += rcar_stack_generic_timer[INDEX_TIMER_COUNT];
#endif
	freq = read_cntfrq_el0(); /* get the frequency	*/
	if (freq == 0U) { /* for zero division	*/
		second = 0U;
		micro_sec = 0U;
	} else {
		second = now_time / freq;
		micro_sec = ((now_time % freq) * 1000000U) / freq;
	}
	i = 14; /* counter initialize	*/
	do {
		t_log[0][i] = second % 10U;
		second = second / 10U;
		i--;
	} while (second != 0U);
	for (; i >= 10; i--) {
		t_log[0][i] = (int)' ';
	}
	start_counter = i + 1;
	t_log[1][0] = micro_sec / 100000U;
	micro_sec %= 100000U;
	t_log[1][1] = micro_sec / 10000U;
	micro_sec %= 10000U;
	t_log[1][2] = micro_sec / 1000U;
	micro_sec %= 1000U;
	t_log[1][3] = micro_sec / 100U;
	micro_sec %= 100U;
	t_log[1][4] = micro_sec / 10U;
	t_log[1][5] = micro_sec % 10U;

	(void)putchar((int)'[');
	for (i = start_counter; i < 15; i++) {
		if (t_log[0][i] <= 9) {
			(void)putchar((int)((int)t_log[0][i] + (int)0x30));
		} else {
			(void)putchar((int)' ');
		}
	}
	(void)putchar((int)'.');
	for (i = 0; i < 6; i++) {
		(void)putchar((int)((int)t_log[1][i] + (int)0x30));
	}
	(void)putchar((int)']');
	(void)putchar((int)' ');
}

int32_t rcar_set_log_data(int32_t c)
{
	logmap_t *t_log;

	t_log = (logmap_t *)RCAR_BL31_LOG_BASE;

	rcar_lock_get();

	/*
	 * If index is broken, then index and size initialize
	 */
	if (t_log->header.index	>= (uint32_t)RCAR_BL31_LOG_MAX) {
		t_log->header.index = 0U;
		t_log->header.size = 0U;
	}
	/*
	 * data store to log area then index and size renewal
	 */
	t_log->log_data[t_log->header.index] = (uint8_t)c;
	t_log->header.index++;
	if (t_log->header.size < t_log->header.index) {
		t_log->header.size = t_log->header.index;
	}
	if (t_log->header.index	>= (uint32_t)RCAR_BL31_LOG_MAX) {
		t_log->header.index = 0U;
	}

	rcar_lock_release();

	return(1);
}

int32_t rcar_log_init(void)
{

	static const uint8_t const_header[] = "TLOG";
	logmap_t *t_log;
	int16_t init_flag = 0;

	t_log = (logmap_t *)RCAR_BL31_LOG_BASE;
	if (memcmp((const void *)t_log->header.head, (const void *)const_header, sizeof(t_log->header.head)) != 0) {
		/*
		 * Log header is not "TLOG", then log area initialize
		 */
		init_flag = 1;
	}
	if (t_log->header.index	>= (uint32_t)RCAR_BL31_LOG_MAX) {
		/*
		 * index is broken, then log area initialize
		 */
		init_flag = 1;
	}
	if (init_flag == 1) {
		(void)memset((void *)t_log->log_data, 0, (size_t)RCAR_BL31_LOG_MAX);
		(void)memcpy((void *)t_log->header.head, (const void *)const_header, sizeof(t_log->header.head));
		t_log->header.index = 0U;
		t_log->header.size = 0U;
#ifndef IMAGE_BL2
		rcar_stack_generic_timer[INDEX_TIMER_COUNT] = 0U;
#endif
	}
	rcar_lock_init();

	return(1);
}
