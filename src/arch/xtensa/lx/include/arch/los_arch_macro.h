/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Macro HeadFile
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

#ifndef _LOS_ARCH_MACRO_H
#define _LOS_ARCH_MACRO_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_PLATFORM_ESP8266
.macro LOADSTORE_EXP_POP SP
    l32i    a0,  \SP,  0
    l32i    a2,  \SP,  0x08
    l32i    a3,  \SP,  0x0c
    l32i    a4,  \SP,  0x10
.endm

.macro LOADSTORE_EXP_PUSH SP
    s32i    a0,  \SP,  0
    s32i    a2,  \SP,  0x08
    s32i    a3,  \SP,  0x0c
    s32i    a4,  \SP,  0x10
.endm

.macro REG_TABLE REGNUM LABEL
    .org    .RegTable + 8 * (\REGNUM - 7)
    mov     a2, a&REGNUM&
    j       \LABEL
.endm

.macro STORE_LOAD_TABLE LAYER
    .org    .StoreLoadTable + (16 * (\LAYER - 5))
    mov a&LAYER&, a4
    l32i    a2,  a1,  0x08
    l32i    a4,  a1,  0x10
    rsr     a1,  EXCSAVE1
    rfe
.endm

.macro GET_EXCVADDR  VADDR ALIGADDR
    movi   \ALIGADDR, (~3)
    rsr    \VADDR, EXCVADDR
    and    \ALIGADDR, \ALIGADDR, \VADDR
.endm
#endif

.macro POP_ALL_REG SP PC PState
#ifdef LOSCFG_ARCH_FPU_ENABLE
    l16ui  a3, \SP, CONTEXT_OFF_CPENABLE
    wsr    a3, CPENABLE
    rsync
    l16ui  a3, \SP, CONTEXT_OFF_CPSTORED
    bbci.l a3, 0, 4f

    l32i   a3, \SP, CONTEXT_OFF_FCR
    wur.FCR a3
    l32i   a3, \SP, CONTEXT_OFF_FSR
    wur.FSR a3

    lsi f0, \SP, CONTEXT_OFF_F0
    lsi f1, \SP, CONTEXT_OFF_F1
    lsi f2, \SP, CONTEXT_OFF_F2
    lsi f3, \SP, CONTEXT_OFF_F3
    lsi f4, \SP, CONTEXT_OFF_F4
    lsi f5, \SP, CONTEXT_OFF_F5
    lsi f6, \SP, CONTEXT_OFF_F6
    lsi f7, \SP, CONTEXT_OFF_F7
    lsi f8, \SP, CONTEXT_OFF_F8
    lsi f9, \SP, CONTEXT_OFF_F9
    lsi f10, \SP, CONTEXT_OFF_F10
    lsi f11, \SP, CONTEXT_OFF_F11
    lsi f12, \SP, CONTEXT_OFF_F12
    lsi f13, \SP, CONTEXT_OFF_F13
    lsi f14, \SP, CONTEXT_OFF_F14
    lsi f15, \SP, CONTEXT_OFF_F15

4:
    movi   a4, 0
    s16i   a4, \SP, CONTEXT_OFF_CPSTORED
#endif /* LOSCFG_ARCH_FPU_ENABLE */
#ifdef LOSCFG_XTENSA_LOOPS
    l32i    a3,  \SP, CONTEXT_OFF_LBEG
    l32i    a4,  \SP, CONTEXT_OFF_LEND
    wsr     a3,  LBEG
    l32i    a3,  \SP, CONTEXT_OFF_LCOUNT
    wsr     a4,  LEND
    wsr     a3,  LCOUNT
#endif
    l32i    a3,  \SP, CONTEXT_OFF_SAR
    l32i    a1,  \SP, CONTEXT_OFF_A1
    wsr     a3,  SAR
    l32i    a3,  \SP, CONTEXT_OFF_A3
    l32i    a4,  \SP, CONTEXT_OFF_A4
    l32i    a5,  \SP, CONTEXT_OFF_A5
    l32i    a6,  \SP, CONTEXT_OFF_A6
    l32i    a7,  \SP, CONTEXT_OFF_A7
    l32i    a8,  \SP, CONTEXT_OFF_A8
    l32i    a9,  \SP, CONTEXT_OFF_A9
    l32i    a10, \SP, CONTEXT_OFF_A10
    l32i    a11, \SP, CONTEXT_OFF_A11
    l32i    a12, \SP, CONTEXT_OFF_A12
    l32i    a13, \SP, CONTEXT_OFF_A13
    l32i    a14, \SP, CONTEXT_OFF_A14
    l32i    a15, \SP, CONTEXT_OFF_A15
    l32i    a0,  \SP, CONTEXT_OFF_PS
    wsr     a0,  \PState
    l32i    a0,  \SP, CONTEXT_OFF_PC
    wsr     a0,  \PC
    l32i    a0,  \SP, CONTEXT_OFF_A0
    l32i    a2,  \SP, CONTEXT_OFF_A2
.endm

.macro PUSH_ALL_REG SP
    s32i    a0,  \SP, CONTEXT_OFF_A0
    s32i    a1,  \SP, CONTEXT_OFF_A1
    s32i    a2,  \SP, CONTEXT_OFF_A2
    s32i    a3,  \SP, CONTEXT_OFF_A3
    s32i    a4,  \SP, CONTEXT_OFF_A4
    s32i    a5,  \SP, CONTEXT_OFF_A5
    s32i    a6,  \SP, CONTEXT_OFF_A6
    s32i    a7,  \SP, CONTEXT_OFF_A7
    s32i    a8,  \SP, CONTEXT_OFF_A8
    s32i    a9,  \SP, CONTEXT_OFF_A9
    s32i    a10, \SP, CONTEXT_OFF_A10
    s32i    a11, \SP, CONTEXT_OFF_A11
    s32i    a12, \SP, CONTEXT_OFF_A12
    s32i    a13, \SP, CONTEXT_OFF_A13
    s32i    a14, \SP, CONTEXT_OFF_A14
    s32i    a15, \SP, CONTEXT_OFF_A15
    rsr     a3,  SAR
    s32i    a3,  \SP, CONTEXT_OFF_SAR
#ifdef LOSCFG_XTENSA_LOOPS
    rsr     a3,  LBEG
    s32i    a3,  \SP, CONTEXT_OFF_LBEG
    rsr     a3,  LEND
    s32i    a3,  \SP, CONTEXT_OFF_LEND
    rsr     a3,  LCOUNT
    s32i    a3,  \SP, CONTEXT_OFF_LCOUNT
#endif
    rsr     a3,  PS
    s32i    a3,  \SP, CONTEXT_OFF_PS
#ifdef LOSCFG_ARCH_FPU_ENABLE
    rsr     a3, CPENABLE
    beqz    a3, 3f
    s16i    a3, \SP, CONTEXT_OFF_CPSTORED
    s16i    a3, \SP, CONTEXT_OFF_CPENABLE

    bbci.l	a3, 0, 3f
    rur.FCR a3
    s32i    a3, \SP, CONTEXT_OFF_FCR
    rur.FSR a3
    s32i    a3, \SP, CONTEXT_OFF_FSR

    ssi f0, \SP, CONTEXT_OFF_F0
    ssi f1, \SP, CONTEXT_OFF_F1
    ssi f2, \SP, CONTEXT_OFF_F2
    ssi f3, \SP, CONTEXT_OFF_F3
    ssi f4, \SP, CONTEXT_OFF_F4
    ssi f5, \SP, CONTEXT_OFF_F5
    ssi f6, \SP, CONTEXT_OFF_F6
    ssi f7, \SP, CONTEXT_OFF_F7
    ssi f8, \SP, CONTEXT_OFF_F8
    ssi f9, \SP, CONTEXT_OFF_F9
    ssi f10, \SP, CONTEXT_OFF_F10
    ssi f11, \SP, CONTEXT_OFF_F11
    ssi f12, \SP, CONTEXT_OFF_F12
    ssi f13, \SP, CONTEXT_OFF_F13
    ssi f14, \SP, CONTEXT_OFF_F14
    ssi f15, \SP, CONTEXT_OFF_F15
3:
#endif /* LOSCFG_ARCH_FPU_ENABLE */
.endm

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_ARCH_MACRO_H */

