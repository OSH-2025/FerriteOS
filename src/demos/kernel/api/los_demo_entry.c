/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Demo Entry Implementation Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_demo_entry.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOWEST_PRIORITY     30
#define DELAY_INTERVAL      100

STATIC UINT32 g_demoTaskId;

STATIC VOID DemoTaskEntry(VOID)
{
#ifdef LOS_KERNEL_DEMO_TASK
    TaskDemo();
    printf("Kernel task demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MEM_DYNAMIC
    DynMemDemo();
    printf("Kernel dynamic memory demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MEM_STATIC
    StaticMemDemo();
    printf("Kernel static memory demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_INTERRUPT
    InterruptDemo();
    printf("Kernel interrupt demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_QUEUE
    MsgQueueDemo();
    printf("Kernel message queue demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_EVENT
    EventDemo();
    printf("Kernel event demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_MUTEX
    MutexLockDemo();
    printf("Kernel mutex demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SEMPHORE
    SemphoreDemo();
    printf("Kernel semaphore demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SYSTICK
    GetTickDemo();
    printf("Kernel systick demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_SWTIMER
    SwTimerDemo();
    printf("Kernel software timer demo finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEMO_LIST
    ListDemo();
    printf("Kernel list demo finished.\n\n");
#endif
}

STATIC VOID DebugTaskEntry(VOID)
{
#ifdef LOS_KERNEL_DEBUG_TASK
    TaskDebug();
    printf("Kernel debug task finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEBUG_QUEUE
    QueueDebug();
    printf("Kernel debug queue finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEBUG_EVENT
    EventDebug();
    printf("Kernel debug event finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEBUG_MUTEX
    MutexDebug();
    printf("Kernel debug mutex finished.\n\n");
#endif
#ifdef LOS_KERNEL_DEBUG_SEMPHORE
    SemphoreDebug();
    printf("Kernel debug semphore finished.\n\n");
#endif
}

STATIC VOID KernelDemoTaskEntry(VOID)
{
    LOS_TaskDelay(DELAY_INTERVAL);
    DemoTaskEntry();
    DebugTaskEntry();
    LOS_TaskDelay(DELAY_INTERVAL);
}

VOID KernelDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry  = (TSK_ENTRY_FUNC)KernelDemoTaskEntry;
    taskInitParam.pcName        = "KernelDemoTask";
    taskInitParam.usTaskPrio    = LOWEST_PRIORITY;
    taskInitParam.uwStackSize   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved      = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create kernel demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
