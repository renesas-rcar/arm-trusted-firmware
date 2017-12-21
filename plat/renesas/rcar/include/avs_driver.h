/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef AVS_DRIVER_H__
#define AVS_DRIVER_H__

/* AVS Setting. 1:enable / 0:disable */
#ifndef AVS_SETTING_ENABLE
#define AVS_SETTING_ENABLE	1
#endif /* AVS_SETTING_ENABLE */

void bl2_avs_init(void);
void bl2_avs_setting(void);
void bl2_avs_end(void);

#endif  /* AVS_DRIVER_H__ */
