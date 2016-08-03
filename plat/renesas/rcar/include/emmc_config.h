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
 * @file  emmc_config.h
 * @brief Configuration file
 *
 */

#ifndef __EMMC_CONFIG_H__
#define __EMMC_CONFIG_H__

/* ************************ HEADER (INCLUDE) SECTION *********************** */

/* ***************** MACROS, CONSTANTS, COMPILATION FLAGS ****************** */

/** @brief MMC driver config
 */
#define EMMC_RCA                1UL                       /* RCA  */
#define EMMC_RW_DATA_TIMEOUT    0x40UL                    /* 314ms (freq = 400KHz, timeout Counter = 0x04(SDCLK * 2^17)  */
#define EMMC_RETRY_COUNT        0                         /* how many times to try after fail. Don't change. */
#define EMMC_CMD_MAX            60UL                      /* Don't change. */

/** @brief etc
 */
#define LOADIMAGE_FLAGS_DMA_ENABLE              0x00000001UL


/* ********************** STRUCTURES, TYPE DEFINITIONS ********************* */

/* ********************** DECLARATION OF EXTERNAL DATA ********************* */

/* ************************** FUNCTION PROTOTYPES ************************** */

/* ********************************* CODE ********************************** */

#endif  /* #ifndef __EMMC_CONFIG_H__ */
/* ******************************** END ************************************ */

