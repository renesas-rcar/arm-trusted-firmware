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
/* SA6 */
/* 0x00180000 */
const unsigned int __attribute__ ((section (".sa6_image_num"))) image_num = 0x00000001;
/* 0x00180004 */
const unsigned int __attribute__ ((section (".sa6_start_addr_1"))) start_addr_1 = 0x00640000;
/* 0x00180400 */
/* 0x00180554 */
const unsigned int __attribute__ ((section (".sa6_bl31cert_addr"))) bl31cert_addr = 0x44000000;
/* 0x00180664 */
const unsigned int __attribute__ ((section (".sa6_bl31cert_size"))) bl31cert_size = 0x00004000;
/* 0x00180800 */
/* 0x00180954 */
const unsigned int __attribute__ ((section (".sa6_bl32cert_addr"))) bl32cert_addr = 0x44100000;
/* 0x00180A64 */
const unsigned int __attribute__ ((section (".sa6_bl32cert_size"))) bl32cert_size = 0x00020000;
/* 0x00180C00 */
/* 0x00180D54 */
const unsigned int __attribute__ ((section (".sa6_bl33cert_addr"))) bl33cert_addr = 0x49000000;
/* 0x00180E64 */
const unsigned int __attribute__ ((section (".sa6_bl33cert_size"))) bl33cert_size = 0x00040000;

