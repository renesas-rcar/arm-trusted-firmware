/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MICRO_WAIT_H__
#define MICRO_WAIT_H__

#define TMU3_MEASUREMENT	(0)	/* for time measurement */

#ifndef __ASSEMBLY__
void micro_wait(uint32_t count_us);

/* Time measurement function using the TMU ch3. */
#if (TMU3_MEASUREMENT == 1)
void init_TMU3(void);
void start_TMU3(void);
uint32_t snapshot_TCNT3(void);
void stop_TMU3(void);
#endif /* TMU3_MEASUREMENT */

#endif /* __ASSEMBLY__ */

#endif /* MICRO_TWAIT_H__ */
