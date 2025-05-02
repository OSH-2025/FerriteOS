/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Task Demo Implementation
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

#include "los_api_task.h"
#include "los_task.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HI_TASK_PRIOR       4
#define LO_TASK_PRIOR       5
#define DELAY_INTERVAL1     5
#define DELAY_INTERVAL2     10
#define DELAY_INTERVAL3     40

STATIC UINT32 g_demoTaskHiId;
STATIC UINT32 g_demoTaskLoId;

STATIC UINT32 HiTaskEntry(VOID)
{
    UINT32 ret;

    printf("Enter high priority task handler.\n");

    /* task delay 5 ticks, task will be suspend */
    ret = LOS_TaskDelay(DELAY_INTERVAL1);
    if (ret != LOS_OK) {
        printf("Delay task failed.\n");
        return LOS_NOK;
    }

    /* task resumed */
    printf("High priority task LOS_TaskDelay successfully.\n");

    /* suspend self */
    ret = LOS_TaskSuspend(g_demoTaskHiId);
    if (ret != LOS_OK) {
        printf("Suspend high priority task failed.\n");
        ret = InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
        return LOS_NOK;
    }

    printf("High priority task LOS_TaskResume successfully.\n");

    ret = InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_SUCCESS);
    if (ret != LOS_OK) {
        printf("Set inspect status failed.\n");
    }

    /* delete self */
    if (LOS_TaskDelete(g_demoTaskHiId) != LOS_OK) {
        printf("Delete high priority task failed.\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

STATIC UINT32 LoTaskEntry(VOID)
{
    UINT32 ret;

    printf("Enter low priority task handler.\n");

    /* task delay 10 ticks, task will be suspend */
    ret = LOS_TaskDelay(DELAY_INTERVAL2);
    if (ret != LOS_OK) {
        printf("Delay low priority task failed.\n");
        return LOS_NOK;
    }

    printf("High priority task LOS_TaskSuspend successfully.\n");

    /* resumed task g_demoTaskHiId */
    ret = LOS_TaskResume(g_demoTaskHiId);
    if (ret != LOS_OK) {
        printf("Resume high priority task failed.\n");
        ret = InspectStatusSetById(LOS_INSPECT_TASK, LOS_INSPECT_STU_ERROR);
        if (LOS_OK != ret) {
            printf("Set inspect status failed.\n");
        }
        return LOS_NOK;
    }

    /* delete self */
    if (LOS_TaskDelete(g_demoTaskLoId) != LOS_OK) {
        printf("Delete low priority task failed.\n");
        return LOS_NOK;
    }

    return LOS_OK;
}

UINT32 TaskDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("\nKernel task demo start to run.\n");
    /* lock task schedule */
    LOS_TaskLock();

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)HiTaskEntry;
    taskInitParam.usTaskPrio = HI_TASK_PRIOR;
    taskInitParam.pcName = "TaskDemoHiTask";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_demoTaskHiId, &taskInitParam);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();
        printf("Create high priority task failed.\n");
        return LOS_NOK;
    }
    printf("Create high priority task successfully.\n");

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LoTaskEntry;
    taskInitParam.usTaskPrio = LO_TASK_PRIOR;
    taskInitParam.pcName = "TaskDemoLoTask";
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskLoId, &taskInitParam);
    if (ret != LOS_OK) {
        /* delete high prio task */
        if (LOS_OK != LOS_TaskDelete(g_demoTaskHiId)) {
            printf("Delete high priority task failed.\n");
        }
        LOS_TaskUnlock();
        printf("Create low priority task failed.\n");
        return LOS_NOK;
    }
    printf("Create low priority task successfully.\n");

    /* unlock task schedule */
    LOS_TaskUnlock();
    LOS_TaskDelay(DELAY_INTERVAL3);

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
