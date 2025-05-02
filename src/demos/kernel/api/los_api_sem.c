/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Semaphore Demo Implementation
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

#include "los_api_sem.h"
#include "los_sem.h"
#include "los_task.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* task priority */
#define TASK_PRIOR          5
#define DELAY_INTERVAL1     20
#define DELAY_INTERVAL2     40

/* task pid */
STATIC UINT32 g_demoTask1Id;
STATIC UINT32 g_demoTask2Id;
/* semaphore id */
STATIC UINT32 g_demoSemId;

STATIC VOID Task1Entry(VOID)
{
    UINT32 ret;

    printf("Task1 try get semphore g_demoSemId, timeout 10 ticks.\n");
    /* get semaphore, timeout is 10 ticks */
    ret = LOS_SemPend(g_demoSemId, 10);
    /* get semaphore ok */
    if (ret == LOS_OK) {
        LOS_SemPost(g_demoSemId);
        return;
    }
    /* timeout, get semaphore fail */
    if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        printf("Task1 timeout and try get semphore g_demoSemId wait forever.\n");
        /* get semaphore wait forever, LOS_SemPend return until has been get semaphore */
        ret = LOS_SemPend(g_demoSemId, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            printf("Task1 wait_forever and got semphore g_demoSemId successfully.\n");
            LOS_SemPost(g_demoSemId);
            ret = InspectStatusSetById(LOS_INSPECT_SEM, LOS_INSPECT_STU_SUCCESS);
            if (ret != LOS_OK) {
                printf("Set inspect status failed.\n");
            }
            return;
        }
    }
    return;
}

STATIC VOID Task2Entry(VOID)
{
    UINT32 ret;
    printf("Task2 try get semaphore g_demoSemId wait forever.\n");
    /* wait forever get semaphore */
    ret = LOS_SemPend(g_demoSemId, LOS_WAIT_FOREVER);
    if (ret == LOS_OK) {
        printf("Task2 get semaphore g_demoSemId and then delay 20ticks.\n");
    }

    /* task delay 20 ticks */
    LOS_TaskDelay(DELAY_INTERVAL1);

    printf("Task2 post semaphore g_demoSemId.\n");
    /* release semaphore */
    LOS_SemPost(g_demoSemId);

    return;
}

UINT32 SemphoreDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("Kernel semaphore demo start to run.\n");

    /* create semaphore */
    ret = LOS_SemCreate(0, &g_demoSemId);
    if (ret != LOS_OK) {
        printf("Creat the semaphore failed.\n");
        return LOS_NOK;
    }

    LOS_TaskLock();
    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task1Entry;
    taskInitParam.pcName = "SemphoreDemoTask1";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio = TASK_PRIOR;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_demoTask1Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create semaphore task1 failed.\n");
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Task2Entry;
    taskInitParam.pcName = "SemphoreDemoTask2";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio = (TASK_PRIOR - 1);
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTask2Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create semaphore task2 failed.\n");
        /* delete task1 */
        if (LOS_OK != LOS_TaskDelete(g_demoTask1Id)) {
            printf("Delete semaphore task1 failed.\n");
        }
        LOS_TaskUnlock();
        return LOS_NOK;
    }

    LOS_TaskUnlock();
    /* post semaphore */
    ret = LOS_SemPost(g_demoSemId);
    if (ret != LOS_OK) {
        printf("Post the semaphore failed.\n");
        return LOS_NOK;
    }

    /* task delay 40 ticks */
    LOS_TaskDelay(DELAY_INTERVAL2);

    /* delete semaphore */
    LOS_SemDelete(g_demoSemId);

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
