/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS Trace Demo Code
 * Author: Huawei LiteOS Team
 * Create: 2020-08-31
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

#ifndef LOSCFG_RECORDER_MODE_OFFLINE
#warning This demo only support Online mode, please enable "Kernel-->Enable Extend Kernel-->Enable Trace Feature-->Trace work mode (Online mode)"
#endif

#include "los_trace.h"
#include "los_swtmr.h"
#include "los_mux.h"
#include "los_sem.h"
#include "los_queue.h"
#include "los_exc.h"
#include "trace_demo.h"
#include "los_trace_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TRACE_DEMO_HWI_NUM    56
#define TRACE_TASK_PRIORITY   25
#define TRACE_DEMO_EVENT_WAIT 0x00000001

STATIC HWI_IRQ_PARAM_S g_demoHwiDev1;
STATIC HWI_IRQ_PARAM_S g_demoHwiDev2;
STATIC EVENT_CB_S g_demoEvent;
STATIC UINT32 g_demoTaskId;
STATIC UINT32 g_demoQueue;
STATIC UINT32 g_demoMux;
STATIC UINT32 g_demoSem;
STATIC UINT32 g_demoMux1;
STATIC UINT32 g_demoMux2;

STATIC VOID TraceEasyMarcoDemo(VOID)
{
    LOS_TraceEventMaskSet(0);    /* 0: mask */
    LOS_TRACE_EASY(0, 1, 2, 3);  /* 0: type; 1: userId; 2, 3: userParam */
    LOS_TRACE_EASY(0, 1);        /* 0: type; 1: userId */

    LOS_TraceEventMaskSet(TRACE_SWTMR_FLAG);
    LOS_TRACE_EASY(1, 2, 2, 3);  /* 1: type; 2: userId; 2, 3: userParam */
    LOS_TRACE_EASY(1, 2);        /* 1: type; 2: userId */
    LOS_TRACE_EASY(1, 2, 1);     /* 1: type; 2: userId; 1: userParam */

    LOS_TraceEventMaskSet(TRACE_TASK_FLAG);
    LOS_TRACE_EASY(2, 3, 2, 3);  /* 2: type; 3: userId; 2, 3: userParam */
    LOS_TRACE_EASY(2, 3);        /* 2: type; 3: userId */
}

STATIC VOID Timer1Callback(UINT32 arg)
{
    printf("%s .\r\n", __FUNCTION__);
}

STATIC VOID Timer2Callback(UINT32 arg)
{
    printf("%s .\r\n", __FUNCTION__);
}

STATIC VOID SwTimerDemo(VOID)
{
    UINT32 ret;
    UINT16 id1;
    UINT16 id2;

    LOS_TraceEventMaskSet(TRACE_SWTMR_FLAG);

    ret = LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1Callback, &id1, 1); /* 1000: interval; 1: timeout interval */
    if (ret != LOS_OK) {
        printf("Create software timer1 failed.\n");
        return;
    }

    ret = LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2Callback, &id2, 1); /* 100: interval; 1: timeout interval */
    if (ret != LOS_OK) {
        printf("Create software timer2 failed.\n");
        return;
    }

    LOS_SwtmrStart(id1);
    (VOID)LOS_TaskDelay(200);   /* delay 200ms */
    LOS_SwtmrStop(id1);

    LOS_SwtmrStart(id2);
    (VOID)LOS_TaskDelay(200);   /* delay 200ms */
    LOS_SwtmrStop(id2);
    LOS_SwtmrDelete(id2);
}

STATIC VOID UserIrqHandler(VOID)
{
    printf("%s .\r\n", __FUNCTION__);
}

STATIC VOID HwiDemo(VOID)
{
    LOS_TraceEventMaskSet(TRACE_HWI_FLAG);
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 0, 0, UserIrqHandler, 0);
    LOS_HwiTrigger(TRACE_DEMO_HWI_NUM);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, 0);

    g_demoHwiDev1.pDevId = (void *)1;  /* irqParam */
    g_demoHwiDev1.swIrq = TRACE_DEMO_HWI_NUM;

    /* 3: interrupt priority */
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 3, IRQF_SHARED, (HWI_PROC_FUNC)UserIrqHandler, &g_demoHwiDev1);

    g_demoHwiDev2.pDevId = (void *)2;  /* irqParam */
    g_demoHwiDev2.swIrq = TRACE_DEMO_HWI_NUM;

    /* 3: interrupt priority */
    LOS_HwiCreate(TRACE_DEMO_HWI_NUM, 3, IRQF_SHARED, (HWI_PROC_FUNC)UserIrqHandler, &g_demoHwiDev2);

    LOS_HwiEnable(TRACE_DEMO_HWI_NUM);
    LOS_HwiTrigger(TRACE_DEMO_HWI_NUM);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, &g_demoHwiDev1);
    LOS_HwiDelete(TRACE_DEMO_HWI_NUM, &g_demoHwiDev2);
}

STATIC VOID MemDemo(VOID)
{
    VOID *p = NULL;
    VOID *pool = NULL;

    LOS_TraceEventMaskSet(TRACE_MEM_FLAG);
    p = LOS_MemAlloc(m_aucSysMem0, 0x100);          /* 0x100: memory size */
    LOS_MemFree(m_aucSysMem0, p);

    p = LOS_MemAllocAlign(m_aucSysMem0, 0x102, 4);  /* 0x102: memory size; 4: aligned boundary */
    LOS_MemFree(m_aucSysMem0, p);

    LOS_MemFree(m_aucSysMem0, (VOID *)0xffffffff);  /* 0xffffffff: Starting address of the memory */

    pool = LOS_MemAlloc(m_aucSysMem0, 0x2000);      /* 0x2000: memory size */
    if (pool == NULL) {
        printf("Memory alloc failed.\n");
        return;
    }

    LOS_MemInit(pool, 0x2000);                      /* 0x2000: memory size */
    p = LOS_MemAlloc(pool, 0x200);                  /* 0x200: memory size */
    LOS_MemFree(pool, p);

    LOS_MemFree(m_aucSysMem0, pool);
}

STATIC VOID PendEntry(VOID)
{
#define QUEUE_BUFFER_SIZE 32
    CHAR buf[QUEUE_BUFFER_SIZE] = {0};

    LOS_EventRead(&g_demoEvent, TRACE_DEMO_EVENT_WAIT, LOS_WAITMODE_AND, 100);  /* 100: timeout interval */
    LOS_EventClear(&g_demoEvent, ~g_demoEvent.uwEventID);

    LOS_SemPend(g_demoSem, 10);   /* 10: The request for the semaphore times out */
    LOS_SemDelete(g_demoSem);

    LOS_QueueRead(g_demoQueue, &buf, QUEUE_BUFFER_SIZE, LOS_WAIT_FOREVER);
    LOS_QueueDelete(g_demoQueue);

    LOS_MuxPend(g_demoMux, LOS_WAIT_FOREVER);
    LOS_MuxPost(g_demoMux);
    LOS_MuxDelete(g_demoMux);
}

STATIC VOID IpcDemo(VOID)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam;

    LOS_TraceEventMaskSet(TRACE_EVENT_FLAG | TRACE_MUX_FLAG | TRACE_SEM_FLAG | TRACE_QUE_FLAG);

    LOS_EventInit(&g_demoEvent);
    LOS_SemCreate(0, &g_demoSem);
    LOS_MuxCreate(&g_demoMux);
    LOS_MuxPend(g_demoMux, LOS_WAIT_FOREVER);

    /* 5: queue length. The value range is [1,0xffff] */
    /* 0: queue mode. Reserved parameter, not used for now */
    /* 24: Node size. The value range is [1,0xffff-4] */
    LOS_QueueCreate("queue", 5, &g_demoQueue, 0, 24);

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio   = TRACE_TASK_PRIORITY -1;
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)PendEntry;
    taskInitParam.pcName       = "PendEntryDemoTask";
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    LOS_TaskCreate(&taskId, &taskInitParam);

    LOS_EventWrite(&g_demoEvent, TRACE_DEMO_EVENT_WAIT);
    LOS_SemPost(g_demoSem);

    /* 8: this parameter is not in use temporarily */
    /* 0: expiry time. The value range is [0,LOS_WAIT_FOREVER](unit: Tick) */
    LOS_QueueWrite(g_demoQueue, "buff", 8, 0);
    LOS_MuxPost(g_demoMux);
}

VOID MuxFunc1(VOID)
{
    LOS_MuxPend(g_demoMux2, LOS_WAIT_FOREVER);
    LOS_TaskDelay(1);
    LOS_MuxPend(g_demoMux1, LOS_WAIT_FOREVER);
    while (1) {
        asm("nop");
    }
}

VOID MuxFunc2(VOID)
{
    LOS_MuxPend(g_demoMux1, LOS_WAIT_FOREVER);
    LOS_TaskDelay(1);
    LOS_MuxPend(g_demoMux2, LOS_WAIT_FOREVER);
    while (1) {
        asm("nop");
    }
}

static VOID DeadLockCheckDemo(VOID)
{
    UINT32 ret;
    UINT32 tid1;
    UINT32 tid2;
    LOS_MuxCreate(&g_demoMux1);
    LOS_MuxCreate(&g_demoMux2);

    LOS_TaskLock();
    TSK_INIT_PARAM_S osTaskInitParam;

    ret = memset_s(&osTaskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    osTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MuxFunc1;
    osTaskInitParam.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;

    osTaskInitParam.pcName       = "mux_task1";
    osTaskInitParam.usTaskPrio   = TRACE_TASK_PRIORITY + 1;
    osTaskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED;
#if (LOSCFG_KERNEL_SMP == YES)
    osTaskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif

    ret = LOS_TaskCreate(&tid1, &osTaskInitParam);
    if (ret != LOS_OK) {
        printf("task create failed 0x%x\n", ret);
        goto EXIT;
    }

    osTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MuxFunc2;
    osTaskInitParam.pcName       = "mux_task2";
    osTaskInitParam.usTaskPrio   = TRACE_TASK_PRIORITY + 1;
    ret = LOS_TaskCreate(&tid2, &osTaskInitParam);
    if (ret != LOS_OK) {
        printf("task create failed 0x%x\n", ret);
        LOS_TaskDelete(tid1);
    }
EXIT:
    LOS_TaskUnlock();    /* let mux_task1 and mux_task2 run */
    LOS_TaskDelay(1000); /* delay 1000ms */
}
STATIC VOID DemoTaskEntry(VOID)
{
    printf("Trace demo task start to run.\n");
    LOS_TaskDelay(20);  /* delay 20ms */

#ifdef LOSCFG_RECORDER_MODE_OFFLINE
    UINT32 ret;
    ret = LOS_TraceStart();
    if (ret != LOS_OK) {
        printf("Trace start failed.\n");
        return;
    }
#else
    while (!OsTraceIsEnable()) { /* wait liteos studio to send start trace cmd */
        LOS_TaskDelay(10);       /* delay 10ms */
    }
#endif

    TraceEasyMarcoDemo();

    /* trigger task switch event */
    LOS_TaskDelay(1);
    LOS_TaskDelay(1);
    LOS_TaskDelay(1);

    /* trigger hwi event */
    HwiDemo();

    /* trigger swtmr event */
    SwTimerDemo();

    /* trigger memory event */
    MemDemo();

    /* trigger sem\queue\event\mux event */
    IpcDemo();

    DeadLockCheckDemo();

#ifdef LOSCFG_RECORDER_MODE_OFFLINE
    LOS_TraceStop();
    LOS_TraceRecordDump(FALSE);
#endif

    printf("Trace demo task finished.\n");
}

VOID TraceDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.pcName       = "TraceDemoTask";
    taskInitParam.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio   = TRACE_TASK_PRIORITY;
    taskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create trace demo task failed.\n");
    }

    /* trace is already started in offline mode, user can stop and reset it before run demo */
#ifdef LOSCFG_RECORDER_MODE_OFFLINE
    LOS_TraceStop();
#endif
    LOS_TraceEventMaskSet(TRACE_TASK_FLAG);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
