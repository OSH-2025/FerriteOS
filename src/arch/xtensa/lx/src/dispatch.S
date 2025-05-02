/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Lx6 Dispatch Implementation
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
.equ OS_TASK_STATUS_RUNNING,       0x0010U
.section .text
.literal_position
.align      4
.type OsTaskSchedule, %function
.global OsTaskSchedule
/*
 * a2: new task
 * a3: run task
 */
OsTaskSchedule:
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    entry   sp, 16
#endif
    mov     a4, a2
    mov     a5, a3
    addi    a2, sp, -CONTEXT_SIZE    
    PUSH_ALL_REG a2 
    rsil    a3, INT_MASK
    s32i    a3, a2, CONTEXT_OFF_PS   
    call0   SaveRetAddr    
    beqi    a3, 1, .switchdone    

    s32i    a2, a5, 0 /* store old sp */
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    l32i    a4, a4, 0 /* get new sp */
    rsr     a5, PS
    movi    a3, ~(WOE_ENABLE | LEVEL_MASK)
    and     a2, a5, a3    
    addi    a2, a2, 3
    wsr     a2, PS
    rsync
    call0   SpillWindow
    mov     a2, a4
#else
    l32i    a2, a4, 0
#endif
    POP_ALL_REG a2 EPC2 EPS2
    rfi     2

.switchdone:
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    retw
#else
    ret
#endif

#ifdef LOSCFG_XTENSA_WINDOWSPILL
.type HakSpillWindow, %function
.global HakSpillWindow
.align      4
HakSpillWindow:

    entry   sp, 16

    addi    a2, sp, -CONTEXT_SIZE
    PUSH_ALL_REG a2
    
    rsr     a5, PS
    movi    a3, ~(WOE_ENABLE | LEVEL_MASK)  /* disable woe and int */
    and     a3, a5, a3    
    addi    a3, a3, LEVEL3
    wsr     a3, PS
    rsync

    mov     a4, a2
    call0   SpillWindow

    l32i    a5, a4, CONTEXT_OFF_PS /* restroe PS */
    wsr     a5,  PS
    rsync
    l32i    a0,  a4, CONTEXT_OFF_A0
    l32i    a1,  a4, CONTEXT_OFF_A1

    retw
#endif
/*
 * a2: new task
 */
.type OsStartToRun, %function
.global OsStartToRun
.align      4
OsStartToRun:
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    entry  sp, 16
#endif
    /* disable interrupt */
    rsil    a3, INT_MASK
    
    /* set the task running status */
    movi    a3, OS_TASK_STATUS_RUNNING
    s32i    a3, a2, 4

    /* a2 is new task, save it on g_newTask */
    movi    a3, g_newTask
    s32i    a2, a3, 0

#ifdef LOSCFG_XTENSA_WINDOWSPILL
    l32i    a5, a2, 0   /* get sp */

    rsr     a4, PS
    movi    a3, ~(WOE_ENABLE | LEVEL_MASK)
    and     a2, a4, a3    
    addi    a2, a2, LEVEL3
    wsr     a2, PS
    rsync

    call0   SpillWindow

    mov     a2, a5
#else 
    l32i    a2, a2, 0   /* get sp */
#endif
    POP_ALL_REG a2 EPC2 EPS2
    rfi     2

.global SaveRetAddr
.type   SaveRetAddr, @function
.literal_position
.align  4

SaveRetAddr:

    movi    a3,  1
    s32i    a3,  a2, CONTEXT_OFF_A3
    s32i    a0,  a2, CONTEXT_OFF_PC    /* save pc */
    movi    a3, 0
    ret

LinkHandleEntry:
    call0 HandleEntry