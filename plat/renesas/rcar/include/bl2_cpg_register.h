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

#ifndef	BL2_CPG_REGISTER_H__
#define	BL2_CPG_REGISTER_H__

/* CPG base address */
#define	CPG_BASE	(0xE6150000U)

/* CPG system module stop control 2 */
#define CPG_SMSTPCR2	(CPG_BASE + 0x0138U)
/* CPG software reset 2 */
#define CPG_SRCR2	(CPG_BASE + 0x00B0U)
/* CPG module stop status 2 */
#define CPG_MSTPSR2	(CPG_BASE + 0x0040U)
/* CPG write protect */
#define CPG_CPGWPR	(CPG_BASE + 0x0900U)
/* CPG write protect control */
#define CPG_CPGWPCR	(CPG_BASE + 0x0904U)
/* CPG system module stop control 9 */
#define CPG_SMSTPCR9    (CPG_BASE + 0x0994U)
/* CPG module stop status 9 */
#define CPG_MSTPSR9     (CPG_BASE + 0x09A4U)

/* CPG (SECURITY) registers */

/* Secure Module Stop Control Register 0 */
#define	SCMSTPCR0	(CPG_BASE + 0x0B20U)
/* Secure Module Stop Control Register 1 */
#define	SCMSTPCR1	(CPG_BASE + 0x0B24U)
/* Secure Module Stop Control Register 2 */
#define	SCMSTPCR2	(CPG_BASE + 0x0B28U)
/* Secure Module Stop Control Register 3 */
#define	SCMSTPCR3	(CPG_BASE + 0x0B2CU)
/* Secure Module Stop Control Register 4 */
#define	SCMSTPCR4	(CPG_BASE + 0x0B30U)
/* Secure Module Stop Control Register 5 */
#define	SCMSTPCR5	(CPG_BASE + 0x0B34U)
/* Secure Module Stop Control Register 6 */
#define	SCMSTPCR6	(CPG_BASE + 0x0B38U)
/* Secure Module Stop Control Register 7 */
#define	SCMSTPCR7	(CPG_BASE + 0x0B3CU)
/* Secure Module Stop Control Register 8 */
#define	SCMSTPCR8	(CPG_BASE + 0x0B40U)
/* Secure Module Stop Control Register 9 */
#define	SCMSTPCR9	(CPG_BASE + 0x0B44U)
/* Secure Module Stop Control Register 10 */
#define	SCMSTPCR10	(CPG_BASE + 0x0B48U)
/* Secure Module Stop Control Register 11 */
#define	SCMSTPCR11	(CPG_BASE + 0x0B4CU)

/* CPG (SECURITY) registers */

/* Secure Software Reset Access Enable Control Register 0 */
#define	SCSRSTECR0	(CPG_BASE + 0x0B80U)
/* Secure Software Reset Access Enable Control Register 1 */
#define	SCSRSTECR1	(CPG_BASE + 0x0B84U)
/* Secure Software Reset Access Enable Control Register 2 */
#define	SCSRSTECR2	(CPG_BASE + 0x0B88U)
/* Secure Software Reset Access Enable Control Register 3 */
#define	SCSRSTECR3	(CPG_BASE + 0x0B8CU)
/* Secure Software Reset Access Enable Control Register 4 */
#define	SCSRSTECR4	(CPG_BASE + 0x0B90U)
/* Secure Software Reset Access Enable Control Register 5 */
#define	SCSRSTECR5	(CPG_BASE + 0x0B94U)
/* Secure Software Reset Access Enable Control Register 6 */
#define	SCSRSTECR6	(CPG_BASE + 0x0B98U)
/* Secure Software Reset Access Enable Control Register 7 */
#define	SCSRSTECR7	(CPG_BASE + 0x0B9CU)
/* Secure Software Reset Access Enable Control Register 8 */
#define	SCSRSTECR8	(CPG_BASE + 0x0BA0U)
/* Secure Software Reset Access Enable Control Register 9 */
#define	SCSRSTECR9	(CPG_BASE + 0x0BA4U)
/* Secure Software Reset Access Enable Control Register 10 */
#define	SCSRSTECR10	(CPG_BASE + 0x0BA8U)
/* Secure Software Reset Access Enable Control Register 11 */
#define	SCSRSTECR11	(CPG_BASE + 0x0BACU)

/* CPG (REALTIME) registers */

/* Realtime Module Stop Control Register 0 */
#define	RMSTPCR0	(CPG_BASE + 0x0110U)
/* Realtime Module Stop Control Register 1 */
#define	RMSTPCR1	(CPG_BASE + 0x0114U)
/* Realtime Module Stop Control Register 2 */
#define	RMSTPCR2	(CPG_BASE + 0x0118U)
/* Realtime Module Stop Control Register 3 */
#define	RMSTPCR3	(CPG_BASE + 0x011CU)
/* Realtime Module Stop Control Register 4 */
#define	RMSTPCR4	(CPG_BASE + 0x0120U)
/* Realtime Module Stop Control Register 5 */
#define	RMSTPCR5	(CPG_BASE + 0x0124U)
/* Realtime Module Stop Control Register 6 */
#define	RMSTPCR6	(CPG_BASE + 0x0128U)
/* Realtime Module Stop Control Register 7 */
#define	RMSTPCR7	(CPG_BASE + 0x012CU)
/* Realtime Module Stop Control Register 8 */
#define	RMSTPCR8	(CPG_BASE + 0x0980U)
/* Realtime Module Stop Control Register 9 */
#define	RMSTPCR9	(CPG_BASE + 0x0984U)
/* Realtime Module Stop Control Register 10 */
#define	RMSTPCR10	(CPG_BASE + 0x0988U)
/* Realtime Module Stop Control Register 11 */
#define	RMSTPCR11	(CPG_BASE + 0x098CU)


#endif	/* BL2_CPG_REGISTER_H__ */
