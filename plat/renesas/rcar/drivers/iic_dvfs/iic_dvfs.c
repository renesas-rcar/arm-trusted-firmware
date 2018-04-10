/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <mmio.h>
#include <debug.h>
#include "rcar_def.h"
#include "bl2_cpg_register.h"
#include "iic_dvfs.h"
#include "rcar_private.h"

#define DVFS_RETRY_MAX			(2U)

#define IIC_DVFS_SET_ICCL_EXTAL_TYPE_0	(0x07U)
#define IIC_DVFS_SET_ICCL_EXTAL_TYPE_1	(0x09U)
#define IIC_DVFS_SET_ICCL_EXTAL_TYPE_2	(0x0BU)
#define IIC_DVFS_SET_ICCL_EXTAL_TYPE_3	(0x0EU)
#define IIC_DVFS_SET_ICCL_EXTAL_TYPE_E	(0x15U)

#define IIC_DVFS_SET_ICCH_EXTAL_TYPE_0	(0x01U)
#define IIC_DVFS_SET_ICCH_EXTAL_TYPE_1	(0x02U)
#define IIC_DVFS_SET_ICCH_EXTAL_TYPE_2	(0x03U)
#define IIC_DVFS_SET_ICCH_EXTAL_TYPE_3	(0x05U)
#define IIC_DVFS_SET_ICCH_EXTAL_TYPE_E	(0x07U)

#define CPG_BIT_SMSTPCR9_DVFS		(0x04000000U)

#define IIC_DVFS_REG_BASE		(0xE60B0000U)
#define IIC_DVFS_REG_ICDR		(IIC_DVFS_REG_BASE + 0x0000U)
#define IIC_DVFS_REG_ICCR		(IIC_DVFS_REG_BASE + 0x0004U)
#define IIC_DVFS_REG_ICSR		(IIC_DVFS_REG_BASE + 0x0008U)
#define IIC_DVFS_REG_ICIC		(IIC_DVFS_REG_BASE + 0x000CU)
#define IIC_DVFS_REG_ICCL		(IIC_DVFS_REG_BASE + 0x0010U)
#define IIC_DVFS_REG_ICCH		(IIC_DVFS_REG_BASE + 0x0014U)

#define IIC_DVFS_BIT_ICSR_BUSY		(0x10U)
#define IIC_DVFS_BIT_ICSR_AL		(0x08U)
#define IIC_DVFS_BIT_ICSR_TACK		(0x04U)
#define IIC_DVFS_BIT_ICSR_WAIT		(0x02U)
#define IIC_DVFS_BIT_ICSR_DTE		(0x01U)

#define IIC_DVFS_BIT_ICCR_ENABLE		(0x80U)
#define IIC_DVFS_SET_ICCR_START			(0x94U)
#define IIC_DVFS_SET_ICCR_STOP			(0x90U)
#define	IIC_DVFS_SET_ICCR_RETRANSMISSION	(0x94U)
#define	IIC_DVFS_SET_ICCR_CHANGE		(0x81U)
#define	IIC_DVFS_SET_ICCR_STOP_READ		(0xC0U)

#define IIC_DVFS_BIT_ICIC_TACKE		(0x04U)
#define IIC_DVFS_BIT_ICIC_WAITE		(0x02U)
#define IIC_DVFS_BIT_ICIC_DTEE		(0x01U)

#define	DVFS_READ_MODE			(0x01U)
#define	DVFS_WRITE_MODE			(0x00U)

#define IIC_DVFS_SET_DUMMY              (0x52U)
#define IIC_DVFS_SET_BUSY_LOOP          (500000000U)

typedef enum {
	DVFS_START_CONDITION = 0,
	DVFS_SET_SLAVE_ADDR,
	DVFS_WRITE_REG_ADDR,
	DVFS_WRITE_REG_DATA,
	DVFS_STOP_CONDITION,
	DVFS_PROCESS_COMPLETE,
	DVFS_SET_RETRANSMISSION_CONDITION,
	DVFS_SET_SLAVE_ADDR_READ,
	DVFS_CHANGE_SEND_TO_RECIEVE,
	DVFS_STOP_CONDITION_READ,
	DVFS_READ_REG_DATA
}DVFS_STATUS_T;

#define DVFS_PROCESS			(1)
#define DVFS_COMPLETE			(0)
#define DVFS_ERROR			(-1)

static int32_t dvfs_check_error(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t mode);
static int32_t dvfs_start_condition(DVFS_STATUS_T *status);
static int32_t dvfs_set_slave_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr);
static int32_t dvfs_write_reg_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr);
static int32_t dvfs_write_reg_data(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_data);
static int32_t dvfs_stop_condition(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_process_complete(void);
static int32_t dvfs_write_reg_addr_read(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr);
static int32_t dvfs_set_retransmission_condition(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_set_slave_addr_read(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr);
static int32_t dvfs_change_send_to_recieve(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_stop_condition_read(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_read_reg_data(DVFS_STATUS_T *status, uint8_t *reg_data);

int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	rcar_iic_dvfs_send(
		uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_data)
{
	int32_t result;
	DVFS_STATUS_T status;
	uint32_t err_count;

	/* Enable clock supply to DVFS. */
	mstpcr_write(SCMSTPCR9, CPG_MSTPSR9, CPG_BIT_SMSTPCR9_DVFS);

	/* Disable IIC-DVFS module */
	mmio_write_8(IIC_DVFS_REG_ICCR, 0x00U);

	status = DVFS_START_CONDITION;
	result = DVFS_PROCESS;
	err_count = 0U;

	while (result == DVFS_PROCESS) {
		switch (status) {
		case DVFS_START_CONDITION:
			result = dvfs_start_condition(&status);
			break;
		case DVFS_SET_SLAVE_ADDR:
			result = dvfs_set_slave_addr(&status, &err_count, slave_addr);
			break;
		case DVFS_WRITE_REG_ADDR:
			result = dvfs_write_reg_addr(&status, &err_count, reg_addr);
			break;
		case DVFS_WRITE_REG_DATA:
			result = dvfs_write_reg_data(&status, &err_count, reg_data);
			break;
		case DVFS_STOP_CONDITION:
			result = dvfs_stop_condition(&status, &err_count);
			break;
		case DVFS_PROCESS_COMPLETE:
			result = dvfs_process_complete();
			break;
		default:
			panic();
			break;
		}
	}

	return result;
}

int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	rcar_iic_dvfs_recieve(
		uint8_t slave_addr, uint8_t reg_addr, uint8_t *reg_data)
{
	int32_t result;
	DVFS_STATUS_T status;
	uint32_t err_count;

	/* Enable clock supply to DVFS. */
	mstpcr_write(SCMSTPCR9, CPG_MSTPSR9, CPG_BIT_SMSTPCR9_DVFS);

	/* Disable IIC-DVFS module */
	mmio_write_8(IIC_DVFS_REG_ICCR, 0x00U);

	status = DVFS_START_CONDITION;
	result = DVFS_PROCESS;
	err_count = 0U;

	while (result == DVFS_PROCESS) {
		switch (status) {
		case DVFS_START_CONDITION:
			result = dvfs_start_condition(&status);
			break;
		case DVFS_SET_SLAVE_ADDR:
			result = dvfs_set_slave_addr(&status, &err_count, slave_addr);
			break;
		case DVFS_WRITE_REG_ADDR:
			result = dvfs_write_reg_addr_read(&status, &err_count, reg_addr);
			break;
		case DVFS_SET_RETRANSMISSION_CONDITION:
			result = dvfs_set_retransmission_condition(&status, &err_count);
			break;
		case DVFS_SET_SLAVE_ADDR_READ:
			result = dvfs_set_slave_addr_read(&status, &err_count, slave_addr);
			break;
		case DVFS_CHANGE_SEND_TO_RECIEVE:
			result = dvfs_change_send_to_recieve(&status, &err_count);
			break;
		case DVFS_STOP_CONDITION_READ:
			result = dvfs_stop_condition_read(&status, &err_count);
			break;
		case DVFS_READ_REG_DATA:
			result = dvfs_read_reg_data(&status, reg_data);
			break;
		case DVFS_PROCESS_COMPLETE:
			result = dvfs_process_complete();
			break;
		default:
			panic();
			break;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_check_error(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t mode)
{
	int32_t error;
	uint8_t reg;
	uint32_t loop_cnt;
	uint8_t stop_code;

	stop_code = IIC_DVFS_SET_ICCR_STOP;
	if (mode == DVFS_READ_MODE) {
		stop_code = IIC_DVFS_SET_ICCR_STOP_READ;
	}
	reg = mmio_read_8(IIC_DVFS_REG_ICSR);
	if ((reg & IIC_DVFS_BIT_ICSR_AL) == IIC_DVFS_BIT_ICSR_AL) {
		reg = mmio_read_8(IIC_DVFS_REG_ICSR)
			& ((uint8_t)(~IIC_DVFS_BIT_ICSR_AL));
		/* Clear the AL flag */
		mmio_write_8(IIC_DVFS_REG_ICSR, reg);
		if (*status == DVFS_SET_SLAVE_ADDR) {
			mmio_write_8(IIC_DVFS_REG_ICDR, IIC_DVFS_SET_DUMMY);
		}
		while ((mmio_read_8(IIC_DVFS_REG_ICSR) & IIC_DVFS_BIT_ICSR_WAIT)
								== 0x00U) {
		}
		/* Write stop condition */
		mmio_write_8(IIC_DVFS_REG_ICCR, stop_code);
		/* Clear the WAIT flag */
		reg = mmio_read_8(IIC_DVFS_REG_ICSR)
			& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
		mmio_write_8(IIC_DVFS_REG_ICSR, reg);
		loop_cnt = 0U;
		while ((mmio_read_8(IIC_DVFS_REG_ICSR) & IIC_DVFS_BIT_ICSR_BUSY)
								!= 0x00U) {
			loop_cnt++;
			if (loop_cnt > IIC_DVFS_SET_BUSY_LOOP) {
				panic();
			}
		}
		mmio_write_8(IIC_DVFS_REG_ICCR, 0x00U);
		/* error count increment */
		(*err_count)++;
		if (*err_count >= DVFS_RETRY_MAX) {
			error = DVFS_ERROR;
		} else {
			/* status initialize */
			*status = DVFS_START_CONDITION;
			error = DVFS_PROCESS;
		}
	} else if ((reg & IIC_DVFS_BIT_ICSR_TACK) == IIC_DVFS_BIT_ICSR_TACK) {
		/* Write stop condition */
		mmio_write_8(IIC_DVFS_REG_ICCR, stop_code);
		/* Disable WAIT and DTE interrupt */
		reg = mmio_read_8(IIC_DVFS_REG_ICIC);
		reg &= ((uint8_t)(~(IIC_DVFS_BIT_ICIC_WAITE | IIC_DVFS_BIT_ICIC_DTEE)));
		mmio_write_8(IIC_DVFS_REG_ICIC, reg);
		/* Clear the TACK flag */
		reg = mmio_read_8(IIC_DVFS_REG_ICSR)
			& ((uint8_t)(~IIC_DVFS_BIT_ICSR_TACK));
		mmio_write_8(IIC_DVFS_REG_ICSR, reg);
		loop_cnt = 0U;
		while ((mmio_read_8(IIC_DVFS_REG_ICSR) & IIC_DVFS_BIT_ICSR_BUSY)
								!= 0x00U) {
			loop_cnt++;
			if (loop_cnt > IIC_DVFS_SET_BUSY_LOOP) {
				panic();
			}
		}
		mmio_write_8(IIC_DVFS_REG_ICCR, 0x00U);
		(*err_count)++;
		if (*err_count >= DVFS_RETRY_MAX) {
			error = DVFS_ERROR;
		} else {
			/* status initialize */
			*status = DVFS_START_CONDITION;
			error = DVFS_PROCESS;
		}
	} else {
		error = DVFS_PROCESS;
	}

	return error;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_start_condition(
		DVFS_STATUS_T *status)
{
	uint32_t reg;
	uint8_t mode;
	int32_t result;
	uint32_t lsi_product;

	result = DVFS_PROCESS;

	lsi_product = (mmio_read_32((uintptr_t)RCAR_PRR) & RCAR_PRODUCT_MASK);

	/* Set ICCR.ICE */
	mode = mmio_read_8(IIC_DVFS_REG_ICCR)
				| ((uint8_t)(IIC_DVFS_BIT_ICCR_ENABLE));
	mmio_write_8(IIC_DVFS_REG_ICCR, mode);

	/* Set clock */
	if (lsi_product == RCAR_PRODUCT_E3) {
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_E);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_E);
	} else {	
		reg = mmio_read_32(RCAR_MODEMR) & CHECK_MD13_MD14;
		switch (reg) {
		case MD14_MD13_TYPE_0:
			mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_0);
			mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_0);
			break;
		case MD14_MD13_TYPE_1:
			mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_1);
			mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_1);
			break;
		case MD14_MD13_TYPE_2:
			mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_2);
			mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_2);
			break;
		case MD14_MD13_TYPE_3:
			mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_3);
			mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_3);
			break;
		default:
			mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_EXTAL_TYPE_3);
			mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_EXTAL_TYPE_3);
			break;
		}
	}
	/* Set ICIC.TACKE, ICIC.WAITE and ICIC.DTEE */
	mode = mmio_read_8(IIC_DVFS_REG_ICIC)
					| IIC_DVFS_BIT_ICIC_TACKE
					| IIC_DVFS_BIT_ICIC_WAITE
					| IIC_DVFS_BIT_ICIC_DTEE;
	mmio_write_8(IIC_DVFS_REG_ICIC, mode);
	/* Write start condition */
	mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_START);
	/* Next status */
	*status = DVFS_SET_SLAVE_ADDR;

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_set_slave_addr(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr)
{
	uint8_t mode;
	int32_t result;
	uint8_t address;

	/* error check */
	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_DTE);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_DTE)) {
			/* Clear ICIC.DTEE */
			mode = mmio_read_8(IIC_DVFS_REG_ICIC)
					& ((uint8_t)(~IIC_DVFS_BIT_ICIC_DTEE));
			mmio_write_8(IIC_DVFS_REG_ICIC, mode);
			/* Send 7bit slave address + write mode bit */
			address = (uint8_t)(slave_addr << 1);
			mmio_write_8(IIC_DVFS_REG_ICDR, address);
			/* Next status */
			*status = DVFS_WRITE_REG_ADDR;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_write_reg_addr(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Send register address */
			mmio_write_8(IIC_DVFS_REG_ICDR, reg_addr);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Next status */
			*status = DVFS_WRITE_REG_DATA;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_write_reg_data(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_data)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Send data */
			mmio_write_8(IIC_DVFS_REG_ICDR, reg_data);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Next status */
			*status = DVFS_STOP_CONDITION;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_stop_condition(
		DVFS_STATUS_T *status, uint32_t *err_count)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Write stop condition */
			mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_STOP);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Next status */
			*status = DVFS_PROCESS_COMPLETE;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_process_complete(
		void)
{
	int32_t result;
	uint32_t loop_cnt;

	loop_cnt = 0U;
	while ((mmio_read_8(IIC_DVFS_REG_ICSR) & ((uint8_t)IIC_DVFS_BIT_ICSR_BUSY))
							!= 0x00U) {
		loop_cnt++;
		if (loop_cnt > IIC_DVFS_SET_BUSY_LOOP) {
			panic();
		}
	}

	/* Disable IIC for DVFS */
	mmio_write_8(IIC_DVFS_REG_ICCR, 0x00U);
	/* process complete */
	result = DVFS_COMPLETE;

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_write_reg_addr_read(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Send register address */
			mmio_write_8(IIC_DVFS_REG_ICDR, reg_addr);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Next status */
			*status = DVFS_SET_RETRANSMISSION_CONDITION;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_set_retransmission_condition(
		DVFS_STATUS_T *status, uint32_t *err_count)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* set ReTransmission condition */
			mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_RETRANSMISSION);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Set ICIC.DTEE */
			mode = mmio_read_8(IIC_DVFS_REG_ICIC) | IIC_DVFS_BIT_ICIC_DTEE;
			mmio_write_8(IIC_DVFS_REG_ICIC, mode);
			/* Next status */
			*status = DVFS_SET_SLAVE_ADDR_READ;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_set_slave_addr_read(
		DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr)
{
	uint8_t mode;
	int32_t result;
	uint8_t address;

	/* error check */
	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_DTE);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_DTE)) {
			/* Clear ICIC.DTEE */
			mode = mmio_read_8(IIC_DVFS_REG_ICIC)
					& ((uint8_t)(~IIC_DVFS_BIT_ICIC_DTEE));
			mmio_write_8(IIC_DVFS_REG_ICIC, mode);
			/* Send 7bit slave address + read mode bit */
			address = ((uint8_t)(slave_addr << 1) + DVFS_READ_MODE);
			mmio_write_8(IIC_DVFS_REG_ICDR, address);
			/* Next status */
			*status = DVFS_CHANGE_SEND_TO_RECIEVE;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_change_send_to_recieve(
		DVFS_STATUS_T *status, uint32_t *err_count)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_WRITE_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Set the change transmission to reception */
			mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_CHANGE);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Next status */
			*status = DVFS_STOP_CONDITION_READ;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_stop_condition_read(
		DVFS_STATUS_T *status, uint32_t *err_count)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count, DVFS_READ_MODE);
	if (result != DVFS_ERROR) {
		mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT);
		if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_WAIT)) {
			/* Write stop condition */
			mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_STOP_READ);
			/* Clear ICSR.WAIT */
			mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)(~IIC_DVFS_BIT_ICSR_WAIT));
			mmio_write_8(IIC_DVFS_REG_ICSR, mode);
			/* Set ICIC.DTEE */
			mode = mmio_read_8(IIC_DVFS_REG_ICIC) | IIC_DVFS_BIT_ICIC_DTEE;
			mmio_write_8(IIC_DVFS_REG_ICIC, mode);
			/* Next status */
			*status = DVFS_READ_REG_DATA;
		}
	}

	return result;
}

static int32_t
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	dvfs_read_reg_data(
		DVFS_STATUS_T *status, uint8_t *reg_data)
{
	uint8_t mode;

	mode = mmio_read_8(IIC_DVFS_REG_ICSR)
					& ((uint8_t)IIC_DVFS_BIT_ICSR_DTE);
	if (mode == ((uint8_t)IIC_DVFS_BIT_ICSR_DTE)) {
		/* Clear ICIC.DTEE */
		mode = mmio_read_8(IIC_DVFS_REG_ICIC)
				& ((uint8_t)(~IIC_DVFS_BIT_ICIC_DTEE));
		mmio_write_8(IIC_DVFS_REG_ICIC, mode);
		/* Recieve data */
		*reg_data = mmio_read_8(IIC_DVFS_REG_ICDR);
		/* Next status */
		*status = DVFS_PROCESS_COMPLETE;
	}

	return DVFS_PROCESS;
}
