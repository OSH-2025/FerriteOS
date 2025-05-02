/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description : LiteOS exception module implemention.
 * Author: Huawei LiteOS Team
 * Create : 2018-03-21
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

/**
 * @file los_hw_exc.S
 * @author Huawei LiteOS
 * @brief RISC-V trap handling.
 */

#ifndef _LOS_HW_EXC_S
#define _LOS_HW_EXC_S

#include "arch/cpu.h"

#define EXC_SIZE_ON_STACK       (36 * REGBYTES)

.extern   OsExcHandleEntry

.global TrapHandle
.global NmiEntry
.global g_excInfo
.section .itcm.text
.align 4

.macro  PUSH_FPU_CALLEE_REG
    FSREG  fs11, 0 * REGBYTES(t0)
    FSREG  fs10, 1 * REGBYTES(t0)
    FSREG  fs9,  2 * REGBYTES(t0)
    FSREG  fs8,  3 * REGBYTES(t0)
    FSREG  fs7,  4 * REGBYTES(t0)
    FSREG  fs6,  5 * REGBYTES(t0)
    FSREG  fs5,  6 * REGBYTES(t0)
    FSREG  fs4,  7 * REGBYTES(t0)
    FSREG  fs3,  8 * REGBYTES(t0)
    FSREG  fs2,  9 * REGBYTES(t0)
    FSREG  fs1, 10 * REGBYTES(t0)
    FSREG  fs0, 11 * REGBYTES(t0)
.endm

.macro  PUSH_INTEGER_CALLEE_REG
    SREG   s11,   4 * REGBYTES(sp)
    SREG   s10,   5 * REGBYTES(sp)
    SREG   s9,    6 * REGBYTES(sp)
    SREG   s8,    7 * REGBYTES(sp)
    SREG   s7,    8 * REGBYTES(sp)
    SREG   s6,    9 * REGBYTES(sp)
    SREG   s5,   10 * REGBYTES(sp)
    SREG   s4,   11 * REGBYTES(sp)
    SREG   s3,   12 * REGBYTES(sp)
    SREG   s2,   13 * REGBYTES(sp)
    SREG   s1,   14 * REGBYTES(sp)
    SREG   s0,   15 * REGBYTES(sp)
.endm

.macro  PUSH_CALLEE_REG
#ifdef LOSCFG_ARCH_FPU_ENABLE
    PUSH_INTEGER_CALLEE_REG
    addi   t0,   sp, 32 * REGBYTES
    PUSH_FPU_CALLEE_REG
#else
    PUSH_INTEGER_CALLEE_REG
#endif
.endm

.macro RESTORE_ALL
    LREG  t0, 4 * REGBYTES(sp)
    csrw  mstatus, t0
    LREG  t0, 5 * REGBYTES(sp)
    csrw  mepc, t0
    LREG  s11, 8 * REGBYTES(sp)
    LREG  s10, 9 * REGBYTES(sp)
    LREG  s9, 10 * REGBYTES(sp)
    LREG  s8, 11 * REGBYTES(sp)
    LREG  s7, 12 * REGBYTES(sp)
    LREG  s6, 13 * REGBYTES(sp)
    LREG  s5, 14 * REGBYTES(sp)
    LREG  s4, 15 * REGBYTES(sp)
    LREG  s3, 16 * REGBYTES(sp)
    LREG  s2, 17 * REGBYTES(sp)
    LREG  s1, 18 * REGBYTES(sp)
    LREG  s0, 19 * REGBYTES(sp)
    LREG  t6, 20 * REGBYTES(sp)
    LREG  t5, 21 * REGBYTES(sp)
    LREG  t4, 22 * REGBYTES(sp)
    LREG  t3, 23 * REGBYTES(sp)
    LREG  a7, 24 * REGBYTES(sp)
    LREG  a6, 25 * REGBYTES(sp)
    LREG  a5, 26 * REGBYTES(sp)
    LREG  a4, 27 * REGBYTES(sp)
    LREG  a3, 28 * REGBYTES(sp)
    LREG  a2, 29 * REGBYTES(sp)
    LREG  a1, 30 * REGBYTES(sp)
    LREG  a0, 31 * REGBYTES(sp)
    LREG  t2, 32 * REGBYTES(sp)
    LREG  t1, 33 * REGBYTES(sp)
    LREG  t0, 34 * REGBYTES(sp)
    LREG  ra, 35 * REGBYTES(sp)
    addi  sp, sp, (EXC_SIZE_ON_STACK)
.endm

TrapHandle:
    PUSH_CALLEE_REG
    addi  sp, sp, -3 * 4
    csrr  s3, mcause
    SREG  s3, 0 * REGBYTES(sp)
    csrr  s2, mbadaddr
    SREG  s2, 1 * REGBYTES(sp)
    SREG  gp, 2 * REGBYTES(sp)
    csrr  s1, mstatus
    SREG  s1, 3 * REGBYTES(sp)
    csrr  s0, mepc
    SREG  s0, 4 * REGBYTES(sp)
    SREG  tp, 5 * REGBYTES(sp)
    SREG  sp, 6 * REGBYTES(sp)
    csrw  mscratch, sp      /* save exc stack */
    tail  trapEntry;

.section .text
.align 4
trapEntry:
    la   t0, g_excInfo
    lh   t1, 12(t0)
    bnez t1, 1f
#ifdef LOSCFG_LIB_CONFIGURABLE
    la    sp, g_excStackTop
    lw    sp, 0(sp)
#else
    la    sp, __exc_stack_top
#endif
1:
    addi t1, t1, 0x1        /* g_excInfo.nestCnt++ */
    sh   t1, 12(t0)
    mv    a0, s3            /* a0 = excType */
    csrr  a1, mscratch      /* a1 = excBufAddr */

    la    t0, OsExcHandleEntry
    jalr  t0

    csrr  sp, mscratch      /* restore exc stack */
# Remain in M-mode after mret
    li    t0, LOS_MSTATUS_MPP
    csrs  mstatus, t0

    RESTORE_ALL
    mret

#endif
