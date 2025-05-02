/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Low Level Impelmentation
 * Author: Huawei LiteOS Team
 * Create: 2021-01-14
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

#include "arch/task.h"
#include "arch/regs.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_BSS LosTask g_losTask;

#define OS_PS_INIT_VALUE        (OS_PS_WOE | OS_PS_CALLINC(1) | OS_PS_UM | OS_PS_EXCM | OS_PS_INTLEVEL(0))
#define REGISTER_A6_ARRAY_NUM   2

VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack)
{
    UINT32 endStack, i;
    TaskContext *context = NULL;

    /* initialize the task stack, write magic num to stack top */
    OsStackInit(topStack, stackSize);

    endStack = TRUNCATE(((UINT32)(UINTPTR)topStack + stackSize), LOSCFG_STACK_POINT_ALIGN_SIZE);
    context = (TaskContext *)(UINTPTR)(endStack - REG_CONTEXT_SIZE);
    /* initialize the task context */
    (VOID)memset_s(context, REG_CONTEXT_SIZE, 0, REG_CONTEXT_SIZE);

    *((UINT32 *)((UINTPTR)context - OS_HW_AR1_SAVE_OFFSET)) = endStack;   /* a1 of the context's parent */
    *((UINT32 *)((UINTPTR)context - OS_HW_AR0_SAVE_OFFSET)) = 0;          /* a0 of the context's parent */
    *((UINT32 *)(UINTPTR)(endStack - OS_HW_AR1_SAVE_OFFSET)) = endStack;  /* a1 of the context's grandparent */
    *((UINT32 *)(UINTPTR)(endStack - OS_HW_AR0_SAVE_OFFSET)) = 0;         /* a0 of the context's grandparent */

    context->pc = (UINTPTR)OsTaskEntry;
    context->ps = (UINTPTR)OS_PS_INIT_VALUE;
    context->excCause = OS_EXCCAUSE_INITIAL;
    for (i = 1; i < TASK_CONTEXT_REG_NUM; i++) {  /* 1: a0 of OsTaskEntry is 0 */
        context->arg[i] = i + 4;                  /* 4: reg Offset */
    }
    context->arg[REGISTER_A6_ARRAY_NUM] = taskId; /* arg[2] is register a6,a6 save taskId */

    return (VOID *)context;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
