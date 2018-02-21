/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation
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

/*
 * Revision history
 *
 * rev.0.01    2018/01/12    New
 * rev.0.02    2018/01/16    DBSC_E3_DBMEMCONF00 16bit(0x0f030a01) -> 32bit(0x0f030a02) 
 *                           ddr_init_e3.h : RCAR_DDR_VERSION "rev.0.01" -> RCAR_E3_DDR_VERSION "rev.0.02"
 * rev.0.03    2018/01/24    add setting for DX2,DX3 ( serch keyword "rev.0.03" )
 *                            *Correction of missing setting of DX2 and DX3
 *                           change setting for QOS
 *                            both "ddr_init_e31866" and "ddr_init_e31600"
 *                            DBSC_E3_DBSCHQOS90 0x00000300 -> 0x00000100
 *                            DBSC_E3_DBSCHQOS91 0x000002F0 -> 0x000000F0
 *                            DBSC_E3_DBSCHQOS92 0x00000200 -> 0x000000A0
 *                            DBSC_E3_DBSCHQOS93 0x00000100 -> 0x00000040
 *            2018/02/05    Switch memory size by compile option
 *                            *option name : RCAR_MEM_DENS
 *                           ddr_init_e3.h : RCAR_DDR_VERSION "rev.0.02" -> RCAR_E3_DDR_VERSION "rev.0.03"
 *                           Switch Refresh rate by compile option ( ddr_init_e3.h )
 *             2018/02/07    Change return code : 1 -> 0
 *                            *To incorporate into IPL software
 *             2018/02/08    Add InitDram
 *                           Add define and etern ( ddr_init_e3.h )
 *                           Change function declaration( ddr_init_e3.h )
 */


#include <stdint.h>
#include "boot_init_dram_regdef_e3.h"
#include "ddr_init_e3.h"

// rev.0.03 add Prototypes
/*******************************************************************************
 *	Prototypes
 ******************************************************************************/
static uint32_t init_ddr_e31866(void); // rev.0.03
//static uint32_t init_ddr_e31600(void); // rev.0.03

// rev.0.03 add Comment
/*******************************************************************************
 *	register write/read function
 ******************************************************************************/
static void    WriteReg_32(uint32_t a, uint32_t v)
{
    (*(volatile uint32_t*)(uintptr_t)a) = v;
}

static uint32_t ReadReg_32(uint32_t a)
{
    uint32_t w = (*(volatile uint32_t*)(uintptr_t)a);
    return w;
}

// rev.0.03 add Comment
/*******************************************************************************
*	Initialize ddr (MD19=1:DDR3L,1856Mbps)
 ******************************************************************************/
uint32_t init_ddr_e31866(void)
{

    uint32_t RegVal_R2, RegVal_R5, RegVal_R6, RegVal_R7, RegVal_R12, i;

   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00001234);
   WriteReg_32(DBSC_E3_DBKIND,0x00000007);

#ifdef ddr_qos_init_setting                        // rev.0.03
//   WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a01);
   WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02);    // rev.0.02
#else                                              // rev.0.03
   #if RCAR_MEM_DENS == 0                          // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #elif RCAR_MEM_DENS == 1                        // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #else                                           // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #endif                                          // rev.0.03
#endif                                             // rev.0.03
   WriteReg_32(DBSC_E3_DBPHYCONF0,0x00000001);
   WriteReg_32(DBSC_E3_DBTR0,0x0000000D);
   WriteReg_32(DBSC_E3_DBTR1,0x00000009);
   WriteReg_32(DBSC_E3_DBTR2,0x00000000);
   WriteReg_32(DBSC_E3_DBTR3,0x0000000D);
   WriteReg_32(DBSC_E3_DBTR4,0x000D000D);
   WriteReg_32(DBSC_E3_DBTR5,0x0000002D);
   WriteReg_32(DBSC_E3_DBTR6,0x00000020);
   WriteReg_32(DBSC_E3_DBTR7,0x00060006);
   WriteReg_32(DBSC_E3_DBTR8,0x00000021);
   WriteReg_32(DBSC_E3_DBTR9,0x00000007);
   WriteReg_32(DBSC_E3_DBTR10,0x0000000E);
   WriteReg_32(DBSC_E3_DBTR11,0x0000000C);
   WriteReg_32(DBSC_E3_DBTR12,0x00140014);
   WriteReg_32(DBSC_E3_DBTR13,0x000000F2);
   WriteReg_32(DBSC_E3_DBTR14,0x00170006);
   WriteReg_32(DBSC_E3_DBTR15,0x00060005);
   WriteReg_32(DBSC_E3_DBTR16,0x09210507);
   WriteReg_32(DBSC_E3_DBTR17,0x040E0000);
   WriteReg_32(DBSC_E3_DBTR18,0x00000200);
   WriteReg_32(DBSC_E3_DBTR19,0x0129004B);
   WriteReg_32(DBSC_E3_DBTR20,0x020000FB);
   WriteReg_32(DBSC_E3_DBTR21,0x00040004);
   WriteReg_32(DBSC_E3_DBBL,0x00000000);
   WriteReg_32(DBSC_E3_DBODT0,0x00000001);
   WriteReg_32(DBSC_E3_DBADJ0,0x00000001);
   WriteReg_32(DBSC_E3_DBSYSCONF1,0x00000002);
   WriteReg_32(DBSC_E3_DBDFICNT0,0x00000010);
   WriteReg_32(DBSC_E3_DBBCAMDIS,0x00000001);
   WriteReg_32(DBSC_E3_DBSCHRW1,0x00000046);
   WriteReg_32(DBSC_E3_SCFCTST0,0x0C050B03);
   WriteReg_32(DBSC_E3_SCFCTST1,0x0305030C);

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step0( INITBYP )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDLK0,0x0000A55A);
   WriteReg_32(DBSC_E3_DBCMD,0x01000001);
   WriteReg_32(DBSC_E3_DBCMD,0x08000000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x80010000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step1( ZCAL,PLLINIT,DCAL,PHYRST training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000008);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000B8000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058A04);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000091);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BB6B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000095);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BBAD);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000099);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BB6B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058A00);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000021);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0024641E);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010073);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step2( DRAMRST/DRAMINT training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0C058A00);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058A00);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000003);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0780C700);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000007);
   while ( (BIT30 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000004);
   WriteReg_32(DBSC_E3_DBPDRGD0,(uint32_t)(REFRESH_RATE*928/125)-400 + 0x0A300000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000022);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x1000040B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000023);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x35A00D77);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000024);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x2A8A2C28);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000025);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x30005E00);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000026);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0014CB49);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000027);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00000F14);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000028);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00000046);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000029);
   if (REFRESH_RATE > 3900) {
       WriteReg_32(DBSC_E3_DBPDRGD0,0x00000020);  //           [7]SRT=0
   } else {
       WriteReg_32(DBSC_E3_DBPDRGD0,0x000000A0);  //           [7]SRT=1
   }
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000002C);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x81003047);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000020);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00181884);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000001A);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x33C03C10);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000107);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000108);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000109);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010181);
   WriteReg_32(DBSC_E3_DBCMD,0x08000001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step3( WL/QSG training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010601);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step4( WLADJ training )
    ***************************************************************************/
   for ( i = 0; i<4; i++)
   {
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B1 + i*0x20);
      RegVal_R5 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x0000FF00) >> 0x8;
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B4 + i*0x20);
      RegVal_R6 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x000000FF);
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B3 + i*0x20);
      RegVal_R7 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x00000007);
      if ( RegVal_R6 > 0 )
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R7 + 0x1) & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | RegVal_R6);
      } else 
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | RegVal_R7);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 + ((RegVal_R5) << 1)) & 0x000000FF));
      }
   }

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000005);
   WriteReg_32(DBSC_E3_DBPDRGD0,0xC1AA00C0);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010801);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step5678( RdWrbitRdWreye )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000005);
   WriteReg_32(DBSC_E3_DBPDRGD0,0xC1AA00D8);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0001F001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step3_2( DQS Gate Training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000002C);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x81003087);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010401);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step5-2_7-2( Rd bit Rd eye )
    ***************************************************************************/
   for ( i = 0; i < 4; i++)
   {
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B1 + i*0x20);
      RegVal_R5 = ((ReadReg_32(DBSC_E3_DBPDRGD0) & 0x0000FF00) >> 0x8);
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B4 + i*0x20);
      RegVal_R6 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x000000FF);

      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B3 + i*0x20);
      RegVal_R7 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x00000007);
      RegVal_R12 = (RegVal_R5 >> 0x2);
      if ( RegVal_R12 < RegVal_R6 )
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R7 + 0x1) & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 - (RegVal_R12)) & 0x000000FF));
      }
      else
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | (RegVal_R7 & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 + (RegVal_R5) + ((RegVal_R5) >> 1) + (RegVal_R12)) & 0x000000FF));
      }
   }

   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00015001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000003);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0380C700);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000007);
   while ( (BIT30 & ReadReg_32(DBSC_E3_DBPDRGD0)) != 0 );
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000021);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0024643E);

   WriteReg_32(DBSC_E3_DBBUS0CNF1,0x00000010);
   WriteReg_32(DBSC_E3_DBCALCNF, (uint32_t)(64000000/REFRESH_RATE) + 0x01000000);
   WriteReg_32(DBSC_E3_DBRFCNF1, (uint32_t)(REFRESH_RATE*116/125) + 0x00080000);
   WriteReg_32(DBSC_E3_DBRFCNF2,0x00010000);
   WriteReg_32(DBSC_E3_DBDFICUPDCNF,0x40100001);

   WriteReg_32(DBSC_E3_DBRFEN,0x00000001);
   WriteReg_32(DBSC_E3_DBACEN,0x00000001);

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step9( Initial End )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDLK0,0x00000000);
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00000000);

#ifdef ddr_qos_init_setting // only for non qos_init
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00001234);
   WriteReg_32(DBSC_E3_DBCAM0CNF1,0x00043218);
   WriteReg_32(DBSC_E3_DBCAM0CNF2,0x000000F4);
   WriteReg_32(DBSC_E3_DBSCHCNT0,0x000f0037);
   WriteReg_32(DBSC_E3_DBSCHSZ0,0x00000001);
   WriteReg_32(DBSC_E3_DBSCHRW0,0x22421111);
   WriteReg_32(DBSC_E3_SCFCTST2,0x012F1123);
   WriteReg_32(DBSC_E3_DBSCHQOS00,0x00000F00);
   WriteReg_32(DBSC_E3_DBSCHQOS01,0x00000B00);
   WriteReg_32(DBSC_E3_DBSCHQOS02,0x00000000);
   WriteReg_32(DBSC_E3_DBSCHQOS03,0x00000000);
   WriteReg_32(DBSC_E3_DBSCHQOS40,0x00000300);
   WriteReg_32(DBSC_E3_DBSCHQOS41,0x000002F0);
   WriteReg_32(DBSC_E3_DBSCHQOS42,0x00000200);
   WriteReg_32(DBSC_E3_DBSCHQOS43,0x00000100);
//   WriteReg_32(DBSC_E3_DBSCHQOS90,0x00000300);
   WriteReg_32(DBSC_E3_DBSCHQOS90,0x00000100); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS91,0x000002F0);
   WriteReg_32(DBSC_E3_DBSCHQOS91,0x000000F0); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS92,0x00000200);
   WriteReg_32(DBSC_E3_DBSCHQOS92,0x000000A0); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS93,0x00000100);
   WriteReg_32(DBSC_E3_DBSCHQOS93,0x00000040); // rev.0.03
   WriteReg_32(DBSC_E3_DBSCHQOS130,0x00000100);
   WriteReg_32(DBSC_E3_DBSCHQOS131,0x000000F0);
   WriteReg_32(DBSC_E3_DBSCHQOS132,0x000000A0);
   WriteReg_32(DBSC_E3_DBSCHQOS133,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS140,0x000000C0);
   WriteReg_32(DBSC_E3_DBSCHQOS141,0x000000B0);
   WriteReg_32(DBSC_E3_DBSCHQOS142,0x00000080);
   WriteReg_32(DBSC_E3_DBSCHQOS143,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS150,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS151,0x00000030);
   WriteReg_32(DBSC_E3_DBSCHQOS152,0x00000020);
   WriteReg_32(DBSC_E3_DBSCHQOS153,0x00000010);
   WriteReg_32(0xE67F0018,0x00000001);
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00000000);
#endif

//   return 1;
   return 0;   // rev.0.03

}

// rev.0.03 add Comment
/*******************************************************************************
*	Initialize ddr (MD19=0:DDR3L,1584Mbps)
 ******************************************************************************/
uint32_t init_ddr_e31600(void)
{

    uint32_t RegVal_R2, RegVal_R5, RegVal_R6, RegVal_R7, RegVal_R12, i;

/* CPG setting ===============================================*/
WriteReg_32(CPG_CPGWPR,0x5A5AFFFF);
WriteReg_32(CPG_CPGWPCR,0xA5A50000);

WriteReg_32(CPG_SRCR4,0x20000000);

WriteReg_32(0xE61500DC,0xe2200000);  // Change to 1584Mbps
while ( (BIT11 & ReadReg_32(CPG_PLLECR)) == 0 );

WriteReg_32(CPG_SRSTCLR4,0x20000000);

WriteReg_32(CPG_CPGWPCR,0xA5A50001);

/* CPG setting ===============================================*/

   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00001234);
   WriteReg_32(DBSC_E3_DBKIND,0x00000007);
#ifdef ddr_qos_init_setting                        // rev.0.03
//   WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a01);
   WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02);    // rev.0.02
#else                                              // rev.0.03
   #if RCAR_MEM_DENS == 0                          // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #elif RCAR_MEM_DENS == 1                        // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #else                                           // rev.0.03
      WriteReg_32(DBSC_E3_DBMEMCONF00,0x0f030a02); // rev.0.03
   #endif                                          // rev.0.03
#endif                                             // rev.0.03
   WriteReg_32(DBSC_E3_DBPHYCONF0,0x00000001);

   WriteReg_32(DBSC_E3_DBTR0,0x0000000B);
   WriteReg_32(DBSC_E3_DBTR1,0x00000008);
   WriteReg_32(DBSC_E3_DBTR2,0x00000000);
   WriteReg_32(DBSC_E3_DBTR3,0x0000000B);
   WriteReg_32(DBSC_E3_DBTR4,0x000B000B);
   WriteReg_32(DBSC_E3_DBTR5,0x00000027);
   WriteReg_32(DBSC_E3_DBTR6,0x0000001C);
   WriteReg_32(DBSC_E3_DBTR7,0x00060006);
   WriteReg_32(DBSC_E3_DBTR8,0x00000020);
   WriteReg_32(DBSC_E3_DBTR9,0x00000006);
   WriteReg_32(DBSC_E3_DBTR10,0x0000000C);
   WriteReg_32(DBSC_E3_DBTR11,0x0000000A);
   WriteReg_32(DBSC_E3_DBTR12,0x00120012);
   WriteReg_32(DBSC_E3_DBTR13,0x000000CE);
   WriteReg_32(DBSC_E3_DBTR14,0x00140005);
   WriteReg_32(DBSC_E3_DBTR15,0x00050004);
   WriteReg_32(DBSC_E3_DBTR16,0x071F0305);
   WriteReg_32(DBSC_E3_DBTR17,0x040C0000);
   WriteReg_32(DBSC_E3_DBTR18,0x00000200);
   WriteReg_32(DBSC_E3_DBTR19,0x01000040);
   WriteReg_32(DBSC_E3_DBTR20,0x020000D6);
   WriteReg_32(DBSC_E3_DBTR21,0x00040004);
   WriteReg_32(DBSC_E3_DBBL,0x00000000);
   WriteReg_32(DBSC_E3_DBODT0,0x00000001);
   WriteReg_32(DBSC_E3_DBADJ0,0x00000001);
   WriteReg_32(DBSC_E3_DBSYSCONF1,0x00000002);
   WriteReg_32(DBSC_E3_DBDFICNT0,0x00000010);
   WriteReg_32(DBSC_E3_DBBCAMDIS,0x00000001);
   WriteReg_32(DBSC_E3_DBSCHRW1,0x00000046);
   WriteReg_32(DBSC_E3_SCFCTST0,0x0D050B03);
   WriteReg_32(DBSC_E3_SCFCTST1,0x0306030C);

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step0( INITBYP )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDLK0,0x0000A55A);
   WriteReg_32(DBSC_E3_DBCMD,0x01000001);
   WriteReg_32(DBSC_E3_DBCMD,0x08000000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x80010000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step1( ZCAL,PLLINIT,DCAL,PHYRST trainig )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000008);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000B8000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058904);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000091);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BB6B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000095);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BBAD);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000099);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0007BB6B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058900);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000021);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0024641E);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010073);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step2( DRAMRST/DRAMINT training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0C058900);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000090);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x04058900);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000003);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0780C700);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000007);
   while ( (BIT30 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000004);
   WriteReg_32(DBSC_E3_DBPDRGD0,(uint32_t)(REFRESH_RATE*792/125)-400 + 0x08B00000);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000022);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x1000040B);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000023);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x2D9C0B66);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000024);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x2A88B400);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000025);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x30005200);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000026);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0014A9C9);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000027);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00000D70);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000028);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00000046);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000029);
   if (REFRESH_RATE > 3900) {
       WriteReg_32(DBSC_E3_DBPDRGD0,0x00000018);  //           [7]SRT=0
   } else {
       WriteReg_32(DBSC_E3_DBPDRGD0,0x00000098);  //           [7]SRT=1
   }
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000002C);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x81003047);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000020);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00181884);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000001A);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x33C03C10);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E7);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E8);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E9);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000107);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000108);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0D0D0D0D);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000109);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x000D0D0D);


   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010181);
   WriteReg_32(DBSC_E3_DBCMD,0x08000001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step3( WL/QSG training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010601);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step4( WLADJ training )
    ***************************************************************************/
   for ( i = 0; i<4; i++)
   {
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B1 + i*0x20);
      RegVal_R5 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x0000FF00) >> 0x8;
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B4 + i*0x20);
      RegVal_R6 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x000000FF);
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B3 + i*0x20);
      RegVal_R7 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x00000007);
      if ( RegVal_R6 > 0 )
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R7 + 0x1) & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | RegVal_R6);
      } else 
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | RegVal_R7);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 + ((RegVal_R5) << 1)) & 0x000000FF));
      }
   }

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000005);
   WriteReg_32(DBSC_E3_DBPDRGD0,0xC1AA00C0);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010801);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step5678( RdWrbitRdWreye )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000005);
   WriteReg_32(DBSC_E3_DBPDRGD0,0xC1AA00D8);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0001F001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );


   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step3_2( DQS Gate Training )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C000285);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x0000002C);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x81003087);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00010401);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step5-2_7-2( Rd bit Rd eye )
    ***************************************************************************/
   for ( i = 0; i < 4; i++)
   {
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B1 + i*0x20);
      RegVal_R5 = ((ReadReg_32(DBSC_E3_DBPDRGD0) & 0x0000FF00) >> 0x8);
      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B4 + i*0x20);
      RegVal_R6 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x000000FF);

      WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B3 + i*0x20);
      RegVal_R7 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0x00000007);
      RegVal_R12 = (RegVal_R5 >> 0x2);
      if ( RegVal_R12 < RegVal_R6 )
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R7 + 0x1) & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);

         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 - (RegVal_R12)) & 0x000000FF));
      }
      else
      {
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFFF8);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B2 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | (RegVal_R7 & 0x00000007));
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         RegVal_R2 = (ReadReg_32(DBSC_E3_DBPDRGD0) & 0xFFFFFF00);
         WriteReg_32(DBSC_E3_DBPDRGA0,0x000000B0 + i*0x20);
         WriteReg_32(DBSC_E3_DBPDRGD0,RegVal_R2 | ((RegVal_R6 + (RegVal_R5) + ((RegVal_R5) >> 1) + (RegVal_R12)) & 0x000000FF));
      }
   }

   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000A0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000C0);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x000000E0); // rev.0.03
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5); // rev.0.03
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000100); // rev.0.03
   WriteReg_32(DBSC_E3_DBPDRGD0,0x7C0002C5); // rev.0.03
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000001);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x00015001);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000006);
   while ( (BIT0 & ReadReg_32(DBSC_E3_DBPDRGD0)) == 0 );

   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000003);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0380C700);
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000007);
   while ( (BIT30 & ReadReg_32(DBSC_E3_DBPDRGD0)) != 0 );
   WriteReg_32(DBSC_E3_DBPDRGA0,0x00000021);
   WriteReg_32(DBSC_E3_DBPDRGD0,0x0024643E);

   WriteReg_32(DBSC_E3_DBBUS0CNF1,0x00000010);
   WriteReg_32(DBSC_E3_DBCALCNF, (uint32_t)(64000000/REFRESH_RATE) + 0x01000000);
   WriteReg_32(DBSC_E3_DBRFCNF1, (uint32_t)(REFRESH_RATE*99/125) + 0x00080000);
   WriteReg_32(DBSC_E3_DBRFCNF2,0x00010000);
   WriteReg_32(DBSC_E3_DBDFICUPDCNF,0x40100001);
   WriteReg_32(DBSC_E3_DBRFEN,0x00000001);
   WriteReg_32(DBSC_E3_DBACEN,0x00000001);

   // rev.0.03 add Comment
   /****************************************************************************
    *	Initial_Step9( Initial End )
    ***************************************************************************/
   WriteReg_32(DBSC_E3_DBPDLK0,0x00000000);
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00000000);

#ifdef ddr_qos_init_setting // only for non qos_init
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00001234);
   WriteReg_32(DBSC_E3_DBCAM0CNF1,0x00043218);
   WriteReg_32(DBSC_E3_DBCAM0CNF2,0x000000F4);
   WriteReg_32(DBSC_E3_DBSCHCNT0,0x000f0037);
   WriteReg_32(DBSC_E3_DBSCHSZ0,0x00000001);
   WriteReg_32(DBSC_E3_DBSCHRW0,0x22421111);
   WriteReg_32(DBSC_E3_SCFCTST2,0x012F1123);
   WriteReg_32(DBSC_E3_DBSCHQOS00,0x00000F00);
   WriteReg_32(DBSC_E3_DBSCHQOS01,0x00000B00);
   WriteReg_32(DBSC_E3_DBSCHQOS02,0x00000000);
   WriteReg_32(DBSC_E3_DBSCHQOS03,0x00000000);
   WriteReg_32(DBSC_E3_DBSCHQOS40,0x00000300);
   WriteReg_32(DBSC_E3_DBSCHQOS41,0x000002F0);
   WriteReg_32(DBSC_E3_DBSCHQOS42,0x00000200);
   WriteReg_32(DBSC_E3_DBSCHQOS43,0x00000100);
//   WriteReg_32(DBSC_E3_DBSCHQOS90,0x00000300);
   WriteReg_32(DBSC_E3_DBSCHQOS90,0x00000100); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS91,0x000002F0);
   WriteReg_32(DBSC_E3_DBSCHQOS91,0x000000F0); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS92,0x00000200);
   WriteReg_32(DBSC_E3_DBSCHQOS92,0x000000A0); // rev.0.03
//   WriteReg_32(DBSC_E3_DBSCHQOS93,0x00000100);
   WriteReg_32(DBSC_E3_DBSCHQOS93,0x00000040); // rev.0.03
   WriteReg_32(DBSC_E3_DBSCHQOS130,0x00000100);
   WriteReg_32(DBSC_E3_DBSCHQOS131,0x000000F0);
   WriteReg_32(DBSC_E3_DBSCHQOS132,0x000000A0);
   WriteReg_32(DBSC_E3_DBSCHQOS133,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS140,0x000000C0);
   WriteReg_32(DBSC_E3_DBSCHQOS141,0x000000B0);
   WriteReg_32(DBSC_E3_DBSCHQOS142,0x00000080);
   WriteReg_32(DBSC_E3_DBSCHQOS143,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS150,0x00000040);
   WriteReg_32(DBSC_E3_DBSCHQOS151,0x00000030);
   WriteReg_32(DBSC_E3_DBSCHQOS152,0x00000020);
   WriteReg_32(DBSC_E3_DBSCHQOS153,0x00000010);
   WriteReg_32(0xE67F0018,0x00000001);
   WriteReg_32(DBSC_E3_DBSYSCNT0,0x00000000);
#endif

//   return 1;
   return 0;   // rev.0.03
}


/*******************************************************************************
 *	DDR Initialize entry
 ******************************************************************************/
int32_t InitDram(void)
{
 	uint32_t dataL;
 	uint32_t failcount;

	dataL=init_ddr_e31866();

	if(dataL==0) {
	failcount =0;
	}
	else {
		failcount =1;
	}

	if(failcount==0) {
		return INITDRAM_OK;
	} else {
		return INITDRAM_NG;
	}
}


/*******************************************************************************
 *	END
 ******************************************************************************/
