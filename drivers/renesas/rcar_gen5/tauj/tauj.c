/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 
/**
 * @file r_scp_tauj.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <arch_helpers.h>
#include <common/debug.h>
#include "tauj.h"


/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup SCP_PRIVATE
 * @defgroup SCP_Private_Defines Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
#define TAUJ3_BASE      0xC1391000U
#define TAUJ3TPS        (*(volatile uint16_t *)(TAUJ3_BASE + 0x0090U))  /* TAUJ3 prescaler clock select register */
#define TAUJ3CDR0       (*(volatile uint32_t *)(TAUJ3_BASE + 0x0000U))  /* TAUJ3 channel data register 0 */
#define TAUJ3CMOR0      (*(volatile uint16_t *)(TAUJ3_BASE + 0x0080U))  /* TAUJ3 channel mode OS register 0 */
#define TAUJ3CMUR0      (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0020U))  /* TAUJ3 channel mode user register 0 */
#define TAUJ3CSC0       (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0040U))  /* TAUJ3 channel status clear trigger register 0 */
#define TAUJ3TS         (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0054U))  /* TAUJ3 channel start trigger register */
#define TAUJ3TT         (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0058U))  /* TAUJ3 channel stop trigger register */
#define TAUJ3TOE        (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0060U))  /* TAUJ3 channel output enable register */
#define TAUJ3TOM        (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0098U))  /* TAUJ3 channel output mode register */
#define TAUJ3TOC        (*(volatile uint8_t  *)(TAUJ3_BASE + 0x009CU))  /* TAUJ3 channel output configuration register */
#define TAUJ3TOL        (*(volatile uint8_t  *)(TAUJ3_BASE + 0x0064U))  /* TAUJ3 channel output active level register */

#define TAUJ3CNT0       (*(volatile uint32_t *)(TAUJ3_BASE + 0x0010U))  /* TAUJ3 channel counter register 0 */

#define SYSSS_MAIN      0xC1320000U
#define CLK_BASE        SYSSS_MAIN
#define CLKKCPROT1      (*(volatile uint32_t *)(CLK_BASE + 0x01F0U)) /* Clock Controller Register Key Code Protection Register 1 */
#define CKSC_ATAUJC     (*(volatile uint32_t *)(CLK_BASE + 0x0120U)) /* CLKA_TAUJ Selector Control Register */
#define CKSC_ATAUJS     (*(volatile uint32_t *)(CLK_BASE + 0x0128U)) /* CLKA_TAUJ Selector Status Register */

#define SYSSS_SCP_AON   0xC1338000U
#define MDLC27_BASE     SYSSS_SCP_AON
#define MDLC27PKCPROT1  (*(volatile uint32_t *)(MDLC27_BASE + 0x0CF4U)) /* Module Standby Register Key Code Protection Register */
#define MDLC27MSRES     (*(volatile uint32_t *)(MDLC27_BASE + 0x0904U)) /* Module Standby Register for TAUJ3 */

/** @} */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
/******************************************************************************************************************//**
 * @ingroup SCP_PRIVATE
 * @defgroup SCP_Private_Functions Private function definitions
 *
 * @{
 *********************************************************************************************************************/

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function tauj_ch00_isr()
***********************************************************************************************************************/
#if defined TEST_ONLY_TAUJ3
/* Please set NVIC_EnableIRQ(Interrupt372_IRQn) on App_SCP_X5H_Sample.c */
/* Please set tauj_ch00_isr() on CM33_startup.c */
/* #define Interrupt372_Handler tauj_ch00_isr */
void tauj_ch00_isr(void)
{
    // SCP_MFIS_CPUIRQ_DISABLE();
    INFO("interrupt of TAUJ3\r\n");
//   TAUJ3CSC0 = 0x01;
    // SCP_MFIS_CPUIRQ_ENABLE();
}
#endif /* TEST_ONLY_TAUJ3 */
/***********************************************************************************************************************
* End of function tauj_ch00_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function setting_tauj_for_test()
***********************************************************************************************************************/
void setting_tauj_for_test(void)
{
    INFO("Setting TAUJ3\r\n");
    TAUJ3TT = 0x01;                     /* STOP TAUJ3 */
    /* Enable CLK_LSIOSC for TAUJ */
    CLKKCPROT1  = 0xA5A5A501U;          /* release protect */
    CKSC_ATAUJC = 0x00000000;           /* select CLK_LSIOSC */
    CLKKCPROT1  = 0xA5A5A500U;          /* set protect */

    /* wait change clock source */
    while (CKSC_ATAUJS != 0x00000000U)
    {
        /* Nothing to do */
    }

    /* Enable clock for TAUJ3 in DeepSTOP mode */
//    MDLC27PKCPROT1 = 0xA5A5A501U;       /* release protect */
//    MDLC27MSRES = 0x00000003U;          /* clock supply */
//    MDLC27PKCPROT1 = 0xA5A5A500U;       /* set protect */

    TAUJ3CDR0 = 0x000003FF;
// #ifdef __VDK__
//     TAUJ3CDR0 = 0x000003FF;
// #else /* __VDK__ */
//     TAUJ3CDR0 = 0x00FFFFFF;
// #endif /* __VDK__ */

    /* Set TAUJ3 timer mode (Countdown/disable Interrupt on starting count) */
    TAUJ3TPS = 0x0000;                  /* PCLK/2^0 */
    TAUJ3CMOR0 = 0x0000;                /* select CK0, Slave, Software trigger, Interval timer mode */
    TAUJ3CMUR0 = 0x00;                  /* Not used, so that set 0 */
    TAUJ3TOE = 0x01;
    TAUJ3TOM = 0x00;
    TAUJ3TOC = 0x00;
    TAUJ3TOL = 0x00;
}
/***********************************************************************************************************************
* End of function setting_tauj_for_test()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function start_tauj_for_test()
***********************************************************************************************************************/
void start_tauj_for_test(void)
{
    INFO("Start TAUJ3\r\n");
    TAUJ3TS = 0x01;                     /* Start TAUJ Countdown from 0xFFFFFFFF */
}
/***********************************************************************************************************************
* End of function start_tauj_for_test()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function stop_tauj_for_test()
***********************************************************************************************************************/
void stop_tauj_for_test(void)
{
    INFO("Stop TAUJ3\r\n");
    TAUJ3TT = 0x01;
}
/***********************************************************************************************************************
* End of function stop_tauj_for_test()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function test_tauj()
***********************************************************************************************************************/
void test_tauj(void)
{
    setting_tauj_for_test();
    start_tauj_for_test();
}
/***********************************************************************************************************************
* End of function test_tauj()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function test_tauj_count()
***********************************************************************************************************************/
void test_tauj_count(void)
{
    uint32_t count = TAUJ3CNT0;
    uint32_t tmp;

    INFO("TAUJ3 count=%d\r\n", count);
    while (0 != count)
    {
        tmp = TAUJ3CNT0;
        if (count != tmp)
        {
            count = tmp;
            INFO("TAUJ3 count=%d\r\n", count);
        }
    }
}
/***********************************************************************************************************************
* End of function test_tauj_count()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/

