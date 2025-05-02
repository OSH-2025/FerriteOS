/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: LiteOS hw Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2018-05-12
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
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID *g_newTask = NULL;
VOID *g_runTask = NULL;
UINT32 *g_losTaskLock = NULL;

UINT32 g_stackDefault[] = {
    0x00000000,     /* REG_OFF_RESERVED */
    0x00000000,     /* REG_OFF_LCOUNT */
    0x00000000,     /* REG_OFF_LEND */
    0x00000000,     /* REG_OFF_LBEG */
    0x00000000,     /* REG_OFF_PC */
    0x00000000,     /* REG_OFF_PS */
    0x00000000,     /* REG_OFF_EXCCAUSE */
    0x00000000,     /* REG_OFF_EXCVASSR */
    0x00000A08,     /* REG_OFF_AR08 */
    0x00000A09,     /* REG_OFF_AR09 */
    0x00000A10,     /* REG_OFF_AR10 */
    0x00000A11,     /* REG_OFF_AR11 */
    0x00000A12,     /* REG_OFF_AR12 */
    0x00000A13,     /* REG_OFF_AR13 */
    0x00000A14,     /* REG_OFF_AR14 */
    0x00000A15,     /* REG_OFF_AR15 */
    0x00000A00,     /* REG_OFF_SPILL_AR00 */
    0x00000A01,     /* REG_OFF_SPILL_AR01 */
    0x00000A02,     /* REG_OFF_SPILL_AR02 */
    0x00000A03,     /* REG_OFF_SPILL_AR03 */
    0x00000A04,     /* REG_OFF_SPILL_AR04 */
    0x00000A05,     /* REG_OFF_SPILL_AR05 */
    0x00000A06,     /* REG_OFF_SPILL_AR06 */
    0x00000A07,     /* REG_OFF_SPILL_AR07 */
};

VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack)
{
    errno_t rc;
    UINT32 endStack;
    TaskContext *context = NULL;

    /* initialize the task stack, write magic num to stack top */
    OsStackInit(topStack, stackSize);

    endStack = TRUNCATE(((UINT32)(UINTPTR)topStack + stackSize), LOSCFG_STACK_POINT_ALIGN_SIZE);
    context = (TaskContext *)(VOID *)(UINTPTR)(endStack - sizeof(TaskContext));

    /* initialize the task context */
    rc = memcpy_s(context, sizeof(TaskContext), g_stackDefault, sizeof(TaskContext));
    if (rc != EOK) {
        PRINT_ERR("[%s] memcpy_s failed!\n", __func__);
    }

    context->pc       = (UINT32)(UINTPTR)OsTaskEntry;
    context->regA[0]  = 0;             /* a0 of OsTaskEntry */
    context->regA[1]  = endStack;      /* endStack */
    context->regA[2]  = taskId;        /* Argument1 */
    context->ps       = SPREG_PS_STACK_FIRSTKER;   /* set to kernel stack */

    return (VOID *)context;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

