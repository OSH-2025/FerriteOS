/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Kernel Test Mutex Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-23
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

#if LOS_KERNEL_DEBUG_MUTEX
#include "los_debug_mutex.h"
#include "los_task.h"
#include "los_mux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HI_TASK_PRIOR   4
#define LO_TASK_PRIOR   5
#define DELAY_INTERVAL  1000

STATIC UINT32 g_demoTaskHiId;
STATIC UINT32 g_demoTaskLoId;
STATIC UINT32 g_demoMux01;
STATIC BOOL g_demoDoneFlag = FALSE;

STATIC VOID TaskHiEntry(VOID)
{
    UINT32 ret;
    INT32 i;

    for (i = 0; i < LOS_KERNEL_MUTEX_CYCLE_TIMES; i++) {
        ret = LOS_MuxPend(g_demoMux01, LOS_KERNEL_MUTEX_OVERTIME);
        if (ret == LOS_OK) {
            printf("High priority task get the mutex successfully.\n");
            LOS_TaskDelay(DELAY_INTERVAL);
            printf("High priority task release the mutex.\n");
            LOS_MuxPost(g_demoMux01);
        } else {
            printf("High priority task get the mutex timeout.\n");
        }
    }
}

STATIC VOID TaskLoEntry(VOID)
{
    UINT32 ret;
    INT32 i;

    for (i = 0; i < LOS_KERNEL_MUTEX_CYCLE_TIMES; i++) {
        ret = LOS_MuxPend(g_demoMux01, LOS_KERNEL_MUTEX_OVERTIME);
        if (ret == LOS_OK) {
            printf("Low priority task get the mutex successfully.\n");
            LOS_TaskDelay(DELAY_INTERVAL);
            printf("Low priority task release the mutex.\n");
            LOS_MuxPost(g_demoMux01);
        } else {
            printf("Low priority task get the mutex timeout.\n");
        }
    }

    LOS_MuxDelete(g_demoMux01);
    printf("Delete the mutex successfully.\n");
    g_demoDoneFlag = TRUE;
}

UINT32 MutexDebug(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("\nKernel debug mutex start to run.\n");
    LOS_TaskLock();
    printf("Mutex init.\n");
    ret = LOS_MuxCreate(&g_demoMux01);
    if (ret != LOS_OK) {
        printf("Create the mutex failed.\n");
        return ret;
    }
    printf("Create the mutex successfully.\n");

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskHiEntry;
    taskInitParam.usTaskPrio = HI_TASK_PRIOR;
    taskInitParam.pcName = "DebugMutexTaskHiTask";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskHiId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create high priority task failed.\n");
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskLoEntry;
    taskInitParam.usTaskPrio = LO_TASK_PRIOR;
    taskInitParam.pcName = "DebugMutexTaskLoTask";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskLoId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create low priority task failed.\n");
        if (LOS_OK != LOS_TaskDelete(g_demoTaskHiId)) {
            printf("Delete high priority task failed.\n");
        }
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    LOS_TaskUnlock();

    while (!g_demoDoneFlag) {
        LOS_TaskDelay(DELAY_INTERVAL);
    }

    return ret;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
