/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Lx6 Exception Handler Implementation
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

#include "arch/exception.h"
#include "los_task_pri.h"
#include "los_typedef.h"
#include "los_printf_pri.h"
#include "xtensa_lx6.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

ExcInfo g_excInfo = {0};

UINT32 HwiNumValid(UINT32 num)
{
    return (num >= OS_SYS_VECTOR_CNT) && (num <= OS_VECTOR_CNT);
}

VOID ArchExcInit(VOID)
{
}

UINT32 ArchGetCurrentPc(VOID)
{
    UINT32 pc;
    __asm__ volatile("mov %0, a0 " : "=r"(pc) : : "memory");
    return pc;
}

STATIC VOID OsExcTypeInfo(const ExcInfo *excInfo)
{
    CHAR *phaseStr[] = {"exc in init", "exc in task", "exc in hwi"};

    PrintExcInfo("Type      = %d\n"
                 "ThrdPid   = %d\n"
                 "Phase     = %s\n"
                 "FaultAddr = 0x%x\n",
                 excInfo->type,
                 excInfo->thrdPid,
                 phaseStr[excInfo->phase], 
                 excInfo->faultAddr);
}

STATIC VOID OsExcCurTaskInfo(const ExcInfo *excInfo)
{
    PrintExcInfo("Current task info:\n");
    if (excInfo->phase == OS_EXC_IN_TASK) {
        LosTaskCB *taskCB = OS_TCB_FROM_TID(LOS_CurTaskIDGet());
        PrintExcInfo("Task name = %s\n"
                     "Task Id = %d\n"
                     "Task stackSize = 0x%x\n"
                     "Task stackPointer = 0x%x\n"
                     "Task topOfStack = 0x%x\n",
                     taskCB->taskName,
                     taskCB->taskId,
                     taskCB->stackSize,
                     (UINT32)taskCB->stackPointer,
                     taskCB->topOfStack);
    } else if (excInfo->phase == OS_EXC_IN_HWI) {
        PrintExcInfo("Exception occur in interrupt phase!\n");
    } else {
        PrintExcInfo("Exception occur in system init phase!\n");
    }
}

STATIC VOID OsExcRegInfo(const ExcInfo *excInfo)
{
    INT32 index;
    PrintExcInfo("Exception reg dump:\n"
                 "sar          = 0x%x\n"
                 "excCause     = 0x%x\n"
                 "excVaddr     = 0x%x\n"
#ifdef LOSCFG_XTENSA_LOOPS
                 "lbeg         = 0x%x\n"
                 "lend         = 0x%x\n"
                 "lcount       = 0x%x\n"
#endif
                 "pc           = 0x%x\n"
                 "ps           = 0x%x\n",
                 excInfo->context->sar, excInfo->context->excCause, excInfo->context->excVaddr,
#ifdef LOSCFG_XTENSA_LOOPS
                 excInfo->context->lbeg, excInfo->context->lend, excInfo->context->lcount,
#endif
                 excInfo->context->pc, excInfo->context->ps);
    for (index = 0; index < XTENSA_LOGREG_NUM; index++) {
        PrintExcInfo("regA%d        = 0x%x\n", index, excInfo->context->regA[index]);
   }
}

#ifdef LOSCFG_BACKTRACE
#ifdef LOSCFG_COMPILER_XTENSA_L106
STATIC UINT32 CurrentTextOffset(const UINT32 pc)
{
    UINT32 size;
    if (pc > (UINT32)&_text_start && pc < (UINT32)&_text_end) {
        size = pc - (UINT32)&_text_start;
    } else if ((pc > (UINT32)&_iram_text_start) && (pc < (UINT32)&_iram_text_end)) {
        size = pc - (UINT32)&_iram_text_start;
    } else {
        size = 0;
    }
    return size;
}

UINT32 ArchBackTraceGet(UINTPTR pc, UINTPTR sp, UINTPTR lr, UINTPTR *backPc, UINTPTR *backSp)
{
    UINT32 offset;
    UINT8 *tmp = NULL;
    BOOL flag = FALSE;
    INT32 stackSize;
    const UINT32 size = CurrentTextOffset(pc);
    for (offset = 0; offset < size; offset++) {
        tmp = (UINT8 *)(pc - offset);
        /* the instuction of addi a1,a1,xx */
        if ((tmp[0] == 0x12) && (tmp[1] == 0xc1)) {
            /* get the stack size */
            stackSize = (INT32)(INT8)tmp[2];
            if ((stackSize >= 0) || (stackSize % 16 != 0)) {
                continue;
            }

            sp -= stackSize;
            if (flag == FALSE) {
                pc = lr;
            } else {
                pc = *(UINT32 *)(sp - 4);
            }
            break;
        /* the instruction of movi xx,xx; sub a1, a1, xx */
        } else if ((tmp[0] == 0x92) && ((tmp[1] & 0xf0) == 0xa0) &&
                   (tmp[3] == 0x90) && (tmp[4] == 0x11) && (tmp[5] == 0xc0)) {
            stackSize = (UINT32)((((UINT16)(tmp[1] & 0xF)) << 8) + (UINT16)tmp[2]);
            if (stackSize <= 0) {
                continue;
            }

            sp += stackSize;
            if (flag == 0) {
                pc = lr;
            }else{
                pc = *(UINT32 *)(sp - 4);
            }
            break;
        /* the instruction of ret.n */
        } else if ((tmp[0] == 0x0d) && (tmp[1] == 0xf0)) {
            pc = lr;
            break;
        /* the instruction of s32i	a0, a1, xx */
        } else if ((tmp[0] == 0x02) && (tmp[1] == 0x61)) {
            flag = TRUE;
        }
    }
    if (offset < size) {
        *backSp = sp;
        *backPc = pc;
        return (CurrentTextOffset(*backPc) ? 1 : 0);
    }
    return 0;
}

VOID BackTraceSub(UINTPTR sp)
{
    EXC_CONTEXT_S * context = (EXC_CONTEXT_S *)sp;
    UINTPTR tmpSp;
    UINTPTR tmpRa;
    UINTPTR backPc;
    UINTPTR backRa;
    UINTPTR backSp;

    backRa = (UINTPTR)context->regA[0];
    backPc = (UINTPTR)context->pc;
    backSp = (UINTPTR)context->regA[1];
    PrintExcInfo("backtrace 0x%x -- lr = 0x%x\n", (VOID *)backSp, (VOID *)backPc);
    while (ArchBackTraceGet(backPc, backSp, backRa, &tmpRa, &tmpSp)) {
        PrintExcInfo("backtrace 0x%x -- lr = 0x%x\n", (VOID *)tmpSp, (VOID *)tmpRa);
        backPc = tmpRa;
        backSp = tmpSp;
    }
    PrintExcInfo("----- backtrace end -----\n");
}
#elif defined (LOSCFG_COMPILER_XTENSA_LX6)
#define OS_BACKTRACE_START     0
#define ALIGN_MASK             (0x3)
#define OS_REG_LR_OFFSET       (CONTEXT_SIZE - 8)
#define VIR_TEXT_ADDR_MASK     0x80000000
#define TEXT_ADDR_MASK         0x40000000
#define RA_OFFSET              16
#define SP_OFFSET              12

BOOL OsStackDataIsCodeAddr(UINTPTR value)
{
    if ((value >= (UINTPTR)&__text_start) && (value < (UINTPTR)&__text_end)) {
        return TRUE;
    }
    if ((value >= (UINTPTR)&_text_start) && (value < (UINTPTR)&_text_end)) {
        return TRUE;
    }
    return FALSE;
}

BOOL IsValidRa(UINTPTR regRA)
{
    regRA &= ~VIR_TEXT_ADDR_MASK;
    regRA |= TEXT_ADDR_MASK;

    return OsStackDataIsCodeAddr(regRA);
}

UINT32 IsSpAligned(UINT32 value)
{
    return (value & (UINT32)(ALIGN_MASK)) == 0;
}

BOOL IsValidSP(UINTPTR regSP, UINTPTR start, UINTPTR end)
{
    return (regSP >= start) && (regSP <= end) && IsSpAligned(regSP);
}

STATIC BOOL FindSuitableStack(UINTPTR regSP, UINTPTR *start, UINTPTR *end)
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
        if (IsValidSP(regSP, stackStart, stackEnd)) {
            found = TRUE;
            goto FOUND;
        }
    }

    if (IsValidSP(regSP, (UINTPTR)&__init_stack_s, (UINTPTR)&__init_stack_e)) {
        stackStart = (UINTPTR)&__init_stack_s;
        stackEnd = (UINTPTR)&__init_stack_e;
        found = TRUE;
        goto FOUND;
    }

FOUND:
    if (found == TRUE) {
        *start = stackStart;
        *end = stackEnd;
    }

    return found;
}

VOID ArchBackTraceGet(UINTPTR sp, UINT32 retAddr)
{
    UINTPTR tmpSp;
    UINT32 tmpRa;
    UINTPTR backRa = retAddr;
    UINTPTR backSp = sp;
    UINTPTR stackStart;
    UINT32 stackEnd;

    if (FindSuitableStack(sp, &stackStart, &stackEnd) == FALSE) {
        PRINTK("sp:0x%x error, backtrace failed!\n", sp);
        return;
    }
    while (IsValidSP(backSp, stackStart, stackEnd)) { 
        backRa &= ~VIR_TEXT_ADDR_MASK;
        backRa |= TEXT_ADDR_MASK;
        PrintExcInfo("backtrace 0x%x -- lr = 0x%x\n", (VOID *)backSp, (VOID *)backRa);

        tmpRa = backRa;
        tmpSp = backSp;
        backRa = *((UINT32 *)(UINTPTR)(tmpSp - RA_OFFSET));
        backSp = *((UINT32 *)(UINTPTR)(tmpSp - SP_OFFSET));
        if ((tmpRa == backRa) || (backSp == tmpSp)  || !IsValidRa(backRa)) {
            break;
        }
    }
}

VOID BackTraceSub(UINTPTR sp)
{
    UINTPTR reglr;
    sp = ((EXC_CONTEXT_S *)sp)->regA[1];
    reglr = ((EXC_CONTEXT_S *)sp)->regA[0];
    HakSpillWindow();
    ArchBackTraceGet(sp, reglr);
}
#endif

STATIC VOID BackTraceWithSp(UINTPTR sp)
{
    PrintExcInfo("*******backtrace begin*******\n");
    BackTraceSub(sp);
    PrintExcInfo("*******backtrace end*******\n");
}
#endif

VOID ArchBackTraceWithSp(const VOID *stackPointer)
{
#ifdef LOSCFG_BACKTRACE
    BackTraceWithSp((UINTPTR)stackPointer);
#else
    (VOID)stackPointer;
#endif
}

VOID ArchBackTrace(VOID)
{
#ifdef LOSCFG_BACKTRACE

    EXC_CONTEXT_S context;
    __asm__ volatile("mov %0, a1 " : "=a"(context.regA[1]) : : "memory");
    __asm__ volatile("mov %0, a0 " : "=a"(context.regA[0]) : : "memory");
    context.pc = ArchGetCurrentPc();
    PrintExcInfo("sp:0x%08x\n", context.regA[1]);
    BackTraceWithSp((UINTPTR)(&context));
#endif
}

STATIC VOID OsExcInfoDisplay(const ExcInfo *excInfo)
{
    PrintExcInfo("*************Exception Information**************\n");
    OsExcTypeInfo(excInfo);
    OsExcCurTaskInfo(excInfo);
    OsExcRegInfo(excInfo);
}

__attribute__((section(".iram1"))) VOID OsExcHandleEntry(UINT32 faultAddr, EXC_CONTEXT_S *excBufAddr, UINT32 type)
{
    g_excInfo.nestCnt++;
    g_excInfo.faultAddr = faultAddr;
    g_excInfo.type = (UINT16)type;

    LosTaskCB *taskCB = (LosTaskCB *)g_newTask;
    if ((taskCB == NULL) || (taskCB == OS_TCB_FROM_TID(g_taskMaxNum))) {
        g_excInfo.phase = OS_EXC_IN_INIT;
        g_excInfo.thrdPid = OS_NULL_INT;
    } else if (ArchCurIrqGet() != OS_NULL_INT) {
        g_excInfo.phase = OS_EXC_IN_HWI;
        g_excInfo.thrdPid = ArchCurIrqGet();
    } else {
        g_excInfo.phase = OS_EXC_IN_TASK;
        g_excInfo.thrdPid = taskCB->taskId;
    }

    g_excInfo.context = excBufAddr;

    OsExcInfoDisplay(&g_excInfo);
#ifdef LOSCFG_BACKTRACE
    ArchBackTraceWithSp((VOID *)excBufAddr);
#endif
    for (;;) {

    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
