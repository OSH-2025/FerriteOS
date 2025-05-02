/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Mutex Demo Implementation
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

#include "los_api_mutex.h"
#include "los_mux.h"
#include "los_task.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HI_TASK_PRIOR       4
#define LO_TASK_PRIOR       5
#define DELAY_INTERVAL1     100
#define DELAY_INTERVAL2     300
#define MUTEX_OVERTIME      10

/* mux handle id */
STATIC UINT32 g_demoMux01;
/* task pid */
STATIC UINT32 g_demoTask1Id;
STATIC UINT32 g_demoTask2Id;

STATIC VOID MutexTask1Entry(VOID)
{
    UINT32 ret;

    printf("Task1 try to get mutex, wait 10 ticks.\n");
    /* get mux */
    ret = LOS_MuxPend(g_demoMux01, MUTEX_OVERTIME);
    if (ret == LOS_OK) {
        printf("Task1 get mutex g_demoMux01.\n");
        /* release mux */
        LOS_MuxPost(g_demoMux01);
        return;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT) {
        printf("Task1 timeout and try to get mutex, wait forever.\n");
        /* LOS_WAIT_FOREVER type get mux, LOS_MuxPend return until has been get mux */
        ret = LOS_MuxPend(g_demoMux01, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            printf("Task1 wait forever, got mutex g_demoMux01 successfully.\n");
            /* release mux */
            LOS_MuxPost(g_demoMux01);
            ret = InspectStatusSetById(LOS_INSPECT_MUTEX, LOS_INSPECT_STU_SUCCESS);
            if (LOS_OK != ret) {
                printf("Set inspect status failed.\n");
            }
            return;
        }
    }
    return;
}

STATIC VOID MutexTask2Entry(VOID)
{
    UINT32 ret;

    printf("Task2 try to get the mutex, wait forever.\n");
    /* get mux */
    ret = LOS_MuxPend(g_demoMux01, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        printf("Task2 LOS_MuxPend failed.\n");
        return;
    }

    printf("Task2 get mutex g_demoMux01 and suspend 100 ticks.\n");

    /* task delay 100 ticks */
    LOS_TaskDelay(DELAY_INTERVAL1);

    printf("Task2 resumed and post the g_demoMux01.\n");
    /* release mux */
    LOS_MuxPost(g_demoMux01);
    return;
}

UINT32 MutexLockDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("Kernel mutex demo start to run.\n");

    /* create mux */
    ret = LOS_MuxCreate(&g_demoMux01);
    if (ret != LOS_OK) {
        printf("Creat the mutex failed.\n");
        return LOS_NOK;
    }
    
    LOS_TaskLock();
    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MutexTask1Entry;
    taskInitParam.pcName = "MutexLockDemoTask1";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio = LO_TASK_PRIOR;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_demoTask1Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create mutex task1 failed.\n");
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MutexTask2Entry;
    taskInitParam.pcName = "MutexLockDemoTask2";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.usTaskPrio = HI_TASK_PRIOR;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTask2Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create mutex task2 failed.\n");
        if (LOS_OK != LOS_TaskDelete(g_demoTask1Id)) {
            printf("Delete mutex task1 failed.\n");
        }
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    LOS_TaskUnlock();

    /* task delay 300 ticks */
    LOS_TaskDelay(DELAY_INTERVAL2);
    /* delete mux */
    LOS_MuxDelete(g_demoMux01);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
