/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Xtensa Exception Architecture 3's exception handle implementation
 * Author: Huawei LiteOS Team
 * Create: 2018-08-01
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

#include <xtensa/hal.h>
#include <xtensa/hal-certified.h>
#include "arch/interrupt.h"
#include "arch/regs.h"
#include "los_sem_pri.h"
#include "los_mux_pri.h"
#include "los_coredump_pri.h"
#include "los_printf_pri.h"
#include "asm/platform.h"
#include "asm/interrupt_config.h"
#ifdef LOSCFG_KERNEL_TRACE
#include "los_trace_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define GET_UINT32_BY_OFFSET(base, offset)       (*(UINT32 *)((UINTPTR)(base) + (offset)))
#define IS_ALIGNED(val, align)                   (((UINTPTR)(val) & ((UINT32)(align) - 1)) == 0)
#define AR7_OFFSET_IN_EXCCONTEXT                 7
#define AR8_OFFSET_IN_EXCCONTEXT                 16
#define AR15_OFFSET_IN_EXCCONTEXT                23

enum RegSurroundingMem {
    FIRST_WORD,
    SECOND_WORD,
    THIRD_WORD,
    FOURTH_WORD
};

STATIC UINT32 g_excNestedCnt = 0;             /* Exception Nested Depth */

#define OS_ENTER_EXC                            (++g_excNestedCnt)
#define OS_EXC_ACTIVE                           (g_excNestedCnt > 0)
#define OS_EXC_NESTED                           (g_excNestedCnt > 1)

typedef VOID (*ExceptionHandler)(VOID);

ExceptionHandler g_excHandlerTable[] = {
    OsExcDefaultHandler,    // No exception
    OsExcDefaultHandler,    // Instruction usage error
    OsExcDefaultHandler,    // Address usage error
    OsExcDefaultHandler,    // External error
    OsExcDefaultHandler,    // Debug error
    OsExcDefaultHandler,    // Syscall exception
    OsExcDefaultHandler,    // Hardware error
    OsExcDefaultHandler,    // Memory mgmt error
    OsExcDefaultHandler,    // TLB miss error
    OsExcDefaultHandler,    // Coprocessor exception
    OsExcDefaultHandler,    // Custom exception
};

STATIC BOOL ExcStackAddrCheck(UINTPTR addr)
{
    if ((addr >= ((UINTPTR)OS_SYS_MEM_ADDR)) && (addr < ((UINTPTR)OS_SYS_MEM_ADDR + OS_SYS_MEM_SIZE))) {
        return TRUE;
    }
    if ((addr >= ((UINTPTR)&__int_stack_start)) && (addr < ((UINTPTR)&__int_stack_end))) {
        return TRUE;
    }
    return FALSE;
}

STATIC VOID ExcBackTrace(UINT32 sp, UINT32 retAddr, UINT32 type, UINT32 epc, UINT32 endofStack)
{
    UINT32 depth = 0;
    UINT32 lastPC = 0;

    PrintExcInfo("  --------Backtrace--------\n");

    if (type == OS_INTEXC_TASK_BT) {
        PrintExcInfo("  %02u pc 0x%08x\n", depth, epc);
        depth++;
    }

    while ((retAddr != 0) && (retAddr != OS_STACK_INIT) && (sp <= endofStack)) {
        PrintExcInfo("  %02u pc 0x%08x\n", depth, retAddr);
        if (!ExcStackAddrCheck(sp) || (lastPC == retAddr)) {
            PrintExcInfo("  Backtrace error! stackpointer: 0x%8x, pc: 0x%8x\n", sp, retAddr);
            break;
        }
        lastPC = retAddr;
        retAddr = GET_UINT32_BY_OFFSET(sp - STKFR_SA_SIZE, STKFR_SA_OFF_A0);
        sp = GET_UINT32_BY_OFFSET(sp - STKFR_SA_SIZE, STKFR_SA_OFF_A1);
        depth++;
    }
}

STATIC BOOL ExcDataAddrCheck(UINTPTR addr)
{
    if ((addr >= DRAM0_ADDR) && (addr < (DRAM0_ADDR + DRAM0_SIZE))) {
        return TRUE;
    }
    if ((addr >= DRAM1_ADDR) && (addr < (DRAM1_ADDR + DRAM1_SIZE))) {
        return TRUE;
    }
    if ((addr >= ((UINTPTR)&_sram_rodata_start)) && (addr < ((UINTPTR)&_sram_rodata_end))) {
        return TRUE;
    }
    if ((addr >= ((UINTPTR)&_sram_data_start)) && (addr < ((UINTPTR)&_sram_data_end))) {
        return TRUE;
    }
    return FALSE;
}

STATIC VOID ExcShowRegisters(const ExcContext *excContext)
{
    PrintExcInfo("  --------Registers--------\n");

    /* a0-a3 */
    PrintExcInfo("  a0  %08x  a1  %08x  a2  %08x  a3  %08x\n",
                 excContext->AR0, excContext->AR1,
                 excContext->AR2, excContext->AR3);

    /* a4-a7 */
    PrintExcInfo("  a4  %08x  a5  %08x  a6  %08x  a7  %08x\n",
                 excContext->AR4, excContext->AR5,
                 excContext->AR6, excContext->AR7);

    /* a8-a11 */
    PrintExcInfo("  a8  %08x  a9  %08x  a10 %08x  a11 %08x\n",
                 excContext->AR8, excContext->AR9,
                 excContext->AR10, excContext->AR11);

    /* a12-a15 */
    PrintExcInfo("  a12 %08x  a13 %08x  a14 %08x  a15 %08x\n",
                 excContext->AR12, excContext->AR13,
                 excContext->AR14, excContext->AR15);
}

STATIC VOID ExcShowMemAroundRegs(const ExcContext *excContext)
{
    UINT32 *ar = NULL;
    UINT32 index;
    UINT32 regId;
    UINT32 tmp;

    PrintExcInfo("  --------Show Partial Memory Around Register(Hex), only valid data address:--------\n");
    regId = 0;
    for (index = 0; index <= AR7_OFFSET_IN_EXCCONTEXT; index++) {
        tmp = *((UINT32 *)excContext + index);
        ar = (UINT32 *)(UINTPTR)tmp;
        if ((!ExcDataAddrCheck((UINTPTR)ar)) || (!IS_ALIGNED(ar, sizeof(UINT32)))) {
            regId++;
            continue;
        }
        PrintExcInfo("  a%-2u: 0x%08x 0x%08x 0x%08x 0x%08x\n", regId, *(ar + FIRST_WORD),
                     *(ar + SECOND_WORD), *(ar + THIRD_WORD), *(ar + FOURTH_WORD));
        regId++;
    }
    for (index = AR8_OFFSET_IN_EXCCONTEXT; index <= AR15_OFFSET_IN_EXCCONTEXT; index++) {
        tmp = *((UINT32 *)excContext + index);
        ar = (UINT32 *)(UINTPTR)tmp;
        if ((!ExcDataAddrCheck((UINTPTR)ar)) || (!IS_ALIGNED(ar, sizeof(UINT32)))) {
            regId++;
            continue;
        }
        PrintExcInfo("  a%-2u: 0x%08x 0x%08x 0x%08x 0x%08x\n", regId, *(ar + FIRST_WORD), *(ar + SECOND_WORD),
                     *(ar + THIRD_WORD), *(ar + FOURTH_WORD));
        regId++;
    }
}

STATIC VOID ExcCurTaskDump(const ExcContext *excContext)
{
    TSK_INFO_S tskInfo;
    UINT32 stkPt;
    UINT32 ret;

    stkPt = OsGetCurrStkPt();

    LosTaskCB *taskCB = g_runTask;
    if (taskCB == NULL) {
        PrintExcInfo("  Exception Happened in the initialization phase, no task info\n");
        return;
    }

    PrintExcInfo("  *************** Current Task [%s] Info Dump *******************\n", taskCB->taskName);
    if (OS_EXC_ACTIVE) {
        if (excContext == NULL) {
            PrintExcInfo("exc context is null");
            return;
        }
        taskCB->stackPointer = (VOID *)(UINTPTR)(excContext->AR1);
        ret = LOS_TaskInfoGet(taskCB->taskId, &tskInfo);
        if (ret != LOS_OK) {
            PrintExcInfo("Task Info Get failed!ret=0x%x\n", ret);
            return;
        }
    } else {
        taskCB->stackPointer = (VOID *)(UINTPTR)stkPt;
        ret = LOS_TaskInfoGet(taskCB->taskId, &tskInfo);
        if (ret != LOS_OK) {
            PrintExcInfo("Task Info Get failed!ret=0x%x\n", ret);
            return;
        }
        ArchBackTrace();
    }

    PrintExcInfo("  TaskID  Status             Priority  StackSize  StackPointer  WaterLine  "
              "TopOfStack  SemID  EventMask  MuxID\n");
    PrintExcInfo("  %-6u  %-8u  %-8u  0x%-7x  0x%-10x  0x%-7x  0x%-8x  %-8u  0x%-7x %-8u\n",
                 tskInfo.uwTaskID, tskInfo.usTaskStatus, tskInfo.usTaskPrio,
                 tskInfo.uwStackSize, tskInfo.uwSP, tskInfo.uwPeakUsed,
                 tskInfo.uwTopOfStack, ((LosSemCB *)(tskInfo.pTaskSem))->semId,
                 tskInfo.uwEventMask, ((LosMuxCB *)(tskInfo.pTaskMux))->muxId);
}

STATIC VOID ExcAllTaskDump(VOID)
{
    UINT32 index;
    TSK_INFO_S tskInfo;
    LosTaskCB *taskCB = NULL;
    ExcContext *excContext = NULL;
    UINT32 ret;

    for (index = 0; index <= LOSCFG_BASE_CORE_TSK_LIMIT; index++) {
        taskCB = OS_TCB_FROM_TID(index);
        if ((taskCB->taskStatus & OS_TASK_STATUS_UNUSED) ||
            (taskCB->taskStatus & OS_TASK_STATUS_RUNNING)) {
            continue;
        }

        PrintExcInfo("  *************** Task [%s] Info Dump*******************\n", taskCB->taskName);
        ret = LOS_TaskInfoGet(taskCB->taskId, &tskInfo);
        if (ret != LOS_OK) {
            PrintExcInfo("Task Info Get failed!ret=0x%x\n", ret);
            continue;
        }

        PrintExcInfo("  TaskID  Status             Priority  StackSize  StackPointer  "
                     "WaterLine  TopOfStack  SemID  EventMask  MuxID\n");
        PrintExcInfo("  %-6u  %-8u  %-8u  0x%-7x  0x%-10x  0x%-7x  0x%-8x  %-8u  0x%-7x %-8u\n",
                     tskInfo.uwTaskID, tskInfo.usTaskStatus, tskInfo.usTaskPrio,
                     tskInfo.uwStackSize, tskInfo.uwSP, tskInfo.uwPeakUsed,
                     tskInfo.uwTopOfStack, ((LosSemCB *)(tskInfo.pTaskSem))->semId,
                     tskInfo.uwEventMask, ((LosMuxCB *)(tskInfo.pTaskMux))->muxId);

        excContext = (ExcContext *)((UINTPTR)(taskCB->stackPointer) - STKFR_SA_SIZE);
        excContext->AR1 = (UINT32)((UINTPTR)(taskCB->stackPointer) + CONTEXT_SIZE);

        if (excContext->EXCCAUSE == OS_SCHED_FLAG_TASKPREEMT) {
            PrintExcInfo("  Task %s scheduled out initiative\n", taskCB->taskName);
            ExcBackTrace(excContext->AR1, excContext->EPC, OS_NORMAL_TASK_BT,
                0, taskCB->stackSize + taskCB->topOfStack);
        } else {
            PrintExcInfo("  Task %s created but not run yet\n", taskCB->taskName);
        }
    }
}

VOID ArchExcInit(VOID)
{
}

VOID ArchBackTrace(VOID)
{
    UINT32 stkPt;
    UINT32 retAddr;
    UINT32 endOfStack;
    LosTaskCB *taskCB = g_runTask;

    xthal_window_spill();

    stkPt = OsGetCurrStkPt();
    retAddr = OsGetCurrCaller();

    if ((taskCB == NULL) || (taskCB == OS_TCB_FROM_TID(g_taskMaxNum))) {
        endOfStack = (UINT32)(UINTPTR)&__int_stack_end;
    } else {
        endOfStack = taskCB->stackSize + taskCB->topOfStack;
    }
    ExcBackTrace(stkPt, retAddr, OS_NORMAL_TASK_BT, 0, endOfStack);
}

VOID ArchBackTraceWithSp(const VOID *stackPointer)
{
    (VOID)stackPointer;
    ExcCurTaskDump(NULL);
}

STATIC VOID OsExcReboot(VOID)
{
    /* Disable irq */
    (VOID)LOS_IntLock();

    (VOID)LOS_HwiDisable(OS_TICK_INT_NUM);
    (VOID)LOS_HwiClear(OS_TICK_INT_NUM);

    PrintExcInfo("enter reboot, wait watchdog timeout!!!");

    /* Wait watchdog timeout and reboot */
    for (;;) {
        /* the waiti instruction will clear interrupt mask, that leads to
           response to interrupt when exception happen. */
    }
}

VOID OsExcHandlerEntry(const ExcContext *excContext)
{
    OS_ENTER_EXC;

    UINT32 pc = excContext->EPC;
    UINT32 excCause = excContext->EXCCAUSE;
    UINT32 excVaddr = excContext->EXCVADDR;
    UINT32 ps = excContext->PS;
    LosTaskCB *taskCB = g_runTask;

    PrintExcInfo("  *******************************************************************\n");
    PrintExcInfo("                   Core Panic                                      \n");
    PrintExcInfo("  Exccause=0x%08x; EPC=0x%08x; EXCVADDR=0x%08x; PS=0x%08x\n", excCause, pc, excVaddr, ps);

    if ((taskCB == NULL) || (taskCB == OS_TCB_FROM_TID(g_taskMaxNum))) {
        PrintExcInfo("  Exception happened in the initialization phase!\n");
    } else if (OS_INT_ACTIVE) {
        PrintExcInfo("  Exception happened in the interrupt phase!\n");
    } else {
        PrintExcInfo("  Exception happened in the task[%s]!\n", taskCB->taskName);
    }

    if (OS_EXC_NESTED) {
        PrintExcInfo("  *********************Nested Exception!*********************\n");
        goto HANDLER_END;
    }

    ExcCurTaskDump(excContext);

    ExcShowRegisters(excContext);

    ExcShowMemAroundRegs(excContext);

    // if the exception happened in the init phase, then skip the backtrace.
    if ((taskCB != NULL) && (taskCB != OS_TCB_FROM_TID(g_taskMaxNum))) {
        ExcBackTrace(excContext->AR1, excContext->AR0, OS_INTEXC_TASK_BT,
            excContext->EPC, taskCB->stackSize + taskCB->topOfStack);
    }

#ifdef LOSCFG_KERNEL_TRACE
    if (g_traceDumpHook != NULL) {
        g_traceDumpHook(FALSE);
    }
#endif
    ExcAllTaskDump();

#ifdef LOSCFG_EXC_COREDUMP
    OsCoreDump(excContext);
#endif

HANDLER_END:
    OsExcReboot();
}

VOID LOS_TasksDump(VOID)
{
    UINTPTR intSave;
    intSave = LOS_IntLock();

    ExcCurTaskDump(NULL);
    ExcAllTaskDump();

    LOS_IntRestore(intSave);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

