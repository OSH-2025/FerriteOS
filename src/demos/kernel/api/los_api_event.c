/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Event Demo Implementation
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

#include "los_event.h"
#include "los_task.h"
#include "los_api_event.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* task pid */
STATIC UINT32 g_demoTaskId;
/* event control struct */
STATIC EVENT_CB_S  g_demoEvent;

/* wait event type */
#define EVENT_WAIT              0x00000001
#define EVENT_READ_OVERTIME     100
#define TASK_PRIOR              5

/* example task entry function */
STATIC VOID ReadTaskEntry(VOID)
{
    UINT32 event;
    UINT32 ret;

    /*
     * timeout, WAITMODE to read event, timeout is 100 ticks,
     * if timeout, wake task directly
     */
    printf("Read event task wait event 0x%x.\n", EVENT_WAIT);

    event = LOS_EventRead(&g_demoEvent, EVENT_WAIT, LOS_WAITMODE_AND, EVENT_READ_OVERTIME);
    if (event == EVENT_WAIT) {
        printf("Read the event : 0x%x.\n", event);
        ret = InspectStatusSetById(LOS_INSPECT_EVENT, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
    } else {
        printf("Read the event timeout.\n");
        ret = InspectStatusSetById(LOS_INSPECT_EVENT, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
    }
    return;
}

UINT32 EventDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("Kernel event demo start to run.\n");
    /* event init */
    ret = LOS_EventInit(&g_demoEvent);
    if (ret != LOS_OK) {
        printf("Init the event failed.\n");
        return LOS_NOK;
    }

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ReadTaskEntry;
    taskInitParam.pcName = "EventDemoReadTask";
    taskInitParam.usTaskPrio = TASK_PRIOR;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create read event task failed.\n");
        return LOS_NOK;
    }

    /* write event */
    printf("Write event.\n");
    ret = LOS_EventWrite(&g_demoEvent, EVENT_WAIT);
    if (ret != LOS_OK) {
        printf("Write the event failed.\n");
        return LOS_NOK;
    }

    /* clear event */
    printf("Current event id : %d\n", g_demoEvent.uwEventID);
    LOS_EventClear(&g_demoEvent, ~g_demoEvent.uwEventID);
    printf("Current event id : %d\n", g_demoEvent.uwEventID);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
