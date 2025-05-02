/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: C-Sky Exception Handler Implementation
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

#include "arch/exception.h"
#include "arch/interrupt.h"
#include "vic.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif
#include "los_task_pri.h"
#include "los_hwi_pri.h"
#include "securec.h"
#include "los_printf_pri.h"
#include "los_memory_pri.h"

#ifdef LOSCFG_KERNEL_TRACE
#include "los_trace_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TRAP0_HANDLER_VEC   22
#define POINTER_SIZE        4
#define MASK_8_BITS         0xff
#define ALGIN_CODE          2
#define STACK_OFFSET        4
#define EXC_VEC_MASK        0x1f

extern CHAR __ram_end;
static const int text_start = (const int)&__text_start;
static const int text_end = (const int)&__text_end;

UINT32 g_curNestCount = 0;
CHAR g_trapstackbase[OS_TRAP_STACK_SIZE];
ExcInfo g_excInfo;

STATIC const CHAR *g_phaseName[] = {
    "fault in init",
    "fault in task",
    "fault in interrupt"
};

STATIC const CHAR *g_excTypeString[] = {
    "Reset",
    "Unalgin",
    "Fetch error",
    "Reserved",
    "Illegal instruction",
    "Privilege violation",
    "Reserved",
    "Breakpoint",
    "Unrecoverable",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Trap 0",
    "Trap 1",
    "Trap 2",
    "Trap 3",
    "Reserved",
    "Tspend",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

STATIC VOID OsExcSysInfo(VOID)
{
    LosTaskCB *runTask = OsCurrTaskGet();

    if (runTask != NULL) {
        PrintExcInfo("TaskName = %s\n"
                     "TaskId = %u\n"
                     "Task stackSize = %u\n",
                     runTask->taskName,
                     runTask->taskId,
                     runTask->stackSize);
    }
}

LITE_OS_SEC_TEXT_INIT VOID OsExcInfoDisplay(const ExcInfo *exc, const ExcContext *excBufAddr)
{
    PrintExcInfo("Phase      = %s\n"
                 "Type       = 0x%x\n"
                 "ExcType    = %s\n"
                 "FaultAddr  = 0x%x\n"
                 "IntNumOrTaskId = 0x%x\n"
                 "R0         = 0x%x\n"
                 "R1         = 0x%x\n"
                 "R2         = 0x%x\n"
                 "R3         = 0x%x\n"
                 "R4         = 0x%x\n"
                 "R5         = 0x%x\n"
                 "R6         = 0x%x\n"
                 "R7         = 0x%x\n"
                 "R8         = 0x%x\n"
                 "R9         = 0x%x\n"
                 "R10        = 0x%x\n"
                 "R11        = 0x%x\n"
                 "R12        = 0x%x\n"
                 "R13        = 0x%x\n"
                 "R14        = 0x%x\n"
                 "R15        = 0x%x\n"
                 "EPSR       = 0x%x\n"
                 "EPC        = 0x%x\n",
                 g_phaseName[exc->phase], exc->type, g_excTypeString[exc->type], exc->faultAddr, exc->idx,
                 excBufAddr->R0, excBufAddr->R1, excBufAddr->R2, excBufAddr->R3, excBufAddr->R4, excBufAddr->R5,
                 excBufAddr->R6, excBufAddr->R7, excBufAddr->R8, excBufAddr->R9, excBufAddr->R10, excBufAddr->R11,
                 excBufAddr->R12, excBufAddr->R13, excBufAddr->R14, excBufAddr->R15, excBufAddr->EPSR,
                 excBufAddr->EPC);
    return;
}

VOID OsExcHandleEntry(const ExcContext *excSp, UINT32 faultAddr)
{
    g_curNestCount++;
    g_excInfo.nestCnt = (UINT16)g_curNestCount;
    UINT32 excIndex = (ArchGetPsr() >> PSR_VEC_OFFSET) & MASK_8_BITS;
    INT32 IntNum = ((excSp->EPSR >> PSR_VEC_OFFSET) & MASK_8_BITS) - OS_SYS_VECTOR_CNT;

    g_excInfo.type = excIndex & EXC_VEC_MASK;
    g_excInfo.faultAddr = faultAddr;
    LosTaskCB *currentTask = ArchCurrTaskGet();
    if (currentTask->stackSize != 0) {
        if (IntNum >= 0) {
            g_excInfo.phase = OS_EXC_IN_HWI;
            g_excInfo.idx = IntNum;
        } else {
            g_excInfo.phase = OS_EXC_IN_TASK;
            g_excInfo.idx = currentTask->taskId;
            OsExcSysInfo();
        }
    } else {
        g_excInfo.phase = OS_EXC_IN_INIT;
        g_excInfo.idx = g_excInfo.type;
    }

    OsExcInfoDisplay((const ExcInfo *)&g_excInfo, (const ExcContext *)(excSp));

    ArchBackTraceWithSp((const VOID *)(excSp->R14));
#ifdef LOSCFG_KERNEL_TRACE
    OsTraceRecordDump(FALSE);
#endif
    while (1) {
    }
}

/* this function is used to validate sp or validate the checking range start and end. */
STATIC INLINE BOOL IsValidSP(UINTPTR regSP, UINTPTR start, UINTPTR end)
{
    return (regSP > start) && (regSP < end);
}

STATIC INLINE BOOL FindSuitableStack(UINTPTR *regSP, UINTPTR *start, UINTPTR *end)
{
    UINT32 index, topOfStack, stackBottom;
    BOOL found = FALSE;
    LosTaskCB *taskCB = NULL;

    /* Search in the task stacks */
    for (index = 0; index < g_taskMaxNum; index++) {
        taskCB = &g_taskCBArray[index];
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
        topOfStack = taskCB->topOfStack;
        stackBottom = taskCB->topOfStack + taskCB->stackSize;

        if (IsValidSP(*regSP, topOfStack, stackBottom)) {
            found = TRUE;
            goto FOUND;
        }
    }

FOUND:
    if (found == TRUE) {
        *start = topOfStack;
        *end = stackBottom;
    } else if (*regSP < (UINT32)&__ram_end) {
        *start = *regSP;
        *end = (UINT32)&__ram_end;
        found = TRUE;
    }

    return found;
}

VOID BackTraceSub(UINTPTR sp)
{
    UINTPTR stackPointer = sp;
    UINT32 count = 0;
    UINTPTR topOfStack = 0;
    UINTPTR tmpStack = 0;
    UINTPTR stackBottom = 0;
    UINTPTR checkBL = 0;

    if (FindSuitableStack(&stackPointer, &topOfStack, &stackBottom) == FALSE) {
        return;
    }

    while ((stackPointer < stackBottom) && (count < OS_MAX_BACKTRACE)) {
        if (IsValidSP(*(UINT32 *)stackPointer, topOfStack, stackBottom) &&
            ((*(UINT32 *)(stackPointer + STACK_OFFSET) >= (UINT32)text_start) &&
            (*(UINT32 *)(stackPointer + STACK_OFFSET) <= (UINT32)text_end)) &&
            IS_ALIGNED(*(UINT32 *)stackPointer, ALGIN_CODE)) {
            if (tmpStack != *(UINT32 *)stackPointer) {
                tmpStack = *(UINT32 *)stackPointer;
                checkBL = *(UINT32 *)(stackPointer + STACK_OFFSET);
                count++;
                PrintExcInfo("traceback %u -- lr = 0x%08x -- fp = 0x%08x\n", count, checkBL, tmpStack);
                stackPointer = tmpStack;
                continue;
            } else {
                break;
            }
        }
        stackPointer += STACK_OFFSET;
    }
}


LITE_OS_SEC_TEXT_INIT VOID ArchExcInit(VOID)
{
    for (int i = 1; i < OS_SYS_VECTOR_CNT; i++) {
        if (i != TRAP0_HANDLER_VEC) {
            g_hwiVec[i] = (HWI_PROC_FUNC)HandleEntry;
        }
    }
}

STATIC VOID BackTraceWithSp(UINTPTR sp)
{
    PrintExcInfo("*******backtrace begin*******\n");

    BackTraceSub(sp);
}

VOID ArchBackTrace(VOID)
{
    UINTPTR stackPointer = ArchGetSp();
    BackTraceWithSp(stackPointer);
}

VOID ArchBackTraceWithSp(const VOID *stackPointer)
{
    BackTraceWithSp((UINTPTR)stackPointer);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
