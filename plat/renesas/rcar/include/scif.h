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

#ifndef __SCIF_H__
#define __SCIF_H__


#define	SCIF_INTERNAL_CLK	0	/* Internal clock(S3D4:66.66MHz) */
#define	SCIF_EXTARNAL_CLK	1	/* External clock(SCK2:14.7456MHz) */
#define	SCIF_CLK		SCIF_INTERNAL_CLK

/* Product Register */
#define	PRR			(0xFFF00044)
#define	PRR_PRODUCT_MASK	(0x00007FFF)
#define	PRR_PRODUCT_H3_ES_1_0	(0x00004F00)	/* R-Car H3 ES1.0 */

/* Pin functon */
#define	PFC_BASE		(0xE6060000)
#define	PMMR			(0x0000)
#define	GPSR5			(0x0114)
#define	IPSR12			(0x0230)
#define	MOD_SEL1		(0x0504)

/* module stop */
#define	CPG_BASE		(0xE6150000)
#define	CPG_SMSTPCR3		(0x013C)
#define	MSTP310			(1 << 10)
#define	CPG_CPGWPR		(0x0900)

/* SCIF2 */
#define	SCIF2_BASE		(0xE6E88000)	/* SCIF-2 base address */
#define	SCIF_BASE		(SCIF2_BASE)

#define	SCIF_SCSMR		(0x00)		/* Serial mode register */
#define	SCIF_SCBRR		(0x04)		/* Bit rate register */
#define	SCIF_SCSCR		(0x08)		/* Serial control register */
#define	SCIF_SCFTDR		(0x0C)		/* Transmit FIFO data register */
#define	SCIF_SCFSR		(0x10)		/* Serial status register */
#define SCIF_SCFRDR		(0x14)		/* Receive FIFO data register */
#define	SCIF_SCFCR		(0x18)		/* FIFO control register */
#define	SCIF_SCFDR		(0x1C)		/* FIFO data count register */
#define SCIF_SCSPTR		(0x20)		/* Serial port register */
#define	SCIF_SCLSR		(0x24)		/* Line status register */
#define	SCIF_DL			(0x30)		/* Frequency division register */
#define	SCIF_CKS		(0x34)		/* Clock Select register */
#define	SCIF_SCSMRIR		(0x40)		/* Serial mode register */


#define	SCSMR_CA_MASK		(1 << 7)
#define	SCSMR_CA_ASYNC		(0x0000)
#define	SCSMR_CHR_MASK		(1 << 6)
#define	SCSMR_CHR_8		(0x0000)
#define	SCSMR_PE_MASK		(1 << 5)
#define	SCSMR_PE_DIS		(0x0000)
#define	SCSMR_STOP_MASK		(1 << 3)
#define	SCSMR_STOP_1		(0x0000)
#define	SCSMR_CKS_MASK		(3 << 0)
#define	SCSMR_CKS_DIV1		(0x0000)
#define	SCSMR_INIT_DATA		(SCSMR_CA_ASYNC + SCSMR_CHR_8 + \
				SCSMR_PE_DIS + SCSMR_STOP_1 + \
				SCSMR_CKS_DIV1)

#define	SCBRR_115200BPS		(17)		/* 115200bps@66MHz */
#define	SCBRR_230400BPS		(8)		/* 230400bps@66MHz */

#define	SCSCR_TE_MASK		(1 << 5)
#define	SCSCR_TE_DIS		(0x0000)
#define	SCSCR_TE_EN		(0x0020)
#define	SCSCR_RE_MASK		(1 << 4)
#define	SCSCR_RE_DIS		(0x0000)
#define	SCSCR_RE_EN		(0x0010)
#define	SCSCR_CKE_MASK		(3 << 0)
#define SCSCR_CKE_INT		(0x0000)
#define SCSCR_CKE_BRG		(0x0002)
#if SCIF_CLK == SCIF_EXTARNAL_CLK
#define	SCSCR_CKE_INT_CLK	(SCSCR_CKE_BRG)
#else
#define	SCSCR_CKE_INT_CLK	(SCSCR_CKE_INT)
#endif

#define	SCFSR_INIT_DATA		(0x0000)

#define	SCFCR_TTRG_MASK		(3 << 4)
#define	SCFCR_TTRG_8		(0x0000)
#define	SCFCR_TTRG_0		(0x0030)
#define	SCFCR_TFRST_MASK	(1 << 2)
#define	SCFCR_TFRST_DIS		(0x0000)
#define	SCFCR_TFRST_EN		(0x0004)
#define	SCFCR_RFRS_MASK		(1 << 1)
#define	SCFCR_RFRS_DIS		(0x0000)
#define	SCFCR_RFRS_EN		(0x0002)
#define	SCFCR_INIT_DATA		(SCFCR_TTRG_8)

#define SCFDR_T_MASK		(0x1f << 8)

#define	DL_INIT_DATA		(8)		/* 14.7456MHz/(115200bps*16)*/
			
#define	CKS_CKS_DIV_MASK		(1 << 15)
#define	CKS_CKS_DIV_CLK		(0x0000)
#define	CKS_XIN_MASK		(1 << 14)
#define	CKS_XIN_SCIF_CLK		(0x0000)
#define	CKS_INIT_DATA		(CKS_CKS_DIV_CLK + CKS_XIN_SCIF_CLK)

#define	GPSR5_MASK		(3 << 10)
#define	GPSR5_RX2_A	(1 << 11)
#define	GPSR5_TX2_A	(1 << 10)
#define	GPSR5_SCK2	(1 << 9)
#if SCIF_CLK == SCIF_EXTARNAL_CLK
#define	GPSR5_INIT_DATA		(GPSR5_RX2_A + GPSR5_TX2_A + GPSR5_SCK2)
#else
#define	GPSR5_INIT_DATA		(GPSR5_RX2_A + GPSR5_TX2_A)
#endif

#define	IPSR12_MASK		(255 << 0)
#define	IPSR12_RX2_A		(0 << 4)
#define	IPSR12_TX2_A		(0 << 0)
#define	IPSR12_INIT_DATA	(IPSR12_RX2_A + IPSR12_TX2_A)

#define	MOD_SEL1_SCIF2_MASK	(1 << 12)
#define	MOD_SEL1_SCIF2		(0 << 12)

#endif	/* __SCIF_H__ */
