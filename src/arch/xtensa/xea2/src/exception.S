/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xea2 Hw Exc Implementation
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

#include <xtensa/coreasm.h>
#include "arch/regs.h"
#include "arch/exception.h"
#include "arch/task.h"

    .section .os.text, "ax"
    .align  4
    .extern Cp1SaveRestore
OsCpExcHandler:
#ifdef LOSCFG_DSP_COPROCESS
    addi    a1, a1, -REG_CONTEXT_SIZE
    rsr     a2, excsave1                   /* restore a2 from excsave1 */
    s32i    a2, a1, REG_OFF_AR4            /* save a2 */
    s32i    a3, a1, REG_OFF_AR5            /* save a3 */
    s32i    a8, a1, REG_OFF_AR8
    s32i    a9, a1, REG_OFF_AR9
    s32i    a10, a1, REG_OFF_AR10
    s32i    a11, a1, REG_OFF_AR11
    s32i    a12, a1, REG_OFF_AR12
    s32i    a13, a1, REG_OFF_AR13
    s32i    a14, a1, REG_OFF_AR14
    s32i    a15, a1, REG_OFF_AR15

    rsr     a2, sar                         /* Successive Approximation Register */
    s32i    a2, a1, REG_OFF_SAR
    rsr     a2, br
    s32i    a2, a1, REG_OFF_BR
    rsr     a2, lcount
    s32i    a2, a1, REG_OFF_LCOUNT
    rsr     a2, lbeg
    s32i    a2, a1, REG_OFF_LBEG
    rsr     a2, lend
    s32i    a2, a1, REG_OFF_LEND
    rsr     a2, epc1                        /* get epc1 before possibly overflow */
    s32i    a2, a1, REG_OFF_PC              /* save epc1 */

    movi    a2, OS_PS_VECTOR_VALUE
    movi    a3, 0x00000020
    or      a2, a2, a3
    xsr     a2, ps                          /* PS.INTLEVEL=EXCMLEVEL */
    s32i    a2, a1, REG_OFF_PS              /* save PS */

    movi    a2, 0x02                        /* enable cp1 */
    wsr     a2, cpenable

    mov     a2, a1
    movi    a1, __stack - 48

    movi    a3, Cp1SaveRestore
    callx8  a3

    mov     a1, a2
    movi    a3, g_losTask                   /* set cp1 owner */
    l32i    a2, a3, 0
    movi    a3, g_cpOwner
    s32i    a2, a3, 0                       /* g_cpOwner=g_losTask.runTask */

    l32i    a2, a1, REG_OFF_PS              /* resore PS */
    xsr     a2, ps
    l32i    a2, a1, REG_OFF_PC              /* resore EPC1 */
    wsr     a2, epc1
    l32i    a2, a1, REG_OFF_BR
    wsr     a2, br
    l32i    a2, a1, REG_OFF_SAR
    wsr     a2, sar
    l32i    a2, a1, REG_OFF_LEND
    wsr     a2, lend
    l32i    a2, a1, REG_OFF_LBEG
    wsr     a2, lbeg
    l32i    a2, a1, REG_OFF_LCOUNT
    wsr     a2, lcount
    l32i    a2, a1, REG_OFF_AR4             /* restore common register */
    l32i    a3, a1, REG_OFF_AR5
    l32i    a8, a1, REG_OFF_AR8
    l32i    a9, a1, REG_OFF_AR9
    l32i    a10, a1, REG_OFF_AR10
    l32i    a11, a1, REG_OFF_AR11
    l32i    a12, a1, REG_OFF_AR12
    l32i    a13, a1, REG_OFF_AR13
    l32i    a14, a1, REG_OFF_AR14
    l32i    a15, a1, REG_OFF_AR15
    addi    a1, a1, REG_CONTEXT_SIZE
#endif
    rfe


    .align  4
    .extern OsExcHandleEntry
    .global ComExcVecExt
ComExcVecExt:
    movi    a3, OS_EXCCAUSE_COPROCESSOR_1
    bne     a2, a3, 1f
    l32i    a3, a1, REG_OFF_AR15            /* restore original a3 */
    addi    a1, a1, REG_CONTEXT_SIZE
    movi    a2, OsCpExcHandler              /* exccause is no use for COPROCESSER exception, so overwrite a2 */
    jx      a2
1:
    bnei    a2, OS_EXCCAUSE_ALLOCA_CAUSE, OsExceptionContextSave
    l32i    a3, a1, REG_OFF_AR15            /* restore original a3 */
    addi    a1, a1, REG_CONTEXT_SIZE        /* restore original a1 */
    movi    a2, OsAllocaExc                 /* exccause is no use for ALLOCA exception, so overwrite a2 */
    jx      a2

OsExceptionContextSave:
    l32i    a3, a1, REG_OFF_AR15            /* restore original a3 */
    addi    a1, a1, (EXC_REG_OFF_EXCVADDR - EXC_REG_OFF_EPC1)
    /* Save EXCVADDR and a0-a3*/
    rsr     a2, excvaddr
    s32i    a2, a1, EXC_REG_OFF_EXCVADDR
    s32i    a0, a1, EXC_REG_OFF_AR0
    addi    a2, a1, (EXC_REG_SIZE + 32)     /* a2's value is sp when exc occurs, 32: 112+20-100 */
    s32i    a2, a1, EXC_REG_OFF_AR1
    rsr     a2, excsave1                    /* restore original a2 */
    s32i    a2, a1, EXC_REG_OFF_AR2
    s32i    a3, a1, EXC_REG_OFF_AR3
    /* Exception state */
    movi    a3, OS_PS_VECTOR_VALUE
    xsr     a3, ps                          /* PS.INTLEVEL=EXCMLEVEL */
    rsync                                   /* wait for WSR to PS to complete */
    s32i    a3, a1, EXC_REG_OFF_PS          /* save PS */
    rsr     a3, exccause
    s32i    a3, a1, EXC_REG_OFF_EXCCAUSE
    rsr     a3, epc1                        /* get epc1 before possibly overflow */
    s32i    a3, a1, EXC_REG_OFF_EPC1        /* save epc1 */
    rsr     a3, br
    s32i    a3, a1, EXC_REG_OFF_BR
    /* Save a4-a7 */
    s32i    a4, a1, EXC_REG_OFF_AR4
    s32i    a5, a1, EXC_REG_OFF_AR5
    s32i    a6, a1, EXC_REG_OFF_AR6
    s32i    a7, a1, EXC_REG_OFF_AR7
    /* Save a8-a11 */
    s32i    a8, a1, EXC_REG_OFF_AR8         /* maybe lead to that the epc1 modified */
    s32i    a9, a1, EXC_REG_OFF_AR9
    s32i    a10, a1, EXC_REG_OFF_AR10
    s32i    a11, a1, EXC_REG_OFF_AR11
    /* Save a12-a15 */
    s32i    a12, a1, EXC_REG_OFF_AR12
    s32i    a13, a1, EXC_REG_OFF_AR13
    s32i    a14, a1, EXC_REG_OFF_AR14
    s32i    a15, a1, EXC_REG_OFF_AR15
    /* Zero overlap loop register */
    rsr     a3, lbeg
    s32i    a3, a1, EXC_REG_OFF_LBEG
    rsr     a3, lend
    s32i    a3, a1, EXC_REG_OFF_LEND
    rsr     a3, lcount
    s32i    a3, a1, EXC_REG_OFF_LCOUNT
    rsr     a3, sar
    s32i    a3, a1, EXC_REG_OFF_SAR

    wsr     a1, excsave1

#if (XCHAL_NUM_AREGS == 64)
    /* Rotate all the physical registers */
    mov a12, a0
    rotw 3
    mov a12, a0
    rotw 3
    mov a12, a0
    rotw 3
    mov a12, a0
    rotw 3
    mov a12, a0
    rotw 4
#else
    /* Rotate all the physical registers */
    mov a12, a0
    rotw 3
    mov a12, a0
    rotw 3
    mov a7, a0
    rotw 2
#endif

    movi a2, 0x50030 | OS_PS_INTLEVEL(OS_INTLEVEL_EXCM)
    wsr a2, ps
    /* rotate window, all regs' value will be changed afterwards */
    movi a2, 0x0
    wsr a2, WindowBase
    movi a2, 1
    wsr a2, WindowStart

    /* All exception saved, initialize stack and jump to c handler */
    movi a1, OsExcHandleEntry
    wsr a1, epc1
	/* switch to exc stack, actually start stack or irq stack */
    movi a5, __stack - 48

    movi a2, 0xfffffff0
    and a5, a5, a2
    addi a5, a5, -32
    mov a1, a5
    rsr  a6, EXCCAUSE
    rsr     a7, excsave1
    rfe

    .text
