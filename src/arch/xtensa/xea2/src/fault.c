/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Exception Architecture 2's exception handle implementation
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
#include "arch/exception.h"
#include "los_task_pri.h"
#include "los_memory_pri.h"
#include "los_printf_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID OsExcHook(UINT32 excType, const ExcContext *excBufAddr);
STATIC EXC_PROC_FUNC g_excHook = OsExcHook;
STATIC ExcInfo g_excInfo;

#define OS_ENTER_EXC                      (++g_excInfo.nestCnt)    /* < Exception Nested Depth */
#define OS_EXC_ACTIVE                     (g_excInfo.nestCnt > 0)
#define OS_EXC_NESTED                     (g_excInfo.nestCnt > 1)
#define OFFSET1                           1
#define OFFSET2                           2
#define OFFSET3                           3
#define OFFSET4                           4
#define OFFSET15                          15
#define REG_OFFSET                        4

#ifdef LOSCFG_BACKTRACE
#define RA_OFFSET                         16
#define SP_OFFSET                         12
#define FP_ALIGN(value)                   (((UINT32)(value) & (UINT32)(LOSCFG_STACK_POINT_ALIGN_SIZE - 1)) == 0)
#define WINDOW_INCREMENT_SHIFT            2

/* this function is used to validate fp or validate the checking range start and end. */
STATIC INLINE BOOL IsValidFP(UINTPTR regFP, UINTPTR start, UINTPTR end)
{
    return (regFP > start) && (regFP <= end) && FP_ALIGN(regFP);
}

STATIC INLINE BOOL FindSuitableStack(UINTPTR regFP, UINTPTR *start, UINTPTR *end)
{
    UINT32 index, stackStart, stackEnd;
    BOOL found = FALSE;
    LosTaskCB *taskCB = NULL;

    /* Search in the task stacks */
    for (index = 0; index < g_taskMaxNum; index++) {
        taskCB = OS_TCB_FROM_TID(index);
        if (taskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }

        stackStart = taskCB->topOfStack;
        stackEnd = taskCB->topOfStack + taskCB->stackSize;
        if (IsValidFP(regFP, stackStart, stackEnd)) {
            found = TRUE;
            goto FOUND;
        }
    }

    /* Search in the irq / start stacks */
    stackStart = (UINTPTR)&__int_stack_start;
    stackEnd = (UINTPTR)&__int_stack_end;
    if (IsValidFP(regFP, stackStart, stackEnd)) {
        found = TRUE;
    }

FOUND:
    if (found == TRUE) {
        *start = stackStart;
        *end = stackEnd;
    }

    return found;
}

UINT32 ArchBackTraceGet(UINTPTR sp, UINT32 retAddr, UINTPTR *callChain, UINT32 maxDepth)
{
    UINTPTR tmpSp;
    UINT32 tmpRa;
    UINTPTR backRa = retAddr;
    UINTPTR backSp = sp;
    UINTPTR stackStart, stackEnd;
    UINT32 count = 0;

    if (FindSuitableStack(sp, &stackStart, &stackEnd) == FALSE) {
        PrintExcInfo("sp:0x%x error, backtrace failed!\n", sp);
        return 0;
    }

    while (IsValidFP(backSp, stackStart, stackEnd)) {
        if (callChain == NULL) {
            PrintExcInfo("trace%u  ra:0x%x  sp:0x%x\n", count,
                (backRa << WINDOW_INCREMENT_SHIFT) >> WINDOW_INCREMENT_SHIFT, backSp);
        } else {
            callChain[count] = backRa;
        }

        tmpRa = backRa;
        tmpSp = backSp;
        backRa = *((UINT32 *)(UINTPTR)(tmpSp - RA_OFFSET));
        backSp = *((UINT32 *)(UINTPTR)(tmpSp - SP_OFFSET));
        count++;

        if ((tmpRa == backRa) || (backSp == tmpSp) || (count == maxDepth)) {
            break;
        }
    }

    return count;
}

VOID BackTraceWithFp(UINTPTR sp, UINTPTR retAddr)
{
    PrintExcInfo("\n/***Trace Beg***/\n");
    ArchBackTraceGet(sp, retAddr, NULL, OS_MAX_BACKTRACE);
    PrintExcInfo("/***Trace End***/\n");
}

LITE_OS_SEC_TEXT VOID ArchBackTraceWithSp(const VOID *stackPointer)
{
    TaskContext *context = (TaskContext *)(UINTPTR)stackPointer;
    ArchBackTraceGet((UINTPTR)stackPointer + FASTSWITCH_REG_CONTEXT_SIZE, context->pc,
        NULL, OS_MAX_BACKTRACE);
}

VOID ArchBackTrace(VOID)
{
    UINTPTR sp;
    UINTPTR ra;

    __asm__ __volatile__("mov  %0,  a1" : "=a"(sp) : : "a1");
    __asm__ __volatile__("mov  %0,  a0" : "=a"(ra) : : "a0");
    xthal_window_spill();
    BackTraceWithFp(sp, ra);
}
#endif

#ifndef LOSCFG_ARCH_EXC_SIMPLE_INFO
#define EXC_AR0_OFFSET_IN_CONTEXT         (LOS_OFF_SET_OF(ExcContext, ar0) / REG_OFFSET)
#define EXC_AR3_OFFSET_IN_CONTEXT         (LOS_OFF_SET_OF(ExcContext, ar3) / REG_OFFSET)
#define EXC_AR4_OFFSET_IN_CONTEXT         (LOS_OFF_SET_OF(ExcContext, ar4) / REG_OFFSET)
#define EXC_AR15_OFFSET_IN_CONTEXT        (LOS_OFF_SET_OF(ExcContext, ar15) / REG_OFFSET)
#define DUMPSIZE                          128U
#define ADDR_IS_ALIGNED(val, align)       ((((UINTPTR)(val)) & ((UINTPTR)((align) - 1))) == 0)
#define IS_VALID_ADDR(ptr)                (((UINTPTR)(ptr) >= SYS_MEM_BASE) && ((UINTPTR)(ptr) <= SYS_MEM_END) && \
                                          ADDR_IS_ALIGNED((ptr), sizeof(CHAR *)))

STATIC const CHAR *g_excOccurStage[] = {
    "Init",
    "Task",
    "Irq",
    "Exc"
};

VOID ArchDumpRegsAroundMem(const UINT32 *registers, UINT32 start, UINT32 end, UINT32 offset)
{
    UINT32 *ar;
    UINT32 regID;

    for (regID = start; regID <= end; regID++, offset++) {
        ar = (UINT32 *)(UINTPTR)(registers[regID]);
        /* ar must be aligned with 4 bytes */
        if (!IS_VALID_ADDR(ar)) {
            continue;
        }
        PrintExcInfo("\ndump mem around a%u:%p", offset, ar);
        OsDumpMemByte(DUMPSIZE, (UINTPTR)ar - (DUMPSIZE >> 1));
    }
    PrintExcInfo("\n");
}

STATIC VOID OsExcRegsMem(const ExcContext *excBufAddr)
{
    const UINT32 *registers = (const UINT32 *)(UINTPTR)excBufAddr;
    ArchDumpRegsAroundMem(registers, EXC_AR0_OFFSET_IN_CONTEXT, EXC_AR3_OFFSET_IN_CONTEXT, 0);
    ArchDumpRegsAroundMem(registers, EXC_AR4_OFFSET_IN_CONTEXT, EXC_AR15_OFFSET_IN_CONTEXT, OFFSET4);
}
#endif /* LOSCFG_ARCH_EXC_SIMPLE_INFO */

#ifdef LOSCFG_DSP_COPROCESS
__attribute((aligned (XCHAL_CP1_SA_ALIGN))) UINT8 g_cpCtx[LOSCFG_BASE_CORE_TSK_LIMIT + 1][XCHAL_CP1_SA_SIZE];
LosTaskCB *g_cpOwnerTcb = NULL;

VOID Cp1SaveRestore(VOID)
{
    UINT32 taskId;
    g_cpOwnerTcb = (VOID *)g_cpOwner;
    if (g_cpOwnerTcb != NULL) {
        taskId = g_cpOwnerTcb->taskId;
        xthal_save_cpregs(g_cpCtx[taskId], 1);
    }
    taskId = ((LosTaskCB *)g_losTask.runTask)->taskId;
    xthal_restore_cpregs(g_cpCtx[taskId], 1);
}
#endif /* LOSCFG_DSP_COPROCESS */

VOID ArchExcInit(VOID)
{
}

UINT32 ArchSetExcHook(EXC_PROC_FUNC excHook)
{
    UINT32 intSave;

    intSave = ArchIntLock();
    g_excHook = excHook;
    ArchIntRestore(intSave);
    return 0;
}

EXC_PROC_FUNC ArchGetExcHook(VOID)
{
    return g_excHook;
}

STATIC VOID OsExcRegsInfo(const ExcContext *excBufAddr)
{
    UINT32 *p = NULL;
    int i = 0;

    PrintExcInfo("***Core Panic***\n"
        "pc:0x%x\n", excBufAddr->epc);

    p = (UINT32 *)&excBufAddr->ar0;
    while (i <= OFFSET15) { /* 15£ºregs a0 ~ a15 */
    	PrintExcInfo("a%02d:0x%08x\n", i, *p);
        i++;
        if (i == OFFSET4) {
            p = (UINT32 *)&(excBufAddr->ar4);
        } else {
            p++;
        }
    }
}

STATIC VOID OsExcSysInfo(UINT32 excType, const ExcContext *excBufAddr)
{
    if (OS_EXC_NESTED) { /* Exception nesting level is 1 */
        g_excInfo.phase = OS_EXC_STAGE_EXC;
        g_excInfo.thrdPid = OS_EXC_STAGE_INIT_VALUE;
    } else if (OS_SCHEDULER_ACTIVE == 0) {
        g_excInfo.phase = OS_EXC_STAGE_INIT;
        g_excInfo.thrdPid = OS_EXC_STAGE_INIT_VALUE;
    } else if (OS_INT_ACTIVE) {
        g_excInfo.phase = OS_EXC_STAGE_IRQ;
        g_excInfo.thrdPid = OS_EXC_STAGE_INIT_VALUE;
    } else {
        g_excInfo.phase = OS_EXC_STAGE_TASK;
        g_excInfo.thrdPid = LOS_CurTaskIDGet();
    }
    g_excInfo.type = excBufAddr->excCause;
    g_excInfo.faultAddr = excBufAddr->excAddr;
    PrintExcInfo("task:%s\n""tid:0x%x\n""ccause:0x%x\n""excAddr:0x%x\n""nestCnt:%u\n"
#ifndef LOSCFG_ARCH_EXC_SIMPLE_INFO
        "phase:%s\n",
#else
        "phase:%u\n",
#endif
        OsCurTaskNameGet(), g_excInfo.thrdPid, g_excInfo.type, g_excInfo.faultAddr, g_excInfo.nestCnt,
#ifndef LOSCFG_ARCH_EXC_SIMPLE_INFO
        g_excOccurStage[g_excInfo.phase]);
#else
        g_excInfo.phase);
#endif
}

VOID OsExcHook(UINT32 excType, const ExcContext *excBufAddr)
{
    OsExcRegsInfo(excBufAddr);
    OsExcSysInfo(excType, excBufAddr);

#ifdef LOSCFG_BACKTRACE
    /* if the exception happened in the init phase, then skip the backtrace. */
    BackTraceWithFp(excBufAddr->ar1,  excBufAddr->ar0);
#endif

#ifndef LOSCFG_ARCH_EXC_SIMPLE_INFO
    (VOID)OsShellCmdTskInfoGet(OS_ALL_TASK_MASK);
    OsExcRegsMem(excBufAddr);
#endif

#ifdef LOSCFG_KERNEL_MEM_BESTFIT
    OsMemIntegrityMultiCheck();
#endif
}

VOID OsExcHandleEntry(UINT32 excType, const ExcContext *excBufAddr)
{
    (VOID)LOS_IntLock();

    OS_ENTER_EXC;
    if (OS_EXC_NESTED) {
        goto HANDLER_END;
    }

    if (g_excHook != NULL) {
        g_excHook(excType, excBufAddr);
    }

HANDLER_END:
    /* Wait watchdog timeout and reboot */
    for (;;) {
    }
}

NMI_PROC_FUNC g_osNmiHook = NULL;
UINT32 ArchSetNMIHook(NMI_PROC_FUNC nmiHook)
{
    UINT32 intSave;

    intSave = ArchIntLock();
    g_osNmiHook = nmiHook;
    ArchIntRestore(intSave);
    return 0;
}

NMI_PROC_FUNC ArchGetNMIHook(VOID)
{
    return g_osNmiHook;
}

LITE_OS_SEC_TEXT VOID OsNMIHandler(UINT32 excType, const VOID *excBufAddr)
{
    PRINT_ERR("NMI cause:%u, SP:0x%x\n", excType, (UINTPTR)excBufAddr);
    if (g_osNmiHook != NULL) {
    	g_osNmiHook(excBufAddr);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
