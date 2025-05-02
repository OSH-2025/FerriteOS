/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Lx6 Exception Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-07
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

#include "arch/regs.h"
#include "arch/los_arch_macro.h"

.extern ArchInterrupt
.extern UserException

.section    .iram1,"ax"
.literal_position
.global HandleEntry
.type       HandleEntry, @function
.align      4

HandleEntry:
    mov     a0, sp
    addi    sp, sp, -CONTEXT_SIZE

    PUSH_ALL_REG a1
    s32i    a0, sp, CONTEXT_OFF_A1
    rsr     a0, EPC1
    s32i    a0, sp, CONTEXT_OFF_PC
    rsr     a0, EXCSAVE1
    s32i    a0, sp, CONTEXT_OFF_A0

#ifdef LOSCFG_XTENSA_WINDOWSPILL
    wsr     a1, EXCSAVE1
    l32i    a1, a1, CONTEXT_OFF_A1
    call0   SpillWindow
    rsr     a1, EXCSAVE1
#endif
    rsr     a0, EXCCAUSE
    s32i    a0, sp, CONTEXT_OFF_EXCCAUSE
    rsr     a0, EXCVADDR
    s32i    a0, sp, CONTEXT_OFF_EXCVADDR
#ifndef LOSCFG_XTENSA_WINDOWSPILL
    movi    a0, INT_MASK | USER_VECTOR_MODE
#else 
    movi    a0, INT_MASK | USER_VECTOR_MODE | WOE_ENABLE
#endif
    wsr     a0, PS

#ifdef LOSCFG_XTENSA_WINDOWSPILL
    rsr     a6, EPC1
    mov     a7, sp
    rsr     a8, EXCCAUSE
    call4   OsExcHandleEntry
#else
    rsr     a2, EPC1
    mov     a3, a1
    rsr     a4, EXCCAUSE
    movi    a0, OsExcHandleEntry
    callx0  a0
#endif
1:
    j       1b

#ifdef LOSCFG_COMPILER_XTENSA_L106
.literal_position
        .type   LoadStoreErr, @function
        .balign 4
LoadStoreErr:
    rsr     a0,  EXCSAVE1
    addi    a1, a1, -28
    LOADSTORE_EXP_PUSH a1
    rsr     a0,  SAR
    s32i    a0, a1, 4
    rsr     a2, EPC1
    ssa8l   a2
    srli    a2,  a2,  2
    slli    a2,  a2,  2         /* epc1 align 4 byte */
    l32i    a4,  a2,  0
    l32i    a2,  a2,  4
    movi    a3,  LOAD_STORE_ERROR_MASK
    src     a2,  a2,  a4        /* get epc1 code */
    and     a3,  a2,  a3
    movi    a4,  LOAD8_OPCODE
    xor     a4,  a4,  a3
    beqz    a4,  .Load8
    movi    a4,  LOAD16_OPCODE
    xor     a4,  a4,  a3
    beqz    a4,  .Load16
    movi    a4,  STOTE8_OPCODE
    xor     a4,  a4,  a3
    beqz    a4,  .Store8
    movi    a4,  STOTE16_OPCODE
    xor     a4,  a4,  a3
    beqz    a4,  .Store16
    j       .ExpCode

    .balign 4
.Load16:
    movi    a4,  0xFF00
.Load8:
    GET_EXCVADDR  a3, a0
    l32i    a0,  a0,  0
    ssa8l   a3
    srl     a3,  a0
    addi    a4,  a4,  0xFF
    and     a3,  a3,  a4
    movi    a0,  0xFF
    beq     a4,  a0, .CommonLoad
    bbci    a3,  15, .CommonLoad
    movi    a4, 0xFFFF0000
    or      a3,  a3,  a4
    j       .CommonLoad

.CommonLoad:
    mov     a4,  a3
    l32i    a0,  a1, 4
    wsr     a0,  SAR
    rsr     a0,  EPC1
    addi    a0,  a0,  0x3
    wsr     a0,  EPC1
    srli    a2,  a2,  4
    movi    a3,  0xF
    and     a2,  a2,  a3
    beqi    a2,  1,  .GetA1
    bgei    a2,  5,  .GetTable
    addx4   a2,  a2,  a1
    s32i    a4,  a2,  0
    LOADSTORE_EXP_POP a1
    addi    a1,  a1, 28
    rfe

.GetTable:
    movi    a3,  .StoreLoadTableBase
    l32i    a0,  a1,  0
    slli    a2,  a2,  4
    add     a2,  a2,  a3
    l32i    a3,  a1,  0x0c
    jx      a2

    .balign 4
.Store16:
    movi    a4,  0xFF00
.Store8:
    addi    a4,  a4,  0xFF
    s32i    a5,  a1,  0x14
    s32i    a6,  a1,  0x18
    mov     a5,  a4
    GET_EXCVADDR a6, a4
    GET_EXCVADDR a3, a4
    ssa8b   a3
    l32i    a3,  a4,  0
    sll     a4,  a5
    movi    a6,  -1
    xor     a4,  a6,  a4
    and     a3,  a3,  a4
    extui   a2,  a2,  4,  4
    movi    a6,  4
    mull    a6,  a2,  a6
    add     a2,  a6,  a1
    l32i    a2,  a2,  0

    and     a2,  a2,  a5
    sll     a2,  a2
    or      a3,  a3,  a2
    s32i    a3,  a4,  0
    wsr     a0,  SAR
    rsr     a0,  EPC1
    addi    a0,  a0,  0x3
    wsr     a0,  EPC1
    l32i    a5,  a1,  0x14
    l32i    a6,  a1,  0x18
    LOADSTORE_EXP_POP a1
    addi    a1,  a1,  28
    rfe

.ExpCode:
    wsr     a0,  SAR
    LOADSTORE_EXP_POP a1
    addi    a1,  a1, 28
    call0   HandleEntry

    .balign 4
.GetA1:
    s32i    a4,  a1,  0x04
    LOADSTORE_EXP_POP a1
    l32i    a1,  a1,  0x04
    rfe

    .balign 4
.StoreLoadTable:
    .set    .StoreLoadTableBase, .StoreLoadTable - 80
    STORE_LOAD_TABLE 5
    STORE_LOAD_TABLE 6
    STORE_LOAD_TABLE 7
    STORE_LOAD_TABLE 8
    STORE_LOAD_TABLE 9
    STORE_LOAD_TABLE 10
    STORE_LOAD_TABLE 11
    STORE_LOAD_TABLE 12
    STORE_LOAD_TABLE 13
    STORE_LOAD_TABLE 14
    STORE_LOAD_TABLE 15
.balign 4
#endif

.begin      literal_prefix .DebugExceptionHandler
.section    .DebugExceptionHandler.text, "ax"
.global     DebugExceptionHandler
.align      4
.literal_position
DebugExceptionHandler:

    call0   UserException

.end        literal_prefix

.begin      literal_prefix .NMIExceptionHandler
.section    .NMIExceptionHandler.text, "ax"
.global     NMIExceptionHandler
.align      4
.literal_position
NMIExceptionHandler:

    call0   UserException

.end        literal_prefix

.begin      literal_prefix .DoubleExceptionHandler
.section    .DoubleExceptionHandler.text, "ax"
.global     DoubleExceptionHandler
.align      4
.literal_position
DoubleExceptionHandler:

    call0   UserException

.end        literal_prefix

.begin      literal_prefix .KernelExceptionHandler
.section    .KernelExceptionHandler.text, "ax"
.global     KernelExceptionHandler
.align      4
.literal_position
KernelExceptionHandler:

    wsr     a0, EXCSAVE1
    call0   KernelException

.end        literal_prefix

.section .iram1,"ax"
.align      4
KernelException:

    movi    a0,5
    wsr     a0,EXCCAUSE
    call0   UserException
    rfe

.begin      literal_prefix .UserExceptionHandler
.section    .UserExceptionHandler.text, "ax"
.global     UserExceptionHandler
.type       UserExceptionHandler,@function
.align      4
.literal_position
UserExceptionHandler:

    wsr a0, EXCSAVE1    /* save lr */
    wsr a1, EXCSAVE2
    call0   UserException
.end        literal_prefix

.section    .iram1,"ax"
.type       UserException,@function
.align      4
UserException:

    rsr     a0, EXCCAUSE    /* read reason */
#ifdef LOSCFG_COMPILER_XTENSA_L106
    beqi    a0, 3, LoadStoreErr
#endif
    beqi    a0, LEVEL1_TINRERRUPT_CAUSE, InterruptEntry1
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    beqi    a0, ALLOC_CAUSE, AllocHandel
#endif
    j       HandleEntry

#ifdef LOSCFG_XTENSA_WINDOWSPILL
.section    .iram1,"ax"
.type       AllocHandel,@function
.align      4
AllocHandel:
    call0 AllocHandelEntry
#endif

.type       InterruptEntry1,@function
.align      4
InterruptEntry1:

    mov     a0, sp
    addi    sp, sp, -CONTEXT_SIZE

    PUSH_ALL_REG a1

    s32i    a0, sp, CONTEXT_OFF_A1
    rsr     a0, EPC1
    s32i    a0, sp, CONTEXT_OFF_PC
    rsr     a0, EXCSAVE1
    s32i    a0, sp, CONTEXT_OFF_A0

#ifdef LOSCFG_XTENSA_WINDOWSPILL
    wsr     a1, EXCSAVE1
    l32i    a1, a1, CONTEXT_OFF_A1
    call0   SpillWindow
    rsr     a1, EXCSAVE1
    movi    a0, LEVEL1_INT_MASK | USER_VECTOR_MODE | WOE_ENABLE
#else
    movi    a0, LEVEL1_INT_MASK | USER_VECTOR_MODE
#endif

    wsr     a0, PS
    rsync
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    call4   ArchInterrupt
#else
    call0   ArchInterrupt
#endif
    mov     a2, a1
    POP_ALL_REG a2 EPC1 PS
    rfe

#ifdef LOSCFG_COMPILER_XTENSA_LX6
.begin      literal_prefix .InterruptEntry2
.section    .InterruptEntry2.text, "ax"
.global     InterruptEntry2
.type       InterruptEntry2,@function
.align      4
.literal_position
InterruptEntry2:

    wsr     a0, EXCSAVE2
    movi    a4, LEVEL2
    call0   HandleEntry

.end        literal_prefix

.begin      literal_prefix .InterruptEntry3
.section    .InterruptEntry3.text, "ax"
.global     InterruptEntry3
.type       InterruptEntry3,@function
.align      4
.literal_position
InterruptEntry3:

    wsr     a0, EXCSAVE3
    movi    a4, LEVEL3
    call0   HandleEntry

.end        literal_prefix

.begin      literal_prefix .InterruptEntry4
.section    .InterruptEntry4.text, "ax"
.global     InterruptEntry4
.type       InterruptEntry4,@function
.align      4
.literal_position
InterruptEntry4:

    wsr     a0, EXCSAVE4
    movi    a4, LEVEL4
    call0   HandleEntry

.end        literal_prefix

.begin      literal_prefix .InterruptEntry5
.section    .InterruptEntry5.text, "ax"
.global     InterruptEntry5
.type       InterruptEntry5,@function
.align      4
.literal_position
InterruptEntry5:

    wsr     a0, EXCSAVE5
    movi    a4, LEVEL5
    call0   HandleEntry

.end        literal_prefix
#endif

#ifdef LOSCFG_XTENSA_WINDOWSPILL
.section .WindowVectors.text, "ax"

.org    0x0
.global OverFlowGroup1
OverFlowGroup1:

    addi    a5, a5, -16
    s32i    a3, a5, 12
    s32i    a2, a5, 8
    s32i    a1, a5, 4
    s32i    a0, a5, 0
    addi    a5, a5, 16
    rfwo

.org    0x40
.global UnderFlowGroup1
UnderFlowGroup1:

    addi    a5, a5, -16
    l32i    a3, a5, 12
    l32i    a2, a5, 8
    l32i    a1, a5, 4
    l32i    a0, a5, 0
    addi    a5, a5, 16
    rfwu

.org    0x80
.global OverFlowGroup2
OverFlowGroup2:

    addi    a9, a9, -16
    s32i    a3, a9, 12
    s32i    a2, a9, 8
    s32i    a1, a9, 4
    s32i    a0, a9, 0
    addi    a9, a9, 16

    addi    a1, a1, -12
    l32i    a0, a1, 0
    addi    a1, a1, 12

    addi    a0, a0, -32

    s32i    a7, a0, 12
    s32i    a6, a0, 8
    s32i    a5, a0, 4
    s32i    a4, a0, 0

    rfwo

.org    0xC0
.global UnderFlowGroup2
UnderFlowGroup2:

    addi  a9, a9, -16
    l32i  a3, a9, 12
    l32i  a2, a9, 8
    l32i  a1, a9, 4
    l32i  a0, a9, 0  
    addi  a9, a9, 16

    addi  a1, a1, -12
    l32i  a4, a1, 0
    addi  a1, a1, 12

    addi    a4, a4, -32
    l32i    a7, a4, 12
    l32i    a6, a4, 8
    l32i    a5, a4, 4
    l32i    a4, a4, 0
    rfwu

.org    0x100
.global OverFlowGroup3
OverFlowGroup3:

    addi    a13, a13, -16
    s32i    a3, a13, 12
    s32i    a2, a13, 8
    s32i    a1, a13, 4
    s32i    a0, a13, 0
    addi    a13, a13, 16

    addi    a1, a1, -12
    l32i    a0, a1, 0
    addi    a1, a1, 12

    addi    a0, a0, -48

    s32i    a11, a0, 28
    s32i    a10, a0, 24
    s32i    a9, a0, 20
    s32i    a8, a0, 16
    s32i    a7, a0, 12      
    s32i    a6, a0, 8
    s32i    a5, a0, 4
    s32i    a4, a0, 0
    rfwo

.org 0x140
.global UnderFlowGroup3
UnderFlowGroup3:

    addi  a13, a13, -16
    l32i  a3, a13, 12
    l32i  a2, a13, 8
    l32i  a1, a13, 4
    l32i  a0, a13, 0  
    addi  a13, a13, 16

    addi  a1, a1, -12
    l32i  a4, a1, 0
    addi  a1, a1, 12

    addi    a4, a4, -48
    l32i    a11, a0, 28
    l32i    a10, a0, 24
    l32i    a9, a0, 20
    l32i    a8, a0, 16
    l32i    a7, a4, 12      
    l32i    a6, a4, 8
    l32i    a5, a4, 4
    l32i    a4, a4, 0
    rfwu

.section    .userVector,"ax"
.type       AllocHandelEntry,@function
.align      4
AllocHandelEntry:

    addi    a1, a1, -(CONTEXT_OFF_A4 - CONTEXT_OFF_A0)
    s32i    a2, a1, (CONTEXT_OFF_A2 - CONTEXT_OFF_A0)
    s32i    a3, a1, (CONTEXT_OFF_A3 - CONTEXT_OFF_A0)

    rsr     a0, PS
    rsr     a2, WINDOWBASE
    extui   a3, a0, PS_OWB_SHIFT, WINDOWBASEBITS
    xor     a3, a3, a2
    slli    a3, a3, PS_OWB_SHIFT
    xor     a0, a0, a3
    wsr     a0, PS
    rsr     a0, EXCSAVE1

    l32i    a2, a1, (CONTEXT_OFF_A2 - CONTEXT_OFF_A0)
    l32i    a3, a1, (CONTEXT_OFF_A3 - CONTEXT_OFF_A0)
    addi    a1, a1, (CONTEXT_OFF_A4 - CONTEXT_OFF_A0)
    rsync

    rotw -1
    bbci    a4, HIGHBIT_CALLNMODE, UnderFlowGroup1
    rotw -1
    bbci    a8, LOWBIT_CALLNMODE, UnderFlowGroup2
    rotw -1
    j       UnderFlowGroup3
#endif
