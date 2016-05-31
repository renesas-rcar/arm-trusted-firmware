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
#include "../../rcar_def.h"
#include "iic_dvfs.h"

#define DVFS_RETRY_MAX			(2U)

#define MODEMR_SET_MASK_13_14		(0x6000U)
#define MODEMR_SET_10p00M		(0x2000U)
#define MODEMR_SET_12p50M		(0x4000U)
#define MODEMR_SET_08p33M		(0x0000U)
#define MODEMR_SET_16p66M		(0x6000U)

#define IIC_DVFS_SET_ICCL_08p33M	(0x07U)
#define IIC_DVFS_SET_ICCL_10p00M	(0x09U)
#define IIC_DVFS_SET_ICCL_12p50M	(0x0BU)
#define IIC_DVFS_SET_ICCL_16p66M	(0x0EU)

#define IIC_DVFS_SET_ICCH_08p33M	(0x01U)
#define IIC_DVFS_SET_ICCH_10p00M	(0x02U)
#define IIC_DVFS_SET_ICCH_12p50M	(0x03U)
#define IIC_DVFS_SET_ICCH_16p66M	(0x05U)

#define CPG_REG_BASE			(0xE6150000U)
#define CPG_REG_SMSTPCR9		(CPG_REG_BASE + 0x0994U)
#define CPG_REG_MSTPSR9			(CPG_REG_BASE + 0x09A4U)

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

#define IIC_DVFS_BIT_ICCR_ENABLE	(0x80U)
#define IIC_DVFS_SET_ICCR_START		(0x94U)
#define IIC_DVFS_SET_ICCR_STOP		(0x90U)

#define IIC_DVFS_BIT_ICIC_TACKE		(0x04U)
#define IIC_DVFS_BIT_ICIC_WAITE		(0x02U)
#define IIC_DVFS_BIT_ICIC_DTEE		(0x01U)

#define IIC_DVFS_SET_DUMMY		(0x52U)
#define IIC_DVFS_SET_BUSY_LOOP		(500000000U)

typedef enum {
	DVFS_START_CONDITION = 0,
	DVFS_SET_SLAVE_ADDR,
	DVFS_WRITE_REG_ADDR,
	DVFS_WRITE_REG_DATA,
	DVFS_STOP_CONDITION,
	DVFS_PROCESS_COMPLETE
}DVFS_STATUS_T;

#define DVFS_PROCESS			(1)
#define DVFS_COMPLETE			(0)
#define DVFS_ERROR			(-1)

static int32_t dvfs_check_error(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_start_condition(DVFS_STATUS_T *status);
static int32_t dvfs_set_slave_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr);
static int32_t dvfs_write_reg_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr);
static int32_t dvfs_write_reg_data(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_data);
static int32_t dvfs_stop_condition(DVFS_STATUS_T *status, uint32_t *err_count);
static int32_t dvfs_process_complete(void);

int32_t rcar_iic_dvfs_send(uint8_t slave_addr, uint8_t reg_addr, uint8_t reg_data)
{
	int32_t result;
	uint32_t reg;
	DVFS_STATUS_T status;
	uint32_t err_count;

	/* Clock supply of DVFS is enabled */
	reg = mmio_read_32(CPG_REG_SMSTPCR9) & ~CPG_BIT_SMSTPCR9_DVFS;
	mmio_write_32(RCAR_CPGWPR, ~reg);
	mmio_write_32(CPG_REG_SMSTPCR9, reg);
	while ((mmio_read_32(CPG_REG_MSTPSR9) & CPG_BIT_SMSTPCR9_DVFS)
							!= 0x00000000U) {
	}

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

static int32_t dvfs_check_error(DVFS_STATUS_T *status, uint32_t *err_count)
{
	int32_t error;
	uint8_t reg;
	uint32_t loop_cnt;

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
		mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_STOP);
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
		mmio_write_8(IIC_DVFS_REG_ICCR, IIC_DVFS_SET_ICCR_STOP);
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

static int32_t dvfs_start_condition(DVFS_STATUS_T *status)
{
	uint32_t reg;
	uint8_t mode;
	int32_t result;

	result = DVFS_PROCESS;
	/* Set ICCR.ICE */
	mode = mmio_read_8(IIC_DVFS_REG_ICCR)
				| ((uint8_t)(IIC_DVFS_BIT_ICCR_ENABLE));
	mmio_write_8(IIC_DVFS_REG_ICCR, mode);
	/* Set clock */
	reg = mmio_read_32(RCAR_MODEMR) & MODEMR_SET_MASK_13_14;
	switch (reg) {
	case MODEMR_SET_08p33M:
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_08p33M);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_08p33M);
		break;
	case MODEMR_SET_10p00M:
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_10p00M);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_10p00M);
		break;
	case MODEMR_SET_12p50M:
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_12p50M);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_12p50M);
		break;
	case MODEMR_SET_16p66M:
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_16p66M);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_16p66M);
		break;
	default:
		mmio_write_8(IIC_DVFS_REG_ICCL, IIC_DVFS_SET_ICCL_16p66M);
		mmio_write_8(IIC_DVFS_REG_ICCH, IIC_DVFS_SET_ICCH_16p66M);
		break;
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

static int32_t dvfs_set_slave_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t slave_addr)
{
	uint8_t mode;
	int32_t result;
	uint8_t address;

	/* error check */
	result = dvfs_check_error(status, err_count);
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

static int32_t dvfs_write_reg_addr(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_addr)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count);
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

static int32_t dvfs_write_reg_data(DVFS_STATUS_T *status, uint32_t *err_count, uint8_t reg_data)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count);
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

static int32_t dvfs_stop_condition(DVFS_STATUS_T *status, uint32_t *err_count)
{
	uint8_t mode;
	int32_t result;

	result = dvfs_check_error(status, err_count);
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

static int32_t dvfs_process_complete(void)
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
