/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS System Start Module Implemention.
 * Author: Huawei LiteOS Team
 * Create: 2021-03-02
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

#ifndef _RESET_VECTOR_S
#define _RESET_VECTOR_S

#include "arch/cpu.h"

.extern TrapVector
.global __startup_stack_top
.global __irq_stack_top
.global __nmi_stack_top
.global __exc_stack_top
.global reset_vector
.extern BoardConfig

.option rvc
.section .entry.text
reset_vector:
    tail HandleReset

.section .text
.option rvc
HandleReset:
    la      t0, TrapVector
    csrw    mtvec, t0
    csrwi   mstatus, 0
    csrwi   mie, 0

#ifdef LOSCFG_ARCH_FPU_ENABLE
   /* set to initial state of FPU */
    li      t0, LOS_MSTATUS_FS
    csrs    mstatus, t0
    fssr    x0
#endif

    .option push
    .option norelax
    /* initialize global pointer */
    la      gp, __global_pointer$
    .option pop

    /* initialize stack pointer */
    la      sp, __startup_stack_top

    /* Board features. */
    call    BoardConfig

    /* jump to main func. */
    tail    main

.section ".int_stack", "wa", %nobits

__irq_stack:
    .space LOSCFG_IRQ_STACK_SIZE
__irq_stack_top:

__startup_stack:
    .space LOSCFG_STARTUP_STACK_SIZE
__startup_stack_top:

__exc_stack:
    .space LOSCFG_EXC_STACK_SIZE
__exc_stack_top:

__nmi_stack:
    .space LOSCFG_NMI_STACK_SIZE
__nmi_stack_top:

#endif
