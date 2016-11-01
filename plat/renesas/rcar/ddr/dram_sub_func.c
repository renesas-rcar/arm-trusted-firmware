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

#include <mmio.h>
#include <debug.h>
#include "dram_sub_func.h"
//#include "iic_dvfs.h"

/* Local defines */
#define DRAM_BACKUP_GPIO_USE	(0)
#if DRAM_BACKUP_GPIO_USE==0
#include "iic_dvfs.h"
#endif

#define	PMIC_SLAVE_ADDR		(0x30U)
#define	PMIC_BKUP_MODE_CNT	(0x20U)
#define	BIT_BKUP_CTRL_OUT	((uint8_t)(1U << 4))

#define GPIO_INDT1		(0xE605100CU)
#if DRAM_BACKUP_GPIO_USE==1
#define	GPIO_OUTDT1		(0xE6051008U)
#endif

void dram_get_boot_status(uint32_t *status)
{
	uint32_t reg_data;

	reg_data = mmio_read_32(GPIO_INDT1);
	if (0 != (reg_data & (1 << 8))) {
		*status = DRAM_BOOT_STATUS_WARM;
	} else {
		*status = DRAM_BOOT_STATUS_COLD;
	}
}

int32_t dram_update_boot_status(uint32_t status)
{
	int32_t ret = 0;
	uint32_t reg_data;
#if DRAM_BACKUP_GPIO_USE==0
	uint8_t bkup_mode_cnt = 0U;
	int32_t i2c_dvfs_ret = -1;
#endif
	uint32_t loop_count;

	if (status == DRAM_BOOT_STATUS_WARM) {

#if DRAM_BACKUP_GPIO_USE==1
		mmio_setbits_32(GPIO_OUTDT1, (1<<9));
#else
		/* Set BKUP_CRTL_OUT=High (BKUP mode cnt register) */
		i2c_dvfs_ret = rcar_iic_dvfs_recieve(PMIC_SLAVE_ADDR,
				PMIC_BKUP_MODE_CNT, &bkup_mode_cnt);
		if (0 != i2c_dvfs_ret) {
			ERROR("BKUP mode cnt READ ERROR.\n");
			ret = DRAM_UPDATE_STATUS_ERR;
		} else {
			INFO("     BKUP mode cnt READ value = %d\n",
				bkup_mode_cnt);
			bkup_mode_cnt &= ~BIT_BKUP_CTRL_OUT;
			i2c_dvfs_ret = rcar_iic_dvfs_send(PMIC_SLAVE_ADDR,
					PMIC_BKUP_MODE_CNT, bkup_mode_cnt);
			if (0 != i2c_dvfs_ret) {
				ERROR("BKUP mode cnt WRITE ERROR. "
					"value = %d\n", bkup_mode_cnt);
				ret = DRAM_UPDATE_STATUS_ERR;
			} else {
				INFO("BKUP_TRG = %s, BKUP_REQB = %s\n",
					*((volatile uint32_t*)GPIO_INDT1)
					& (1<<8) ? "1" : "0",
					*((volatile uint32_t*)GPIO_INDT1)
					& (1<<9) ? "1" : "0");
			}
		}
#endif
		/* Wait GP1_8(BKUP_TRG)=Low */
		loop_count = 1000;
		while (0 < loop_count) {
			reg_data = mmio_read_32(GPIO_INDT1);
			if ((reg_data & (1 << 8)) == 0) {
				break;
			}
			loop_count--;
		}
		if (0 == loop_count) {
			ret = DRAM_UPDATE_STATUS_ERR;
		}
	}

	return ret;
}
