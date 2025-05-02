/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xea2 Dispatch Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-01-01
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

#include <xtensa/config/core.h>
#include "arch/regs.h"
#include "arch/task.h"
#include "asm/interrupt_config.h"

    .section .os.init.text, "ax"
    .align  4
    .type OsStartToRun, @function
    .global OsStartToRun
OsStartToRun:
#if (XCHAL_NUM_AREGS == 64)
    entry   a1, 80
    xor     a8, a8, a8        /* a8 = 0 */
    rotw    2
    mov     a8, a0
    rotw    2
    mov     a8, a0
    rotw    2
    mov     a8, a0
    rotw    2                 /* roll back to the begin point, a0 no need to update */
    mov     a8, a0
    rotw    2
    mov     a8, a0
    rotw    2
    mov     a8, a0
    rotw    4
#else
    entry   a1, 48
    xor     a8, a8, a8        /* a8 = 0 */
    rotw    2
    mov     a8, a0
    rotw    2
    mov     a8, a0
    rotw    2
    rotw    2                 /* roll back to the begin point, a0 no need to update */
#endif

    movi    a15, __stack
    addi    a14, a15, -16
    s32i    a8, a14, 0        /* a0 */
    s32i    a15, a14, 4       /* a1 */
    addi    a14, a15, -64
    s32i    a8, a14, 0        /* a0 */
    s32i    a15, a14, 4       /* a1 */

    /* disable interrupts and set user vector mode */
    movi    a4, (OS_PS_UM | OS_INTLEVEL_EXCM)
    rsr     a3, ps
    or      a3, a3, a4
    wsr     a3, ps
    rsync
    /* a2 is new task */
    movi    a10, g_losTask
    s32i    a2, a10, 4        /* g_losTask.runTask = a2 */
    s32i    a2, a10, 0        /* g_losTask.newTask = a2 */

    /* switch to newtask */
    movi    a4, OsSwitchToNewTask
    jx      a4

    .text


    .section        .os.data, "a"
    .align    4
    .global g_intMask
g_intMask:
    .word   OS_INTLEVEL0_MASK
    .word   OS_INTLEVEL1_MASK
    .word   OS_INTLEVEL2_MASK
    .word   OS_INTLEVEL3_MASK
    .word   OS_INTLEVEL4_MASK
    .word   OS_INTLEVEL5_MASK
    .word   OS_INTLEVEL6_MASK
    .word   OS_INTLEVEL7_MASK


    .section .os.kernel.text, "ax"
    .global g_losTask

/* Save common context, include a4~a15, lbeg, lend, lcount, sar */
.macro OsSaveCmmContext
    s32i    a4, a1, REG_OFF_AR8
    s32i    a5, a1, REG_OFF_AR9
    s32i    a6, a1, REG_OFF_AR10
    s32i    a7, a1, REG_OFF_AR11
    s32i    a8, a1, REG_OFF_AR12
    s32i    a9, a1, REG_OFF_AR13
    s32i    a10, a1, REG_OFF_AR14
    s32i    a11, a1, REG_OFF_AR15

    rsr     a15, sar
    s32i    a15, a1, REG_OFF_SAR

    rsr     a15, br
    s32i    a15, a1, REG_OFF_BR

    rsr     a15, lcount
    s32i    a15, a1, REG_OFF_LCOUNT
    rsr     a15, lbeg
    s32i    a15, a1, REG_OFF_LBEG
    rsr     a15, lend
    s32i    a15, a1, REG_OFF_LEND
.endm


    .align  4
    .global OsKernelHwiHandler
    .type   OsKernelHwiHandler, @function
OsKernelHwiHandler:
1:
    j 1b


    .align  4
    .global OsUserHwiHandler
    .type   OsUserHwiHandler, @function
    .extern IrqEntryXea2
OsUserHwiHandler:
    entry   a1, REG_CONTEXT_SIZE

    OsSaveCmmContext

    movi    a2, OS_EXCCAUSE_INT
    s32i    a2, a1, REG_OFF_EXCCAUSE

#if (XCHAL_NUM_AREGS == 64)
    /* trigger possibly overflow, to save registers into stack */
    movi    a12, 0
    rotw    3
    mov     a12, a0
    rotw    3
    mov     a12, a12
    rotw    3
    mov     a12, a12
    rotw    3
    mov     a12, a12

    rotw    4                 /* roll back to the begin point */
#else
    /* trigger possibly overflow, to save registers into stack */
    movi    a12, 0
    rotw    3
    mov     a12, a0
    rotw    3
    mov     a7,  a7
    rotw    2                 /* roll back to the begin point */
#endif
    /* save a1 to excsave1, not g_losTask.runTask->stackPointer */
    wsr     a1, excsave1
	/* switch from task stack to int stack */
    movi    a1, __stack - 48

OsHwiDispatcher:
    movi    a4, g_intMask
    addx4   a4, a3, a4                      /* a4 = INTLEVEL_MASK */
    l32i    a4, a4, 0                       /* a3 = INTLEVEL, that is a7 before rotw 1 */
    rsr     a15, interrupt
    rsr     a14, intenable
    and     a15, a15, a4
    and     a15, a15, a14                   /* a15 = cur int state = interrupt & intenable & g_intMask */

OsHwiScanLoop:
    neg     a8, a15                         /* a8 = -a15 */
    and     a8, a8, a15                     /* get the rightmost bit of current interrupts state */
    wsr     a8, intclear                    /* clear this interrupt */

    nsau    a8, a8                          /* get index of this interrupt */
    movi    a7, 0x1f                        /* 0x1f: size of 32-bit register */
    sub     a7, a7, a8                      /* maybe a8 is 32 when CLZ(0), then a7 large number */

    mov     a10, a7                         /* a7 is int number */
    call8   IrqEntryXea2                    /* need to check a7 in c-funciton */

    rsr     a15, interrupt
    rsr     a14, intenable
    and     a15, a15, a4
    and     a15, a15, a14                   /* a15 = cur int state = interrupt & intenable & g_intMask */
    bnez    a15, OsHwiScanLoop              /* a15 still non-zero, goto OsHwiScanLoop */

IrqExit:
    /* switch from int stack to task stack */
    rsr     a1, excsave1

    movi    a14, OsSchedProcSchedFlag
    callx8  a14
    /* pop irq */
    j       OsThrdContextLoad


/*
 * a2: new task
 * a3: run task
 */
    .align  4
    .global OsTaskSchedule
OsTaskSchedule:
#if (XCHAL_NUM_AREGS == 64)
    /* 32: stack size (at least 32B for callx8 OsTaskSchedule) */
    entry   a1, 32
    movi    a14, 0x70004
    xsr     a14, ps
    rsync
    movi    a15, 0
    xor     a12, a12, a12
    rotw    3
    mov     a12,  a12
    rotw    3
    movi    a15, 0
    rotw    3
    mov     a12, a12
    rotw    3
    movi    a15, 0
    rotw    -3
    rotw    -3
    rotw    -3
    rotw    -3
#else
    entry   a1, 32
    movi    a14, 0x70004
    xsr     a14, ps
    rsync
    movi    a15, 0
    xor     a12, a12, a12
    rotw    3
    mov     a12,  a12
    rotw    -3
#endif

SaveRunTask:
    addi    a1, a1, -FASTSWITCH_REG_CONTEXT_SIZE
    s32i    a0, a1, REG_OFF_PC
    s32i    a14, a1, REG_OFF_PS
    /* save br */
    rsr     a3, br
    s32i    a3, a1, REG_OFF_BR
    /* save exccause */
    movi    a3, OS_EXCCAUSE_SWITCH          /* go to OsFastContextLoad */
    s32i    a3, a1, REG_OFF_EXCCAUSE
    /* save a1 to g_losTask.runTask->stackPointer */
    movi    a10, g_losTask
    l32i    a7, a10, 0                      /* a7 = g_losTask.runTask */
    s32i    a1, a7, 0                       /* g_losTask.runTask->stackPointer = a1 */

OsSwitchToNewTask:
    l32i    a6, a10, 4                      /* a6 = g_losTask.newTask's value */
    s32i    a6, a10, 0                      /* g_losTask.runTask = g_losTask.newTask */
    /* switch to newTask's sp */
    l32i    a1, a6, 0                       /* a1 = g_losTask.newTask->stackPointer */

OsSwitchModeCheck:
    /* if g_cpOwner is g_losTask.runTask, then EnableCP1, else DisableCP1 */
    movi    a4, g_cpOwner                   /* a4 = &g_cpOwner */
    l32i    a5, a4, 0                       /* a5 = g_cpOwner */
    movi    a2, 0                           /* a2 default to 0 */
    movi    a3, 2                           /* a3 = 0x2 */
    sub     a5, a5, a6                      /* a5 = g_cpOwner - g_losTask.runTask */
    moveqz  a2, a3, a5                      /* set a3(value: 0x2) to a2 if a5 is 0 */
#ifdef LOSCFG_DSP_COPROCESS
    wsr     a2, cpenable                    /* cpenable = (g_cpOwner == g_losTask.runTask) ? 0x2 : 0x0 */
#endif
    /* if task firstly sched, go OsThrdContextLoad, else go OsFastContextLoad */
    l32i    a7, a1, REG_OFF_EXCCAUSE
    addi    a7, a7, -OS_EXCCAUSE_SWITCH
    beqz    a7, OsFastContextLoad
    j       OsThrdContextLoad

OsFastContextLoad:
    l32i    a2, a1, REG_OFF_BR
    l32i    a0, a1, REG_OFF_PC

    wsr     a2, br
    l32i    a3, a1, REG_OFF_PS
    movi    a2, 0
    addi    a1, a1, FASTSWITCH_REG_CONTEXT_SIZE
    wsr     a3, ps
    rsync
    retw

OsThrdContextLoad:
    l32i    a4, a1, REG_OFF_AR8
    l32i    a5, a1, REG_OFF_AR9
    l32i    a6, a1, REG_OFF_AR10
    l32i    a7, a1, REG_OFF_AR11
    l32i    a8, a1, REG_OFF_AR12
    l32i    a9, a1, REG_OFF_AR13
    l32i    a10, a1, REG_OFF_AR14
    l32i    a11, a1, REG_OFF_AR15

    l32i    a15, a1, REG_OFF_BR
    wsr     a15, br
    l32i    a12, a1, REG_OFF_SAR
    l32i    a13, a1, REG_OFF_LCOUNT
    l32i    a14, a1, REG_OFF_LBEG
    l32i    a15, a1, REG_OFF_LEND
    wsr     a12, sar
    wsr     a13, lcount
    wsr     a14, lbeg
    wsr     a15, lend
    /* force this function called by call4 */
    movi    a14, 0xC0000000
    movi    a0, OsReturnFromExc
    or      a0, a14, a0
    addx2   a0, a14, a0                     /* a0 = (a14 << 0x1) + a0 */

    /*
     * retw return to the address defined by a0(= OsReturnFromExc) and ps.callinginc will be changed to 0x1.
     * retw will trigger window underflow like rotw -1, because all windows have been triggerd overflow before.
     * Then, sp -16 ~ sp - 4 are restored to a0 ~ a3, e.g. 0 -> a0, endStack -> a1 that is defined in OsTaskStackInit
     * when a task firstly scheduled.
     */
    retw

OsReturnFromExc:
    /* restore epc1 and ps */
    l32i    a4, a5, REG_OFF_PC
    l32i    a7, a5, REG_OFF_PS
    movi    a6, OS_PS_EXCM
    wsr     a4, epc1            /* epc1 = OsTaskEntry or interrupted addr */
    or      a7, a7, a6
    wsr     a7, ps
    rsync
    /* restore a4 ~ a7 */
    l32i    a4, a5, REG_OFF_AR4 /* e.g. arg[0]: a4 = 0 defined in OsTaskStackInit when a task firstly scheduled */
    l32i    a6, a5, REG_OFF_AR6 /* e.g. arg[2]: a6 = taskId when a task firstly scheduled */
    l32i    a7, a5, REG_OFF_AR7
    l32i    a5, a5, REG_OFF_AR5

    /*
     * rfe return to the address defined by epc1(OsTaskEntry or interrupted addr) and clear ps.excm.
     * Considered for a running task is interrupted by an irq requests:
     * a1 and window base usually don't change after rfe.
     * But for a task firstly scheduled:
     * a1 and window base doesn't change after rfe, until executing 'entry a1, 64' in OsTaskEntry,
     * which means a1 -= 64 and window base will rotw +1 according to ps.callinginc = 1 like callx4,
     * so a4 ~ a7(cur win) changed to a0 ~ a3(next win), a8 ~ a15(cur win) changed to a4 ~ a11(next win).
     */
    rfe

    .text
