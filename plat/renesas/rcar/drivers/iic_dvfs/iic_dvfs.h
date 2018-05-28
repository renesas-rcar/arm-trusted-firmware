/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IIC_DVFS_H__
#define IIC_DVFS_H__

#define SLAVE_ADDR_PMIC		(0x30U)

#define	REG_ADDR_DVFS_SetVID	(0x54U)
#define	REG_DATA_DVFS_SetVID_0V	(0x00U)

#define	REG_ADDR_BKUP_Mode_Cnt	(0x20U)
#define	REG_DATA_P_ALL_OFF	(0x80U)
#define	REG_DATA_KEEPON_DDR1C	(0x08U)
#define	REG_DATA_KEEPON_DDR0C	(0x04U)
#define	REG_DATA_KEEPON_DDR1	(0x02U)
#define	REG_DATA_KEEPON_DDR0	(0x01U)

#define	REG_ADDR_REG_KEEP10		(0x79U)
#define	REG_DATA_REG_KEEP10_MAGIC	(0x55U)

int32_t rcar_iic_dvfs_send(uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_data);
int32_t rcar_iic_dvfs_recieve(uint8_t slave_addr, uint8_t reg_addr, uint8_t *reg_data);

#endif  /* IIC_DVFS_H__ */
