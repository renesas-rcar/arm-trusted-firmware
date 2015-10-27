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

/* CPG (SAFETY) registers */

/* Safety Module Stop Control Register 0 */
#define	SAMSTPCR0	(CPG_BASE + 0x0C20U)
/* Safety Module Stop Control Register 1 */
#define	SAMSTPCR1	(CPG_BASE + 0x0C24U)
/* Safety Module Stop Control Register 2 */
#define	SAMSTPCR2	(CPG_BASE + 0x0C28U)
/* Safety Module Stop Control Register 3 */
#define	SAMSTPCR3	(CPG_BASE + 0x0C2CU)
/* Safety Module Stop Control Register 4 */
#define	SAMSTPCR4	(CPG_BASE + 0x0C30U)
/* Safety Module Stop Control Register 5 */
#define	SAMSTPCR5	(CPG_BASE + 0x0C34U)
/* Safety Module Stop Control Register 6 */
#define	SAMSTPCR6	(CPG_BASE + 0x0C38U)
/* Safety Module Stop Control Register 7 */
#define	SAMSTPCR7	(CPG_BASE + 0x0C3CU)
/* Safety Module Stop Control Register 8 */
#define	SAMSTPCR8	(CPG_BASE + 0x0C40U)
/* Safety Module Stop Control Register 9 */
#define	SAMSTPCR9	(CPG_BASE + 0x0C44U)
/* Safety Module Stop Control Register 10 */
#define	SAMSTPCR10	(CPG_BASE + 0x0C48U)
/* Safety Module Stop Control Register 11 */
#define	SAMSTPCR11	(CPG_BASE + 0x0C4CU)

/* CPG (SAFETY) registers */

/* Safety Software Reset Access Enable Control Register 0 */
#define	SASRSTECR0	(CPG_BASE + 0x0C80U)
/* Safety Software Reset Access Enable Control Register 1 */
#define	SASRSTECR1	(CPG_BASE + 0x0C84U)
/* Safety Software Reset Access Enable Control Register 2 */
#define	SASRSTECR2	(CPG_BASE + 0x0C88U)
/* Safety Software Reset Access Enable Control Register 3 */
#define	SASRSTECR3	(CPG_BASE + 0x0C8CU)
/* Safety Software Reset Access Enable Control Register 4 */
#define	SASRSTECR4	(CPG_BASE + 0x0C90U)
/* Safety Software Reset Access Enable Control Register 5 */
#define	SASRSTECR5	(CPG_BASE + 0x0C94U)
/* Safety Software Reset Access Enable Control Register 6 */
#define	SASRSTECR6	(CPG_BASE + 0x0C98U)
/* Safety Software Reset Access Enable Control Register 7 */
#define	SASRSTECR7	(CPG_BASE + 0x0C9CU)
/* Safety Software Reset Access Enable Control Register 8 */
#define	SASRSTECR8	(CPG_BASE + 0x0CA0U)
/* Safety Software Reset Access Enable Control Register 9 */
#define	SASRSTECR9	(CPG_BASE + 0x0CA4U)
/* Safety Software Reset Access Enable Control Register 10 */
#define	SASRSTECR10	(CPG_BASE + 0x0CA8U)
/* Safety Software Reset Access Enable Control Register 11 */
#define	SASRSTECR11	(CPG_BASE + 0x0CACU)

#endif	/* BL2_CPG_REGISTER_H__ */
