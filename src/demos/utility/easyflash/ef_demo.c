/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Easyflash Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-01
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

#include "ef_demo.h"
#include "easyflash.h"
#include "los_task.h"
#include "los_event.h"
#include "los_swtmr.h"
#include "los_mux.h"
#include "sys_init.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DEFAULT_TASK_PRIORITY   7
#define DEFAULT_TASK_STACK_SIZE 2048

#define READ_EVENT 0x1 // read event marker bit
#define WRITE_EVENT 0x2 // write event marker bit

#define RUN_COUNT 3 // swtmr run count
#define SWTMR_PERIOD 2000 // swtmr run period

STATIC UINT32 g_demoTaskId;
STATIC EVENT_CB_S g_eventGroup;
STATIC UINT16 g_timerHandle;
STATIC UINT32 g_mutexHandle;

STATIC VOID EventTask(VOID)
{
    UINT32 ret;
    UINT32 count = 0;
    while (1) {
        /* read event */
        ret = LOS_EventRead(&g_eventGroup, READ_EVENT | WRITE_EVENT, LOS_WAITMODE_OR, LOS_WAIT_FOREVER);
        if ((ret & READ_EVENT) == READ_EVENT) {
            (VOID)LOS_MuxPend(g_mutexHandle, LOS_WAIT_FOREVER);
            printf("Get Env, TickCount=%s\n", ef_get_env("TickCount"));
            (VOID)LOS_MuxPost(g_mutexHandle);
            /* clear read event mark */
            (VOID)LOS_EventClear(&g_eventGroup, ~READ_EVENT);
            break;
        }

        if ((ret & WRITE_EVENT) == WRITE_EVENT) {
            (VOID)LOS_MuxPend(g_mutexHandle ,LOS_WAIT_FOREVER);
            /* clear write event mark */
            UINT64 tickTime = LOS_TickCountGet();
            CHAR tick[64];
            printf("Save env, \"TickCount\"=%llu\n", tickTime);
            (VOID)sprintf_s(tick, sizeof(tick), "%llu", tickTime);
            (VOID)ef_set_and_save_env("TickCount", tick);
            count += 1;
            (VOID)LOS_MuxPost(g_mutexHandle);
            (VOID)LOS_EventClear(&g_eventGroup, ~WRITE_EVENT);
            if (count > RUN_COUNT) {
                (VOID)LOS_EventWrite(&g_eventGroup, READ_EVENT);
            }
        }
    }
    printf("Swtmr delete.\n");
    (VOID)LOS_SwtmrDelete(g_timerHandle);
    printf("Mutex delete.\n");
    (VOID)LOS_MuxDelete(g_mutexHandle);
    printf("Event destroy.\n");
    (VOID)LOS_EventDestroy(&g_eventGroup);
    printf("Easyflash task delete.\n");
    (VOID)LOS_TaskDelete(g_demoTaskId);
}

STATIC VOID SwtmrCallback(VOID)
{
    printf("Ready to write Event.\n");
    (VOID)LOS_EventWrite(&g_eventGroup, WRITE_EVENT);
}

STATIC VOID EasyflashDemoEntry(VOID)
{
    if (easyflash_init() == EF_NO_ERR) {
       (VOID)ef_set_and_save_env("TickCount", "0");
       EventTask();
    }
}

VOID EasyFlashDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    ret = LOS_EventInit(&g_eventGroup);
    if (ret != LOS_OK) {
        printf("Event init failed.\n");
        return;
    }

    ret = LOS_MuxCreate(&g_mutexHandle);
    if (ret != LOS_OK) {
        printf("Create Mutex failed.\n");
        return;
    }

    ret = LOS_SwtmrCreate(SWTMR_PERIOD, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)SwtmrCallback, &g_timerHandle, 0);
    if (ret != LOS_OK) {
        printf("Create Swtmr failed.\n");
        return;
    }
    (VOID)LOS_SwtmrStart(g_timerHandle); // start run software timer

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)EasyflashDemoEntry;
    taskInitParam.pcName = "EasyflashDemoTask";
    taskInitParam.uwStackSize = DEFAULT_TASK_STACK_SIZE;
    taskInitParam.usTaskPrio = DEFAULT_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create easyflash demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

