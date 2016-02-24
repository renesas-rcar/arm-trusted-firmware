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
#include "bl2_cpg_write.h"
#include "bl2_cpg_register.h"
#include "avs_driver.h"

#if AVS_SETTING_ENABLE == 1
/* Read PMIC register for debug. 1:enable / 0:disable */
#define AVS_READ_PMIC_REG_ENABLE	0
/* The re-try number of times of the AVS setting. */
#define AVS_RETRY_NUM			(1U)

/* Base address of ADVFS Module registers*/
#define	ADVFS_BASE			(0xE60A0000U)
/* Register address in eFuse indicates individual R-Car information */
#define	ADVFS_KSEN_ADJCNTS		(ADVFS_BASE + 0x013CU)

/* Mask VOLCOND bit in KSEN_ADJCNTS registers */
#define	KSEN_ADJCNTS_VOLCOND_MASK	(0x0000000FU)
#define	VOLCOND_INVALID_VAL		(0x7U)	/* Invalid value */
#define	VOLCOND_CANCEL_BIT		(0x8U)	/* Cancel bit */

/* Mode Monitor Register */
#define	RCAR_MODEMR			(0xE6160060U)	/* Mode pin register */
#define MODEMR_MD13			(0x2000U)	/* MD13 bit mask */
#define MODEMR_MD14			(0x4000U)	/* MD14 bit mask */
/* CP Phy input frequency */
#define	CP_PHY_FREQ_8p33M		(0x0000U)	/* 8.3333MHz */
#define	CP_PHY_FREQ_10M			(0x2000U)	/* 10MHz */
#define	CP_PHY_FREQ_12p5M		(0x4000U)	/* 12.5MHz */
#define	CP_PHY_FREQ_16p66M		(0x6000U)	/* 16.6666MHz */

/* I2C for DVFS bit in CPG registers for module standby and software reset*/
#define CPG_SYS_DVFS_BIT		(0x04000000U)
/* ADVFS Module bit in CPG registers for module standby and software reset*/
#define CPG_SYS_ADVFS_BIT		(0x02000000U)

/* Base address of IICDVFS registers*/
#define	IIC_DVFS_BASE			(0xE60B0000U)
/* IIC bus data register */
#define	IIC_ICDR			(IIC_DVFS_BASE + 0x0000U)
/* IIC bus control register */
#define	IIC_ICCR			(IIC_DVFS_BASE + 0x0004U)
/* IIC bus status register */
#define	IIC_ICSR			(IIC_DVFS_BASE + 0x0008U)
/* IIC interrupt control register */
#define	IIC_ICIC			(IIC_DVFS_BASE + 0x000CU)
/* IIC clock control register low */
#define	IIC_ICCL			(IIC_DVFS_BASE + 0x0010U)
/* IIC clock control register high */
#define	IIC_ICCH			(IIC_DVFS_BASE + 0x0014U)

/* Bit in ICSR register */
#define ICSR_BUSY			(0x10U)
#define ICSR_AL				(0x08U)
#define ICSR_TACK			(0x04U)
#define ICSR_WAIT			(0x02U)
#define ICSR_DTE			(0x01U)

/* Bit in ICIC register */
#define ICIC_TACKE			(0x04U)
#define ICIC_WAITE			(0x02U)
#define ICIC_DTEE			(0x01U)

/* I2C bus interface enable */
#define ICCR_ENABLE			(0x80U)
/* Start condition */
#define ICCR_START			(0x94U)
/* Stop condition */
#define ICCR_STOP			(0x90U)
/* Restart condition with change to receive mode change */
#define ICCR_START_RECV			(0x81U)
/* Stop condition for receive mode */
#define ICCR_STOP_RECV			(0xC0U)

/* Low-level period of SCL */
#define	ICCL_FREQ_8p33M			(0x07U)	/* for CP Phy 8.3333MHz */
#define	ICCL_FREQ_10M			(0x09U)	/* for CP Phy 10MHz */
#define	ICCL_FREQ_12p5M			(0x0BU)	/* for CP Phy 12.5MHz */
#define	ICCL_FREQ_16p66M		(0x0EU)	/* for CP Phy 16.6666MHz */
/* High-level period of SCL */
#define	ICCH_FREQ_8p33M			(0x01U)	/* for CP Phy 8.3333MHz */
#define	ICCH_FREQ_10M			(0x02U)	/* for CP Phy 10MHz */
#define	ICCH_FREQ_12p5M			(0x03U)	/* for CP Phy 12.5MHz */
#define	ICCH_FREQ_16p66M		(0x05U)	/* for CP Phy 16.6666MHz */

#define	PMIC_W_SLAVE_ADDRESS		(0x60U)	/* PMIC slave address + (W) */
#define	PMIC_R_SLAVE_ADDRESS		(0x61U)	/* PMIC slave address + (R) */
#define	PMIC_DVFS_SETVID		(0x54U)	/* PMIC DVFS SetVID register */

/* Individual information */
#define EFUSE_AVS0			(0U)
#define EFUSE_AVS_NUM			(7U)
static const uint8_t pmic_setvid_reg[EFUSE_AVS_NUM] = {
		0x52U, /* AVS0 0.82V */
		0x52U, /* AVS1 0.82V */
		0x52U, /* AVS2 0.82V */
		0x4FU, /* AVS3 0.79V */
		0x4FU, /* AVS4 0.79V */
		0x4DU, /* AVS5 0.77V */
		0x4BU  /* AVS6 0.75V */
};

/* Kind of AVS settings status */
typedef enum
{
		avs_status_none = 0,
		avs_status_init,
		avs_status_start_condition,
		avs_status_set_slave_addr,
		avs_status_write_reg_addr,
		avs_status_write_reg_data,
		avs_status_stop_condition,
		avs_status_end,
		avs_status_complete,
		avs_status_al_start,
		avs_status_al_transfer,
		avs_status_nack,
		avs_status_error_stop,
		ave_status_error_end
} avs_status_t;

/* Kind of AVS error */
typedef enum
{
		avs_error_none = 0,
		avs_error_al,
		avs_error_nack
} avs_error_t;

static avs_status_t avs_status = avs_status_none;
static uint32_t avs_retry = 0U;
static uint32_t efuse_avs = EFUSE_AVS0;

/* prototype */
static avs_error_t avs_check_error(void);
static void avs_set_iic_clock(void);
#if AVS_READ_PMIC_REG_ENABLE == 1
static uint8_t avs_read_pmic_reg(uint8_t addr);
static void avs_poll(uint8_t bit_pos, uint8_t val);
#endif
#endif	/* AVS_SETTING_ENABLE */

/*
 * Initialize to enable the AVS setting.
 */
void bl2_avs_init(void)
{
#if AVS_SETTING_ENABLE == 1
	uint32_t val;
	uint32_t mstp;

	INFO("BL2: AVS setting start. \n");

	/* Initialize AVS status */
	avs_status = avs_status_init;

	/* Bit of the module which wants to enable clock supply. */
	mstp = CPG_SYS_DVFS_BIT | CPG_SYS_ADVFS_BIT;
	/* Enables the clock supply to the CPG. */
	cpg_write(CPG_SMSTPCR9, mmio_read_32(CPG_SMSTPCR9) & (~mstp));
	/* Is the clock supply to the CPG disabled ? */
	while((mmio_read_32(CPG_MSTPSR9) & mstp) != 0U) {
		/* Enables the clock supply to the CPG. */
		cpg_write(CPG_SMSTPCR9,
				mmio_read_32(CPG_SMSTPCR9) & (~mstp));
	}

	/* Disable I2C module and All internal registers initialized. */
	mmio_write_8(IIC_ICCR, 0x00U);
	while ((mmio_read_8(IIC_ICCR) & ICCR_ENABLE) != 0U) {
		/* Disable I2C module and All internal registers initialized. */
		mmio_write_8(IIC_ICCR, 0x00U);
	}

	/* Read eFuse indicates individual R-Car information */
	val = mmio_read_32(ADVFS_KSEN_ADJCNTS) & KSEN_ADJCNTS_VOLCOND_MASK;

	if ((val == VOLCOND_INVALID_VAL)
			|| ((val & VOLCOND_CANCEL_BIT) == VOLCOND_CANCEL_BIT)) {
		efuse_avs = EFUSE_AVS0;
	} else {
		efuse_avs = val;
	}

	/* Set next status */
	avs_status = avs_status_start_condition;

	INFO("BL2: Read KSEN_ADJCNTS.VOLCOND=0x%x efuse_avs=%u\n",
		val, efuse_avs);
#endif	/* AVS_SETTING_ENABLE */
}

/*
 * Set the value of register corresponding to the voltage
 * by transfer of I2C to PIMC.
 */
void bl2_avs_setting(void)
{
#if AVS_SETTING_ENABLE == 1
	avs_error_t err;

	INFO("BL2: bl2_avs_setting avs_status=%d\n", avs_status);

	switch (avs_status) {
	case avs_status_start_condition:
		/* Set ICCR.ICE=1 to activate the I2C module. */
		mmio_write_8(IIC_ICCR, mmio_read_8(IIC_ICCR) | ICCR_ENABLE);
		/* Set frequency of 400kHz */
		avs_set_iic_clock();
		/* Set ICIC.TACKE=1, ICIC.WAITE=1, ICIC.DTEE=1 to */
		/* enable interrupt control. 			  */
		mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC)
				| ICIC_TACKE | ICIC_WAITE | ICIC_DTEE);
		/* Write H'94 in ICCR to issue start condition */
		mmio_write_8(IIC_ICCR, ICCR_START);
		/* Set next status */
		avs_status = avs_status_set_slave_addr;
		break;
	case avs_status_set_slave_addr:
		/* Check error. */
		err = avs_check_error();
		if (err == avs_error_al) {
			/* Recovery sequence of just after start. */
			avs_status = avs_status_al_start;
		} else if (err == avs_error_nack) {
			/* Recovery sequence of detected NACK */
			avs_status = avs_status_nack;
		} else {
			/* Was data transmission enabled ? */
			if ((mmio_read_8(IIC_ICSR) & ICSR_DTE) == ICSR_DTE) {
				/* Clear ICIC.DTEE to disable a DTE interrupt */
				mmio_write_8(IIC_ICIC,
					mmio_read_8(IIC_ICIC)
						& (uint8_t)(~ICIC_DTEE));
				/* Send PMIC slave address + (W) */
				mmio_write_8(IIC_ICDR, PMIC_W_SLAVE_ADDRESS);
				/* Set next status */
				avs_status = avs_status_write_reg_addr;
			}
		}
		break;
	case avs_status_write_reg_addr:
		/* Check error. */
		err = avs_check_error();
		if (err == avs_error_al) {
			/* Recovery sequence of during data transfer. */
			avs_status = avs_status_al_transfer;
		} else if (err == avs_error_nack) {
			/* Recovery sequence of detected NACK */
			avs_status = avs_status_nack;
		} else {
			/* If wait state after data transmission. */
			if ((mmio_read_8(IIC_ICSR) & ICSR_WAIT) == ICSR_WAIT) {
				/* Write PMIC DVFS_SetVID address */
				mmio_write_8(IIC_ICDR, PMIC_DVFS_SETVID);
				/* Clear ICSR.WAIT to exit from wait state. */
				mmio_write_8(IIC_ICSR,
					mmio_read_8(IIC_ICSR)
						& (uint8_t)(~ICSR_WAIT));
				/* Set next status */
				avs_status = avs_status_write_reg_data;
			}
		}
		break;
	case avs_status_write_reg_data:
		/* Check error. */
		err = avs_check_error();
		if (err == avs_error_al) {
			/* Recovery sequence of during data transfer. */
			avs_status = avs_status_al_transfer;
		} else if (err == avs_error_nack) {
			/* Recovery sequence of detected NACK */
			avs_status = avs_status_nack;
		} else {
			/* If wait state after data transmission. */
			if ((mmio_read_8(IIC_ICSR) & ICSR_WAIT) == ICSR_WAIT) {
				/* Dose efuse_avs exceed the number of */
				/* the tables? */
				if (efuse_avs >= EFUSE_AVS_NUM) {
					ERROR("BL2: AVS number of eFuse is out "
						"of a range. number=%u\n",
						 efuse_avs);
					/* Infinite loop */
					panic();
				}
				/* Write PMIC DVFS_SetVID value */
				mmio_write_8(IIC_ICDR,
						pmic_setvid_reg[efuse_avs]);
				/* Clear ICSR.WAIT to exit from wait state. */
				mmio_write_8(IIC_ICSR,
					mmio_read_8(IIC_ICSR)
						& (uint8_t)(~ICSR_WAIT));
				/* Set next status */
				avs_status = avs_status_stop_condition;
			}
		}
		break;
	case avs_status_stop_condition:
		err = avs_check_error();
		if (err == avs_error_al) {
			/* Recovery sequence of during data transfer. */
			avs_status = avs_status_al_transfer;
		} else if (err == avs_error_nack) {
			/* Recovery sequence of detected NACK */
			avs_status = avs_status_nack;
		} else {
			/* If wait state after data transmission. */
			if ((mmio_read_8(IIC_ICSR) & ICSR_WAIT) == ICSR_WAIT) {
				/* Write H'90 in ICCR to issue stop condition */
				mmio_write_8(IIC_ICCR, ICCR_STOP);
				/* Clear ICSR.WAIT to exit from wait state. */
				mmio_write_8(IIC_ICSR,
					mmio_read_8(IIC_ICSR)
						& (uint8_t)(~ICSR_WAIT));
				/* Set next status */
				avs_status = avs_status_end;
			}
		}
		break;
	case avs_status_end:
		/* Is this module not busy?. */
		if ((mmio_read_8(IIC_ICSR) & ICSR_BUSY) == 0U) {
			/* Set ICCR=H'00 to disable the I2C module. */
			mmio_write_8(IIC_ICCR, 0x00U);
			/* Set next status */
			avs_status = avs_status_complete;
		}
		break;
	case avs_status_al_start:
		/* Clear ICSR.AL bit */
		mmio_write_8(IIC_ICSR,
				(mmio_read_8(IIC_ICSR)
					& (uint8_t)(~ICSR_AL)));
		/* Transmit a clock pulse */
		mmio_write_8(IIC_ICDR, pmic_setvid_reg[EFUSE_AVS0]);
		/* Set next status */
		avs_status = avs_status_error_stop;
		break;
	case avs_status_al_transfer:
		/* Clear ICSR.AL bit */
		mmio_write_8(IIC_ICSR,
				(mmio_read_8(IIC_ICSR)
					& (uint8_t)(~ICSR_AL)));
		/* Set next status */
		avs_status = avs_status_error_stop;
		break;
	case avs_status_nack:
		/* Write H'90 in ICCR to issue stop condition */
		mmio_write_8(IIC_ICCR, ICCR_STOP);
		/* Disable a WAIT and DTEE interrupt. */
		mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC)
				& (uint8_t)(~(ICIC_WAITE | ICIC_DTEE)));
		/* Clear ICSR.TACK bit */
		mmio_write_8(IIC_ICSR, mmio_read_8(IIC_ICSR)
				& (uint8_t)(~ICSR_TACK));
		/* Set next status */
		avs_status = ave_status_error_end;
		break;
	case avs_status_error_stop:
		/* If wait state after data transmission. */
		if ((mmio_read_8(IIC_ICSR) & ICSR_WAIT) == ICSR_WAIT) {
			/* Write H'90 in ICCR to issue stop condition */
			mmio_write_8(IIC_ICCR, ICCR_STOP);
			/* Clear ICSR.WAIT to exit from wait state. */
			mmio_write_8(IIC_ICSR,
					mmio_read_8(IIC_ICSR)
						& (uint8_t)(~ICSR_WAIT));
			/* Set next status */
			avs_status = ave_status_error_end;
		}
		break;
	case ave_status_error_end:
		/* Is this module not busy?. */
		if ((mmio_read_8(IIC_ICSR) & ICSR_BUSY) == 0U) {
			/* Set ICCR=H'00 to disable the I2C module. */
			mmio_write_8(IIC_ICCR, 0x00U);
			/* Increment the re-try number of times. */
			avs_retry++;
			/* Set start a re-try to status. */
			avs_status = avs_status_start_condition;
		}
		break;
	case avs_status_complete:
		/* After "avs_status" became the "avs_status_complete", */
		/* "bl2_avs_setting()" function may be called. */
		break;
	default:
		/* This case is not possible. */
		ERROR("BL2: AVS setting is in invalid status. status=%u\n",
			avs_status);
		/* Infinite loop */
		panic();
		break;
	}
#endif	/* AVS_SETTING_ENABLE */
}

/*
 * Finish the AVS setting.
 */
void bl2_avs_end(void)
{
#if AVS_SETTING_ENABLE == 1
	uint32_t mstp;

	INFO("BL2: bl2_avs_end avs_status=%d\n", avs_status);

	/* While status is not completion, be repeated. */
	while (avs_status != avs_status_complete) {
		bl2_avs_setting();
	}
	NOTICE("BL2: AVS setting succeeded. DVFS_SetVID=0x%x\n",
			pmic_setvid_reg[efuse_avs]);

#if AVS_READ_PMIC_REG_ENABLE == 1
	{
		uint8_t addr = PMIC_DVFS_SETVID;
		uint8_t value;
		value = avs_read_pmic_reg(addr);
		NOTICE("BL2: Read PMIC register. address=0x%x value=0x%x \n",
				addr, value);
	}
#endif

	/* Bit of the module which wants to disable clock supply. */
	mstp = CPG_SYS_DVFS_BIT | CPG_SYS_ADVFS_BIT;
	/* Disables the supply of clock signal to a module. */
	cpg_write(CPG_SMSTPCR9,
			mmio_read_32(CPG_SMSTPCR9) | mstp);
#endif	/* AVS_SETTING_ENABLE */
}

#if AVS_SETTING_ENABLE == 1
/*
 * Check error and judge re-try.
 */
static avs_error_t avs_check_error(void)
{
	avs_error_t ret;

	INFO("BL2: avs_check_error avs_status=%d\n", avs_status);

	if ((mmio_read_8(IIC_ICSR) & ICSR_AL) == ICSR_AL) {
		NOTICE("BL2: Loss of arbitration is detected. "
				"AVS status=%d Retry=%u\n",
				avs_status, avs_retry);
		/* Check of retry number of times*/
		if (avs_retry >= AVS_RETRY_NUM) {
			ERROR("BL2: AVS setting failed in retry. max=%u\n",
				AVS_RETRY_NUM);
			/* Infinite loop */
			panic();
		}
		/* Set the error detected to error status. */
		ret = avs_error_al;
	} else if ((mmio_read_8(IIC_ICSR) & ICSR_TACK) == ICSR_TACK) {
		NOTICE("BL2: Non-acknowledge is detected. "
				"AVS status=%d Retry=%u\n",
				avs_status, avs_retry);
		/* Check of retry number of times*/
		if (avs_retry >= AVS_RETRY_NUM) {
			ERROR("BL2: AVS setting failed in retry. max=%u\n",
				AVS_RETRY_NUM);
			/* Infinite loop */
			panic();
		}
		/* Set the error detected to error status. */
		ret = avs_error_nack;
	} else {
		/* Not error. */
		ret = avs_error_none;
	}
	return ret;
}

/*
 * Set I2C for DVFS clock.
 */
static void avs_set_iic_clock(void)
{
	uint32_t md_pin;

	/* Read Mode pin register. */
	md_pin = mmio_read_32(RCAR_MODEMR) & (MODEMR_MD13 | MODEMR_MD14);
	switch (md_pin) {
	case CP_PHY_FREQ_8p33M:		/* 8.3333MHz */
		mmio_write_8(IIC_ICCL, ICCL_FREQ_8p33M);
		mmio_write_8(IIC_ICCH, ICCH_FREQ_8p33M);
		break;
	case CP_PHY_FREQ_10M:		/* 10MHz */
		mmio_write_8(IIC_ICCL, ICCL_FREQ_10M);
		mmio_write_8(IIC_ICCH, ICCH_FREQ_10M);
		break;
	case CP_PHY_FREQ_12p5M:		/* 12.5MHz */
		mmio_write_8(IIC_ICCL, ICCL_FREQ_12p5M);
		mmio_write_8(IIC_ICCH, ICCH_FREQ_12p5M);
		break;
	case CP_PHY_FREQ_16p66M:	/* 16.6666MHz */
		mmio_write_8(IIC_ICCL, ICCL_FREQ_16p66M);
		mmio_write_8(IIC_ICCH, ICCH_FREQ_16p66M);
		break;
	default:			/* This case is not possible. */
		/* CP Phy frequency is to be set for the 16.66MHz */
		mmio_write_8(IIC_ICCL, ICCL_FREQ_16p66M);
		mmio_write_8(IIC_ICCH, ICCH_FREQ_16p66M);
		break;
	}
}

#if AVS_READ_PMIC_REG_ENABLE == 1
/*
 * Read the value of the register of PMIC.
 */
static uint8_t avs_read_pmic_reg(uint8_t addr)
{
	uint8_t reg;

	INFO("BL2: Read PMIC register start.\n");

	/* Set ICCR.ICE=1 to activate the I2C module. */
	mmio_write_8(IIC_ICCR, mmio_read_8(IIC_ICCR) | ICCR_ENABLE);

	/* Set frequency of 400kHz */
	avs_set_iic_clock();

	/* Set ICIC.WAITE=1, ICIC.DTEE=1 to enable data transmission	*/
	/* interrupt and wait interrupt.				*/
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) | ICIC_WAITE | ICIC_DTEE);

	/* Write H'94 in ICCR to issue start condition */
	mmio_write_8(IIC_ICCR, ICCR_START);

	/* Wait for a until ICSR.DTE becomes 1. */
	avs_poll( ICSR_DTE, 1U);

	/* Clear ICIC.DTEE to disable a DTE interrupt. */
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) & (uint8_t)(~ICIC_DTEE));
	/* Send slave address of PMIC */
	mmio_write_8(IIC_ICDR, PMIC_W_SLAVE_ADDRESS);

	/* Wait for a until ICSR.WAIT becomes 1. */
	avs_poll(ICSR_WAIT, 1U);

	/* write PMIC address */
	mmio_write_8(IIC_ICDR, addr);
	/* Clear ICSR.WAIT to exit from WAIT status. */
	mmio_write_8(IIC_ICSR, mmio_read_8(IIC_ICSR) & (uint8_t)(~ICSR_WAIT));

	/* Wait for a until ICSR.WAIT becomes 1. */
	avs_poll(ICSR_WAIT, 1U);

	/* Write H'94 in ICCR to issue restart condition */
	mmio_write_8(IIC_ICCR, ICCR_START);
	/* Clear ICSR.WAIT to exit from WAIT status. */
	mmio_write_8(IIC_ICSR, mmio_read_8(IIC_ICSR) & (uint8_t)(~ICSR_WAIT));
	/* Set ICIC.DTEE=1 to enable data transmission interrupt. */
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) | ICIC_DTEE);

	/* Wait for a until ICSR.DTE becomes 1. */
	avs_poll( ICSR_DTE, 1U);

	/* Clear ICIC.DTEE to disable a DTE interrupt. */
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) & (uint8_t)(~ICIC_DTEE));
	/* Send slave address of PMIC */
	mmio_write_8(IIC_ICDR, PMIC_R_SLAVE_ADDRESS);

	/* Wait for a until ICSR.WAIT becomes 1. */
	avs_poll(ICSR_WAIT, 1U);

	/* Write H'81 to ICCR to issue the repeated START condition 	*/
	/* for changing the transmission mode to the receive mode.	*/
	mmio_write_8(IIC_ICCR, ICCR_START_RECV);
	/* Clear ICSR.WAIT to exit from WAIT status. */
	mmio_write_8(IIC_ICSR, mmio_read_8(IIC_ICSR) & (uint8_t)(~ICSR_WAIT));

	/* Wait for a until ICSR.WAIT becomes 1. */
	avs_poll(ICSR_WAIT, 1U);

	/* Set ICCR to H'C0 for the STOP condition */
	mmio_write_8(IIC_ICCR, ICCR_STOP_RECV);
	/* Clear ICSR.WAIT to exit from WAIT status. */
	mmio_write_8(IIC_ICSR, mmio_read_8(IIC_ICSR) & (uint8_t)(~ICSR_WAIT));
	/* Set ICIC.DTEE=1 to enable data transmission interrupt. */
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) | ICIC_DTEE);

	/* Wait for a until ICSR.DTE becomes 1. */
	avs_poll( ICSR_DTE, 1U);

	/* Receive DVFS SetVID register */
	/* Clear ICIC.DTEE to disable a DTE interrupt. */
	mmio_write_8(IIC_ICIC, mmio_read_8(IIC_ICIC) & (uint8_t)(~ICIC_DTEE));
	/* Receive DVFS SetVID register */
	reg = mmio_read_8(IIC_ICDR);

	/* Wait until ICSR.BUSY is cleared. */
	avs_poll( ICSR_BUSY, 0U);

	/* Set ICCR=H'00 to disable the I2C module. */
	mmio_write_8(IIC_ICCR, 0x00U);

	return reg;
}

/*
 * Wait processing by the polling.
 */
static void avs_poll(uint8_t bit_pos, uint8_t val)
{
	uint8_t bit_val = 0U;

	if (val != 0U) {
		bit_val = bit_pos;
	}

	while (1) {
		if ((mmio_read_8(IIC_ICSR) & bit_pos) == bit_val) {
			break;
		}
	}
}
#endif	/* AVS_READ_PMIC_REG_ENABLE */
#endif	/* AVS_SETTING_ENABLE */
