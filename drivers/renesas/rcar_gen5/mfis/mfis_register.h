/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MFIS_REGISTER_H
#define MFIS_REGISTER_H

/******************************************************************************/
/* Definition                                                                 */
/******************************************************************************/
/* MFIS_COMMON_BASE base address  */
#define MFIS_COMMON_BASE		(0x189E0000uL)
/* MFIS lock 0 Register Address  */
#define MFIS_MFISLCKR0			(MFIS_COMMON_BASE + (0x000000C0uL))
/* MFIS lock 1 Register Address  */
#define MFIS_MFISLCKR1			(MFIS_COMMON_BASE + (0x000000C4uL))
/* MFIS lock 2 Register Address  */
#define MFIS_MFISLCKR2			(MFIS_COMMON_BASE + (0x000000C8uL))
/* MFIS lock 3 Register Address  */
#define MFIS_MFISLCKR3			(MFIS_COMMON_BASE + (0x000000CCuL))
/* MFIS lock 4 Register Address  */
#define MFIS_MFISLCKR4			(MFIS_COMMON_BASE + (0x000000D0uL))
/* MFIS lock 5 Register Address  */
#define MFIS_MFISLCKR5			(MFIS_COMMON_BASE + (0x000000D4uL))
/* MFIS lock 6 Register Address  */
#define MFIS_MFISLCKR6			(MFIS_COMMON_BASE + (0x000000D8uL))
/* MFIS lock 7 Register Address  */
#define MFIS_MFISLCKR7			(MFIS_COMMON_BASE + (0x000000DCuL))
/* Write Protection Control Register  */
#define MFIS_MFISWPCNTR			(MFIS_COMMON_BASE + (0x00000900uL))
/* Write Access Control Register Address  */
#define MFIS_MFISWACNTR			(MFIS_COMMON_BASE + (0x00000904uL))


/* MFISLCKR bit[0] (LCK) Mask   */
#define MFISLCKR_LCK_MASK                                     (0x00000001uL)
/* MFISLCKR bit[0] (LCK) Mutex Control Forbidden Set   */
#define MFISLCKR_LCK_FORBIDDEN_SET                            (0x00000001uL)
/* MFISLCKR bit[0] (LCK) Mutex Control Release Set   */
#define MFISLCKR_LCK_RELEASE_SET                              (0x00000000uL)
/* MFISLCKR bit[0] (LCK) Mutex acquired Check  */
#define MFISLCKR_LCK_ACQUIRED_CHECK                           (0x00000001uL)
/* MFISLCKR bit[0] (LCK) Mutex not acquired Check  */
#define MFISLCKR_LCK_NOT_ACQUIRED_CHECK                       (0x00000000uL)
/* MFISWPCNTR bit[31:16] (Code value) Set  */
#define MFISWPCNTR_CODEVALUE_SET                              (0xACCE0000uL)
/* MFISWPCNTR bit[0] (WPD) 0 = write protection ENABLED  */
#define MFISWPCNTR_WPD_SET                                    (0x00000000uL)
/* MFISWPCNTR bit[0] (WPD) 1 = write protection DISABLED (unlocked)  */
#define MFISWPCNTR_WPD_UNLOCK                                 (0x00000001uL)
/* MFISWACNTR bit[31:16] (Code value) Mask  */
#define MFISWACNTR_CODEVALUE_MASK                             (0xFFFF0000uL)
/* MFISWACNTR bit[31:16] (Code value) Set  */
#define MFISWACNTR_CODEVALUE_SET                              (0xACCE0000uL)
#define MFISWACNTR_SCP_CODEVALUE_SET                          (0xACC00000uL)
/* MFISWACNTR bit[15:0] (RegisterAddress) Set  */
#define MFISWACNTR_REGISTERADDRESS_MASK                       (0x0000FFFFuL)

/******************************************************************************/
/* Prototype                                                                  */
/******************************************************************************/

#endif /* MFIS_REGISTER_H */
