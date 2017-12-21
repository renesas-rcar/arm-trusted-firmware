/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DRAM_SUB_FUNC_H_
#define DRAM_SUB_FUNC_H_

#define DRAM_BOOT_STATUS_COLD	(0U)
#define DRAM_BOOT_STATUS_WARM	(1U)

#define DRAM_UPDATE_STATUS_ERR	(-1)

void dram_get_boot_status(uint32_t *status);
int32_t dram_update_boot_status(uint32_t status);

#endif /* DRAM_SUB_FUNC_H_ */
