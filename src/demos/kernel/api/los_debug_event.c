/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Kernel Test Event Demo Implementation
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

#if LOS_KERNEL_DEBUG_EVENT
#include "los_debug_event.h"
#include "los_task.h"
#include "los_event.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TASK_PRIOR      4
#define DELAY_INTERVAL1 1000
#define DELAY_INTERVAL2 2500
#define EVENT_NUM       LOS_KERNEL_EVENT_ID

STATIC UINT32 g_demoTaskId;
STATIC EVENT_CB_S g_demoEvent;

STATIC VOID ReadTaskEntry(VOID)
{
    UINT32 event;
    INT32 i;
    for (i = 0; i < LOS_KERNEL_EVENT_CYCLE_TIMES; i++) {
        event = LOS_EventRead(&g_demoEvent, EVENT_NUM, LOS_WAITMODE_AND, LOS_KERNEL_EVENT_OVERTIME);
        if (event == EVENT_NUM) {
            printf("Read the event : 0x%x.\n", event);
        } else {
            printf("Read the event timeout.\n");
        }
        LOS_TaskDelay(DELAY_INTERVAL1);
    }
}

UINT32 EventDebug(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("\nKernel debug event start to run.\n");
    LOS_TaskLock();
    /* event init */
    ret = LOS_EventInit(&g_demoEvent);
    if (ret != LOS_OK) {
        printf("Init the event failed.\n");
        return ret;
    }
    printf("Init the event successfully.\n");

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry  = (TSK_ENTRY_FUNC)ReadTaskEntry;
    taskInitParam.usTaskPrio    = TASK_PRIOR;
    taskInitParam.pcName        = "DebugEventReadTask";
    taskInitParam.uwStackSize   = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved      = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create read event task failed.\n");
        LOS_TaskUnlock();
        return LOS_NOK;
    }

    printf("Write event.\n");
    LOS_EventWrite(&g_demoEvent, EVENT_NUM);
    LOS_TaskUnlock();

    LOS_TaskDelay(DELAY_INTERVAL2);
    printf("Clear the event flag.\n");
    LOS_EventClear(&g_demoEvent, ~EVENT_NUM);
    LOS_TaskDelay(DELAY_INTERVAL2);

    LOS_EventDestroy(&g_demoEvent);
    printf("Delete the event successfully.\n");

    return ret;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
