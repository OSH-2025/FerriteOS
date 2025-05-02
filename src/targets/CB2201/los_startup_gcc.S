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
 * @file     los_startup_gcc.S
 * @brief    startup file for CH2201. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     24. August 2017
 ******************************************************************************/
 /* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: System Reset Process
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

#define AHB_BASE        0x40000000
#undef  EFLASH_CONTROL_BASE
#define EFLASH_CONTROL_BASE  0x4003f000

#undef  PMU_LP_CONTROL
#define PMU_LP_CONTROL  0x40002000

.text
.long _start
.align   2
_start:
.text
.align
.globl   Reset_Handler
.type    Reset_Handler, %function
Reset_Handler:
    /* set the priority cpu ahb */
    lrw     r0, AHB_BASE
    movi    r1, 0x1
    movi    r2, 0x2
    movi    r3, 0x3
    movi    r4, 0x4
    stw     r1, (r0, 0x0)
    stw     r2, (r0, 0xc)
    stw     r3, (r0, 0x4)
    stw     r4, (r0, 0x8)

    /* restore the eflash state when system reboot from deep sleep */
    lrw     r0, PMU_LP_CONTROL
    ldw     r1, (r0, 0)
    btsti   r1, 5
    bf      .LCPUINIT
    lrw     r0, EFLASH_CONTROL_BASE
    movi    r1, 0x35
    stw     r1, (r0, 0x24)
    movi    r1, 0x16
    stw     r1, (r0, 0x28)
    movi    r1, 0x35
    stw     r1, (r0, 0x2c)
    movi    r1, 0x1b9
    stw     r1, (r0, 0x30)
    movi    r1, 0x8b10
    stw     r1, (r0, 0x34)

.LCPUINIT:
    lrw     r0, 0xe0000004
    mtcr    r0, psr

/* Initialize the normal stack pointer from the linker definition. */
    lrw     r0, __ram_end
    mov     sp, r0

/*
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
    lrw     r1, __erodata
    lrw     r2, __data_start__
    lrw     r3, __data_end__

    subu    r3, r2
    cmpnei  r3, 0
    bf      .L_loop0_done

.L_loop0:
    ldw     r0, (r1, 0)
    stw     r0, (r2, 0)
    addi    r1, 4
    addi    r2, 4
    subi    r3, 4
    cmpnei  r3, 0
    bt      .L_loop0

.L_loop0_done:

/*
 *  The BSS section is specified by following symbols
 *    __bss_start: start of the BSS section.
 *    __bss_end: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
    lrw     r1, __bss_start
    lrw     r2, __bss_end

    movi    r0, 0

    subu    r2, r1
    cmpnei  r2, 0
    bf      .L_loop1_done

.L_loop1:
    stw     r0, (r1, 0)
    addi    r1, 4
    subi    r2, 4
    cmpnei  r2, 0
    bt      .L_loop1
.L_loop1_done:

    jbsr    SystemInit

    jbsr    uart_early_init

    jbsr    main

    .size   Reset_Handler, . - Reset_Handler
__exit:
    br __exit

.text
.align  2
.global IrqEntry
IrqEntry:
    psrset  ee
    subi   sp, 72
    stm    r0-r15, (sp)
    mfcr   r0, epsr
    stw    r0, (sp, 64)
    mfcr   r0, epc
    stw    r0, (sp, 68)

    jbsr      IrqEntryCsky

    ldw    r0, (sp, 68)
    mtcr   r0, epc
    ldw    r0, (sp, 64)
    bseti  r0, r0, 6
    mtcr   r0, epsr
    ldm    r0-r15, (sp)
    addi   sp, 72
    rte

