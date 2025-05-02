/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Kernel Test Task Demo Implementation
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

#if LOS_KERNEL_DEBUG_TASK
#include "los_debug_task.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DELAY_INTERVAL  1000

STATIC UINT32 g_demoTaskId;
STATIC BOOL g_demodoneFlag = FALSE;

STATIC VOID TaskEntry(VOID)
{
    INT32 i;
    UINT32 delayTime = LOS_KERNEL_TASK_DELAYINTERVAL;

    for (i = 0; i < LOS_KERNEL_TASK_CYCLE_TIMES; i++) {
        printf("Task running. Interval:%d\n", delayTime);
        LOS_TaskDelay(delayTime);
    }

    g_demodoneFlag = TRUE;
}

uint32_t TaskDebug(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    TSK_INFO_S taskInfo;

    printf("\nKernel debug task start to run.\n");

    LOS_TaskLock();
    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskEntry;
    taskInitParam.pcName = LOS_KERNEL_TASK_NAME;
    taskInitParam.usTaskPrio = LOS_KERNEL_TASK_PRIORITY;
    taskInitParam.uwStackSize = LOS_KERNEL_TASK_STACKSIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();
        printf("Create the task failed.\n");
        return ret;
    }
    printf("Create the task successfully.\n");

    LOS_TaskInfoGet(g_demoTaskId, &taskInfo);
    printf("\tTask information:\n\
            Task name:%s\n\
            Task id:%d\n\
            Task status:%d\n\
            Task priority:%d\n",
        taskInfo.acName, taskInfo.uwTaskID, taskInfo.usTaskStatus, taskInfo.usTaskPrio);

    printf("Start Scheduling.\n");
    LOS_TaskUnlock();

    while (!g_demodoneFlag) {
        LOS_TaskDelay(DELAY_INTERVAL);
    }
    if (LOS_TaskDelete(g_demoTaskId) != LOS_OK) {
        printf("Delete the task failed.\n");
        return ret;
    }
    printf("Delete the task successfully.\n");

    return ret;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
