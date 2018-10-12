/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICRO_WAIT_H__
#define MICRO_WAIT_H__

void micro_wait(uint64_t micro_sec);
uint64_t snapshot_CNTPCT_EL0(void);

#endif /* MICRO_TWAIT_H__ */
