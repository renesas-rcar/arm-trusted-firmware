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

/**
 * @file  emmc_read.c
 * @brief read api
 *
 */

/* ************************ HEADER (INCLUDE) SECTION *********************** */
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_registers.h"
#include "emmc_def.h"

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

#define EMMC_RW_SECTOR_COUNT_MAX        0x0000ffffUL

/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

static EMMC_ERROR_CODE emmc_multiple_block_read (uint32_t *buff_address_virtual, uint32_t sector_number, uint32_t count, HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode);


/* ********************************* CODE ********************************** */

/** @brief function of read sector
 *
 * This function always use block read.
 * Single block read is not used.
 *
 * - Pre-conditions:<BR>
 * MMC card is mounted.
 *
 * - Post-conditions:<BR>
 * .
 *
 * @param[in,out] buff_address_virtual  virtual address of read data buffer.
 * @param[in] sector_number data address for MMC device (sector number).
 * @param[in] count number of sector.
 * @param[in] transfer_mode Mode of data transfer, DMA or not DMA.
 */
EMMC_ERROR_CODE emmc_read_sector (
    uint32_t *buff_address_virtual,
    uint32_t sector_number,
    uint32_t count,
    uint32_t feature_flags
    )
{
    uint32_t trans_count;
    uint32_t remain;
    EMMC_ERROR_CODE result;
    HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode;

    /* parameter check */
    if (count == 0)
    {
        emmc_write_error_info(EMMC_FUNCNO_READ_SECTOR, EMMC_ERR_PARAM);
        return EMMC_ERR_PARAM;
    }       
        
    /* state check */
    if (mmc_drv_obj.mount != TRUE)
    {
        emmc_write_error_info(EMMC_FUNCNO_READ_SECTOR, EMMC_ERR_STATE);
        return EMMC_ERR_STATE;
    }

    /* DMA? */
    if ((feature_flags & LOADIMAGE_FLAGS_DMA_ENABLE) != 0)
    {
        transfer_mode = HAL_MEMCARD_DMA;
    }
    else
    {
        transfer_mode = HAL_MEMCARD_NOT_DMA;
    }

    remain = count;
    while (remain != 0)
    {
        trans_count = MIN(remain, EMMC_RW_SECTOR_COUNT_MAX);
        result = emmc_multiple_block_read(buff_address_virtual, sector_number, trans_count, transfer_mode);
        if (result != EMMC_SUCCESS)
        {
            return result;
        }

        buff_address_virtual += (EMMC_BLOCK_LENGTH_DW*trans_count);
        sector_number += trans_count;
        remain -= trans_count;
    }
    
    return EMMC_SUCCESS;
}

/** @brief multiple block read
 *
 * Multiple block read with pre-defined block count.
 *
 * - Pre-conditions:<BR>
 * MMC card is mounted.
 *
 * - Post-conditions:<BR>
 * .
 *
 * @param[in,out] buff_address_virtual  virtual address of read data buffer.
 * @param[in] sector_number data address for MMC device (sector number).
 * @param[in] count number of sector. (0x1 - 0xffff)
 * @param[in] transfer_mode Mode of data transfer, DMA or not DMA.
 */
static EMMC_ERROR_CODE emmc_multiple_block_read
    (
    uint32_t *buff_address_virtual,
    uint32_t sector_number,
    uint32_t count,
    HAL_MEMCARD_DATA_TRANSFER_MODE transfer_mode
    )
{
    EMMC_ERROR_CODE result;
//    st_error_info   backup;

    /* parameter check */
    if ((count > EMMC_RW_SECTOR_COUNT_MAX)
        || (count == 0)
        || ((transfer_mode != HAL_MEMCARD_DMA) && (transfer_mode != HAL_MEMCARD_NOT_DMA))
        )
    {
        emmc_write_error_info(EMMC_FUNCNO_READ_SECTOR, EMMC_ERR_PARAM);
        return EMMC_ERR_PARAM;
    }

	/* CMD23 */
    emmc_make_nontrans_cmd(CMD23_SET_BLOCK_COUNT, count);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }
    SETR_32( SD_SECCNT, count );
	SETR_32( SD_STOP, 0x00000100 );
	SETR_32( CC_EXT_MODE, (CC_EXT_MODE_CLEAR | CC_EXT_MODE_DMASDRW_ENABLE) );     /* SD_BUF Read/Write DMA Transfer enable */

	/* CMD18 */
    emmc_make_trans_cmd(CMD18_READ_MULTIPLE_BLOCK, sector_number, buff_address_virtual, count<<EMMC_SECTOR_SIZE_SHIFT, HAL_MEMCARD_READ, transfer_mode);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;                  /* CMD18 error code */
    }

	/* CMD13 */
    emmc_make_nontrans_cmd(CMD13_SEND_STATUS, EMMC_RCA<<16);
    result = emmc_exec_cmd(EMMC_R1_ERROR_MASK, mmc_drv_obj.response);
    if (result != EMMC_SUCCESS)
    {
        return result;
    }

    /* ready status check */
    if ( (mmc_drv_obj.r1_card_status & EMMC_R1_READY) == 0) 
    {
        emmc_write_error_info(EMMC_FUNCNO_READ_SECTOR, EMMC_ERR_CARD_BUSY);
        return EMMC_ERR_CARD_BUSY;
    }

    /* state check */
    if (mmc_drv_obj.current_state != EMMC_R1_STATE_TRAN)
    {
        emmc_write_error_info(EMMC_FUNCNO_READ_SECTOR, EMMC_ERR_CARD_STATE);
        return EMMC_ERR_CARD_STATE;
    }

    return EMMC_SUCCESS;
}

/* ******************************** END ************************************ */

