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
SECTIONS
{
	. = 0x00000000;
	.rodata : {
		KEEP(*(.sa6_image_num))
		. = 0x00000008;
		KEEP(*(.sa6_bl31src_addr))
		. = 0x00000010;
		KEEP(*(.sa6_bl31partition))
		. = 0x00000018;
		KEEP(*(.sa6_bl32src_addr))
		. = 0x00000020;
		KEEP(*(.sa6_bl32partition))
		. = 0x00000028;
		KEEP(*(.sa6_bl33src_addr))
		. = 0x00000030;
		KEEP(*(.sa6_bl33partition))
		. = 0x00000038;
		KEEP(*(.sa6_bl332src_addr))
		. = 0x00000040;
		KEEP(*(.sa6_bl332partition))
		. = 0x00000048;
		KEEP(*(.sa6_bl333src_addr))
		. = 0x00000050;
		KEEP(*(.sa6_bl333partition))
		. = 0x00000058;
		KEEP(*(.sa6_bl334src_addr))
		. = 0x00000060;
		KEEP(*(.sa6_bl334partition))
		. = 0x00000068;
		KEEP(*(.sa6_bl335src_addr))
		. = 0x00000070;
		KEEP(*(.sa6_bl335partition))
		. = 0x00000078;
		KEEP(*(.sa6_bl336src_addr))
		. = 0x00000080;
		KEEP(*(.sa6_bl336partition))
		. = 0x00000088;
		KEEP(*(.sa6_bl337src_addr))
		. = 0x00000090;
		KEEP(*(.sa6_bl337partition))
		. = 0x00000098;
		KEEP(*(.sa6_bl338src_addr))
		. = 0x000000A0;
		KEEP(*(.sa6_bl338partition))
		. = 0x00000554;
		KEEP(*(.sa6_bl31dst_addr))
		. = 0x00000558;
		KEEP(*(.sa6_bl31dst_addrh))
		. = 0x00000664;
		KEEP(*(.sa6_bl31dst_size))
		. = 0x00000D54;
		KEEP(*(.sa6_bl32dst_addr))
		. = 0x00000D58;
		KEEP(*(.sa6_bl32dst_addrh))
		. = 0x00000E64;
		KEEP(*(.sa6_bl32dst_size))
		. = 0x00001554;
		KEEP(*(.sa6_bl33dst_addr))
		. = 0x00001558;
		KEEP(*(.sa6_bl33dst_addrh))
		. = 0x00001664;
		KEEP(*(.sa6_bl33dst_size))
		. = 0x00001D54;
		KEEP(*(.sa6_bl332dst_addr))
		. = 0x00001D58;
		KEEP(*(.sa6_bl332dst_addrh))
		. = 0x00001E64;
		KEEP(*(.sa6_bl332dst_size))
		. = 0x00002554;
		KEEP(*(.sa6_bl333dst_addr))
		. = 0x00002558;
		KEEP(*(.sa6_bl333dst_addrh))
		. = 0x00002664;
		KEEP(*(.sa6_bl333dst_size))
		. = 0x00002D54;
		KEEP(*(.sa6_bl334dst_addr))
		. = 0x00002D58;
		KEEP(*(.sa6_bl334dst_addrh))
		. = 0x00002E64;
		KEEP(*(.sa6_bl334dst_size))
		. = 0x00003554;
		KEEP(*(.sa6_bl335dst_addr))
		. = 0x00003558;
		KEEP(*(.sa6_bl335dst_addrh))
		. = 0x00003664;
		KEEP(*(.sa6_bl335dst_size))
		. = 0x00003D54;
		KEEP(*(.sa6_bl336dst_addr))
		. = 0x00003D58;
		KEEP(*(.sa6_bl336dst_addrh))
		. = 0x00003E64;
		KEEP(*(.sa6_bl336dst_size))
		. = 0x00004554;
		KEEP(*(.sa6_bl337dst_addr))
		. = 0x00004558;
		KEEP(*(.sa6_bl337dst_addrh))
		. = 0x00004664;
		KEEP(*(.sa6_bl337dst_size))
		. = 0x00004D54;
		KEEP(*(.sa6_bl338dst_addr))
		. = 0x00004D58;
		KEEP(*(.sa6_bl338dst_addrh))
		. = 0x00004E64;
		KEEP(*(.sa6_bl338dst_size))
	}

}
