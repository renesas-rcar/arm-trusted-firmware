/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MFIS_H
#define MFIS_H

/******************************************************************************/
/* Definition                                                                 */
/******************************************************************************/
/* MFIS lock targets */
typedef enum {
	MFIS_TARGET_RPC = 0,
	MFIS_TARGET_RESERVED,
	MFIS_TARGET_UFS,
	MFIS_TARGET_OVERLAP_CHECK,
	MFIS_TARGET_MEASUREMENT,
	MFIS_TARGET_HSCIF,
	MFIS_TARGET_MAX,
} mfis_target_t;

#define MFIS_LOCK_TIMEOUT  (1000000U)

/******************************************************************************/
/* Prototype                                                                  */
/******************************************************************************/
void rcar_mfis_init(void);
void rcar_mfis_lock(mfis_target_t target);
void rcar_mfis_unlock(mfis_target_t target);

#endif /* MFIS_H */
