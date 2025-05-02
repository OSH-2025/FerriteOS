/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: iCunit Test Frame
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
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

#ifndef _TEST_COMMON_H
#define _TEST_COMMON_H

#include "los_task.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TASK_PRIO_TEST        25
#define TASK_STACK_SIZE_TEST  LOS_TASK_MIN_STACK_SIZE

#define TEST_DEV_HWI_CLEAR(hwiNum) LOS_HwiClear(hwiNum)
#define TASK_EXISTED_NUM (TaskCountGetTest())

#ifdef LOSCFG_KERNEL_SMP
#define TEST_TASK_PARAM_INIT_AFFI(stTestTask, task_name, entry, prio, affi) \
    do { \
        (VOID)memset_s(&(stTestTask), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));      \
        (stTestTask).pfnTaskEntry  = (TSK_ENTRY_FUNC)(entry);            \
        (stTestTask).uwStackSize   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE; \
        (stTestTask).pcName        = (task_name);                        \
        (stTestTask).usTaskPrio    = (prio);                             \
        (stTestTask).uwResved      = LOS_TASK_STATUS_DETACHED;         \
        (stTestTask).usCpuAffiMask = (affi); \
    } while (0)
#else
#define TEST_TASK_PARAM_INIT_AFFI(stTestTask, task_name, entry, prio, affi) \
    do { \
        (VOID)memset_s(&(stTestTask), sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));      \
        (stTestTask).pfnTaskEntry  = (TSK_ENTRY_FUNC)(entry);            \
        (stTestTask).uwStackSize   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE; \
        (stTestTask).pcName        = (task_name);                        \
        (stTestTask).usTaskPrio    = (prio);                             \
        (stTestTask).uwResved      = LOS_TASK_STATUS_DETACHED;         \
        (VOID)(affi); \
    } while (0)
#endif

/* extern declaration */
extern volatile INT32 g_testCount;
extern UINT32 g_testTaskID01;
extern UINT32 g_testTaskID02;
extern UINT32 g_testTaskID03;

extern UINT32 g_usSemID;
extern UINT32 g_mutexTest;
extern UINT32 g_testPeriod;
extern UINT16 g_usSwTmrID;
extern UINT32 g_testQueueID01;
extern UINT32 g_testQueueID02;
extern UINT32 g_testQueueID03;
extern EVENT_CB_S g_pevent;

extern volatile UINT64 g_tickCount[];
extern UINT32 g_testTskHandle;

extern VOID TaskHoldCycles(UINT64 cycles);
extern VOID TestDumpCpuid(VOID);
extern UINT64 TestTickCountGet(VOID);
extern UINT64 TestTickCountByCurrCpuid(VOID);
extern VOID TestExtraTaskDelay(UINT32 uwTick);
extern VOID TestBusyTaskDelay(UINT32 tick);
extern VOID TestAssertBusyTaskDelay(UINT32 timeout, UINT32 flag);
extern UINT32 TaskCountGetTest(VOID);
extern UINT32 LOS_MemTotalUsedGet(VOID *pPool);
extern VOID  dprintf(const CHAR *fmt, ...);
extern UINT32 MemGetFreeSize(VOID *pool);
extern VOID TestHwiTrigger(UINT32 irq);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _TEST_COMMON_H */
