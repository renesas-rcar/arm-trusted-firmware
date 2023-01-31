/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PTP_H
#define PTP_H

#if PTP_NONSECURE_ACCESS
void rcar_ptp_setup(void);
#else  /* PTP_NONSECURE_ACCESS */
void rcar_ptp_setup(void) {};
#endif /* PTP_NONSECURE_ACCESS */

#endif /* PTP_H */
