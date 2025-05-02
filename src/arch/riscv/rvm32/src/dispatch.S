/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: LiteOS task schedule module implemention.
 * Author: Huawei LiteOS Team
 * Create: 2018-03-21
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
 * ---------------------------------------------------------------------------- */

#include "arch/cpu.h"
#include "asm/interrupt_config.h"

/* task TCB offset */
#define TASK_CB_KERNEL_SP       0x0
#define TASK_CB_STATUS          0x4

.global  OsIntLock
.global  OsIntUnLock
.global  OsIntRestore
.global  OsStartToRun
.global  OsTaskSchedule
.global  TrapVector
.global  OsDisableIRQ
#ifdef LOSCFG_LIB_CONFIGURABLE
.extern g_irqStackTop
#else
.extern  __irq_stack_top
#endif
.extern  TrapHandle

.equ        OS_TASK_STATUS_RUNNING,     0x0010
.equ        MCAUSE_MASK_INT_BIT,        0x80000000
.equ        MCAUSE_MASK_INT_NUM,        0x7FFFFFFF

/* Use the KEEP declaration in the linked file(ld) */
.section .itcm.text
.macro  PUSH_FPU_CALLER_REG
    FSREG  ft11,  0 * REGBYTES(sp)
    FSREG  ft10,  1 * REGBYTES(sp)
    FSREG  ft9,   2 * REGBYTES(sp)
    FSREG  ft8,   3 * REGBYTES(sp)
    FSREG  fa7,   4 * REGBYTES(sp)
    FSREG  fa6,   5 * REGBYTES(sp)
    FSREG  fa5,   6 * REGBYTES(sp)
    FSREG  fa4,   7 * REGBYTES(sp)
    FSREG  fa3,   8 * REGBYTES(sp)
    FSREG  fa2,   9 * REGBYTES(sp)
    FSREG  fa1,  10 * REGBYTES(sp)
    FSREG  fa0,  11 * REGBYTES(sp)
    FSREG  ft7,  12 * REGBYTES(sp)
    FSREG  ft6,  13 * REGBYTES(sp)
    FSREG  ft5,  14 * REGBYTES(sp)
    FSREG  ft4,  15 * REGBYTES(sp)
    FSREG  ft3,  16 * REGBYTES(sp)
    FSREG  ft2,  17 * REGBYTES(sp)
    FSREG  ft1,  18 * REGBYTES(sp)
    FSREG  ft0,  19 * REGBYTES(sp)
.endm

.macro PUSH_INTEGER_CALLER_REG
    SREG   t6,    0 * REGBYTES(sp)
    SREG   t5,    1 * REGBYTES(sp)
    SREG   t4,    2 * REGBYTES(sp)
    SREG   t3,    3 * REGBYTES(sp)
    SREG   a7,    4 * REGBYTES(sp)
    SREG   a6,    5 * REGBYTES(sp)
    SREG   a5,    6 * REGBYTES(sp)
    SREG   a4,    7 * REGBYTES(sp)
    SREG   a3,    8 * REGBYTES(sp)
    SREG   a2,    9 * REGBYTES(sp)
    SREG   a1,   10 * REGBYTES(sp)
    SREG   a0,   11 * REGBYTES(sp)
    SREG   t2,   12 * REGBYTES(sp)
    SREG   t1,   13 * REGBYTES(sp)
    SREG   t0,   14 * REGBYTES(sp)
    SREG   ra,   15 * REGBYTES(sp)
.endm

.macro PUSH_CALLER_REG
#ifdef LOSCFG_ARCH_FPU_ENABLE
    addi  sp, sp, -(20 * REGBYTES)
    PUSH_FPU_CALLER_REG
    addi  sp, sp, -(28 * REGBYTES)
    PUSH_INTEGER_CALLER_REG
    addi  sp, sp, -(16 * REGBYTES)
#else
    addi  sp, sp, -(16 * REGBYTES)
    PUSH_INTEGER_CALLER_REG
    addi  sp, sp, -(16 * REGBYTES)
#endif
.endm

.macro POP_FPU_CALLER_REG
    FLREG  ft11,  0 * REGBYTES(sp)
    FLREG  ft10,  1 * REGBYTES(sp)
    FLREG  ft9,   2 * REGBYTES(sp)
    FLREG  ft8,   3 * REGBYTES(sp)
    FLREG  fa7,   4 * REGBYTES(sp)
    FLREG  fa6,   5 * REGBYTES(sp)
    FLREG  fa5,   6 * REGBYTES(sp)
    FLREG  fa4,   7 * REGBYTES(sp)
    FLREG  fa3,   8 * REGBYTES(sp)
    FLREG  fa2,   9 * REGBYTES(sp)
    FLREG  fa1,  10 * REGBYTES(sp)
    FLREG  fa0,  11 * REGBYTES(sp)
    FLREG  ft7,  12 * REGBYTES(sp)
    FLREG  ft6,  13 * REGBYTES(sp)
    FLREG  ft5,  14 * REGBYTES(sp)
    FLREG  ft4,  15 * REGBYTES(sp)
    FLREG  ft3,  16 * REGBYTES(sp)
    FLREG  ft2,  17 * REGBYTES(sp)
    FLREG  ft1,  18 * REGBYTES(sp)
    FLREG  ft0,  19 * REGBYTES(sp)
.endm

.macro POP_INTEGER_CALLER_REG
    LREG   t6,    0 * REGBYTES(sp)
    LREG   t5,    1 * REGBYTES(sp)
    LREG   t4,    2 * REGBYTES(sp)
    LREG   t3,    3 * REGBYTES(sp)
    LREG   a7,    4 * REGBYTES(sp)
    LREG   a6,    5 * REGBYTES(sp)
    LREG   a5,    6 * REGBYTES(sp)
    LREG   a4,    7 * REGBYTES(sp)
    LREG   a3,    8 * REGBYTES(sp)
    LREG   a2,    9 * REGBYTES(sp)
    LREG   a1,   10 * REGBYTES(sp)
    LREG   a0,   11 * REGBYTES(sp)
    LREG   t2,   12 * REGBYTES(sp)
    LREG   t1,   13 * REGBYTES(sp)
    LREG   t0,   14 * REGBYTES(sp)
    LREG   ra,   15 * REGBYTES(sp)
.endm

.macro POP_CALLER_REG
#ifdef LOSCFG_ARCH_FPU_ENABLE
    addi  sp, sp, 16 * REGBYTES
    POP_INTEGER_CALLER_REG
    addi  sp, sp, 28 * REGBYTES
    POP_FPU_CALLER_REG
    addi  sp, sp, 24 * REGBYTES
#else
    addi  sp, sp, 16 * REGBYTES
    POP_INTEGER_CALLER_REG
    addi  sp, sp, 16 * REGBYTES
#endif

.endm

.macro POP_IRQ_REG
    lw      t0, 1 * REGBYTES(sp)  /* retrieve the address at which exception happened */
    csrw    mepc, t0              /* load mepc from previously reseverd (4 * REGBYTES) space */
    lw      t0, 0 * REGBYTES(sp)  /* enable global interrupts */
    csrw    mstatus, t0           /* load mstatus from previously reseverd (4 * REGBYTES) space */
    POP_CALLER_REG
.endm

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

.macro POP_ALL_FPU_REG
    lw     t0, 64 * REGBYTES(sp)
    fscsr  t0
    addi   t0, sp, 32 * REGBYTES
    FLREG  fs11,  0 * REGBYTES(t0)
    FLREG  fs10,  1 * REGBYTES(t0)
    FLREG  fs9,   2 * REGBYTES(t0)
    FLREG  fs8,   3 * REGBYTES(t0)
    FLREG  fs7,   4 * REGBYTES(t0)
    FLREG  fs6,   5 * REGBYTES(t0)
    FLREG  fs5,   6 * REGBYTES(t0)
    FLREG  fs4,   7 * REGBYTES(t0)
    FLREG  fs3,   8 * REGBYTES(t0)
    FLREG  fs2,   9 * REGBYTES(t0)
    FLREG  fs1,  10 * REGBYTES(t0)
    FLREG  fs0,  11 * REGBYTES(t0)
    FLREG  ft11, 12 * REGBYTES(t0)
    FLREG  ft10, 13 * REGBYTES(t0)
    FLREG  ft9,  14 * REGBYTES(t0)
    FLREG  ft8,  15 * REGBYTES(t0)
    FLREG  fa7,  16 * REGBYTES(t0)
    FLREG  fa6,  17 * REGBYTES(t0)
    FLREG  fa5,  18 * REGBYTES(t0)
    FLREG  fa4,  19 * REGBYTES(t0)
    FLREG  fa3,  20 * REGBYTES(t0)
    FLREG  fa2,  21 * REGBYTES(t0)
    FLREG  fa1,  22 * REGBYTES(t0)
    FLREG  fa0,  23 * REGBYTES(t0)
    FLREG  ft7,  24 * REGBYTES(t0)
    FLREG  ft6,  25 * REGBYTES(t0)
    FLREG  ft5,  26 * REGBYTES(t0)
    FLREG  ft4,  27 * REGBYTES(t0)
    FLREG  ft3,  28 * REGBYTES(t0)
    FLREG  ft2,  29 * REGBYTES(t0)
    FLREG  ft1,  30 * REGBYTES(t0)
    FLREG  ft0,  31 * REGBYTES(t0)
.endm

.macro POP_ALL_INTEGER_REG
    LREG   s11,   4 * REGBYTES(sp)
    LREG   s10,   5 * REGBYTES(sp)
    LREG   s9,    6 * REGBYTES(sp)
    LREG   s8,    7 * REGBYTES(sp)
    LREG   s7,    8 * REGBYTES(sp)
    LREG   s6,    9 * REGBYTES(sp)
    LREG   s5,   10 * REGBYTES(sp)
    LREG   s4,   11 * REGBYTES(sp)
    LREG   s3,   12 * REGBYTES(sp)
    LREG   s2,   13 * REGBYTES(sp)
    LREG   s1,   14 * REGBYTES(sp)
    LREG   s0,   15 * REGBYTES(sp)
    LREG   t6,   16 * REGBYTES(sp)
    LREG   t5,   17 * REGBYTES(sp)
    LREG   t4,   18 * REGBYTES(sp)
    LREG   t3,   19 * REGBYTES(sp)
    LREG   a7,   20 * REGBYTES(sp)
    LREG   a6,   21 * REGBYTES(sp)
    LREG   a5,   22 * REGBYTES(sp)
    LREG   a4,   23 * REGBYTES(sp)
    LREG   a3,   24 * REGBYTES(sp)
    LREG   a2,   25 * REGBYTES(sp)
    LREG   a1,   26 * REGBYTES(sp)
    LREG   a0,   27 * REGBYTES(sp)
    LREG   t2,   28 * REGBYTES(sp)
    LREG   t1,   29 * REGBYTES(sp)
    LREG   t0,   30 * REGBYTES(sp)
    LREG   ra,   31 * REGBYTES(sp)
.endm

.macro POP_ALL_REG
    lw      t0, 1 * REGBYTES(sp)
    csrw    mepc, t0              /* load mepc from previously reseverd (4 * REGBYTES) space */
    lw      t0, 0 * REGBYTES(sp)
    csrw    mstatus, t0           /* load mstatus from previously reseverd (4 * REGBYTES) space */
#ifdef LOSCFG_ARCH_FPU_ENABLE
    POP_ALL_FPU_REG
    POP_ALL_INTEGER_REG
    addi   sp, sp, INT_SIZE_ON_STACK
#else
    POP_ALL_INTEGER_REG
    addi  sp, sp, INT_SIZE_ON_STACK
#endif
.endm

/*
 * a0: new task
 * a1: run task
 */
OsTaskSchedule:
    li      t0, LOS_MSTATUS_MIE | LOS_MSTATUS_MPIE
    csrrc   t1, mstatus, t0
    addi    sp, sp, -INT_SIZE_ON_STACK
    andi    t1, t1, LOS_MSTATUS_MIE
    ori     t1, t1, 0x180
    slli    t1, t1, 0x4
    csrs    mstatus, t1
    csrw    mepc, ra

SaveRunTask:
    PUSH_CALLEE_REG
#ifdef LOSCFG_ARCH_FPU_ENABLE
    addi   t0, sp, 64 * REGBYTES
    frcsr  t2
    SREG   t2,  0 * REGBYTES(t0)
#endif
    csrr    t0, mstatus
    SREG    t0, 0 * REGBYTES(sp)
    csrr    t0, mepc
    SREG    t0, 1 * REGBYTES(sp)

    sw      sp, TASK_CB_KERNEL_SP(a1)

/* a0: new task */
SwitchNewTask:
    /* retireve stack pointer */
    lw      sp, TASK_CB_KERNEL_SP(a0)

    /* retrieve the registers */
    POP_ALL_REG

    mret

/* riscv interrupt & exception entry */
.align 4
TrapVector:
#ifdef LOSCFG_ARCH_FPU_ENABLE
    /* reseverd (4*REGBYTES) for FPU, use 1 REGBYTES but sp must be 16 aligned when compile option -mpush-pop is on */
    addi   sp, sp, -(4 * REGBYTES)
#endif
    PUSH_CALLER_REG /* sp must be 16 bytes aligned, now size is 32 REGBYTES (64 REGBYTES when FPU on) */
#ifdef LOSCFG_ARCH_FPU_ENABLE
    frcsr  t2 /* use t2 after save it in PUSH_CALLER_REG */
    SREG   t2,  64 * REGBYTES(sp) /* store frcsr to to previously reseverd (4*REGBYTES) space for FPU */
#endif

    csrr    a0, mcause
    li      a1, MCAUSE_MASK_INT_BIT
    li      a2, MCAUSE_MASK_INT_NUM
    and     a1, a0, a1
    and     a0, a0, a2            /* interrupt num */

    beqz    a1, TrapHandle        /* mstatus and mepc will be saved in TrapHandle */

    csrr    t0, mstatus
    sw      t0, 0 * REGBYTES(sp)  /* store mstatus to previously reseverd (4*REGBYTES) space by PUSH_CALLER_REG */
    csrr    t0, mepc
    sw      t0, 1 * REGBYTES(sp)  /* store mepc to previously reseverd (4*REGBYTES) space by PUSH_CALLER_REG */


    csrw    mscratch, sp
#ifdef LOSCFG_LIB_CONFIGURABLE
    la      sp, g_irqStackTop
    lw      sp, 0(sp)
#else
    la      sp, __irq_stack_top
#endif
    call    IrqEntryRiscv
    csrr    sp, mscratch
    call    OsTaskProcSignal
    beqz    a0, NoSwitch

    call    OsSchedPreempt

NoSwitch:
    POP_IRQ_REG
    mret

/*
 * a0: new task
 */
.section .text
OsStartToRun:
    /* disable interrupts */
    li      t0, LOS_MSTATUS_MIE
    csrrc   zero, mstatus, t0

    /* set the task running status */
    li      t1, OS_TASK_STATUS_RUNNING
    sh      t1, TASK_CB_STATUS(a0)

    /* a0 is new task, save it on g_newTask */
    la      t1, g_newTask
    sw      a0, 0(t1)

    tail    SwitchNewTask

OsDisableIRQ:
    li      t0, (LOS_MSTATUS_MPIE | LOS_MSTATUS_MIE) /* mpie | mie */
    csrrc   zero, mstatus, t0
    ret

OsIntLock:
    csrr    a0, mstatus           /* return value */
    li      t0, LOS_MSTATUS_MIE   /* mie */
    csrrc   zero, mstatus, t0
    ret

OsIntUnLock:
    csrr    a0, mstatus           /* return value */
    li      t0, LOS_MSTATUS_MIE   /* mie */
    csrrs   zero, mstatus, t0
    ret

OsIntRestore:
    csrw mstatus, a0
    ret
