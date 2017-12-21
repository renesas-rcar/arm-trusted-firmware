/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include <debug.h>
#include "dram_sub_func.h"

#if RCAR_SYSTEM_SUSPEND
/* Local defines */
#define DRAM_BACKUP_GPIO_USE	(0)
#if DRAM_BACKUP_GPIO_USE==0
#include "iic_dvfs.h"
#endif
#if PMIC_ROHM_BD9571
#define	PMIC_SLAVE_ADDR		(0x30U)
#define	PMIC_BKUP_MODE_CNT	(0x20U)
#define	BIT_BKUP_CTRL_OUT	((uint8_t)(1U << 4))
#endif /* PMIC_ROHM_BD9571 */

#define GPIO_INDT1		(0xE605100CU)
#if DRAM_BACKUP_GPIO_USE==1
#define GPIO_BKUP_REQB_SHIFT	(9U)
#define	GPIO_OUTDT1		(0xE6051008U)
#endif
#define GPIO_BKUP_TRG_SHIFT	(8U)
#define DRAM_BKUP_TRG_LOOP_CNT	(1000U)
#endif /* RCAR_SYSTEM_SUSPEND */

void dram_get_boot_status(uint32_t *status)
{
#if RCAR_SYSTEM_SUSPEND

	uint32_t reg_data;

	reg_data = mmio_read_32(GPIO_INDT1);
	if (0U != (reg_data & ((uint32_t)1U << GPIO_BKUP_TRG_SHIFT))) {
		*status = DRAM_BOOT_STATUS_WARM;
	} else {
		*status = DRAM_BOOT_STATUS_COLD;
	}
#else	/* RCAR_SYSTEM_SUSPEND */
	*status = DRAM_BOOT_STATUS_COLD;
#endif	/* RCAR_SYSTEM_SUSPEND */
}

int32_t dram_update_boot_status(uint32_t status)
{
	int32_t ret = 0;
#if RCAR_SYSTEM_SUSPEND
	uint32_t reg_data;
#if DRAM_BACKUP_GPIO_USE==0
#if PMIC_ROHM_BD9571
	uint8_t bkup_mode_cnt = 0U;
	int32_t i2c_dvfs_ret = -1;
#endif /* PMIC_ROHM_BD9571 */
#endif
	uint32_t loop_count;

	if (status == DRAM_BOOT_STATUS_WARM) {
#if DRAM_BACKUP_GPIO_USE==1
		mmio_setbits_32(GPIO_OUTDT1, ((uint32_t)1U<<GPIO_BKUP_REQB_SHIFT));
#else
#if PMIC_ROHM_BD9571
		/* Set BKUP_CRTL_OUT=High (BKUP mode cnt register) */
		i2c_dvfs_ret = rcar_iic_dvfs_recieve(PMIC_SLAVE_ADDR,
				PMIC_BKUP_MODE_CNT, &bkup_mode_cnt);
		if (0 != i2c_dvfs_ret) {
			ERROR("BKUP mode cnt READ ERROR.\n");
			ret = DRAM_UPDATE_STATUS_ERR;
		} else {
			bkup_mode_cnt &= (uint8_t)~BIT_BKUP_CTRL_OUT;
			i2c_dvfs_ret = rcar_iic_dvfs_send(PMIC_SLAVE_ADDR,
					PMIC_BKUP_MODE_CNT, bkup_mode_cnt);
			if (0 != i2c_dvfs_ret) {
				ERROR("BKUP mode cnt WRITE ERROR. "
					"value = %d\n", bkup_mode_cnt);
				ret = DRAM_UPDATE_STATUS_ERR;
			}
		}
#endif /* PMIC_ROHM_BD9571 */
#endif /* DRAM_BACKUP_GPIO_USE==1 */
		/* Wait GP1_8(BKUP_TRG)=Low */
		loop_count = DRAM_BKUP_TRG_LOOP_CNT;
		while (0U < loop_count) {
			reg_data = mmio_read_32(GPIO_INDT1);
			if ((reg_data &
				((uint32_t)1U << GPIO_BKUP_TRG_SHIFT)) == 0U) {
				break;
			}
			loop_count--;
		}
		if (0U == loop_count) {
			ERROR(	"\nWarm booting...\n" \
				" The potential of GP-1-8 did not switch " \
				"to Low.\n If you expect the operation of " \
				"cold boot,\n check the board configuration" \
				" (ex, Dip-SW) and/or the H/W failure.\n");
			ret = DRAM_UPDATE_STATUS_ERR;
		}
	}
#endif /* RCAR_SYSTEM_SUSPEND */
	return ret;
}
