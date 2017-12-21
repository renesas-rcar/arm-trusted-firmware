/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* SA0 */
/* 0x00000000 */
const unsigned int __attribute__ ((section (".sa0_bootrom"))) bootrom_paramA = 0x00000100;
/* 0x00000080 (Map Type 3 for eMMC Boot)*/
/* 0x000001D4 */
const unsigned int __attribute__ ((section (".sa0_bl2dst_addr3"))) bl2dst_addr3 = 0xE6304000;
/* 0x000002E4 */
const unsigned int __attribute__ ((section (".sa0_bl2dst_size3"))) bl2dst_size3 = 0x0000AA00;
/* 0x00000C00 (Map Type 1 for HyperFlash/QSPI Flash Boot)*/
/* 0x00000D54 */
const unsigned int __attribute__ ((section (".sa0_bl2dst_addr1"))) bl2dst_addr1 = 0xE6304000;
/* 0x00000E64 */
const unsigned int __attribute__ ((section (".sa0_bl2dst_size1"))) bl2dst_size1 = 0x0000AA00;

