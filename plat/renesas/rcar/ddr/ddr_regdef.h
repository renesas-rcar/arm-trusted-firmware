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

#define _reg_PHY_DQ_DM_SWIZZLE0                            0x00000000U
#define _reg_PHY_DQ_DM_SWIZZLE1                            0x00000001U
#define _reg_PHY_CLK_WR_BYPASS_SLAVE_DELAY                 0x00000002U
#define _reg_PHY_CLK_BYPASS_OVERRIDE                       0x00000003U
#define _reg_PHY_SW_WRDQ0_SHIFT                            0x00000004U
#define _reg_PHY_SW_WRDQ1_SHIFT                            0x00000005U
#define _reg_PHY_SW_WRDQ2_SHIFT                            0x00000006U
#define _reg_PHY_SW_WRDQ3_SHIFT                            0x00000007U
#define _reg_PHY_SW_WRDQ4_SHIFT                            0x00000008U
#define _reg_PHY_SW_WRDQ5_SHIFT                            0x00000009U
#define _reg_PHY_SW_WRDQ6_SHIFT                            0x0000000aU
#define _reg_PHY_SW_WRDQ7_SHIFT                            0x0000000bU
#define _reg_PHY_SW_WRDM_SHIFT                             0x0000000cU
#define _reg_PHY_SW_WRDQS_SHIFT                            0x0000000dU
#define _reg_PHY_DQ_TSEL_ENABLE                            0x0000000eU
#define _reg_PHY_DQ_TSEL_SELECT                            0x0000000fU
#define _reg_PHY_DQS_TSEL_ENABLE                           0x00000010U
#define _reg_PHY_DQS_TSEL_SELECT                           0x00000011U
#define _reg_PHY_TWO_CYC_PREAMBLE                          0x00000012U
#define _reg_PHY_DBI_MODE                                  0x00000013U
#define _reg_PHY_PER_RANK_CS_MAP                           0x00000014U
#define _reg_PHY_PER_CS_TRAINING_MULTICAST_EN              0x00000015U
#define _reg_PHY_PER_CS_TRAINING_INDEX                     0x00000016U
#define _reg_PHY_LP4_BOOT_RDDATA_EN_IE_DLY                 0x00000017U
#define _reg_PHY_LP4_BOOT_RDDATA_EN_DLY                    0x00000018U
#define _reg_PHY_LP4_BOOT_RDDATA_EN_TSEL_DLY               0x00000019U
#define _reg_PHY_LP4_BOOT_RPTR_UPDATE                      0x0000001aU
#define _reg_PHY_LP4_BOOT_RDDQS_GATE_SLAVE_DELAY           0x0000001bU
#define _reg_PHY_LP4_BOOT_RDDQS_LATENCY_ADJUST             0x0000001cU
#define _reg_PHY_LPBK_CONTROL                              0x0000001dU
#define _reg_PHY_RDDQS_DQ_BYPASS_SLAVE_DELAY               0x0000001eU
#define _reg_PHY_GATE_ERROR_DELAY_SELECT                   0x0000001fU
#define _reg_SC_PHY_SNAP_OBS_REGS                          0x00000020U
#define _reg_PHY_LPDDR                                     0x00000021U
#define _reg_PHY_LPDDR_TYPE                                0x00000022U
#define _reg_PHY_GATE_SMPL1_SLAVE_DELAY                    0x00000023U
#define _reg_PHY_GATE_SMPL2_SLAVE_DELAY                    0x00000024U
#define _reg_ON_FLY_GATE_ADJUST_EN                         0x00000025U
#define _reg_PHY_GATE_TRACKING_OBS                         0x00000026U
#define _reg_PHY_LP4_PST_AMBLE                             0x00000027U
#define _reg_PHY_LP4_RDLVL_PATT8                           0x00000028U
#define _reg_PHY_LP4_RDLVL_PATT9                           0x00000029U
#define _reg_PHY_LP4_RDLVL_PATT10                          0x0000002aU
#define _reg_PHY_LP4_RDLVL_PATT11                          0x0000002bU
#define _reg_PHY_SLAVE_LOOP_CNT_UPDATE                     0x0000002cU
#define _reg_PHY_SW_FIFO_PTR_RST_DISABLE                   0x0000002dU
#define _reg_PHY_MASTER_DLY_LOCK_OBS_SELECT                0x0000002eU
#define _reg_PHY_RDDQ_ENC_OBS_SELECT                       0x0000002fU
#define _reg_PHY_RDDQS_DQ_ENC_OBS_SELECT                   0x00000030U
#define _reg_PHY_WR_ENC_OBS_SELECT                         0x00000031U
#define _reg_PHY_WR_SHIFT_OBS_SELECT                       0x00000032U
#define _reg_PHY_FIFO_PTR_OBS_SELECT                       0x00000033U
#define _reg_PHY_LVL_DEBUG_MODE                            0x00000034U
#define _reg_SC_PHY_LVL_DEBUG_CONT                         0x00000035U
#define _reg_PHY_WRLVL_CAPTURE_CNT                         0x00000036U
#define _reg_PHY_WRLVL_UPDT_WAIT_CNT                       0x00000037U
#define _reg_PHY_GTLVL_CAPTURE_CNT                         0x00000038U
#define _reg_PHY_GTLVL_UPDT_WAIT_CNT                       0x00000039U
#define _reg_PHY_RDLVL_CAPTURE_CNT                         0x0000003aU
#define _reg_PHY_RDLVL_UPDT_WAIT_CNT                       0x0000003bU
#define _reg_PHY_RDLVL_OP_MODE                             0x0000003cU
#define _reg_PHY_RDLVL_RDDQS_DQ_OBS_SELECT                 0x0000003dU
#define _reg_PHY_RDLVL_DATA_MASK                           0x0000003eU
#define _reg_PHY_RDLVL_DATA_SWIZZLE                        0x0000003fU
#define _reg_PHY_WDQLVL_BURST_CNT                          0x00000040U
#define _reg_PHY_WDQLVL_PATT                               0x00000041U
#define _reg_PHY_WDQLVL_DQDM_SLV_DLY_JUMP_OFFSET           0x00000042U
#define _reg_PHY_WDQLVL_UPDT_WAIT_CNT                      0x00000043U
#define _reg_PHY_WDQLVL_DQDM_OBS_SELECT                    0x00000044U
#define _reg_PHY_WDQLVL_QTR_DLY_STEP                       0x00000045U
#define _reg_PHY_WDQLVL_CLR_PREV_RESULTS                   0x00000046U
#define _reg_PHY_WDQLVL_DATADM_MASK                        0x00000047U
#define _reg_PHY_USER_PATT0                                0x00000048U
#define _reg_PHY_USER_PATT1                                0x00000049U
#define _reg_PHY_USER_PATT2                                0x0000004aU
#define _reg_PHY_USER_PATT3                                0x0000004bU
#define _reg_PHY_USER_PATT4                                0x0000004cU
#define _reg_PHY_DQ_SWIZZLING                              0x0000004dU
#define _reg_PHY_CALVL_VREF_DRIVING_SLICE                  0x0000004eU
#define _reg_SC_PHY_MANUAL_CLEAR                           0x0000004fU
#define _reg_PHY_FIFO_PTR_OBS                              0x00000050U
#define _reg_PHY_LPBK_RESULT_OBS                           0x00000051U
#define _reg_PHY_MASTER_DLY_LOCK_OBS                       0x00000052U
#define _reg_PHY_RDDQ_SLV_DLY_ENC_OBS                      0x00000053U
#define _reg_PHY_RDDQS_BASE_SLV_DLY_ENC_OBS                0x00000054U
#define _reg_PHY_RDDQS_DQ_RISE_ADDER_SLV_DLY_ENC_OBS       0x00000055U
#define _reg_PHY_RDDQS_DQ_FALL_ADDER_SLV_DLY_ENC_OBS       0x00000056U
#define _reg_PHY_RDDQS_GATE_SLV_DLY_ENC_OBS                0x00000057U
#define _reg_PHY_WRDQS_BASE_SLV_DLY_ENC_OBS                0x00000058U
#define _reg_PHY_WRDQ_BASE_SLV_DLY_ENC_OBS                 0x00000059U
#define _reg_PHY_WR_ADDER_SLV_DLY_ENC_OBS                  0x0000005aU
#define _reg_PHY_WR_SHIFT_OBS                              0x0000005bU
#define _reg_PHY_WRLVL_HARD0_DELAY_OBS                     0x0000005cU
#define _reg_PHY_WRLVL_HARD1_DELAY_OBS                     0x0000005dU
#define _reg_PHY_WRLVL_STATUS_OBS                          0x0000005eU
#define _reg_PHY_GATE_SMPL1_SLV_DLY_ENC_OBS                0x0000005fU
#define _reg_PHY_GATE_SMPL2_SLV_DLY_ENC_OBS                0x00000060U
#define _reg_PHY_WRLVL_ERROR_OBS                           0x00000061U
#define _reg_PHY_GTLVL_HARD0_DELAY_OBS                     0x00000062U
#define _reg_PHY_GTLVL_HARD1_DELAY_OBS                     0x00000063U
#define _reg_PHY_GTLVL_STATUS_OBS                          0x00000064U
#define _reg_PHY_RDLVL_RDDQS_DQ_LE_DLY_OBS                 0x00000065U
#define _reg_PHY_RDLVL_RDDQS_DQ_TE_DLY_OBS                 0x00000066U
#define _reg_PHY_RDLVL_RDDQS_DQ_NUM_WINDOWS_OBS            0x00000067U
#define _reg_PHY_RDLVL_STATUS_OBS                          0x00000068U
#define _reg_PHY_WDQLVL_DQDM_LE_DLY_OBS                    0x00000069U
#define _reg_PHY_WDQLVL_DQDM_TE_DLY_OBS                    0x0000006aU
#define _reg_PHY_WDQLVL_STATUS_OBS                         0x0000006bU
#define _reg_PHY_DDL_MODE                                  0x0000006cU
#define _reg_PHY_DDL_TEST_OBS                              0x0000006dU
#define _reg_PHY_DDL_TEST_MSTR_DLY_OBS                     0x0000006eU
#define _reg_SC_PHY_RX_CAL_START                           0x0000006fU
#define _reg_PHY_RX_CAL_OVERRIDE                           0x00000070U
#define _reg_PHY_RX_CAL_SAMPLE_WAIT                        0x00000071U
#define _reg_PHY_RX_CAL_DQ0                                0x00000072U
#define _reg_PHY_RX_CAL_DQ1                                0x00000073U
#define _reg_PHY_RX_CAL_DQ2                                0x00000074U
#define _reg_PHY_RX_CAL_DQ3                                0x00000075U
#define _reg_PHY_RX_CAL_DQ4                                0x00000076U
#define _reg_PHY_RX_CAL_DQ5                                0x00000077U
#define _reg_PHY_RX_CAL_DQ6                                0x00000078U
#define _reg_PHY_RX_CAL_DQ7                                0x00000079U
#define _reg_PHY_RX_CAL_DM                                 0x0000007aU
#define _reg_PHY_RX_CAL_DQS                                0x0000007bU
#define _reg_PHY_RX_CAL_FDBK                               0x0000007cU
#define _reg_PHY_RX_CAL_OBS                                0x0000007dU
#define _reg_PHY_CLK_WRDQ0_SLAVE_DELAY                     0x0000007eU
#define _reg_PHY_CLK_WRDQ1_SLAVE_DELAY                     0x0000007fU
#define _reg_PHY_CLK_WRDQ2_SLAVE_DELAY                     0x00000080U
#define _reg_PHY_CLK_WRDQ3_SLAVE_DELAY                     0x00000081U
#define _reg_PHY_CLK_WRDQ4_SLAVE_DELAY                     0x00000082U
#define _reg_PHY_CLK_WRDQ5_SLAVE_DELAY                     0x00000083U
#define _reg_PHY_CLK_WRDQ6_SLAVE_DELAY                     0x00000084U
#define _reg_PHY_CLK_WRDQ7_SLAVE_DELAY                     0x00000085U
#define _reg_PHY_CLK_WRDM_SLAVE_DELAY                      0x00000086U
#define _reg_PHY_CLK_WRDQS_SLAVE_DELAY                     0x00000087U
#define _reg_PHY_RDDQ0_SLAVE_DELAY                         0x00000088U
#define _reg_PHY_RDDQ1_SLAVE_DELAY                         0x00000089U
#define _reg_PHY_RDDQ2_SLAVE_DELAY                         0x0000008aU
#define _reg_PHY_RDDQ3_SLAVE_DELAY                         0x0000008bU
#define _reg_PHY_RDDQ4_SLAVE_DELAY                         0x0000008cU
#define _reg_PHY_RDDQ5_SLAVE_DELAY                         0x0000008dU
#define _reg_PHY_RDDQ6_SLAVE_DELAY                         0x0000008eU
#define _reg_PHY_RDDQ7_SLAVE_DELAY                         0x0000008fU
#define _reg_PHY_RDDM_SLAVE_DELAY                          0x00000090U
#define _reg_PHY_RDDQS_DQ0_RISE_SLAVE_DELAY                0x00000091U
#define _reg_PHY_RDDQS_DQ0_FALL_SLAVE_DELAY                0x00000092U
#define _reg_PHY_RDDQS_DQ1_RISE_SLAVE_DELAY                0x00000093U
#define _reg_PHY_RDDQS_DQ1_FALL_SLAVE_DELAY                0x00000094U
#define _reg_PHY_RDDQS_DQ2_RISE_SLAVE_DELAY                0x00000095U
#define _reg_PHY_RDDQS_DQ2_FALL_SLAVE_DELAY                0x00000096U
#define _reg_PHY_RDDQS_DQ3_RISE_SLAVE_DELAY                0x00000097U
#define _reg_PHY_RDDQS_DQ3_FALL_SLAVE_DELAY                0x00000098U
#define _reg_PHY_RDDQS_DQ4_RISE_SLAVE_DELAY                0x00000099U
#define _reg_PHY_RDDQS_DQ4_FALL_SLAVE_DELAY                0x0000009aU
#define _reg_PHY_RDDQS_DQ5_RISE_SLAVE_DELAY                0x0000009bU
#define _reg_PHY_RDDQS_DQ5_FALL_SLAVE_DELAY                0x0000009cU
#define _reg_PHY_RDDQS_DQ6_RISE_SLAVE_DELAY                0x0000009dU
#define _reg_PHY_RDDQS_DQ6_FALL_SLAVE_DELAY                0x0000009eU
#define _reg_PHY_RDDQS_DQ7_RISE_SLAVE_DELAY                0x0000009fU
#define _reg_PHY_RDDQS_DQ7_FALL_SLAVE_DELAY                0x000000a0U
#define _reg_PHY_RDDQS_DM_RISE_SLAVE_DELAY                 0x000000a1U
#define _reg_PHY_RDDQS_DM_FALL_SLAVE_DELAY                 0x000000a2U
#define _reg_PHY_RDDQS_GATE_SLAVE_DELAY                    0x000000a3U
#define _reg_PHY_RDDQS_LATENCY_ADJUST                      0x000000a4U
#define _reg_PHY_WRITE_PATH_LAT_ADD                        0x000000a5U
#define _reg_PHY_WRLVL_DELAY_EARLY_THRESHOLD               0x000000a6U
#define _reg_PHY_WRLVL_DELAY_PERIOD_THRESHOLD              0x000000a7U
#define _reg_PHY_WRLVL_EARLY_FORCE_ZERO                    0x000000a8U
#define _reg_PHY_GTLVL_RDDQS_SLV_DLY_START                 0x000000a9U
#define _reg_PHY_GTLVL_LAT_ADJ_START                       0x000000aaU
#define _reg_PHY_WDQLVL_DQDM_SLV_DLY_START                 0x000000abU
#define _reg_PHY_RDLVL_RDDQS_DQ_SLV_DLY_START              0x000000acU
#define _reg_PHY_DQ_OE_TIMING                              0x000000adU
#define _reg_PHY_DQ_TSEL_RD_TIMING                         0x000000aeU
#define _reg_PHY_DQ_TSEL_WR_TIMING                         0x000000afU
#define _reg_PHY_DQS_OE_TIMING                             0x000000b0U
#define _reg_PHY_DQS_TSEL_RD_TIMING                        0x000000b1U
#define _reg_PHY_DQS_TSEL_WR_TIMING                        0x000000b2U
#define _reg_PHY_PER_CS_TRAINING_EN                        0x000000b3U
#define _reg_PHY_DQ_IE_TIMING                              0x000000b4U
#define _reg_PHY_DQS_IE_TIMING                             0x000000b5U
#define _reg_PHY_RDDATA_EN_IE_DLY                          0x000000b6U
#define _reg_PHY_IE_MODE                                   0x000000b7U
#define _reg_PHY_RDDATA_EN_DLY                             0x000000b8U
#define _reg_PHY_RDDATA_EN_TSEL_DLY                        0x000000b9U
#define _reg_PHY_SW_MASTER_MODE                            0x000000baU
#define _reg_PHY_MASTER_DELAY_START                        0x000000bbU
#define _reg_PHY_MASTER_DELAY_STEP                         0x000000bcU
#define _reg_PHY_MASTER_DELAY_WAIT                         0x000000bdU
#define _reg_PHY_RPTR_UPDATE                               0x000000beU
#define _reg_PHY_WRLVL_DLY_STEP                            0x000000bfU
#define _reg_PHY_WRLVL_RESP_WAIT_CNT                       0x000000c0U
#define _reg_PHY_GTLVL_DLY_STEP                            0x000000c1U
#define _reg_PHY_GTLVL_RESP_WAIT_CNT                       0x000000c2U
#define _reg_PHY_GTLVL_BACK_STEP                           0x000000c3U
#define _reg_PHY_GTLVL_FINAL_STEP                          0x000000c4U
#define _reg_PHY_WDQLVL_DLY_STEP                           0x000000c5U
#define _reg_PHY_RDLVL_DLY_STEP                            0x000000c6U
#define _reg_PHY_ADR0_SW_WRADDR_SHIFT                      0x000000c7U
#define _reg_PHY_ADR1_SW_WRADDR_SHIFT                      0x000000c8U
#define _reg_PHY_ADR2_SW_WRADDR_SHIFT                      0x000000c9U
#define _reg_PHY_ADR3_SW_WRADDR_SHIFT                      0x000000caU
#define _reg_PHY_ADR4_SW_WRADDR_SHIFT                      0x000000cbU
#define _reg_PHY_ADR5_SW_WRADDR_SHIFT                      0x000000ccU
#define _reg_PHY_ADR_CLK_WR_BYPASS_SLAVE_DELAY             0x000000cdU
#define _reg_PHY_ADR_CLK_BYPASS_OVERRIDE                   0x000000ceU
#define _reg_SC_PHY_ADR_MANUAL_CLEAR                       0x000000cfU
#define _reg_PHY_ADR_LPBK_RESULT_OBS                       0x000000d0U
#define _reg_PHY_ADR_MASTER_DLY_LOCK_OBS_SELECT            0x000000d1U
#define _reg_PHY_ADR_MASTER_DLY_LOCK_OBS                   0x000000d2U
#define _reg_PHY_ADR_SLAVE_LOOP_CNT_UPDATE                 0x000000d3U
#define _reg_PHY_ADR_SLV_DLY_ENC_OBS_SELECT                0x000000d4U
#define _reg_SC_PHY_ADR_SNAP_OBS_REGS                      0x000000d5U
#define _reg_PHY_ADR_TSEL_ENABLE                           0x000000d6U
#define _reg_PHY_ADR_LPBK_CONTROL                          0x000000d7U
#define _reg_PHY_ADR_TYPE                                  0x000000d8U
#define _reg_PHY_ADR_WRADDR_SHIFT_OBS                      0x000000d9U
#define _reg_PHY_ADR_IE_MODE                               0x000000daU
#define _reg_PHY_ADR_DDL_MODE                              0x000000dbU
#define _reg_PHY_ADR_DDL_TEST_OBS                          0x000000dcU
#define _reg_PHY_ADR_DDL_TEST_MSTR_DLY_OBS                 0x000000ddU
#define _reg_PHY_ADR_CALVL_START                           0x000000deU
#define _reg_PHY_ADR_CALVL_COARSE_DLY                      0x000000dfU
#define _reg_PHY_ADR_CALVL_QTR                             0x000000e0U
#define _reg_PHY_ADR_CALVL_SWIZZLE0_0                      0x000000e1U
#define _reg_PHY_ADR_CALVL_SWIZZLE1_0                      0x000000e2U
#define _reg_PHY_ADR_CALVL_SWIZZLE0_1                      0x000000e3U
#define _reg_PHY_ADR_CALVL_SWIZZLE1_1                      0x000000e4U
#define _reg_PHY_ADR_CALVL_DEVICE_MAP                      0x000000e5U
#define _reg_PHY_ADR_CALVL_RANK_CTRL                       0x000000e6U
#define _reg_PHY_ADR_CALVL_NUM_PATTERNS                    0x000000e7U
#define _reg_PHY_ADR_CALVL_CAPTURE_CNT                     0x000000e8U
#define _reg_PHY_ADR_CALVL_RESP_WAIT_CNT                   0x000000e9U
#define _reg_PHY_ADR_CALVL_DEBUG_MODE                      0x000000eaU
#define _reg_SC_PHY_ADR_CALVL_DEBUG_CONT                   0x000000ebU
#define _reg_SC_PHY_ADR_CALVL_ERROR_CLR                    0x000000ecU
#define _reg_PHY_ADR_CALVL_OBS_SELECT                      0x000000edU
#define _reg_PHY_ADR_CALVL_OBS0                            0x000000eeU
#define _reg_PHY_ADR_CALVL_OBS1                            0x000000efU
#define _reg_PHY_ADR_CALVL_RESULT                          0x000000f0U
#define _reg_PHY_ADR_CALVL_FG_0                            0x000000f1U
#define _reg_PHY_ADR_CALVL_BG_0                            0x000000f2U
#define _reg_PHY_ADR_CALVL_FG_1                            0x000000f3U
#define _reg_PHY_ADR_CALVL_BG_1                            0x000000f4U
#define _reg_PHY_ADR_CALVL_FG_2                            0x000000f5U
#define _reg_PHY_ADR_CALVL_BG_2                            0x000000f6U
#define _reg_PHY_ADR_CALVL_FG_3                            0x000000f7U
#define _reg_PHY_ADR_CALVL_BG_3                            0x000000f8U
#define _reg_PHY_ADR_ADDR_SEL                              0x000000f9U
#define _reg_PHY_ADR_LP4_BOOT_SLV_DELAY                    0x000000faU
#define _reg_PHY_ADR_BIT_MASK                              0x000000fbU
#define _reg_PHY_ADR_SEG_MASK                              0x000000fcU
#define _reg_PHY_ADR_CALVL_TRAIN_MASK                      0x000000fdU
#define _reg_PHY_ADR_SW_TXIO_CTRL                          0x000000feU
#define _reg_PHY_ADR_TSEL_SELECT                           0x000000ffU
#define _reg_PHY_ADR0_CLK_WR_SLAVE_DELAY                   0x00000100U
#define _reg_PHY_ADR1_CLK_WR_SLAVE_DELAY                   0x00000101U
#define _reg_PHY_ADR2_CLK_WR_SLAVE_DELAY                   0x00000102U
#define _reg_PHY_ADR3_CLK_WR_SLAVE_DELAY                   0x00000103U
#define _reg_PHY_ADR4_CLK_WR_SLAVE_DELAY                   0x00000104U
#define _reg_PHY_ADR5_CLK_WR_SLAVE_DELAY                   0x00000105U
#define _reg_PHY_ADR_SW_MASTER_MODE                        0x00000106U
#define _reg_PHY_ADR_MASTER_DELAY_START                    0x00000107U
#define _reg_PHY_ADR_MASTER_DELAY_STEP                     0x00000108U
#define _reg_PHY_ADR_MASTER_DELAY_WAIT                     0x00000109U
#define _reg_PHY_ADR_CALVL_DLY_STEP                        0x0000010aU
#define _reg_PHY_FREQ_SEL_MULTICAST_EN                     0x0000010bU
#define _reg_PHY_FREQ_SEL_INDEX                            0x0000010cU
#define _reg_PHY_SW_GRP_SHIFT_0                            0x0000010dU
#define _reg_PHY_SW_GRP_SHIFT_1                            0x0000010eU
#define _reg_PHY_SW_GRP_SHIFT_2                            0x0000010fU
#define _reg_PHY_SW_GRP_SHIFT_3                            0x00000110U
#define _reg_PHY_GRP_BYPASS_SLAVE_DELAY                    0x00000111U
#define _reg_PHY_SW_GRP_BYPASS_SHIFT                       0x00000112U
#define _reg_PHY_GRP_BYPASS_OVERRIDE                       0x00000113U
#define _reg_SC_PHY_MANUAL_UPDATE                          0x00000114U
#define _reg_PHY_LP4_BOOT_DISABLE                          0x00000115U
#define _reg_PHY_CSLVL_ENABLE                              0x00000116U
#define _reg_PHY_CSLVL_CS_MAP                              0x00000117U
#define _reg_PHY_CSLVL_START                               0x00000118U
#define _reg_PHY_CSLVL_QTR                                 0x00000119U
#define _reg_PHY_CSLVL_CAPTURE_CNT                         0x0000011aU
#define _reg_PHY_CSLVL_COARSE_DLY                          0x0000011bU
#define _reg_PHY_CSLVL_COARSE_CAPTURE_CNT                  0x0000011cU
#define _reg_PHY_CSLVL_DEBUG_MODE                          0x0000011dU
#define _reg_SC_PHY_CSLVL_DEBUG_CONT                       0x0000011eU
#define _reg_SC_PHY_CSLVL_ERROR_CLR                        0x0000011fU
#define _reg_PHY_CSLVL_OBS0                                0x00000120U
#define _reg_PHY_CSLVL_OBS1                                0x00000121U
#define _reg_PHY_CALVL_CS_MAP                              0x00000122U
#define _reg_PHY_GRP_SLV_DLY_ENC_OBS_SELECT                0x00000123U
#define _reg_PHY_GRP_SHIFT_OBS_SELECT                      0x00000124U
#define _reg_PHY_GRP_SLV_DLY_ENC_OBS                       0x00000125U
#define _reg_PHY_GRP_SHIFT_OBS                             0x00000126U
#define _reg_PHY_ADRCTL_SLAVE_LOOP_CNT_UPDATE              0x00000127U
#define _reg_PHY_ADRCTL_SNAP_OBS_REGS                      0x00000128U
#define _reg_PHY_DFI_PHYUPD_TYPE                           0x00000129U
#define _reg_PHY_ADRCTL_LPDDR                              0x0000012aU
#define _reg_PHY_LP4_ACTIVE                                0x0000012bU
#define _reg_PHY_LPDDR3_CS                                 0x0000012cU
#define _reg_PHY_CALVL_RESULT_MASK                         0x0000012dU
#define _reg_PHY_SW_TXIO_CTRL_0                            0x0000012eU
#define _reg_PHY_SW_TXIO_CTRL_1                            0x0000012fU
#define _reg_PHY_SW_TXIO_CTRL_2                            0x00000130U
#define _reg_PHY_SW_TXIO_CTRL_3                            0x00000131U
#define _reg_PHY_UPDATE_CLK_CAL_VALUES                     0x00000132U
#define _reg_PHY_CONTINUOUS_CLK_CAL_UPDATE                 0x00000133U
#define _reg_PHY_PLL_CTRL                                  0x00000134U
#define _reg_PHY_PLL_CTRL_TOP                              0x00000135U
#define _reg_PHY_PLL_CTRL_CA                               0x00000136U
#define _reg_PHY_PLL_BYPASS                                0x00000137U
#define _reg_PHY_LOW_FREQ_SEL                              0x00000138U
#define _reg_PHY_PAD_VREF_CTRL_DQ_0                        0x00000139U
#define _reg_PHY_PAD_VREF_CTRL_DQ_1                        0x0000013aU
#define _reg_PHY_PAD_VREF_CTRL_DQ_2                        0x0000013bU
#define _reg_PHY_PAD_VREF_CTRL_DQ_3                        0x0000013cU
#define _reg_PHY_PAD_VREF_CTRL_AC                          0x0000013dU
#define _reg_PHY_CSLVL_DLY_STEP                            0x0000013eU
#define _reg_PHY_SET_DFI_INPUT_0                           0x0000013fU
#define _reg_PHY_SET_DFI_INPUT_1                           0x00000140U
#define _reg_PHY_SET_DFI_INPUT_2                           0x00000141U
#define _reg_PHY_SET_DFI_INPUT_3                           0x00000142U
#define _reg_PHY_GRP_SLAVE_DELAY_0                         0x00000143U
#define _reg_PHY_GRP_SLAVE_DELAY_1                         0x00000144U
#define _reg_PHY_GRP_SLAVE_DELAY_2                         0x00000145U
#define _reg_PHY_GRP_SLAVE_DELAY_3                         0x00000146U
#define _reg_PHY_LP4_BOOT_PLL_CTRL                         0x00000147U
#define _reg_PHY_LP4_BOOT_PLL_CTRL_CA                      0x00000148U
#define _reg_PHY_LP4_BOOT_TOP_PLL_CTRL                     0x00000149U
#define _reg_PHY_PLL_CTRL_OVERRIDE                         0x0000014aU
#define _reg_PHY_PLL_WAIT                                  0x0000014bU
#define _reg_PHY_PLL_WAIT_TOP                              0x0000014cU
#define _reg_PHY_PLL_OBS_0                                 0x0000014dU
#define _reg_PHY_PLL_OBS_1                                 0x0000014eU
#define _reg_PHY_PLL_OBS_2                                 0x0000014fU
#define _reg_PHY_PLL_OBS_3                                 0x00000150U
#define _reg_PHY_PLL_OBS_4                                 0x00000151U
#define _reg_PHY_PLL_TESTOUT_SEL                           0x00000152U
#define _reg_PHY_TCKSRE_WAIT                               0x00000153U
#define _reg_PHY_LP4_BOOT_LOW_FREQ_SEL                     0x00000154U
#define _reg_PHY_LP_WAKEUP                                 0x00000155U
#define _reg_PHY_TDFI_PHY_WRDELAY                          0x00000156U
#define _reg_PHY_PAD_FDBK_DRIVE                            0x00000157U
#define _reg_PHY_PAD_DATA_DRIVE                            0x00000158U
#define _reg_PHY_PAD_DQS_DRIVE                             0x00000159U
#define _reg_PHY_PAD_ADDR_DRIVE                            0x0000015aU
#define _reg_PHY_PAD_CLK_DRIVE                             0x0000015bU
#define _reg_PHY_PAD_FDBK_TERM                             0x0000015cU
#define _reg_PHY_PAD_DATA_TERM                             0x0000015dU
#define _reg_PHY_PAD_DQS_TERM                              0x0000015eU
#define _reg_PHY_PAD_ADDR_TERM                             0x0000015fU
#define _reg_PHY_PAD_CLK_TERM                              0x00000160U
#define _reg_PHY_PAD_CKE_DRIVE                             0x00000161U
#define _reg_PHY_PAD_CKE_TERM                              0x00000162U
#define _reg_PHY_PAD_RST_DRIVE                             0x00000163U
#define _reg_PHY_PAD_RST_TERM                              0x00000164U
#define _reg_PHY_PAD_CS_DRIVE                              0x00000165U
#define _reg_PHY_PAD_CS_TERM                               0x00000166U
#define _reg_PHY_ADRCTL_RX_CAL                             0x00000167U
#define _reg_PHY_ADRCTL_LP3_RX_CAL                         0x00000168U
#define _reg_PHY_TST_CLK_PAD_CTRL                          0x00000169U
#define _reg_PHY_TST_CLK_PAD_CTRL2                         0x0000016aU
#define _reg_PHY_CAL_MODE_0                                0x0000016bU
#define _reg_PHY_CAL_CLEAR_0                               0x0000016cU
#define _reg_PHY_CAL_START_0                               0x0000016dU
#define _reg_PHY_CAL_INTERVAL_COUNT_0                      0x0000016eU
#define _reg_PHY_CAL_SAMPLE_WAIT_0                         0x0000016fU
#define _reg_PHY_CAL_CLK_SELECT_0                          0x00000170U
#define _reg_PHY_CAL_RESULT_OBS_0                          0x00000171U
#define _reg_PHY_CAL_RESULT2_OBS_0                         0x00000172U
#define _reg_PHY_PAD_ATB_CTRL                              0x00000173U
#define _reg_PHY_ADRCTL_MANUAL_UPDATE                      0x00000174U
#define _reg_PHY_AC_LPBK_ERR_CLEAR                         0x00000175U
#define _reg_PHY_AC_LPBK_OBS_SELECT                        0x00000176U
#define _reg_PHY_AC_LPBK_ENABLE                            0x00000177U
#define _reg_PHY_AC_LPBK_CONTROL                           0x00000178U
#define _reg_PHY_AC_LPBK_RESULT_OBS                        0x00000179U
#define _reg_PHY_AC_CLK_LPBK_OBS_SELECT                    0x0000017aU
#define _reg_PHY_AC_CLK_LPBK_ENABLE                        0x0000017bU
#define _reg_PHY_AC_CLK_LPBK_CONTROL                       0x0000017cU
#define _reg_PHY_AC_CLK_LPBK_RESULT_OBS                    0x0000017dU
#define _reg_PHY_DATA_BYTE_ORDER_SEL                       0x0000017eU
#define _reg_PHY_ADR_DISABLE                               0x0000017fU
#define _reg_PHY_ADRCTL_MSTR_DLY_ENC_SEL                   0x00000180U
#define _reg_PHY_DDL_AC_ENABLE                             0x00000181U
#define _reg_PHY_DDL_AC_MODE                               0x00000182U
#define _reg_PHY_DLL_RST_EN                                0x00000183U
#define _reg_PI_START                                      0x00000184U
#define _reg_PI_DRAM_CLASS                                 0x00000185U
#define _reg_PI_VERSION                                    0x00000186U
#define _reg_PI_NORMAL_LVL_SEQ                             0x00000187U
#define _reg_PI_INIT_LVL_EN                                0x00000188U
#define _reg_PI_TCMD_GAP                                   0x00000189U
#define _reg_PI_TDFI_PHYMSTR_MAX_F0                        0x0000018aU
#define _reg_PI_TDFI_PHYMSTR_RESP_F0                       0x0000018bU
#define _reg_PI_TDFI_PHYMSTR_MAX_F1                        0x0000018cU
#define _reg_PI_TDFI_PHYMSTR_RESP_F1                       0x0000018dU
#define _reg_PI_TDFI_PHYMSTR_MAX_F2                        0x0000018eU
#define _reg_PI_TDFI_PHYMSTR_RESP_F2                       0x0000018fU
#define _reg_PI_TDFI_PHYUPD_RESP_F0                        0x00000190U
#define _reg_PI_TDFI_PHYUPD_TYPE0_F0                       0x00000191U
#define _reg_PI_TDFI_PHYUPD_TYPE1_F0                       0x00000192U
#define _reg_PI_TDFI_PHYUPD_TYPE2_F0                       0x00000193U
#define _reg_PI_TDFI_PHYUPD_TYPE3_F0                       0x00000194U
#define _reg_PI_TDFI_PHYUPD_RESP_F1                        0x00000195U
#define _reg_PI_TDFI_PHYUPD_TYPE0_F1                       0x00000196U
#define _reg_PI_TDFI_PHYUPD_TYPE1_F1                       0x00000197U
#define _reg_PI_TDFI_PHYUPD_TYPE2_F1                       0x00000198U
#define _reg_PI_TDFI_PHYUPD_TYPE3_F1                       0x00000199U
#define _reg_PI_TDFI_PHYUPD_RESP_F2                        0x0000019aU
#define _reg_PI_TDFI_PHYUPD_TYPE0_F2                       0x0000019bU
#define _reg_PI_TDFI_PHYUPD_TYPE1_F2                       0x0000019cU
#define _reg_PI_TDFI_PHYUPD_TYPE2_F2                       0x0000019dU
#define _reg_PI_TDFI_PHYUPD_TYPE3_F2                       0x0000019eU
#define _reg_PI_CONTROL_ERROR_STATUS                       0x0000019fU
#define _reg_PI_EXIT_AFTER_INIT_CALVL                      0x000001a0U
#define _reg_PI_FREQ_MAP                                   0x000001a1U
#define _reg_PI_INIT_WORK_FREQ                             0x000001a2U
#define _reg_PI_INIT_DFS_CALVL_ONLY                        0x000001a3U
#define _reg_PI_POWER_ON_SEQ_BYPASS_ARRAY                  0x000001a4U
#define _reg_PI_POWER_ON_SEQ_END_ARRAY                     0x000001a5U
#define _reg_PI_SEQ1_PAT                                   0x000001a6U
#define _reg_PI_SEQ1_PAT_MASK                              0x000001a7U
#define _reg_PI_SEQ2_PAT                                   0x000001a8U
#define _reg_PI_SEQ2_PAT_MASK                              0x000001a9U
#define _reg_PI_SEQ3_PAT                                   0x000001aaU
#define _reg_PI_SEQ3_PAT_MASK                              0x000001abU
#define _reg_PI_SEQ4_PAT                                   0x000001acU
#define _reg_PI_SEQ4_PAT_MASK                              0x000001adU
#define _reg_PI_SEQ5_PAT                                   0x000001aeU
#define _reg_PI_SEQ5_PAT_MASK                              0x000001afU
#define _reg_PI_SEQ6_PAT                                   0x000001b0U
#define _reg_PI_SEQ6_PAT_MASK                              0x000001b1U
#define _reg_PI_SEQ7_PAT                                   0x000001b2U
#define _reg_PI_SEQ7_PAT_MASK                              0x000001b3U
#define _reg_PI_SEQ8_PAT                                   0x000001b4U
#define _reg_PI_SEQ8_PAT_MASK                              0x000001b5U
#define _reg_PI_WDT_DISABLE                                0x000001b6U
#define _reg_PI_SW_RST_N                                   0x000001b7U
#define _reg_RESERVED_R0                                   0x000001b8U
#define _reg_PI_CS_MAP                                     0x000001b9U
#define _reg_PI_TDELAY_RDWR_2_BUS_IDLE_F0                  0x000001baU
#define _reg_PI_TDELAY_RDWR_2_BUS_IDLE_F1                  0x000001bbU
#define _reg_PI_TDELAY_RDWR_2_BUS_IDLE_F2                  0x000001bcU
#define _reg_PI_TMRR                                       0x000001bdU
#define _reg_PI_WRLAT_F0                                   0x000001beU
#define _reg_PI_ADDITIVE_LAT_F0                            0x000001bfU
#define _reg_PI_CASLAT_LIN_F0                              0x000001c0U
#define _reg_PI_WRLAT_F1                                   0x000001c1U
#define _reg_PI_ADDITIVE_LAT_F1                            0x000001c2U
#define _reg_PI_CASLAT_LIN_F1                              0x000001c3U
#define _reg_PI_WRLAT_F2                                   0x000001c4U
#define _reg_PI_ADDITIVE_LAT_F2                            0x000001c5U
#define _reg_PI_CASLAT_LIN_F2                              0x000001c6U
#define _reg_PI_PREAMBLE_SUPPORT                           0x000001c7U
#define _reg_PI_AREFRESH                                   0x000001c8U
#define _reg_PI_TRFC_F0                                    0x000001c9U
#define _reg_PI_TREF_F0                                    0x000001caU
#define _reg_PI_TRFC_F1                                    0x000001cbU
#define _reg_PI_TREF_F1                                    0x000001ccU
#define _reg_PI_TRFC_F2                                    0x000001cdU
#define _reg_PI_TREF_F2                                    0x000001ceU
#define _reg_PI_TREF_INTERVAL                              0x000001cfU
#define _reg_PI_SWLVL_LOAD                                 0x000001d0U
#define _reg_PI_SWLVL_OP_DONE                              0x000001d1U
#define _reg_PI_SW_WRLVL_RESP_0                            0x000001d2U
#define _reg_PI_SW_WRLVL_RESP_1                            0x000001d3U
#define _reg_PI_SW_WRLVL_RESP_2                            0x000001d4U
#define _reg_PI_SW_WRLVL_RESP_3                            0x000001d5U
#define _reg_PI_SW_RDLVL_RESP_0                            0x000001d6U
#define _reg_PI_SW_RDLVL_RESP_1                            0x000001d7U
#define _reg_PI_SW_RDLVL_RESP_2                            0x000001d8U
#define _reg_PI_SW_RDLVL_RESP_3                            0x000001d9U
#define _reg_PI_SW_CALVL_RESP_0                            0x000001daU
#define _reg_PI_SW_LEVELING_MODE                           0x000001dbU
#define _reg_PI_SWLVL_START                                0x000001dcU
#define _reg_PI_SWLVL_EXIT                                 0x000001ddU
#define _reg_PI_SWLVL_WR_SLICE_0                           0x000001deU
#define _reg_PI_SWLVL_RD_SLICE_0                           0x000001dfU
#define _reg_PI_SW_WDQLVL_RESP_0                           0x000001e0U
#define _reg_PI_SWLVL_WR_SLICE_1                           0x000001e1U
#define _reg_PI_SWLVL_RD_SLICE_1                           0x000001e2U
#define _reg_PI_SW_WDQLVL_RESP_1                           0x000001e3U
#define _reg_PI_SWLVL_WR_SLICE_2                           0x000001e4U
#define _reg_PI_SWLVL_RD_SLICE_2                           0x000001e5U
#define _reg_PI_SW_WDQLVL_RESP_2                           0x000001e6U
#define _reg_PI_SWLVL_WR_SLICE_3                           0x000001e7U
#define _reg_PI_SWLVL_RD_SLICE_3                           0x000001e8U
#define _reg_PI_SW_WDQLVL_RESP_3                           0x000001e9U
#define _reg_PI_SW_WDQLVL_VREF                             0x000001eaU
#define _reg_PI_SWLVL_SM2_START                            0x000001ebU
#define _reg_PI_SWLVL_SM2_WR                               0x000001ecU
#define _reg_PI_SWLVL_SM2_RD                               0x000001edU
#define _reg_PI_SEQUENTIAL_LVL_REQ                         0x000001eeU
#define _reg_PI_DFS_PERIOD_EN                              0x000001efU
#define _reg_PI_SRE_PERIOD_EN                              0x000001f0U
#define _reg_PI_16BIT_DRAM_CONNECT                         0x000001f1U
#define _reg_PI_TDFI_CTRL_DELAY_F0                         0x000001f2U
#define _reg_PI_TDFI_CTRL_DELAY_F1                         0x000001f3U
#define _reg_PI_TDFI_CTRL_DELAY_F2                         0x000001f4U
#define _reg_PI_WRLVL_REQ                                  0x000001f5U
#define _reg_PI_WRLVL_CS                                   0x000001f6U
#define _reg_PI_WLDQSEN                                    0x000001f7U
#define _reg_PI_WLMRD                                      0x000001f8U
#define _reg_PI_WRLVL_EN                                   0x000001f9U
#define _reg_PI_WRLVL_INTERVAL                             0x000001faU
#define _reg_PI_WRLVL_PERIODIC                             0x000001fbU
#define _reg_PI_WRLVL_ON_SREF_EXIT                         0x000001fcU
#define _reg_PI_WRLVL_RESP_MASK                            0x000001fdU
#define _reg_PI_WRLVL_ROTATE                               0x000001feU
#define _reg_PI_WRLVL_CS_MAP                               0x000001ffU
#define _reg_PI_WRLVL_ERROR_STATUS                         0x00000200U
#define _reg_PI_TDFI_WRLVL_EN                              0x00000201U
#define _reg_PI_TDFI_WRLVL_WW                              0x00000202U
#define _reg_PI_TDFI_WRLVL_RESP                            0x00000203U
#define _reg_PI_TDFI_WRLVL_MAX                             0x00000204U
#define _reg_PI_WRLVL_STROBE_NUM                           0x00000205U
#define _reg_PI_WRLVL_MRR_DQ_RETURN_HIZ                    0x00000206U
#define _reg_PI_WRLVL_EN_DEASSERT_2_MRR                    0x00000207U
#define _reg_PI_TODTL_2CMD_F0                              0x00000208U
#define _reg_PI_ODT_EN_F0                                  0x00000209U
#define _reg_PI_TODTL_2CMD_F1                              0x0000020aU
#define _reg_PI_ODT_EN_F1                                  0x0000020bU
#define _reg_PI_TODTL_2CMD_F2                              0x0000020cU
#define _reg_PI_ODT_EN_F2                                  0x0000020dU
#define _reg_PI_TODTH_WR                                   0x0000020eU
#define _reg_PI_TODTH_RD                                   0x0000020fU
#define _reg_PI_ODT_RD_MAP_CS0                             0x00000210U
#define _reg_PI_ODT_WR_MAP_CS0                             0x00000211U
#define _reg_PI_ODT_RD_MAP_CS1                             0x00000212U
#define _reg_PI_ODT_WR_MAP_CS1                             0x00000213U
#define _reg_PI_ODT_RD_MAP_CS2                             0x00000214U
#define _reg_PI_ODT_WR_MAP_CS2                             0x00000215U
#define _reg_PI_ODT_RD_MAP_CS3                             0x00000216U
#define _reg_PI_ODT_WR_MAP_CS3                             0x00000217U
#define _reg_PI_EN_ODT_ASSERT_EXCEPT_RD                    0x00000218U
#define _reg_PI_ODTLON_F0                                  0x00000219U
#define _reg_PI_TODTON_MIN_F0                              0x0000021aU
#define _reg_PI_ODTLON_F1                                  0x0000021bU
#define _reg_PI_TODTON_MIN_F1                              0x0000021cU
#define _reg_PI_ODTLON_F2                                  0x0000021dU
#define _reg_PI_TODTON_MIN_F2                              0x0000021eU
#define _reg_PI_WR_TO_ODTH_F0                              0x0000021fU
#define _reg_PI_WR_TO_ODTH_F1                              0x00000220U
#define _reg_PI_WR_TO_ODTH_F2                              0x00000221U
#define _reg_PI_RD_TO_ODTH_F0                              0x00000222U
#define _reg_PI_RD_TO_ODTH_F1                              0x00000223U
#define _reg_PI_RD_TO_ODTH_F2                              0x00000224U
#define _reg_PI_ADDRESS_MIRRORING                          0x00000225U
#define _reg_PI_RDLVL_REQ                                  0x00000226U
#define _reg_PI_RDLVL_GATE_REQ                             0x00000227U
#define _reg_PI_RDLVL_CS                                   0x00000228U
#define _reg_PI_RDLVL_SEQ_EN                               0x00000229U
#define _reg_PI_RDLVL_GATE_SEQ_EN                          0x0000022aU
#define _reg_PI_RDLVL_PERIODIC                             0x0000022bU
#define _reg_PI_RDLVL_ON_SREF_EXIT                         0x0000022cU
#define _reg_PI_RDLVL_GATE_PERIODIC                        0x0000022dU
#define _reg_PI_RDLVL_GATE_ON_SREF_EXIT                    0x0000022eU
#define _reg_RESERVED_R1                                   0x0000022fU
#define _reg_PI_RDLVL_ROTATE                               0x00000230U
#define _reg_PI_RDLVL_GATE_ROTATE                          0x00000231U
#define _reg_PI_RDLVL_CS_MAP                               0x00000232U
#define _reg_PI_RDLVL_GATE_CS_MAP                          0x00000233U
#define _reg_PI_TDFI_RDLVL_RR                              0x00000234U
#define _reg_PI_TDFI_RDLVL_RESP                            0x00000235U
#define _reg_PI_RDLVL_RESP_MASK                            0x00000236U
#define _reg_PI_TDFI_RDLVL_EN                              0x00000237U
#define _reg_PI_RDLVL_EN                                   0x00000238U
#define _reg_PI_RDLVL_GATE_EN                              0x00000239U
#define _reg_PI_TDFI_RDLVL_MAX                             0x0000023aU
#define _reg_PI_RDLVL_ERROR_STATUS                         0x0000023bU
#define _reg_PI_RDLVL_INTERVAL                             0x0000023cU
#define _reg_PI_RDLVL_GATE_INTERVAL                        0x0000023dU
#define _reg_PI_RDLVL_PATTERN_START                        0x0000023eU
#define _reg_PI_RDLVL_PATTERN_NUM                          0x0000023fU
#define _reg_PI_RDLVL_STROBE_NUM                           0x00000240U
#define _reg_PI_RDLVL_GATE_STROBE_NUM                      0x00000241U
#define _reg_PI_RD_PREAMBLE_TRAINING_EN                    0x00000242U
#define _reg_PI_REG_DIMM_ENABLE                            0x00000243U
#define _reg_PI_RDLAT_ADJ_F0                               0x00000244U
#define _reg_PI_RDLAT_ADJ_F1                               0x00000245U
#define _reg_PI_RDLAT_ADJ_F2                               0x00000246U
#define _reg_PI_TDFI_RDDATA_EN                             0x00000247U
#define _reg_PI_WRLAT_ADJ_F0                               0x00000248U
#define _reg_PI_WRLAT_ADJ_F1                               0x00000249U
#define _reg_PI_WRLAT_ADJ_F2                               0x0000024aU
#define _reg_PI_TDFI_PHY_WRLAT                             0x0000024bU
#define _reg_PI_TDFI_WRCSLAT_F0                            0x0000024cU
#define _reg_PI_TDFI_WRCSLAT_F1                            0x0000024dU
#define _reg_PI_TDFI_WRCSLAT_F2                            0x0000024eU
#define _reg_PI_TDFI_PHY_WRDATA                            0x0000024fU
#define _reg_PI_CALVL_REQ                                  0x00000250U
#define _reg_PI_CALVL_CS                                   0x00000251U
#define _reg_RESERVED_R2                                   0x00000252U
#define _reg_RESERVED_R3                                   0x00000253U
#define _reg_PI_CALVL_SEQ_EN                               0x00000254U
#define _reg_PI_CALVL_PERIODIC                             0x00000255U
#define _reg_PI_CALVL_ON_SREF_EXIT                         0x00000256U
#define _reg_PI_CALVL_ROTATE                               0x00000257U
#define _reg_PI_CALVL_CS_MAP                               0x00000258U
#define _reg_PI_TDFI_CALVL_EN                              0x00000259U
#define _reg_PI_TDFI_CALVL_CC_F0                           0x0000025aU
#define _reg_PI_TDFI_CALVL_CAPTURE_F0                      0x0000025bU
#define _reg_PI_TDFI_CALVL_CC_F1                           0x0000025cU
#define _reg_PI_TDFI_CALVL_CAPTURE_F1                      0x0000025dU
#define _reg_PI_TDFI_CALVL_CC_F2                           0x0000025eU
#define _reg_PI_TDFI_CALVL_CAPTURE_F2                      0x0000025fU
#define _reg_PI_TDFI_CALVL_RESP                            0x00000260U
#define _reg_PI_TDFI_CALVL_MAX                             0x00000261U
#define _reg_PI_CALVL_RESP_MASK                            0x00000262U
#define _reg_PI_CALVL_EN                                   0x00000263U
#define _reg_PI_CALVL_ERROR_STATUS                         0x00000264U
#define _reg_PI_CALVL_INTERVAL                             0x00000265U
#define _reg_PI_TCACKEL                                    0x00000266U
#define _reg_PI_TCAMRD                                     0x00000267U
#define _reg_PI_TCACKEH                                    0x00000268U
#define _reg_PI_TMRZ_F0                                    0x00000269U
#define _reg_PI_TCAENT_F0                                  0x0000026aU
#define _reg_PI_TMRZ_F1                                    0x0000026bU
#define _reg_PI_TCAENT_F1                                  0x0000026cU
#define _reg_PI_TMRZ_F2                                    0x0000026dU
#define _reg_PI_TCAENT_F2                                  0x0000026eU
#define _reg_PI_TCAEXT                                     0x0000026fU
#define _reg_PI_CA_TRAIN_VREF_EN                           0x00000270U
#define _reg_PI_TDFI_CACSCA_F0                             0x00000271U
#define _reg_PI_TDFI_CASEL_F0                              0x00000272U
#define _reg_PI_TVREF_SHORT_F0                             0x00000273U
#define _reg_PI_TVREF_LONG_F0                              0x00000274U
#define _reg_PI_TDFI_CACSCA_F1                             0x00000275U
#define _reg_PI_TDFI_CASEL_F1                              0x00000276U
#define _reg_PI_TVREF_SHORT_F1                             0x00000277U
#define _reg_PI_TVREF_LONG_F1                              0x00000278U
#define _reg_PI_TDFI_CACSCA_F2                             0x00000279U
#define _reg_PI_TDFI_CASEL_F2                              0x0000027aU
#define _reg_PI_TVREF_SHORT_F2                             0x0000027bU
#define _reg_PI_TVREF_LONG_F2                              0x0000027cU
#define _reg_PI_CALVL_VREF_INITIAL_START_POINT             0x0000027dU
#define _reg_PI_CALVL_VREF_INITIAL_STOP_POINT              0x0000027eU
#define _reg_PI_CALVL_VREF_INITIAL_STEPSIZE                0x0000027fU
#define _reg_PI_CALVL_VREF_NORMAL_STEPSIZE                 0x00000280U
#define _reg_PI_CALVL_VREF_DELTA                           0x00000281U
#define _reg_PI_TDFI_INIT_START_MIN                        0x00000282U
#define _reg_PI_TDFI_INIT_COMPLETE_MIN                     0x00000283U
#define _reg_PI_TDFI_CALVL_STROBE_F0                       0x00000284U
#define _reg_PI_TDFI_CALVL_STROBE_F1                       0x00000285U
#define _reg_PI_TDFI_CALVL_STROBE_F2                       0x00000286U
#define _reg_PI_TCKCKEH                                    0x00000287U
#define _reg_PI_CALVL_STROBE_NUM                           0x00000288U
#define _reg_PI_SW_CA_TRAIN_VREF                           0x00000289U
#define _reg_PI_TDFI_INIT_START_F0                         0x0000028aU
#define _reg_PI_TDFI_INIT_COMPLETE_F0                      0x0000028bU
#define _reg_PI_TDFI_INIT_START_F1                         0x0000028cU
#define _reg_PI_TDFI_INIT_COMPLETE_F1                      0x0000028dU
#define _reg_PI_TDFI_INIT_START_F2                         0x0000028eU
#define _reg_PI_TDFI_INIT_COMPLETE_F2                      0x0000028fU
#define _reg_PI_CLKDISABLE_2_INIT_START                    0x00000290U
#define _reg_PI_INIT_STARTORCOMPLETE_2_CLKDISABLE          0x00000291U
#define _reg_PI_DRAM_CLK_DISABLE_DEASSERT_SEL              0x00000292U
#define _reg_PI_REFRESH_BETWEEN_SEGMENT_DISABLE            0x00000293U
#define _reg_PI_TCKEHDQS_F0                                0x00000294U
#define _reg_PI_TCKEHDQS_F1                                0x00000295U
#define _reg_PI_TCKEHDQS_F2                                0x00000296U
#define _reg_PI_WDQLVL_VREF_EN                             0x00000297U
#define _reg_PI_WDQLVL_BST_NUM                             0x00000298U
#define _reg_PI_TDFI_WDQLVL_WR                             0x00000299U
#define _reg_PI_TDFI_WDQLVL_RW                             0x0000029aU
#define _reg_PI_WDQLVL_RESP_MASK                           0x0000029bU
#define _reg_PI_WDQLVL_ROTATE                              0x0000029cU
#define _reg_PI_WDQLVL_CS_MAP                              0x0000029dU
#define _reg_PI_WDQLVL_VREF_INITIAL_START_POINT            0x0000029eU
#define _reg_PI_WDQLVL_VREF_INITIAL_STOP_POINT             0x0000029fU
#define _reg_PI_WDQLVL_VREF_INITIAL_STEPSIZE               0x000002a0U
#define _reg_PI_WDQLVL_VREF_NORMAL_STEPSIZE                0x000002a1U
#define _reg_PI_WDQLVL_VREF_DELTA                          0x000002a2U
#define _reg_PI_WDQLVL_PERIODIC                            0x000002a3U
#define _reg_PI_WDQLVL_REQ                                 0x000002a4U
#define _reg_PI_WDQLVL_CS                                  0x000002a5U
#define _reg_PI_TDFI_WDQLVL_EN                             0x000002a6U
#define _reg_PI_TDFI_WDQLVL_RESP                           0x000002a7U
#define _reg_PI_TDFI_WDQLVL_MAX                            0x000002a8U
#define _reg_PI_WDQLVL_INTERVAL                            0x000002a9U
#define _reg_PI_WDQLVL_EN                                  0x000002aaU
#define _reg_PI_WDQLVL_ON_SREF_EXIT                        0x000002abU
#define _reg_PI_WDQLVL_ERROR_STATUS                        0x000002acU
#define _reg_PI_MR1_DATA_F0_0                              0x000002adU
#define _reg_PI_MR2_DATA_F0_0                              0x000002aeU
#define _reg_PI_MR3_DATA_F0_0                              0x000002afU
#define _reg_PI_MR11_DATA_F0_0                             0x000002b0U
#define _reg_PI_MR12_DATA_F0_0                             0x000002b1U
#define _reg_PI_MR14_DATA_F0_0                             0x000002b2U
#define _reg_PI_MR1_DATA_F1_0                              0x000002b3U
#define _reg_PI_MR2_DATA_F1_0                              0x000002b4U
#define _reg_PI_MR3_DATA_F1_0                              0x000002b5U
#define _reg_PI_MR11_DATA_F1_0                             0x000002b6U
#define _reg_PI_MR12_DATA_F1_0                             0x000002b7U
#define _reg_PI_MR14_DATA_F1_0                             0x000002b8U
#define _reg_PI_MR1_DATA_F2_0                              0x000002b9U
#define _reg_PI_MR2_DATA_F2_0                              0x000002baU
#define _reg_PI_MR3_DATA_F2_0                              0x000002bbU
#define _reg_PI_MR11_DATA_F2_0                             0x000002bcU
#define _reg_PI_MR12_DATA_F2_0                             0x000002bdU
#define _reg_PI_MR14_DATA_F2_0                             0x000002beU
#define _reg_PI_MR13_DATA_0                                0x000002bfU
#define _reg_PI_MR1_DATA_F0_1                              0x000002c0U
#define _reg_PI_MR2_DATA_F0_1                              0x000002c1U
#define _reg_PI_MR3_DATA_F0_1                              0x000002c2U
#define _reg_PI_MR11_DATA_F0_1                             0x000002c3U
#define _reg_PI_MR12_DATA_F0_1                             0x000002c4U
#define _reg_PI_MR14_DATA_F0_1                             0x000002c5U
#define _reg_PI_MR1_DATA_F1_1                              0x000002c6U
#define _reg_PI_MR2_DATA_F1_1                              0x000002c7U
#define _reg_PI_MR3_DATA_F1_1                              0x000002c8U
#define _reg_PI_MR11_DATA_F1_1                             0x000002c9U
#define _reg_PI_MR12_DATA_F1_1                             0x000002caU
#define _reg_PI_MR14_DATA_F1_1                             0x000002cbU
#define _reg_PI_MR1_DATA_F2_1                              0x000002ccU
#define _reg_PI_MR2_DATA_F2_1                              0x000002cdU
#define _reg_PI_MR3_DATA_F2_1                              0x000002ceU
#define _reg_PI_MR11_DATA_F2_1                             0x000002cfU
#define _reg_PI_MR12_DATA_F2_1                             0x000002d0U
#define _reg_PI_MR14_DATA_F2_1                             0x000002d1U
#define _reg_PI_MR13_DATA_1                                0x000002d2U
#define _reg_PI_MR1_DATA_F0_2                              0x000002d3U
#define _reg_PI_MR2_DATA_F0_2                              0x000002d4U
#define _reg_PI_MR3_DATA_F0_2                              0x000002d5U
#define _reg_PI_MR11_DATA_F0_2                             0x000002d6U
#define _reg_PI_MR12_DATA_F0_2                             0x000002d7U
#define _reg_PI_MR14_DATA_F0_2                             0x000002d8U
#define _reg_PI_MR1_DATA_F1_2                              0x000002d9U
#define _reg_PI_MR2_DATA_F1_2                              0x000002daU
#define _reg_PI_MR3_DATA_F1_2                              0x000002dbU
#define _reg_PI_MR11_DATA_F1_2                             0x000002dcU
#define _reg_PI_MR12_DATA_F1_2                             0x000002ddU
#define _reg_PI_MR14_DATA_F1_2                             0x000002deU
#define _reg_PI_MR1_DATA_F2_2                              0x000002dfU
#define _reg_PI_MR2_DATA_F2_2                              0x000002e0U
#define _reg_PI_MR3_DATA_F2_2                              0x000002e1U
#define _reg_PI_MR11_DATA_F2_2                             0x000002e2U
#define _reg_PI_MR12_DATA_F2_2                             0x000002e3U
#define _reg_PI_MR14_DATA_F2_2                             0x000002e4U
#define _reg_PI_MR13_DATA_2                                0x000002e5U
#define _reg_PI_MR1_DATA_F0_3                              0x000002e6U
#define _reg_PI_MR2_DATA_F0_3                              0x000002e7U
#define _reg_PI_MR3_DATA_F0_3                              0x000002e8U
#define _reg_PI_MR11_DATA_F0_3                             0x000002e9U
#define _reg_PI_MR12_DATA_F0_3                             0x000002eaU
#define _reg_PI_MR14_DATA_F0_3                             0x000002ebU
#define _reg_PI_MR1_DATA_F1_3                              0x000002ecU
#define _reg_PI_MR2_DATA_F1_3                              0x000002edU
#define _reg_PI_MR3_DATA_F1_3                              0x000002eeU
#define _reg_PI_MR11_DATA_F1_3                             0x000002efU
#define _reg_PI_MR12_DATA_F1_3                             0x000002f0U
#define _reg_PI_MR14_DATA_F1_3                             0x000002f1U
#define _reg_PI_MR1_DATA_F2_3                              0x000002f2U
#define _reg_PI_MR2_DATA_F2_3                              0x000002f3U
#define _reg_PI_MR3_DATA_F2_3                              0x000002f4U
#define _reg_PI_MR11_DATA_F2_3                             0x000002f5U
#define _reg_PI_MR12_DATA_F2_3                             0x000002f6U
#define _reg_PI_MR14_DATA_F2_3                             0x000002f7U
#define _reg_PI_MR13_DATA_3                                0x000002f8U
#define _reg_PI_BANK_DIFF                                  0x000002f9U
#define _reg_PI_ROW_DIFF                                   0x000002faU
#define _reg_PI_TFC_F0                                     0x000002fbU
#define _reg_PI_TFC_F1                                     0x000002fcU
#define _reg_PI_TFC_F2                                     0x000002fdU
#define _reg_PI_TCCD                                       0x000002feU
#define _reg_PI_TRTP_F0                                    0x000002ffU
#define _reg_PI_TRP_F0                                     0x00000300U
#define _reg_PI_TRCD_F0                                    0x00000301U
#define _reg_PI_TWTR_F0                                    0x00000302U
#define _reg_PI_TWR_F0                                     0x00000303U
#define _reg_PI_TRAS_MAX_F0                                0x00000304U
#define _reg_PI_TRAS_MIN_F0                                0x00000305U
#define _reg_PI_TDQSCK_MAX_F0                              0x00000306U
#define _reg_PI_TCCDMW_F0                                  0x00000307U
#define _reg_PI_TMRD_F0                                    0x00000308U
#define _reg_PI_TMRW_F0                                    0x00000309U
#define _reg_PI_TMOD_F0                                    0x0000030aU
#define _reg_PI_TRTP_F1                                    0x0000030bU
#define _reg_PI_TRP_F1                                     0x0000030cU
#define _reg_PI_TRCD_F1                                    0x0000030dU
#define _reg_PI_TWTR_F1                                    0x0000030eU
#define _reg_PI_TWR_F1                                     0x0000030fU
#define _reg_PI_TRAS_MAX_F1                                0x00000310U
#define _reg_PI_TRAS_MIN_F1                                0x00000311U
#define _reg_PI_TDQSCK_MAX_F1                              0x00000312U
#define _reg_PI_TCCDMW_F1                                  0x00000313U
#define _reg_PI_TMRD_F1                                    0x00000314U
#define _reg_PI_TMRW_F1                                    0x00000315U
#define _reg_PI_TMOD_F1                                    0x00000316U
#define _reg_PI_TRTP_F2                                    0x00000317U
#define _reg_PI_TRP_F2                                     0x00000318U
#define _reg_PI_TRCD_F2                                    0x00000319U
#define _reg_PI_TWTR_F2                                    0x0000031aU
#define _reg_PI_TWR_F2                                     0x0000031bU
#define _reg_PI_TRAS_MAX_F2                                0x0000031cU
#define _reg_PI_TRAS_MIN_F2                                0x0000031dU
#define _reg_PI_TDQSCK_MAX_F2                              0x0000031eU
#define _reg_PI_TCCDMW_F2                                  0x0000031fU
#define _reg_PI_TMRD_F2                                    0x00000320U
#define _reg_PI_TMRW_F2                                    0x00000321U
#define _reg_PI_TMOD_F2                                    0x00000322U
#define _reg_RESERVED_R4                                   0x00000323U
#define _reg_RESERVED_R5                                   0x00000324U
#define _reg_RESERVED_R6                                   0x00000325U
#define _reg_RESERVED_R7                                   0x00000326U
#define _reg_RESERVED_R8                                   0x00000327U
#define _reg_RESERVED_R9                                   0x00000328U
#define _reg_RESERVED_R10                                  0x00000329U
#define _reg_RESERVED_R11                                  0x0000032aU
#define _reg_RESERVED_R12                                  0x0000032bU
#define _reg_RESERVED_R13                                  0x0000032cU
#define _reg_RESERVED_R14                                  0x0000032dU
#define _reg_RESERVED_R15                                  0x0000032eU
#define _reg_RESERVED_R16                                  0x0000032fU
#define _reg_RESERVED_R17                                  0x00000330U
#define _reg_RESERVED_R18                                  0x00000331U
#define _reg_RESERVED_R19                                  0x00000332U
#define _reg_RESERVED_R20                                  0x00000333U
#define _reg_RESERVED_R21                                  0x00000334U
#define _reg_RESERVED_R22                                  0x00000335U
#define _reg_RESERVED_R23                                  0x00000336U
#define _reg_PI_INT_STATUS                                 0x00000337U
#define _reg_PI_INT_ACK                                    0x00000338U
#define _reg_PI_INT_MASK                                   0x00000339U
#define _reg_PI_BIST_EXP_DATA_P0                           0x0000033aU
#define _reg_PI_BIST_EXP_DATA_P1                           0x0000033bU
#define _reg_PI_BIST_EXP_DATA_P2                           0x0000033cU
#define _reg_PI_BIST_EXP_DATA_P3                           0x0000033dU
#define _reg_PI_BIST_FAIL_DATA_P0                          0x0000033eU
#define _reg_PI_BIST_FAIL_DATA_P1                          0x0000033fU
#define _reg_PI_BIST_FAIL_DATA_P2                          0x00000340U
#define _reg_PI_BIST_FAIL_DATA_P3                          0x00000341U
#define _reg_PI_BIST_FAIL_ADDR_P0                          0x00000342U
#define _reg_PI_BIST_FAIL_ADDR_P1                          0x00000343U
#define _reg_PI_BSTLEN                                     0x00000344U
#define _reg_PI_LONG_COUNT_MASK                            0x00000345U
#define _reg_PI_CMD_SWAP_EN                                0x00000346U
#define _reg_PI_CKE_MUX_0                                  0x00000347U
#define _reg_PI_CKE_MUX_1                                  0x00000348U
#define _reg_PI_CKE_MUX_2                                  0x00000349U
#define _reg_PI_CKE_MUX_3                                  0x0000034aU
#define _reg_PI_CS_MUX_0                                   0x0000034bU
#define _reg_PI_CS_MUX_1                                   0x0000034cU
#define _reg_PI_CS_MUX_2                                   0x0000034dU
#define _reg_PI_CS_MUX_3                                   0x0000034eU
#define _reg_PI_RAS_N_MUX                                  0x0000034fU
#define _reg_PI_CAS_N_MUX                                  0x00000350U
#define _reg_PI_WE_N_MUX                                   0x00000351U
#define _reg_PI_BANK_MUX_0                                 0x00000352U
#define _reg_PI_BANK_MUX_1                                 0x00000353U
#define _reg_PI_BANK_MUX_2                                 0x00000354U
#define _reg_PI_ODT_MUX_0                                  0x00000355U
#define _reg_PI_ODT_MUX_1                                  0x00000356U
#define _reg_PI_ODT_MUX_2                                  0x00000357U
#define _reg_PI_ODT_MUX_3                                  0x00000358U
#define _reg_PI_RESET_N_MUX_0                              0x00000359U
#define _reg_PI_RESET_N_MUX_1                              0x0000035aU
#define _reg_PI_RESET_N_MUX_2                              0x0000035bU
#define _reg_PI_RESET_N_MUX_3                              0x0000035cU
#define _reg_PI_DATA_BYTE_SWAP_EN                          0x0000035dU
#define _reg_PI_DATA_BYTE_SWAP_SLICE0                      0x0000035eU
#define _reg_PI_DATA_BYTE_SWAP_SLICE1                      0x0000035fU
#define _reg_PI_DATA_BYTE_SWAP_SLICE2                      0x00000360U
#define _reg_PI_DATA_BYTE_SWAP_SLICE3                      0x00000361U
#define _reg_PI_CTRLUPD_REQ_PER_AREF_EN                    0x00000362U
#define _reg_PI_TDFI_CTRLUPD_MIN                           0x00000363U
#define _reg_PI_TDFI_CTRLUPD_MAX_F0                        0x00000364U
#define _reg_PI_TDFI_CTRLUPD_INTERVAL_F0                   0x00000365U
#define _reg_PI_TDFI_CTRLUPD_MAX_F1                        0x00000366U
#define _reg_PI_TDFI_CTRLUPD_INTERVAL_F1                   0x00000367U
#define _reg_PI_TDFI_CTRLUPD_MAX_F2                        0x00000368U
#define _reg_PI_TDFI_CTRLUPD_INTERVAL_F2                   0x00000369U
#define _reg_PI_UPDATE_ERROR_STATUS                        0x0000036aU
#define _reg_PI_BIST_GO                                    0x0000036bU
#define _reg_PI_BIST_RESULT                                0x0000036cU
#define _reg_PI_ADDR_SPACE                                 0x0000036dU
#define _reg_PI_BIST_DATA_CHECK                            0x0000036eU
#define _reg_PI_BIST_ADDR_CHECK                            0x0000036fU
#define _reg_PI_BIST_START_ADDRESS_P0                      0x00000370U
#define _reg_PI_BIST_START_ADDRESS_P1                      0x00000371U
#define _reg_PI_BIST_DATA_MASK_P0                          0x00000372U
#define _reg_PI_BIST_DATA_MASK_P1                          0x00000373U
#define _reg_PI_COL_DIFF                                   0x00000374U

#define DDR_REGDEF_ADR(regdef) ((regdef)&0xffff)
#define DDR_REGDEF_LEN(regdef) (((regdef)>>16)&0xff)
#define DDR_REGDEF_LSB(regdef) (((regdef)>>24)&0xff)

const static uint32_t DDR_REGDEF_TBL[2][885] =
{
	{
/*0000*/	0xffffffffU,
/*0001*/	0xffffffffU,
/*0002*/	0x000b0400U,
/*0003*/	0x10010400U,
/*0004*/	0x18050400U,
/*0005*/	0x00050401U,
/*0006*/	0x08050401U,
/*0007*/	0x10050401U,
/*0008*/	0x18050401U,
/*0009*/	0x00050402U,
/*000a*/	0x08050402U,
/*000b*/	0x10050402U,
/*000c*/	0x18050402U,
/*000d*/	0x00040403U,
/*000e*/	0x08030403U,
/*000f*/	0x00180404U,
/*0010*/	0x18030404U,
/*0011*/	0x00180405U,
/*0012*/	0x18020405U,
/*0013*/	0x00010406U,
/*0014*/	0x08020406U,
/*0015*/	0x10010406U,
/*0016*/	0x18010406U,
/*0017*/	0x00020407U,
/*0018*/	0x08040407U,
/*0019*/	0x10040407U,
/*001a*/	0x18040407U,
/*001b*/	0x000a0408U,
/*001c*/	0x10040408U,
/*001d*/	0x18070408U,
/*001e*/	0x000a0409U,
/*001f*/	0x10040409U,
/*0020*/	0x18010409U,
/*0021*/	0x0001040aU,
/*0022*/	0x0802040aU,
/*0023*/	0x1009040aU,
/*0024*/	0x0009040bU,
/*0025*/	0x1002040bU,
/*0026*/	0x0020040cU,
/*0027*/	0x0001040dU,
/*0028*/	0x0020040eU,
/*0029*/	0x0020040fU,
/*002a*/	0x00200410U,
/*002b*/	0x00200411U,
/*002c*/	0x00030412U,
/*002d*/	0x08010412U,
/*002e*/	0x10030412U,
/*002f*/	0x18030412U,
/*0030*/	0x00040413U,
/*0031*/	0x08040413U,
/*0032*/	0x10040413U,
/*0033*/	0x18040413U,
/*0034*/	0x00010414U,
/*0035*/	0x08010414U,
/*0036*/	0x10060414U,
/*0037*/	0x18040414U,
/*0038*/	0x00060415U,
/*0039*/	0x08040415U,
/*003a*/	0x10060415U,
/*003b*/	0x18040415U,
/*003c*/	0x00020416U,
/*003d*/	0x08050416U,
/*003e*/	0x10080416U,
/*003f*/	0x00200417U,
/*0040*/	0x00060418U,
/*0041*/	0x08030418U,
/*0042*/	0x100b0418U,
/*0043*/	0x00040419U,
/*0044*/	0x08040419U,
/*0045*/	0x10040419U,
/*0046*/	0x18010419U,
/*0047*/	0x0009041aU,
/*0048*/	0x0020041bU,
/*0049*/	0x0020041cU,
/*004a*/	0x0020041dU,
/*004b*/	0x0020041eU,
/*004c*/	0x0010041fU,
/*004d*/	0x00200420U,
/*004e*/	0x00010421U,
/*004f*/	0x08060421U,
/*0050*/	0x10080421U,
/*0051*/	0x00200422U,
/*0052*/	0x000a0423U,
/*0053*/	0x10060423U,
/*0054*/	0x18070423U,
/*0055*/	0x00080424U,
/*0056*/	0x08080424U,
/*0057*/	0x100a0424U,
/*0058*/	0x00070425U,
/*0059*/	0x08080425U,
/*005a*/	0x10080425U,
/*005b*/	0x18030425U,
/*005c*/	0x000a0426U,
/*005d*/	0x100a0426U,
/*005e*/	0x00110427U,
/*005f*/	0x00090428U,
/*0060*/	0x10090428U,
/*0061*/	0x00100429U,
/*0062*/	0x100e0429U,
/*0063*/	0x000e042aU,
/*0064*/	0x100c042aU,
/*0065*/	0x000a042bU,
/*0066*/	0x100a042bU,
/*0067*/	0x0002042cU,
/*0068*/	0x0020042dU,
/*0069*/	0x000b042eU,
/*006a*/	0x100b042eU,
/*006b*/	0x0020042fU,
/*006c*/	0x00120430U,
/*006d*/	0x00200431U,
/*006e*/	0x00200432U,
/*006f*/	0x00010433U,
/*0070*/	0x08010433U,
/*0071*/	0x10080433U,
/*0072*/	0x000c0434U,
/*0073*/	0x100c0434U,
/*0074*/	0x000c0435U,
/*0075*/	0x100c0435U,
/*0076*/	0x000c0436U,
/*0077*/	0x100c0436U,
/*0078*/	0x000c0437U,
/*0079*/	0x100c0437U,
/*007a*/	0x000c0438U,
/*007b*/	0x100c0438U,
/*007c*/	0x000c0439U,
/*007d*/	0x100b0439U,
/*007e*/	0x000b043aU,
/*007f*/	0x100b043aU,
/*0080*/	0x000b043bU,
/*0081*/	0x100b043bU,
/*0082*/	0x000b043cU,
/*0083*/	0x100b043cU,
/*0084*/	0x000b043dU,
/*0085*/	0x100b043dU,
/*0086*/	0x000b043eU,
/*0087*/	0x100a043eU,
/*0088*/	0x000a043fU,
/*0089*/	0x100a043fU,
/*008a*/	0x000a0440U,
/*008b*/	0x100a0440U,
/*008c*/	0x000a0441U,
/*008d*/	0x100a0441U,
/*008e*/	0x000a0442U,
/*008f*/	0x100a0442U,
/*0090*/	0xffffffffU,
/*0091*/	0x000a0443U,
/*0092*/	0x100a0443U,
/*0093*/	0x000a0444U,
/*0094*/	0x100a0444U,
/*0095*/	0x000a0445U,
/*0096*/	0x100a0445U,
/*0097*/	0x000a0446U,
/*0098*/	0x100a0446U,
/*0099*/	0x000a0447U,
/*009a*/	0x100a0447U,
/*009b*/	0x000a0448U,
/*009c*/	0x100a0448U,
/*009d*/	0x000a0449U,
/*009e*/	0x100a0449U,
/*009f*/	0x000a044aU,
/*00a0*/	0x100a044aU,
/*00a1*/	0x000a044bU,
/*00a2*/	0x100a044bU,
/*00a3*/	0x000a044cU,
/*00a4*/	0x1004044cU,
/*00a5*/	0x1803044cU,
/*00a6*/	0x000a044dU,
/*00a7*/	0x100a044dU,
/*00a8*/	0x0001044eU,
/*00a9*/	0x080a044eU,
/*00aa*/	0x1804044eU,
/*00ab*/	0x000b044fU,
/*00ac*/	0x100a044fU,
/*00ad*/	0x00080450U,
/*00ae*/	0x08080450U,
/*00af*/	0x10080450U,
/*00b0*/	0x18080450U,
/*00b1*/	0x00080451U,
/*00b2*/	0x08080451U,
/*00b3*/	0x10010451U,
/*00b4*/	0x18080451U,
/*00b5*/	0x00080452U,
/*00b6*/	0x08020452U,
/*00b7*/	0x10020452U,
/*00b8*/	0x18040452U,
/*00b9*/	0x00040453U,
/*00ba*/	0x08040453U,
/*00bb*/	0x100a0453U,
/*00bc*/	0x00060454U,
/*00bd*/	0x08080454U,
/*00be*/	0x10040454U,
/*00bf*/	0x18040454U,
/*00c0*/	0x00050455U,
/*00c1*/	0x08040455U,
/*00c2*/	0x10050455U,
/*00c3*/	0x000a0456U,
/*00c4*/	0x100a0456U,
/*00c5*/	0x00080457U,
/*00c6*/	0x08040457U,
/*00c7*/	0x00050600U,
/*00c8*/	0x08050600U,
/*00c9*/	0x10050600U,
/*00ca*/	0x18050600U,
/*00cb*/	0x00050601U,
/*00cc*/	0x08050601U,
/*00cd*/	0x100b0601U,
/*00ce*/	0x00010602U,
/*00cf*/	0x08030602U,
/*00d0*/	0x00200603U,
/*00d1*/	0x00030604U,
/*00d2*/	0x080a0604U,
/*00d3*/	0x18030604U,
/*00d4*/	0x00030605U,
/*00d5*/	0x08010605U,
/*00d6*/	0x10010605U,
/*00d7*/	0x18060605U,
/*00d8*/	0x00020606U,
/*00d9*/	0x08030606U,
/*00da*/	0x10010606U,
/*00db*/	0x000f0607U,
/*00dc*/	0x00200608U,
/*00dd*/	0x00200609U,
/*00de*/	0x000b060aU,
/*00df*/	0x100b060aU,
/*00e0*/	0x000b060bU,
/*00e1*/	0x0018060cU,
/*00e2*/	0x0018060dU,
/*00e3*/	0x0018060eU,
/*00e4*/	0x0018060fU,
/*00e5*/	0x1804060fU,
/*00e6*/	0x00050610U,
/*00e7*/	0x08020610U,
/*00e8*/	0x10040610U,
/*00e9*/	0x18040610U,
/*00ea*/	0x00010611U,
/*00eb*/	0x08010611U,
/*00ec*/	0x10010611U,
/*00ed*/	0x18030611U,
/*00ee*/	0x00200612U,
/*00ef*/	0x00200613U,
/*00f0*/	0x00010614U,
/*00f1*/	0x08140614U,
/*00f2*/	0x00140615U,
/*00f3*/	0x00140616U,
/*00f4*/	0x00140617U,
/*00f5*/	0x00140618U,
/*00f6*/	0x00140619U,
/*00f7*/	0x0014061aU,
/*00f8*/	0x0014061bU,
/*00f9*/	0x0018061cU,
/*00fa*/	0x000a061dU,
/*00fb*/	0x1006061dU,
/*00fc*/	0x1806061dU,
/*00fd*/	0x0006061eU,
/*00fe*/	0xffffffffU,
/*00ff*/	0x0008061fU,
/*0100*/	0x080b061fU,
/*0101*/	0x000b0620U,
/*0102*/	0x100b0620U,
/*0103*/	0x000b0621U,
/*0104*/	0x100b0621U,
/*0105*/	0x000b0622U,
/*0106*/	0x10040622U,
/*0107*/	0x000a0623U,
/*0108*/	0x10060623U,
/*0109*/	0x18080623U,
/*010a*/	0x00040624U,
/*010b*/	0x00010700U,
/*010c*/	0x08020700U,
/*010d*/	0x10050700U,
/*010e*/	0x18050700U,
/*010f*/	0x00050701U,
/*0110*/	0x08050701U,
/*0111*/	0x100b0701U,
/*0112*/	0x00050702U,
/*0113*/	0x08010702U,
/*0114*/	0x10010702U,
/*0115*/	0x18010702U,
/*0116*/	0x00010703U,
/*0117*/	0x08040703U,
/*0118*/	0x100b0703U,
/*0119*/	0x000b0704U,
/*011a*/	0x10040704U,
/*011b*/	0x000b0705U,
/*011c*/	0x10040705U,
/*011d*/	0x18010705U,
/*011e*/	0x00010706U,
/*011f*/	0x08010706U,
/*0120*/	0x00200707U,
/*0121*/	0x00200708U,
/*0122*/	0x00080709U,
/*0123*/	0x080a0709U,
/*0124*/	0x18050709U,
/*0125*/	0x000a070aU,
/*0126*/	0x1003070aU,
/*0127*/	0x1803070aU,
/*0128*/	0x0001070bU,
/*0129*/	0x0802070bU,
/*012a*/	0x1001070bU,
/*012b*/	0x1801070bU,
/*012c*/	0x0001070cU,
/*012d*/	0x0802070cU,
/*012e*/	0xffffffffU,
/*012f*/	0xffffffffU,
/*0130*/	0xffffffffU,
/*0131*/	0xffffffffU,
/*0132*/	0x1001070cU,
/*0133*/	0x1801070cU,
/*0134*/	0x000d070dU,
/*0135*/	0xffffffffU,
/*0136*/	0xffffffffU,
/*0137*/	0x0005070eU,
/*0138*/	0x0001070fU,
/*0139*/	0x080e070fU,
/*013a*/	0x000e0710U,
/*013b*/	0x100e0710U,
/*013c*/	0x000e0711U,
/*013d*/	0x100e0711U,
/*013e*/	0x00040712U,
/*013f*/	0xffffffffU,
/*0140*/	0xffffffffU,
/*0141*/	0xffffffffU,
/*0142*/	0xffffffffU,
/*0143*/	0x080b0712U,
/*0144*/	0x000b0713U,
/*0145*/	0x100b0713U,
/*0146*/	0x000b0714U,
/*0147*/	0x000d0715U,
/*0148*/	0xffffffffU,
/*0149*/	0xffffffffU,
/*014a*/	0x10100715U,
/*014b*/	0x00080716U,
/*014c*/	0xffffffffU,
/*014d*/	0x08100716U,
/*014e*/	0x00100717U,
/*014f*/	0x10100717U,
/*0150*/	0x00100718U,
/*0151*/	0x10100718U,
/*0152*/	0x00030719U,
/*0153*/	0x08040719U,
/*0154*/	0x10010719U,
/*0155*/	0x18040719U,
/*0156*/	0x0001071aU,
/*0157*/	0x0812071aU,
/*0158*/	0x000a071bU,
/*0159*/	0x100c071bU,
/*015a*/	0x0012071cU,
/*015b*/	0x0014071dU,
/*015c*/	0x0012071eU,
/*015d*/	0x0011071fU,
/*015e*/	0x00110720U,
/*015f*/	0x00120721U,
/*0160*/	0x00120722U,
/*0161*/	0x00120723U,
/*0162*/	0x00120724U,
/*0163*/	0x00120725U,
/*0164*/	0x00120726U,
/*0165*/	0x00120727U,
/*0166*/	0x00120728U,
/*0167*/	0x00190729U,
/*0168*/	0x0019072aU,
/*0169*/	0x0020072bU,
/*016a*/	0x0017072cU,
/*016b*/	0x1808072cU,
/*016c*/	0x0001072dU,
/*016d*/	0x0801072dU,
/*016e*/	0x0020072eU,
/*016f*/	0x0008072fU,
/*0170*/	0x0803072fU,
/*0171*/	0x00180730U,
/*0172*/	0x00180731U,
/*0173*/	0x00100732U,
/*0174*/	0x10010732U,
/*0175*/	0x18010732U,
/*0176*/	0x00050733U,
/*0177*/	0x00200734U,
/*0178*/	0x00090735U,
/*0179*/	0x00200736U,
/*017a*/	0x00040737U,
/*017b*/	0x08100737U,
/*017c*/	0x18060737U,
/*017d*/	0x00100738U,
/*017e*/	0xffffffffU,
/*017f*/	0xffffffffU,
/*0180*/	0xffffffffU,
/*0181*/	0x00200739U,
/*0182*/	0x000b073aU,
/*0183*/	0xffffffffU,
/*0184*/	0x00010200U,
/*0185*/	0x08040200U,
/*0186*/	0x10100200U,
/*0187*/	0x00010201U,
/*0188*/	0x08010201U,
/*0189*/	0x10100201U,
/*018a*/	0x00200202U,
/*018b*/	0x00100203U,
/*018c*/	0x00200204U,
/*018d*/	0x00100205U,
/*018e*/	0x00200206U,
/*018f*/	0x00100207U,
/*0190*/	0x10100207U,
/*0191*/	0x00200208U,
/*0192*/	0x00200209U,
/*0193*/	0x0020020aU,
/*0194*/	0x0020020bU,
/*0195*/	0x0010020cU,
/*0196*/	0x0020020dU,
/*0197*/	0x0020020eU,
/*0198*/	0x0020020fU,
/*0199*/	0x00200210U,
/*019a*/	0x00100211U,
/*019b*/	0x00200212U,
/*019c*/	0x00200213U,
/*019d*/	0x00200214U,
/*019e*/	0x00200215U,
/*019f*/	0x00090216U,
/*01a0*/	0x10010216U,
/*01a1*/	0x00200217U,
/*01a2*/	0x00050218U,
/*01a3*/	0x08010218U,
/*01a4*/	0x10080218U,
/*01a5*/	0x18080218U,
/*01a6*/	0x001c0219U,
/*01a7*/	0x001c021aU,
/*01a8*/	0x001c021bU,
/*01a9*/	0x001c021cU,
/*01aa*/	0x001c021dU,
/*01ab*/	0x001c021eU,
/*01ac*/	0x001c021fU,
/*01ad*/	0x001c0220U,
/*01ae*/	0x001c0221U,
/*01af*/	0x001c0222U,
/*01b0*/	0x001c0223U,
/*01b1*/	0x001c0224U,
/*01b2*/	0x001c0225U,
/*01b3*/	0x001c0226U,
/*01b4*/	0x001c0227U,
/*01b5*/	0x001c0228U,
/*01b6*/	0x00010229U,
/*01b7*/	0x08010229U,
/*01b8*/	0x10010229U,
/*01b9*/	0x18040229U,
/*01ba*/	0x0008022aU,
/*01bb*/	0x0808022aU,
/*01bc*/	0x1008022aU,
/*01bd*/	0x1804022aU,
/*01be*/	0x0006022bU,
/*01bf*/	0xffffffffU,
/*01c0*/	0x0807022bU,
/*01c1*/	0x1006022bU,
/*01c2*/	0xffffffffU,
/*01c3*/	0x1807022bU,
/*01c4*/	0x0006022cU,
/*01c5*/	0xffffffffU,
/*01c6*/	0x0807022cU,
/*01c7*/	0x1002022cU,
/*01c8*/	0x1801022cU,
/*01c9*/	0x000a022dU,
/*01ca*/	0x1010022dU,
/*01cb*/	0x000a022eU,
/*01cc*/	0x1010022eU,
/*01cd*/	0x000a022fU,
/*01ce*/	0x1010022fU,
/*01cf*/	0x00100230U,
/*01d0*/	0x10010230U,
/*01d1*/	0x18010230U,
/*01d2*/	0x00010231U,
/*01d3*/	0x08010231U,
/*01d4*/	0x10010231U,
/*01d5*/	0x18010231U,
/*01d6*/	0x00020232U,
/*01d7*/	0x08020232U,
/*01d8*/	0x10020232U,
/*01d9*/	0x18020232U,
/*01da*/	0x00020233U,
/*01db*/	0x08030233U,
/*01dc*/	0x10010233U,
/*01dd*/	0x18010233U,
/*01de*/	0x00010234U,
/*01df*/	0x08010234U,
/*01e0*/	0x10020234U,
/*01e1*/	0x18010234U,
/*01e2*/	0x00010235U,
/*01e3*/	0x08020235U,
/*01e4*/	0x10010235U,
/*01e5*/	0x18010235U,
/*01e6*/	0x00020236U,
/*01e7*/	0x08010236U,
/*01e8*/	0x10010236U,
/*01e9*/	0x18020236U,
/*01ea*/	0x00070237U,
/*01eb*/	0x08010237U,
/*01ec*/	0x10010237U,
/*01ed*/	0x18010237U,
/*01ee*/	0x00010238U,
/*01ef*/	0x08010238U,
/*01f0*/	0x10010238U,
/*01f1*/	0x18010238U,
/*01f2*/	0x00040239U,
/*01f3*/	0x08040239U,
/*01f4*/	0x10040239U,
/*01f5*/	0x18010239U,
/*01f6*/	0x0002023aU,
/*01f7*/	0x0806023aU,
/*01f8*/	0x1006023aU,
/*01f9*/	0x1802023aU,
/*01fa*/	0x0010023bU,
/*01fb*/	0x1001023bU,
/*01fc*/	0x1801023bU,
/*01fd*/	0x0004023cU,
/*01fe*/	0x0801023cU,
/*01ff*/	0x1004023cU,
/*0200*/	0x1802023cU,
/*0201*/	0x0008023dU,
/*0202*/	0x080a023dU,
/*0203*/	0x0020023eU,
/*0204*/	0x0020023fU,
/*0205*/	0x00050240U,
/*0206*/	0x08010240U,
/*0207*/	0x10050240U,
/*0208*/	0x18080240U,
/*0209*/	0x00010241U,
/*020a*/	0x08080241U,
/*020b*/	0x10010241U,
/*020c*/	0x18080241U,
/*020d*/	0x00010242U,
/*020e*/	0x08040242U,
/*020f*/	0x10040242U,
/*0210*/	0x18040242U,
/*0211*/	0x00040243U,
/*0212*/	0x08040243U,
/*0213*/	0x10040243U,
/*0214*/	0x18040243U,
/*0215*/	0x00040244U,
/*0216*/	0x08040244U,
/*0217*/	0x10040244U,
/*0218*/	0x18010244U,
/*0219*/	0x00040245U,
/*021a*/	0x08040245U,
/*021b*/	0x10040245U,
/*021c*/	0x18040245U,
/*021d*/	0x00040246U,
/*021e*/	0x08040246U,
/*021f*/	0x10060246U,
/*0220*/	0x18060246U,
/*0221*/	0x00060247U,
/*0222*/	0x08060247U,
/*0223*/	0x10060247U,
/*0224*/	0x18060247U,
/*0225*/	0xffffffffU,
/*0226*/	0x00010248U,
/*0227*/	0x08010248U,
/*0228*/	0x10020248U,
/*0229*/	0x18040248U,
/*022a*/	0x00040249U,
/*022b*/	0x08010249U,
/*022c*/	0x10010249U,
/*022d*/	0x18010249U,
/*022e*/	0x0001024aU,
/*022f*/	0x0801024aU,
/*0230*/	0x1001024aU,
/*0231*/	0x1801024aU,
/*0232*/	0x0004024bU,
/*0233*/	0x0804024bU,
/*0234*/	0x100a024bU,
/*0235*/	0x0020024cU,
/*0236*/	0x0004024dU,
/*0237*/	0x0808024dU,
/*0238*/	0x1002024dU,
/*0239*/	0x1802024dU,
/*023a*/	0x0020024eU,
/*023b*/	0x0002024fU,
/*023c*/	0x0810024fU,
/*023d*/	0x00100250U,
/*023e*/	0x10040250U,
/*023f*/	0x18040250U,
/*0240*/	0x00050251U,
/*0241*/	0x08050251U,
/*0242*/	0x10010251U,
/*0243*/	0x18010251U,
/*0244*/	0x00070252U,
/*0245*/	0x08070252U,
/*0246*/	0x10070252U,
/*0247*/	0x18070252U,
/*0248*/	0x00070253U,
/*0249*/	0x08070253U,
/*024a*/	0x10070253U,
/*024b*/	0x18070253U,
/*024c*/	0x00070254U,
/*024d*/	0x08070254U,
/*024e*/	0x10070254U,
/*024f*/	0x18030254U,
/*0250*/	0x00010255U,
/*0251*/	0x08020255U,
/*0252*/	0x10010255U,
/*0253*/	0x18040255U,
/*0254*/	0x00020256U,
/*0255*/	0x08010256U,
/*0256*/	0x10010256U,
/*0257*/	0x18010256U,
/*0258*/	0x00040257U,
/*0259*/	0x08080257U,
/*025a*/	0x100a0257U,
/*025b*/	0x000a0258U,
/*025c*/	0x100a0258U,
/*025d*/	0x000a0259U,
/*025e*/	0x100a0259U,
/*025f*/	0x000a025aU,
/*0260*/	0x0020025bU,
/*0261*/	0x0020025cU,
/*0262*/	0x0001025dU,
/*0263*/	0x0802025dU,
/*0264*/	0x1002025dU,
/*0265*/	0x0010025eU,
/*0266*/	0x1005025eU,
/*0267*/	0x1806025eU,
/*0268*/	0x0005025fU,
/*0269*/	0x0805025fU,
/*026a*/	0x100e025fU,
/*026b*/	0x00050260U,
/*026c*/	0x080e0260U,
/*026d*/	0x18050260U,
/*026e*/	0x000e0261U,
/*026f*/	0x10050261U,
/*0270*/	0x18010261U,
/*0271*/	0x00050262U,
/*0272*/	0x08050262U,
/*0273*/	0x100a0262U,
/*0274*/	0x000a0263U,
/*0275*/	0x10050263U,
/*0276*/	0x18050263U,
/*0277*/	0x000a0264U,
/*0278*/	0x100a0264U,
/*0279*/	0x00050265U,
/*027a*/	0x08050265U,
/*027b*/	0x100a0265U,
/*027c*/	0x000a0266U,
/*027d*/	0x10070266U,
/*027e*/	0x18070266U,
/*027f*/	0x00040267U,
/*0280*/	0x08040267U,
/*0281*/	0x10040267U,
/*0282*/	0x18080267U,
/*0283*/	0x00080268U,
/*0284*/	0x08040268U,
/*0285*/	0x10040268U,
/*0286*/	0x18040268U,
/*0287*/	0x00040269U,
/*0288*/	0x08050269U,
/*0289*/	0x10070269U,
/*028a*/	0x18080269U,
/*028b*/	0x0010026aU,
/*028c*/	0x1008026aU,
/*028d*/	0x0010026bU,
/*028e*/	0x1008026bU,
/*028f*/	0x0010026cU,
/*0290*/	0x1008026cU,
/*0291*/	0x1808026cU,
/*0292*/	0x0001026dU,
/*0293*/	0x0801026dU,
/*0294*/	0x1006026dU,
/*0295*/	0x1806026dU,
/*0296*/	0x0006026eU,
/*0297*/	0x0801026eU,
/*0298*/	0x1003026eU,
/*0299*/	0x000a026fU,
/*029a*/	0x100a026fU,
/*029b*/	0x00040270U,
/*029c*/	0x08010270U,
/*029d*/	0x10040270U,
/*029e*/	0x18070270U,
/*029f*/	0x00070271U,
/*02a0*/	0x08050271U,
/*02a1*/	0x10050271U,
/*02a2*/	0x18040271U,
/*02a3*/	0x00010272U,
/*02a4*/	0x08010272U,
/*02a5*/	0x10020272U,
/*02a6*/	0x18080272U,
/*02a7*/	0x00200273U,
/*02a8*/	0x00200274U,
/*02a9*/	0x00100275U,
/*02aa*/	0x10020275U,
/*02ab*/	0x18010275U,
/*02ac*/	0x00020276U,
/*02ad*/	0x08080276U,
/*02ae*/	0x10080276U,
/*02af*/	0x18080276U,
/*02b0*/	0x00080277U,
/*02b1*/	0x08080277U,
/*02b2*/	0x10080277U,
/*02b3*/	0x18080277U,
/*02b4*/	0x00080278U,
/*02b5*/	0x08080278U,
/*02b6*/	0x10080278U,
/*02b7*/	0x18080278U,
/*02b8*/	0x00080279U,
/*02b9*/	0x08080279U,
/*02ba*/	0x10080279U,
/*02bb*/	0x18080279U,
/*02bc*/	0x0008027aU,
/*02bd*/	0x0808027aU,
/*02be*/	0x1008027aU,
/*02bf*/	0x1808027aU,
/*02c0*/	0x0008027bU,
/*02c1*/	0x0808027bU,
/*02c2*/	0x1008027bU,
/*02c3*/	0x1808027bU,
/*02c4*/	0x0008027cU,
/*02c5*/	0x0808027cU,
/*02c6*/	0x1008027cU,
/*02c7*/	0x1808027cU,
/*02c8*/	0x0008027dU,
/*02c9*/	0x0808027dU,
/*02ca*/	0x1008027dU,
/*02cb*/	0x1808027dU,
/*02cc*/	0x0008027eU,
/*02cd*/	0x0808027eU,
/*02ce*/	0x1008027eU,
/*02cf*/	0x1808027eU,
/*02d0*/	0x0008027fU,
/*02d1*/	0x0808027fU,
/*02d2*/	0x1008027fU,
/*02d3*/	0x1808027fU,
/*02d4*/	0x00080280U,
/*02d5*/	0x08080280U,
/*02d6*/	0x10080280U,
/*02d7*/	0x18080280U,
/*02d8*/	0x00080281U,
/*02d9*/	0x08080281U,
/*02da*/	0x10080281U,
/*02db*/	0x18080281U,
/*02dc*/	0x00080282U,
/*02dd*/	0x08080282U,
/*02de*/	0x10080282U,
/*02df*/	0x18080282U,
/*02e0*/	0x00080283U,
/*02e1*/	0x08080283U,
/*02e2*/	0x10080283U,
/*02e3*/	0x18080283U,
/*02e4*/	0x00080284U,
/*02e5*/	0x08080284U,
/*02e6*/	0x10080284U,
/*02e7*/	0x18080284U,
/*02e8*/	0x00080285U,
/*02e9*/	0x08080285U,
/*02ea*/	0x10080285U,
/*02eb*/	0x18080285U,
/*02ec*/	0x00080286U,
/*02ed*/	0x08080286U,
/*02ee*/	0x10080286U,
/*02ef*/	0x18080286U,
/*02f0*/	0x00080287U,
/*02f1*/	0x08080287U,
/*02f2*/	0x10080287U,
/*02f3*/	0x18080287U,
/*02f4*/	0x00080288U,
/*02f5*/	0x08080288U,
/*02f6*/	0x10080288U,
/*02f7*/	0x18080288U,
/*02f8*/	0x00080289U,
/*02f9*/	0x08020289U,
/*02fa*/	0x10030289U,
/*02fb*/	0x000a028aU,
/*02fc*/	0x100a028aU,
/*02fd*/	0x000a028bU,
/*02fe*/	0x1005028bU,
/*02ff*/	0x1804028bU,
/*0300*/	0x0008028cU,
/*0301*/	0x0808028cU,
/*0302*/	0x1006028cU,
/*0303*/	0x1806028cU,
/*0304*/	0x0011028dU,
/*0305*/	0x1808028dU,
/*0306*/	0x0004028eU,
/*0307*/	0x0806028eU,
/*0308*/	0x1006028eU,
/*0309*/	0x1808028eU,
/*030a*/	0xffffffffU,
/*030b*/	0x0004028fU,
/*030c*/	0x0808028fU,
/*030d*/	0x1008028fU,
/*030e*/	0x1806028fU,
/*030f*/	0x00060290U,
/*0310*/	0x08110290U,
/*0311*/	0x00080291U,
/*0312*/	0x08040291U,
/*0313*/	0x10060291U,
/*0314*/	0x18060291U,
/*0315*/	0x00080292U,
/*0316*/	0xffffffffU,
/*0317*/	0x08040292U,
/*0318*/	0x10080292U,
/*0319*/	0x18080292U,
/*031a*/	0x00060293U,
/*031b*/	0x08060293U,
/*031c*/	0x00110294U,
/*031d*/	0x18080294U,
/*031e*/	0x00040295U,
/*031f*/	0x08060295U,
/*0320*/	0x10060295U,
/*0321*/	0x18080295U,
/*0322*/	0xffffffffU,
/*0323*/	0x00040296U,
/*0324*/	0x08040296U,
/*0325*/	0x10040296U,
/*0326*/	0x18040296U,
/*0327*/	0x00040297U,
/*0328*/	0x08040297U,
/*0329*/	0x10040297U,
/*032a*/	0x18040297U,
/*032b*/	0x00040298U,
/*032c*/	0x08040298U,
/*032d*/	0x10040298U,
/*032e*/	0x18040298U,
/*032f*/	0x00040299U,
/*0330*/	0x08040299U,
/*0331*/	0x10040299U,
/*0332*/	0x18040299U,
/*0333*/	0x0004029aU,
/*0334*/	0x0804029aU,
/*0335*/	0x1004029aU,
/*0336*/	0x1804029aU,
/*0337*/	0x0011029bU,
/*0338*/	0x0010029cU,
/*0339*/	0x0011029dU,
/*033a*/	0x0020029eU,
/*033b*/	0x0020029fU,
/*033c*/	0x002002a0U,
/*033d*/	0x002002a1U,
/*033e*/	0x002002a2U,
/*033f*/	0x002002a3U,
/*0340*/	0x002002a4U,
/*0341*/	0x002002a5U,
/*0342*/	0x002002a6U,
/*0343*/	0x000202a7U,
/*0344*/	0x080502a7U,
/*0345*/	0x100502a7U,
/*0346*/	0xffffffffU,
/*0347*/	0xffffffffU,
/*0348*/	0xffffffffU,
/*0349*/	0xffffffffU,
/*034a*/	0xffffffffU,
/*034b*/	0xffffffffU,
/*034c*/	0xffffffffU,
/*034d*/	0xffffffffU,
/*034e*/	0xffffffffU,
/*034f*/	0xffffffffU,
/*0350*/	0xffffffffU,
/*0351*/	0xffffffffU,
/*0352*/	0xffffffffU,
/*0353*/	0xffffffffU,
/*0354*/	0xffffffffU,
/*0355*/	0xffffffffU,
/*0356*/	0xffffffffU,
/*0357*/	0xffffffffU,
/*0358*/	0xffffffffU,
/*0359*/	0xffffffffU,
/*035a*/	0xffffffffU,
/*035b*/	0xffffffffU,
/*035c*/	0xffffffffU,
/*035d*/	0xffffffffU,
/*035e*/	0xffffffffU,
/*035f*/	0xffffffffU,
/*0360*/	0xffffffffU,
/*0361*/	0xffffffffU,
/*0362*/	0x180102a7U,
/*0363*/	0x000402a8U,
/*0364*/	0x081002a8U,
/*0365*/	0x002002a9U,
/*0366*/	0x001002aaU,
/*0367*/	0x002002abU,
/*0368*/	0x001002acU,
/*0369*/	0x002002adU,
/*036a*/	0x000702aeU,
/*036b*/	0x080102aeU,
/*036c*/	0x100202aeU,
/*036d*/	0x180602aeU,
/*036e*/	0x000102afU,
/*036f*/	0x080102afU,
/*0370*/	0x002002b0U,
/*0371*/	0x000202b1U,
/*0372*/	0x002002b2U,
/*0373*/	0x002002b3U,
/*0374*/	0x000402b4U,
	},
	{
/*0000*/	0x00200800U,
/*0001*/	0x00040801U,
/*0002*/	0x080b0801U,
/*0003*/	0x18010801U,
/*0004*/	0x00050802U,
/*0005*/	0x08050802U,
/*0006*/	0x10050802U,
/*0007*/	0x18050802U,
/*0008*/	0x00050803U,
/*0009*/	0x08050803U,
/*000a*/	0x10050803U,
/*000b*/	0x18050803U,
/*000c*/	0x00050804U,
/*000d*/	0x08040804U,
/*000e*/	0x10030804U,
/*000f*/	0x00180805U,
/*0010*/	0x18030805U,
/*0011*/	0x00180806U,
/*0012*/	0x18020806U,
/*0013*/	0x00010807U,
/*0014*/	0x08020807U,
/*0015*/	0x10010807U,
/*0016*/	0x18010807U,
/*0017*/	0x00020808U,
/*0018*/	0x08040808U,
/*0019*/	0x10040808U,
/*001a*/	0x18040808U,
/*001b*/	0x000a0809U,
/*001c*/	0x10040809U,
/*001d*/	0x18070809U,
/*001e*/	0x000a080aU,
/*001f*/	0x1005080aU,
/*0020*/	0x1801080aU,
/*0021*/	0x0001080bU,
/*0022*/	0x0802080bU,
/*0023*/	0x1009080bU,
/*0024*/	0x0009080cU,
/*0025*/	0x1002080cU,
/*0026*/	0x0020080dU,
/*0027*/	0x0001080eU,
/*0028*/	0x0020080fU,
/*0029*/	0x00200810U,
/*002a*/	0x00200811U,
/*002b*/	0x00200812U,
/*002c*/	0x00030813U,
/*002d*/	0x08010813U,
/*002e*/	0x10030813U,
/*002f*/	0x18030813U,
/*0030*/	0x00040814U,
/*0031*/	0x08040814U,
/*0032*/	0x10040814U,
/*0033*/	0x18040814U,
/*0034*/	0x00010815U,
/*0035*/	0x08010815U,
/*0036*/	0x10060815U,
/*0037*/	0x18040815U,
/*0038*/	0x00060816U,
/*0039*/	0x08040816U,
/*003a*/	0x10060816U,
/*003b*/	0x18040816U,
/*003c*/	0x00020817U,
/*003d*/	0x08050817U,
/*003e*/	0x10080817U,
/*003f*/	0x00200818U,
/*0040*/	0x00060819U,
/*0041*/	0x08030819U,
/*0042*/	0x100b0819U,
/*0043*/	0x0004081aU,
/*0044*/	0x0804081aU,
/*0045*/	0x1004081aU,
/*0046*/	0x1801081aU,
/*0047*/	0x0009081bU,
/*0048*/	0x0020081cU,
/*0049*/	0x0020081dU,
/*004a*/	0x0020081eU,
/*004b*/	0x0020081fU,
/*004c*/	0x00100820U,
/*004d*/	0xffffffffU,
/*004e*/	0x10010820U,
/*004f*/	0x18060820U,
/*0050*/	0x00080821U,
/*0051*/	0x00200822U,
/*0052*/	0x000a0823U,
/*0053*/	0x10060823U,
/*0054*/	0x18070823U,
/*0055*/	0x00080824U,
/*0056*/	0x08080824U,
/*0057*/	0x100a0824U,
/*0058*/	0x00070825U,
/*0059*/	0x08080825U,
/*005a*/	0x10080825U,
/*005b*/	0x18030825U,
/*005c*/	0x000a0826U,
/*005d*/	0x100a0826U,
/*005e*/	0x00110827U,
/*005f*/	0x00090828U,
/*0060*/	0x10090828U,
/*0061*/	0x00100829U,
/*0062*/	0x100e0829U,
/*0063*/	0x000e082aU,
/*0064*/	0x100c082aU,
/*0065*/	0x000a082bU,
/*0066*/	0x100a082bU,
/*0067*/	0x0002082cU,
/*0068*/	0x0020082dU,
/*0069*/	0x000b082eU,
/*006a*/	0x100b082eU,
/*006b*/	0x0020082fU,
/*006c*/	0x00120830U,
/*006d*/	0x00200831U,
/*006e*/	0x00200832U,
/*006f*/	0x00010833U,
/*0070*/	0x08010833U,
/*0071*/	0x10080833U,
/*0072*/	0x000c0834U,
/*0073*/	0x100c0834U,
/*0074*/	0x000c0835U,
/*0075*/	0x100c0835U,
/*0076*/	0x000c0836U,
/*0077*/	0x100c0836U,
/*0078*/	0x000c0837U,
/*0079*/	0x100c0837U,
/*007a*/	0x000c0838U,
/*007b*/	0x100c0838U,
/*007c*/	0x000c0839U,
/*007d*/	0x100b0839U,
/*007e*/	0x000b083aU,
/*007f*/	0x100b083aU,
/*0080*/	0x000b083bU,
/*0081*/	0x100b083bU,
/*0082*/	0x000b083cU,
/*0083*/	0x100b083cU,
/*0084*/	0x000b083dU,
/*0085*/	0x100b083dU,
/*0086*/	0x000b083eU,
/*0087*/	0x100a083eU,
/*0088*/	0x000a083fU,
/*0089*/	0x100a083fU,
/*008a*/	0x000a0840U,
/*008b*/	0x100a0840U,
/*008c*/	0x000a0841U,
/*008d*/	0x100a0841U,
/*008e*/	0x000a0842U,
/*008f*/	0x100a0842U,
/*0090*/	0x000a0843U,
/*0091*/	0x100a0843U,
/*0092*/	0x000a0844U,
/*0093*/	0x100a0844U,
/*0094*/	0x000a0845U,
/*0095*/	0x100a0845U,
/*0096*/	0x000a0846U,
/*0097*/	0x100a0846U,
/*0098*/	0x000a0847U,
/*0099*/	0x100a0847U,
/*009a*/	0x000a0848U,
/*009b*/	0x100a0848U,
/*009c*/	0x000a0849U,
/*009d*/	0x100a0849U,
/*009e*/	0x000a084aU,
/*009f*/	0x100a084aU,
/*00a0*/	0x000a084bU,
/*00a1*/	0x100a084bU,
/*00a2*/	0x000a084cU,
/*00a3*/	0x100a084cU,
/*00a4*/	0x0004084dU,
/*00a5*/	0x0803084dU,
/*00a6*/	0x100a084dU,
/*00a7*/	0x000a084eU,
/*00a8*/	0x1001084eU,
/*00a9*/	0x000a084fU,
/*00aa*/	0x1004084fU,
/*00ab*/	0x000b0850U,
/*00ac*/	0x100a0850U,
/*00ad*/	0x00080851U,
/*00ae*/	0x08080851U,
/*00af*/	0x10080851U,
/*00b0*/	0x18080851U,
/*00b1*/	0x00080852U,
/*00b2*/	0x08080852U,
/*00b3*/	0x10010852U,
/*00b4*/	0x18080852U,
/*00b5*/	0x00080853U,
/*00b6*/	0x08020853U,
/*00b7*/	0x10020853U,
/*00b8*/	0x18040853U,
/*00b9*/	0x00040854U,
/*00ba*/	0x08040854U,
/*00bb*/	0x100a0854U,
/*00bc*/	0x00060855U,
/*00bd*/	0x08080855U,
/*00be*/	0x10040855U,
/*00bf*/	0x18040855U,
/*00c0*/	0x00050856U,
/*00c1*/	0x08040856U,
/*00c2*/	0x10050856U,
/*00c3*/	0x000a0857U,
/*00c4*/	0x100a0857U,
/*00c5*/	0x00080858U,
/*00c6*/	0x08040858U,
/*00c7*/	0x00050a00U,
/*00c8*/	0x08050a00U,
/*00c9*/	0x10050a00U,
/*00ca*/	0x18050a00U,
/*00cb*/	0x00050a01U,
/*00cc*/	0x08050a01U,
/*00cd*/	0x100b0a01U,
/*00ce*/	0x00010a02U,
/*00cf*/	0x08030a02U,
/*00d0*/	0x00200a03U,
/*00d1*/	0x00030a04U,
/*00d2*/	0x080a0a04U,
/*00d3*/	0x18030a04U,
/*00d4*/	0x00030a05U,
/*00d5*/	0x08010a05U,
/*00d6*/	0x10010a05U,
/*00d7*/	0x18060a05U,
/*00d8*/	0x00020a06U,
/*00d9*/	0x08030a06U,
/*00da*/	0x10010a06U,
/*00db*/	0x000f0a07U,
/*00dc*/	0x00200a08U,
/*00dd*/	0x00200a09U,
/*00de*/	0x000b0a0aU,
/*00df*/	0x100b0a0aU,
/*00e0*/	0x000b0a0bU,
/*00e1*/	0x00180a0cU,
/*00e2*/	0x00180a0dU,
/*00e3*/	0x00180a0eU,
/*00e4*/	0x00180a0fU,
/*00e5*/	0x18040a0fU,
/*00e6*/	0x00020a10U,
/*00e7*/	0x08020a10U,
/*00e8*/	0x10040a10U,
/*00e9*/	0x18040a10U,
/*00ea*/	0x00010a11U,
/*00eb*/	0x08010a11U,
/*00ec*/	0x10010a11U,
/*00ed*/	0x18030a11U,
/*00ee*/	0x00200a12U,
/*00ef*/	0x00200a13U,
/*00f0*/	0xffffffffU,
/*00f1*/	0x00140a14U,
/*00f2*/	0x00140a15U,
/*00f3*/	0x00140a16U,
/*00f4*/	0x00140a17U,
/*00f5*/	0x00140a18U,
/*00f6*/	0x00140a19U,
/*00f7*/	0x00140a1aU,
/*00f8*/	0x00140a1bU,
/*00f9*/	0x001e0a1cU,
/*00fa*/	0x000a0a1dU,
/*00fb*/	0x10060a1dU,
/*00fc*/	0x18060a1dU,
/*00fd*/	0x00060a1eU,
/*00fe*/	0x08060a1eU,
/*00ff*/	0x00080a1fU,
/*0100*/	0x080b0a1fU,
/*0101*/	0x000b0a20U,
/*0102*/	0x100b0a20U,
/*0103*/	0x000b0a21U,
/*0104*/	0x100b0a21U,
/*0105*/	0x000b0a22U,
/*0106*/	0x10040a22U,
/*0107*/	0x000a0a23U,
/*0108*/	0x10060a23U,
/*0109*/	0x18080a23U,
/*010a*/	0x00040a24U,
/*010b*/	0x00010b80U,
/*010c*/	0x08020b80U,
/*010d*/	0x10050b80U,
/*010e*/	0x18050b80U,
/*010f*/	0x00050b81U,
/*0110*/	0x08050b81U,
/*0111*/	0x100b0b81U,
/*0112*/	0x00050b82U,
/*0113*/	0x08010b82U,
/*0114*/	0x10010b82U,
/*0115*/	0x18010b82U,
/*0116*/	0x00010b83U,
/*0117*/	0x08040b83U,
/*0118*/	0x100b0b83U,
/*0119*/	0x000b0b84U,
/*011a*/	0x10040b84U,
/*011b*/	0x000b0b85U,
/*011c*/	0x10040b85U,
/*011d*/	0x18010b85U,
/*011e*/	0x00010b86U,
/*011f*/	0x08010b86U,
/*0120*/	0x00200b87U,
/*0121*/	0x00200b88U,
/*0122*/	0x00080b89U,
/*0123*/	0x080a0b89U,
/*0124*/	0x18050b89U,
/*0125*/	0x000a0b8aU,
/*0126*/	0x10030b8aU,
/*0127*/	0x18030b8aU,
/*0128*/	0x00010b8bU,
/*0129*/	0x08020b8bU,
/*012a*/	0x10010b8bU,
/*012b*/	0x18010b8bU,
/*012c*/	0x00010b8cU,
/*012d*/	0x08030b8cU,
/*012e*/	0x10040b8cU,
/*012f*/	0x18040b8cU,
/*0130*/	0x00040b8dU,
/*0131*/	0x08040b8dU,
/*0132*/	0xffffffffU,
/*0133*/	0xffffffffU,
/*0134*/	0x000d0b8eU,
/*0135*/	0x100d0b8eU,
/*0136*/	0x000d0b8fU,
/*0137*/	0x00050b90U,
/*0138*/	0x00010b91U,
/*0139*/	0x080e0b91U,
/*013a*/	0x000e0b92U,
/*013b*/	0x100e0b92U,
/*013c*/	0x000e0b93U,
/*013d*/	0x100e0b93U,
/*013e*/	0x00040b94U,
/*013f*/	0x08040b94U,
/*0140*/	0x10040b94U,
/*0141*/	0x18040b94U,
/*0142*/	0x00040b95U,
/*0143*/	0x080b0b95U,
/*0144*/	0x000b0b96U,
/*0145*/	0x100b0b96U,
/*0146*/	0x000b0b97U,
/*0147*/	0x000d0b98U,
/*0148*/	0x100d0b98U,
/*0149*/	0x000d0b99U,
/*014a*/	0x10100b99U,
/*014b*/	0x10080b8dU,
/*014c*/	0x18080b8dU,
/*014d*/	0x00100b9aU,
/*014e*/	0x10100b9aU,
/*014f*/	0x00100b9bU,
/*0150*/	0x10100b9bU,
/*0151*/	0x00100b9cU,
/*0152*/	0x10030b9cU,
/*0153*/	0x18040b9cU,
/*0154*/	0x00010b9dU,
/*0155*/	0x08040b9dU,
/*0156*/	0x10010b9dU,
/*0157*/	0x00140b9eU,
/*0158*/	0x000a0b9fU,
/*0159*/	0x100c0b9fU,
/*015a*/	0x00120ba0U,
/*015b*/	0x00140ba1U,
/*015c*/	0x00120ba2U,
/*015d*/	0x00110ba3U,
/*015e*/	0x00110ba4U,
/*015f*/	0x00120ba5U,
/*0160*/	0x00120ba6U,
/*0161*/	0x00120ba7U,
/*0162*/	0x00120ba8U,
/*0163*/	0x00120ba9U,
/*0164*/	0x00120baaU,
/*0165*/	0x00120babU,
/*0166*/	0x00120bacU,
/*0167*/	0x00190badU,
/*0168*/	0x00190baeU,
/*0169*/	0x00200bafU,
/*016a*/	0x00170bb0U,
/*016b*/	0x18080bb0U,
/*016c*/	0x00010bb1U,
/*016d*/	0x08010bb1U,
/*016e*/	0x00200bb2U,
/*016f*/	0x00080bb3U,
/*0170*/	0x08030bb3U,
/*0171*/	0x00180bb4U,
/*0172*/	0x00180bb5U,
/*0173*/	0x00100bb6U,
/*0174*/	0x10010bb6U,
/*0175*/	0x18010bb6U,
/*0176*/	0x00050bb7U,
/*0177*/	0x00200bb8U,
/*0178*/	0x00090bb9U,
/*0179*/	0x00200bbaU,
/*017a*/	0x00040bbbU,
/*017b*/	0x08100bbbU,
/*017c*/	0x18060bbbU,
/*017d*/	0x00100bbcU,
/*017e*/	0x10080bbcU,
/*017f*/	0x18030bbcU,
/*0180*/	0x00020bbdU,
/*0181*/	0x00200bbeU,
/*0182*/	0x000b0bbfU,
/*0183*/	0x10020bbfU,
/*0184*/	0x00010200U,
/*0185*/	0x08040200U,
/*0186*/	0x10100200U,
/*0187*/	0x00010201U,
/*0188*/	0x08010201U,
/*0189*/	0x10100201U,
/*018a*/	0x00200202U,
/*018b*/	0x00100203U,
/*018c*/	0x00200204U,
/*018d*/	0x00100205U,
/*018e*/	0x00200206U,
/*018f*/	0x00100207U,
/*0190*/	0x10100207U,
/*0191*/	0x00200208U,
/*0192*/	0x00200209U,
/*0193*/	0x0020020aU,
/*0194*/	0x0020020bU,
/*0195*/	0x0010020cU,
/*0196*/	0x0020020dU,
/*0197*/	0x0020020eU,
/*0198*/	0x0020020fU,
/*0199*/	0x00200210U,
/*019a*/	0x00100211U,
/*019b*/	0x00200212U,
/*019c*/	0x00200213U,
/*019d*/	0x00200214U,
/*019e*/	0x00200215U,
/*019f*/	0x00090216U,
/*01a0*/	0x10010216U,
/*01a1*/	0x00200217U,
/*01a2*/	0x00050218U,
/*01a3*/	0x08010218U,
/*01a4*/	0x10080218U,
/*01a5*/	0x18080218U,
/*01a6*/	0x001e0219U,
/*01a7*/	0x001e021aU,
/*01a8*/	0x001e021bU,
/*01a9*/	0x001e021cU,
/*01aa*/	0x001e021dU,
/*01ab*/	0x001e021eU,
/*01ac*/	0x001e021fU,
/*01ad*/	0x001e0220U,
/*01ae*/	0x001e0221U,
/*01af*/	0x001e0222U,
/*01b0*/	0x001e0223U,
/*01b1*/	0x001e0224U,
/*01b2*/	0x001e0225U,
/*01b3*/	0x001e0226U,
/*01b4*/	0x001e0227U,
/*01b5*/	0x001e0228U,
/*01b6*/	0x00010229U,
/*01b7*/	0x08010229U,
/*01b8*/	0x10010229U,
/*01b9*/	0x18040229U,
/*01ba*/	0x0008022aU,
/*01bb*/	0x0808022aU,
/*01bc*/	0x1008022aU,
/*01bd*/	0x1804022aU,
/*01be*/	0x0005022bU,
/*01bf*/	0x0806022bU,
/*01c0*/	0x1007022bU,
/*01c1*/	0x1805022bU,
/*01c2*/	0x0006022cU,
/*01c3*/	0x0807022cU,
/*01c4*/	0x1005022cU,
/*01c5*/	0x1806022cU,
/*01c6*/	0x0007022dU,
/*01c7*/	0x0802022dU,
/*01c8*/	0x1001022dU,
/*01c9*/	0x000a022eU,
/*01ca*/	0x1010022eU,
/*01cb*/	0x000a022fU,
/*01cc*/	0x1010022fU,
/*01cd*/	0x000a0230U,
/*01ce*/	0x10100230U,
/*01cf*/	0x00100231U,
/*01d0*/	0x10010231U,
/*01d1*/	0x18010231U,
/*01d2*/	0x00010232U,
/*01d3*/	0x08010232U,
/*01d4*/	0x10010232U,
/*01d5*/	0x18010232U,
/*01d6*/	0x00020233U,
/*01d7*/	0x08020233U,
/*01d8*/	0x10020233U,
/*01d9*/	0x18020233U,
/*01da*/	0x00020234U,
/*01db*/	0x08030234U,
/*01dc*/	0x10010234U,
/*01dd*/	0x18010234U,
/*01de*/	0x00010235U,
/*01df*/	0x08010235U,
/*01e0*/	0x10020235U,
/*01e1*/	0x18010235U,
/*01e2*/	0x00010236U,
/*01e3*/	0x08020236U,
/*01e4*/	0x10010236U,
/*01e5*/	0x18010236U,
/*01e6*/	0x00020237U,
/*01e7*/	0x08010237U,
/*01e8*/	0x10010237U,
/*01e9*/	0x18020237U,
/*01ea*/	0x00070238U,
/*01eb*/	0x08010238U,
/*01ec*/	0x10010238U,
/*01ed*/	0x18010238U,
/*01ee*/	0x00010239U,
/*01ef*/	0x08010239U,
/*01f0*/	0x10010239U,
/*01f1*/	0x18010239U,
/*01f2*/	0x0004023aU,
/*01f3*/	0x0804023aU,
/*01f4*/	0x1004023aU,
/*01f5*/	0x1801023aU,
/*01f6*/	0x0002023bU,
/*01f7*/	0x0806023bU,
/*01f8*/	0x1006023bU,
/*01f9*/	0x1802023bU,
/*01fa*/	0x0010023cU,
/*01fb*/	0x1001023cU,
/*01fc*/	0x1801023cU,
/*01fd*/	0x0004023dU,
/*01fe*/	0x0801023dU,
/*01ff*/	0x1004023dU,
/*0200*/	0x1802023dU,
/*0201*/	0x0008023eU,
/*0202*/	0x080a023eU,
/*0203*/	0x0020023fU,
/*0204*/	0x00200240U,
/*0205*/	0x00050241U,
/*0206*/	0x08010241U,
/*0207*/	0x10050241U,
/*0208*/	0x18080241U,
/*0209*/	0x00010242U,
/*020a*/	0x08080242U,
/*020b*/	0x10010242U,
/*020c*/	0x18080242U,
/*020d*/	0x00010243U,
/*020e*/	0x08040243U,
/*020f*/	0x10040243U,
/*0210*/	0x18040243U,
/*0211*/	0x00040244U,
/*0212*/	0x08040244U,
/*0213*/	0x10040244U,
/*0214*/	0x18040244U,
/*0215*/	0x00040245U,
/*0216*/	0x08040245U,
/*0217*/	0x10040245U,
/*0218*/	0x18010245U,
/*0219*/	0x00040246U,
/*021a*/	0x08040246U,
/*021b*/	0x10040246U,
/*021c*/	0x18040246U,
/*021d*/	0x00040247U,
/*021e*/	0x08040247U,
/*021f*/	0x10060247U,
/*0220*/	0x18060247U,
/*0221*/	0x00060248U,
/*0222*/	0x08060248U,
/*0223*/	0x10060248U,
/*0224*/	0x18060248U,
/*0225*/	0x00040249U,
/*0226*/	0x08010249U,
/*0227*/	0x10010249U,
/*0228*/	0x18020249U,
/*0229*/	0x0004024aU,
/*022a*/	0x0804024aU,
/*022b*/	0x1001024aU,
/*022c*/	0x1801024aU,
/*022d*/	0x0001024bU,
/*022e*/	0x0801024bU,
/*022f*/	0x1001024bU,
/*0230*/	0x1801024bU,
/*0231*/	0x0001024cU,
/*0232*/	0x0804024cU,
/*0233*/	0x1004024cU,
/*0234*/	0x000a024dU,
/*0235*/	0x0020024eU,
/*0236*/	0x0004024fU,
/*0237*/	0x0808024fU,
/*0238*/	0x1002024fU,
/*0239*/	0x1802024fU,
/*023a*/	0x00200250U,
/*023b*/	0x00020251U,
/*023c*/	0x08100251U,
/*023d*/	0x00100252U,
/*023e*/	0x10040252U,
/*023f*/	0x18040252U,
/*0240*/	0x00050253U,
/*0241*/	0x08050253U,
/*0242*/	0x10010253U,
/*0243*/	0x18010253U,
/*0244*/	0x00080254U,
/*0245*/	0x08080254U,
/*0246*/	0x10080254U,
/*0247*/	0x18080254U,
/*0248*/	0x00080255U,
/*0249*/	0x08080255U,
/*024a*/	0x10080255U,
/*024b*/	0x18080255U,
/*024c*/	0x00080256U,
/*024d*/	0x08080256U,
/*024e*/	0x10080256U,
/*024f*/	0x18030256U,
/*0250*/	0x00010257U,
/*0251*/	0x08020257U,
/*0252*/	0x10010257U,
/*0253*/	0x18040257U,
/*0254*/	0x00020258U,
/*0255*/	0x08010258U,
/*0256*/	0x10010258U,
/*0257*/	0x18010258U,
/*0258*/	0x00040259U,
/*0259*/	0x08080259U,
/*025a*/	0x100a0259U,
/*025b*/	0x000a025aU,
/*025c*/	0x100a025aU,
/*025d*/	0x000a025bU,
/*025e*/	0x100a025bU,
/*025f*/	0x000a025cU,
/*0260*/	0x0020025dU,
/*0261*/	0x0020025eU,
/*0262*/	0x0001025fU,
/*0263*/	0x0802025fU,
/*0264*/	0x1002025fU,
/*0265*/	0x00100260U,
/*0266*/	0x10050260U,
/*0267*/	0x18060260U,
/*0268*/	0x00050261U,
/*0269*/	0x08050261U,
/*026a*/	0x100e0261U,
/*026b*/	0x00050262U,
/*026c*/	0x080e0262U,
/*026d*/	0x18050262U,
/*026e*/	0x000e0263U,
/*026f*/	0x10050263U,
/*0270*/	0x18010263U,
/*0271*/	0x00050264U,
/*0272*/	0x08050264U,
/*0273*/	0x100a0264U,
/*0274*/	0x000a0265U,
/*0275*/	0x10050265U,
/*0276*/	0x18050265U,
/*0277*/	0x000a0266U,
/*0278*/	0x100a0266U,
/*0279*/	0x00050267U,
/*027a*/	0x08050267U,
/*027b*/	0x100a0267U,
/*027c*/	0x000a0268U,
/*027d*/	0x10070268U,
/*027e*/	0x18070268U,
/*027f*/	0x00040269U,
/*0280*/	0x08040269U,
/*0281*/	0x10040269U,
/*0282*/	0x18080269U,
/*0283*/	0x0008026aU,
/*0284*/	0x0804026aU,
/*0285*/	0x1004026aU,
/*0286*/	0x1804026aU,
/*0287*/	0x0004026bU,
/*0288*/	0x0805026bU,
/*0289*/	0x1007026bU,
/*028a*/	0x1808026bU,
/*028b*/	0x0010026cU,
/*028c*/	0x1008026cU,
/*028d*/	0x0010026dU,
/*028e*/	0x1008026dU,
/*028f*/	0x0010026eU,
/*0290*/	0x1008026eU,
/*0291*/	0x1808026eU,
/*0292*/	0x0001026fU,
/*0293*/	0x0801026fU,
/*0294*/	0x1006026fU,
/*0295*/	0x1806026fU,
/*0296*/	0x00060270U,
/*0297*/	0x08010270U,
/*0298*/	0x10030270U,
/*0299*/	0x000a0271U,
/*029a*/	0x100a0271U,
/*029b*/	0x00040272U,
/*029c*/	0x08010272U,
/*029d*/	0x10040272U,
/*029e*/	0x18070272U,
/*029f*/	0x00070273U,
/*02a0*/	0x08050273U,
/*02a1*/	0x10050273U,
/*02a2*/	0x18040273U,
/*02a3*/	0x00010274U,
/*02a4*/	0x08010274U,
/*02a5*/	0x10020274U,
/*02a6*/	0x18080274U,
/*02a7*/	0x00200275U,
/*02a8*/	0x00200276U,
/*02a9*/	0x00100277U,
/*02aa*/	0x10020277U,
/*02ab*/	0x18010277U,
/*02ac*/	0x00020278U,
/*02ad*/	0x08100278U,
/*02ae*/	0x00100279U,
/*02af*/	0x10100279U,
/*02b0*/	0x0008027aU,
/*02b1*/	0x0808027aU,
/*02b2*/	0x1008027aU,
/*02b3*/	0x0010027bU,
/*02b4*/	0x1010027bU,
/*02b5*/	0x0010027cU,
/*02b6*/	0x1008027cU,
/*02b7*/	0x1808027cU,
/*02b8*/	0x0008027dU,
/*02b9*/	0x0810027dU,
/*02ba*/	0x0010027eU,
/*02bb*/	0x1010027eU,
/*02bc*/	0x0008027fU,
/*02bd*/	0x0808027fU,
/*02be*/	0x1008027fU,
/*02bf*/	0x1808027fU,
/*02c0*/	0x00100280U,
/*02c1*/	0x10100280U,
/*02c2*/	0x00100281U,
/*02c3*/	0x10080281U,
/*02c4*/	0x18080281U,
/*02c5*/	0x00080282U,
/*02c6*/	0x08100282U,
/*02c7*/	0x00100283U,
/*02c8*/	0x10100283U,
/*02c9*/	0x00080284U,
/*02ca*/	0x08080284U,
/*02cb*/	0x10080284U,
/*02cc*/	0x00100285U,
/*02cd*/	0x10100285U,
/*02ce*/	0x00100286U,
/*02cf*/	0x10080286U,
/*02d0*/	0x18080286U,
/*02d1*/	0x00080287U,
/*02d2*/	0x08080287U,
/*02d3*/	0x10100287U,
/*02d4*/	0x00100288U,
/*02d5*/	0x10100288U,
/*02d6*/	0x00080289U,
/*02d7*/	0x08080289U,
/*02d8*/	0x10080289U,
/*02d9*/	0x0010028aU,
/*02da*/	0x1010028aU,
/*02db*/	0x0010028bU,
/*02dc*/	0x1008028bU,
/*02dd*/	0x1808028bU,
/*02de*/	0x0008028cU,
/*02df*/	0x0810028cU,
/*02e0*/	0x0010028dU,
/*02e1*/	0x1010028dU,
/*02e2*/	0x0008028eU,
/*02e3*/	0x0808028eU,
/*02e4*/	0x1008028eU,
/*02e5*/	0x1808028eU,
/*02e6*/	0x0010028fU,
/*02e7*/	0x1010028fU,
/*02e8*/	0x00100290U,
/*02e9*/	0x10080290U,
/*02ea*/	0x18080290U,
/*02eb*/	0x00080291U,
/*02ec*/	0x08100291U,
/*02ed*/	0x00100292U,
/*02ee*/	0x10100292U,
/*02ef*/	0x00080293U,
/*02f0*/	0x08080293U,
/*02f1*/	0x10080293U,
/*02f2*/	0x00100294U,
/*02f3*/	0x10100294U,
/*02f4*/	0x00100295U,
/*02f5*/	0x10080295U,
/*02f6*/	0x18080295U,
/*02f7*/	0x00080296U,
/*02f8*/	0x08080296U,
/*02f9*/	0x10020296U,
/*02fa*/	0x18030296U,
/*02fb*/	0x000a0297U,
/*02fc*/	0x100a0297U,
/*02fd*/	0x000a0298U,
/*02fe*/	0x10050298U,
/*02ff*/	0x18040298U,
/*0300*/	0x00080299U,
/*0301*/	0x08080299U,
/*0302*/	0x10060299U,
/*0303*/	0x18060299U,
/*0304*/	0x0011029aU,
/*0305*/	0x1808029aU,
/*0306*/	0x0004029bU,
/*0307*/	0x0806029bU,
/*0308*/	0x1006029bU,
/*0309*/	0x1808029bU,
/*030a*/	0x0008029cU,
/*030b*/	0x0804029cU,
/*030c*/	0x1008029cU,
/*030d*/	0x1808029cU,
/*030e*/	0x0006029dU,
/*030f*/	0x0806029dU,
/*0310*/	0x0011029eU,
/*0311*/	0x1808029eU,
/*0312*/	0x0004029fU,
/*0313*/	0x0806029fU,
/*0314*/	0x1006029fU,
/*0315*/	0x1808029fU,
/*0316*/	0x000802a0U,
/*0317*/	0x080402a0U,
/*0318*/	0x100802a0U,
/*0319*/	0x180802a0U,
/*031a*/	0x000602a1U,
/*031b*/	0x080602a1U,
/*031c*/	0x001102a2U,
/*031d*/	0x180802a2U,
/*031e*/	0x000402a3U,
/*031f*/	0x080602a3U,
/*0320*/	0x100602a3U,
/*0321*/	0x180802a3U,
/*0322*/	0x000802a4U,
/*0323*/	0x080402a4U,
/*0324*/	0x100402a4U,
/*0325*/	0x180402a4U,
/*0326*/	0x000402a5U,
/*0327*/	0x080402a5U,
/*0328*/	0x100402a5U,
/*0329*/	0x180402a5U,
/*032a*/	0x000402a6U,
/*032b*/	0x080402a6U,
/*032c*/	0x100402a6U,
/*032d*/	0x180402a6U,
/*032e*/	0x000402a7U,
/*032f*/	0x080402a7U,
/*0330*/	0x100402a7U,
/*0331*/	0x180402a7U,
/*0332*/	0x000402a8U,
/*0333*/	0x080402a8U,
/*0334*/	0x100402a8U,
/*0335*/	0x180402a8U,
/*0336*/	0x000402a9U,
/*0337*/	0x081202a9U,
/*0338*/	0x001102aaU,
/*0339*/	0x001202abU,
/*033a*/	0x002002acU,
/*033b*/	0x002002adU,
/*033c*/	0x002002aeU,
/*033d*/	0x002002afU,
/*033e*/	0x002002b0U,
/*033f*/	0x002002b1U,
/*0340*/	0x002002b2U,
/*0341*/	0x002002b3U,
/*0342*/	0x002002b4U,
/*0343*/	0x000302b5U,
/*0344*/	0x080502b5U,
/*0345*/	0x100502b5U,
/*0346*/	0x180102b5U,
/*0347*/	0x000502b6U,
/*0348*/	0x080502b6U,
/*0349*/	0x100502b6U,
/*034a*/	0x180502b6U,
/*034b*/	0x000502b7U,
/*034c*/	0x080502b7U,
/*034d*/	0x100502b7U,
/*034e*/	0x180502b7U,
/*034f*/	0x000502b8U,
/*0350*/	0x080502b8U,
/*0351*/	0x100502b8U,
/*0352*/	0x180502b8U,
/*0353*/	0x000502b9U,
/*0354*/	0x080502b9U,
/*0355*/	0x100502b9U,
/*0356*/	0x180502b9U,
/*0357*/	0x000502baU,
/*0358*/	0x080502baU,
/*0359*/	0x100502baU,
/*035a*/	0x180502baU,
/*035b*/	0x000502bbU,
/*035c*/	0x080502bbU,
/*035d*/	0x100102bbU,
/*035e*/	0x180202bbU,
/*035f*/	0x000202bcU,
/*0360*/	0x080202bcU,
/*0361*/	0x100202bcU,
/*0362*/	0x180102bcU,
/*0363*/	0x000402bdU,
/*0364*/	0x081002bdU,
/*0365*/	0x002002beU,
/*0366*/	0x001002bfU,
/*0367*/	0x002002c0U,
/*0368*/	0x001002c1U,
/*0369*/	0x002002c2U,
/*036a*/	0x000702c3U,
/*036b*/	0x080102c3U,
/*036c*/	0x100202c3U,
/*036d*/	0x180602c3U,
/*036e*/	0x000102c4U,
/*036f*/	0x080102c4U,
/*0370*/	0x002002c5U,
/*0371*/	0x000302c6U,
/*0372*/	0x002002c7U,
/*0373*/	0x002002c8U,
/*0374*/	0x000402c9U,
	}
};
