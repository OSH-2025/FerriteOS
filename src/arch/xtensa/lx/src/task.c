/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Arch32 C-Sky Hw Task Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-28
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

#include "los_hwi_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define STACK_ALING    (0xF)

/* Save the TaskTcb of the new task */
VOID *g_newTask = NULL;

UINT32 g_stackDefault[] = {
    0x00000000,     /* REG_OFF_PC */
    0x00000000,     /* REG_OFF_PS */
    0x00000A00,     /* REG_OFF_AR00 */
    0x00000A01,     /* REG_OFF_AR01 */
    0x00000A02,     /* REG_OFF_AR02 */
    0x00000A03,     /* REG_OFF_AR03 */
    0x00000A04,     /* REG_OFF_AR04 */
    0x00000A05,     /* REG_OFF_AR05 */
    0x00000A06,     /* REG_OFF_AR06 */
    0x00000A07,     /* REG_OFF_AR07 */
    0x00000A08,     /* REG_OFF_AR08 */
    0x00000A09,     /* REG_OFF_AR09 */
    0x00000A10,     /* REG_OFF_AR10 */
    0x00000A11,     /* REG_OFF_AR11 */
    0x00000A12,     /* REG_OFF_AR12 */
    0x00000A13,     /* REG_OFF_AR13 */
    0x00000A14,     /* REG_OFF_AR14 */
    0x00000A15,     /* REG_OFF_AR15 */
    0x00000000,     /* REG_OFF_RESERVED */
    0x00000000,     /* REG_OFF_EXCCAUSE */
    0x00000000,     /* REG_OFF_EXCVASSR */
#ifdef LOSCFG_XTENSA_LOOPS
    0x00000000,     /* REG_OFF_LCOUNT */
    0x00000000,     /* REG_OFF_LEND */
    0x00000000,     /* REG_OFF_LBEG */
#endif
#ifdef LOSCFG_ARCH_FPU_ENABLE
    0x00000000,     /* REG_OFF_TMP0 */
    0x00000000,     /* REG_OFF_CPENABLE | CONTEXT_OFF_CPSTORED */
    0x00000000,     /* REG_OFF_FCR */
    0x00000000,     /* REG_OFF_FSR */
    0x00000000,     /* REG_OFF_F0 */
    0x00000000,     /* REG_OFF_F1 */
    0x00000000,     /* REG_OFF_F2 */
    0x00000000,     /* REG_OFF_F3 */
    0x00000000,     /* REG_OFF_F4 */
    0x00000000,     /* REG_OFF_F5 */
    0x00000000,     /* REG_OFF_F6 */
    0x00000000,     /* REG_OFF_F7 */
    0x00000000,     /* REG_OFF_F8 */
    0x00000000,     /* REG_OFF_F9 */
    0x00000000,     /* REG_OFF_F10 */
    0x00000000,     /* REG_OFF_F11 */
    0x00000000,     /* REG_OFF_F12 */
    0x00000000,     /* REG_OFF_F13 */
    0x00000000,     /* REG_OFF_F14 */
    0x00000000,     /* REG_OFF_F15 */
#endif /* LOSCFG_ARCH_FPU_ENABLE */
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    0x00000000,     /* REG_OFF_SPILL_RESERVED */
    0x00000000,     /* REG_OFF_SPILL_RESERVED */
    0x00000000,     /* REG_OFF_SPILL_RESERVED */
    0x00000000,     /* REG_OFF_SPILL_RESERVED */
#endif
};

LITE_OS_SEC_TEXT_MINOR VOID OsTaskExit(VOID)
{
    LOS_IntLock();
    while (1) {
    }
}

LITE_OS_SEC_TEXT_INIT VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack)
{
    TaskContext *taskContext = NULL;
    errno_t result;

    result = memset_s(topStack, stackSize, (INT32)(OS_STACK_INIT & 0xFF), stackSize);
    if (result != EOK) {
        printf("memset_s is failed:%s[%d]\r\n", __FUNCTION__, __LINE__);
    }
    *((UINT32 *)(topStack)) = OS_STACK_MAGIC_WORD;

    taskContext = (TaskContext *)(((((UINTPTR)topStack + stackSize) - sizeof(TaskContext))) & ~STACK_ALING);
    /* initialize the task context */
    result = memcpy_s(taskContext, sizeof(TaskContext), g_stackDefault, sizeof(TaskContext));
    if (result != EOK) {
        PRINT_ERR("[%s] memcpy_s failed!\n", __func__);
    }

    taskContext->pc = (UINT32)(UINTPTR)OsTaskEntry;
    taskContext->regA[INDEX_OF_SP] = ((UINTPTR)topStack + stackSize) & ~STACK_ALING;        /* endStack */
    taskContext->regA[INDEX_OF_ARGS0] = taskId;                                             /* argument1 */
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    taskContext->ps = SPREG_PS_STACK_CROSS | WOE_ENABLE | 1 << BIT_CALLINC; /* set to kernel stack */
#else
    taskContext->ps = SPREG_PS_STACK_CROSS; /* set to kernel stack */
#endif
#ifdef LOSCFG_ARCH_FPU_ENABLE
    taskContext->cpenable = 1; /* 1: enable */
#endif

    return (VOID *)taskContext;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

