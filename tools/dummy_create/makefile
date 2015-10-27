#
# Copyright (c) 2015-2016, Renesas Electronics Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   - Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#   - Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#   - Neither the name of Renesas nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

###################################################
# makefile
###################################################

#output file name
FILE_NAME_SA0   = bootparam_sa0
FILE_NAME_SA6   = cert_header_sa6

OUTPUT_FILE_SA0 = $(FILE_NAME_SA0).elf
OUTPUT_FILE_SA6 = $(FILE_NAME_SA6).elf

#object file name
OBJ_FILE_SA0 =	sa0.o
OBJ_FILE_SA6 =	sa6.o

#linker script name 
MEMORY_DEF_SA0 = sa0.ld.S
MEMORY_DEF_SA6 = sa6.ld.S

###################################################

#c compiler
CC = $(CROSS_COMPILE)gcc

#Linker
LD = $(CROSS_COMPILE)ld

#objcopy
objcopy = $(CROSS_COMPILE)objcopy

#clean
CL = rm -f

###################################################
.SUFFIXES : .s .c .o

###################################################
# command

.PHONY: all
all: $(OUTPUT_FILE_SA0) $(OUTPUT_FILE_SA6)
###################################################
# Linker
###################################################
$(OUTPUT_FILE_SA0) : $(MEMORY_DEF_SA0) $(OBJ_FILE_SA0)
	$(LD) $(OBJ_FILE_SA0)		 	\
	-T $(MEMORY_DEF_SA0)			\
	-o $(OUTPUT_FILE_SA0)			\
	-Map $(FILE_NAME_SA0).map 		\

	$(objcopy) -O srec --adjust-vma=0xE6320000 --srec-forceS3  $(OUTPUT_FILE_SA0) $(FILE_NAME_SA0).srec
	$(objcopy) -O binary --adjust-vma=0xE6320000 --srec-forceS3  $(OUTPUT_FILE_SA0) $(FILE_NAME_SA0).bin

$(OUTPUT_FILE_SA6) : $(MEMORY_DEF_SA6) $(OBJ_FILE_SA6)
	$(LD) $(OBJ_FILE_SA6)		 	\
	-T $(MEMORY_DEF_SA6)			\
	-o $(OUTPUT_FILE_SA6)			\
	-Map $(FILE_NAME_SA6).map 		\

	$(objcopy) -O srec --adjust-vma=0xE6320000 --srec-forceS3  $(OUTPUT_FILE_SA6) $(FILE_NAME_SA6).srec
	$(objcopy) -O binary --adjust-vma=0xE6320000 --srec-forceS3  $(OUTPUT_FILE_SA6) $(FILE_NAME_SA6).bin

###################################################
# Compile
###################################################

%.o:../%.c
	$(CC) -c -I $< -o $@

.PHONY: clean
clean:
	$(CL)  *.bin *.map *.srec *.elf *.o
