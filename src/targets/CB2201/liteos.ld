/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     liteos.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
 /* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Linker File
 * Author: Huawei LiteOS Team
 * Create: 2021-10-19
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
MEMORY
{
  ROM (rx)    : ORIGIN = 0x0        , LENGTH = 8k     /* ROM  8KB */
  EFLASH (rx) : ORIGIN = 0x10000800 , LENGTH = 252k   /* E-FLASH 256KB*/
  SRAM (rwx)  : ORIGIN = 0x60000000 , LENGTH = 80k    /* on-chip SRAM 80KB */
}

_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x1000;

PROVIDE (__ram_end  = ORIGIN(SRAM) + LENGTH(SRAM));

REGION_ALIAS("REGION_TEXT",    EFLASH);
REGION_ALIAS("REGION_RODATA",  EFLASH);
REGION_ALIAS("REGION_DATA",    SRAM);
REGION_ALIAS("REGION_BSS",     SRAM);

ENTRY(Reset_Handler)
SECTIONS
{
  .text : {
    . = ALIGN(0x4) ;
    __text_start = .;
    __stext = . ;
    *(.text)
    *(.text*)
    *(.text.*)
    *(.gnu.warning)
    *(.stub)
    *(.gnu.linkonce.t*)
    *(.glue_7t)
    *(.glue_7)
    *(.jcr)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN (4) ;
    PROVIDE(__ctbp = .);
    *(.call_table_data)
    *(.call_table_text)
    . = ALIGN(0x10) ;
    __etext = . ;
    __text_end = .;
  } > REGION_TEXT

  .eh_frame_hdr : {
    *(.eh_frame_hdr)
  } > REGION_TEXT

  .eh_frame : ONLY_IF_RO {
    KEEP (*(.eh_frame))
  } > REGION_TEXT
 
  .gcc_except_table : ONLY_IF_RO {
    *(.gcc_except_table .gcc_except_table.*)
  } > REGION_TEXT

  .rodata : {
    . = ALIGN(0x4) ;
    __rodata_start = .;
    __srodata = .;
    *(.rdata)
    *(.rdata*)
    *(.rdata1)
    *(.rdata.*)
    *(.rodata)
    *(.rodata1)
    *(.rodata*)
    *(.rodata.*)
    *(.rodata.str1.4)
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    . = ALIGN(0x4) ;
    __erodata = .;
    __rodata_end = .;
  } > REGION_RODATA

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >REGION_RODATA

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >REGION_RODATA

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >REGION_RODATA

  .data : {
    . = ALIGN(0x4) ;
    __sdata = . ;
    __data_start__ = . ;
    __ram_data_start = __data_start__;
    data_start = . ;
    _s_liteos_vector = .;
    *(.data.vector)    /* liteos vector in ram */
    _e_liteos_vector = .;
    *(.got.plt)
    *(.got)
    *(.gnu.linkonce.r*)
    *(.data)
    *(.data*)
    *(.data1)
    *(.data.*)
    *(.gnu.linkonce.d*)
    *(.data1)
    *(.gcc_except_table)
    *(.gcc_except_table*)
    __start_init_call = .;
    *(.initcall.init)
    __stop_init_call = .;
    __start_cmd = .;
    *(.bootloaddata.cmd)
    . = ALIGN(4) ;
    __stop_cmd = .;
    *(.sdata)
    *(.sdata.*)
    *(.gnu.linkonce.s.*)
    *(__libc_atexit)
    *(__libc_subinit)
    *(__libc_subfreeres)
    *(.note.ABI-tag)
    KEEP(*( SORT (.liteos.table.*)));
    . = ALIGN(0x4) ;
    __edata = .;
    __data_end__ = .;
    __ram_data_end = __data_end__;
  } > REGION_DATA AT > REGION_RODATA

  .eh_frame : ONLY_IF_RW {
    KEEP (*(.eh_frame))
  } > REGION_DATA AT > REGION_RODATA

  .gcc_except_table : ONLY_IF_RW {
    *(.gcc_except_table .gcc_except_table.*)
    __edata = .;
    __data_end__ = .;
  } > REGION_DATA AT > REGION_RODATA

  .bss : {
    . = ALIGN(0x4) ;
    __sbss = ALIGN(0x4) ;
    __bss_start = . ;
    *(.dynsbss)
    *(.sbss)
    *(.sbss.*)
    *(.scommon)
    *(.dynbss)
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(0x4) ;
    __ebss = . ;
    __bss_end = .;
    __end = . ;
    end = . ;
  } > REGION_BSS

  . = ALIGN(0x8);
  __heap_start = .;
  __heap_end = ORIGIN(SRAM) + LENGTH(SRAM) - _Min_Stack_Size - 1;
  __fast_end = .;

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}
