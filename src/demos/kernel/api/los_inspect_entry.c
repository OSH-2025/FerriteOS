/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Demo Inspect Entry Implementation
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

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/
#include "los_inspect_entry.h"
#include "los_task.h"

/* task schedule */
#include "los_api_task.h"
/* dynamic memory */
#include "los_api_dynamic_mem.h"
/* static memory */
#include "los_api_static_mem.h"
/* interrupt */
#include "los_api_interrupt.h"
/* message queue */
#include "los_api_msgqueue.h"
/* event  */
#include "los_api_event.h"
/* mutex */
#include "los_api_mutex.h"
/* semphore */
#include "los_api_sem.h"
/* sw timer */
#include "los_api_timer.h"
/* system tick */
#include "los_api_systick.h"
/* dlist */
#include "los_api_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TASK_PRIOR      10
#define DELAY_INTERVAL  500

/*****************************************************************************
    global var
 *****************************************************************************/
STATIC UINT32 g_demoInspectErrCnt = 0;
STATIC UINT32 g_demoTaskId;
InspectStu g_enCurStatus = LOS_INSPECT_STU_START;

STATIC InspectDef g_inspect[LOS_INSPECT_BUFF] = {

    {LOS_INSPECT_TASK, LOS_INSPECT_STU_START, TaskDemo, "task"},

    {LOS_INSPECT_DMEM, LOS_INSPECT_STU_START, DynMemDemo, "dynamic memory"},

    {LOS_INSPECT_SMEM, LOS_INSPECT_STU_START, StaticMemDemo, "static memory"},

    {LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_START, InterruptDemo, "interrupt"},

    {LOS_INSPECT_MSG, LOS_INSPECT_STU_START, MsgQueueDemo, "message queue"},

    {LOS_INSPECT_EVENT, LOS_INSPECT_STU_START, EventDemo, "event"},

    {LOS_INSPECT_MUTEX, LOS_INSPECT_STU_START, MutexLockDemo, "mutex"},

    {LOS_INSPECT_SEM, LOS_INSPECT_STU_START, SemphoreDemo, "semaphore"},

    {LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_START, GetTickDemo, "systick"},

    {LOS_INSPECT_TIMER, LOS_INSPECT_STU_START, SwTimerDemo, "timer"},

    {LOS_INSPECT_LIST, LOS_INSPECT_STU_START, ListDemo, "list"},

};

/*****************************************************************************
 Function    : InspectStatusSetById
 Description : Set InspectStatus by id
 Input       : InspectId  inspectId
               InspectStu inspectStu
 Output      : None
 Return      : None
 *****************************************************************************/
UINT32 InspectStatusSetById(InspectId inspectId, InspectStu inspectStu)
{
    UINT32 index;

    if (inspectId >= LOS_INSPECT_BUFF) {
        printf("\ninspectId = [%d] failed.\n", inspectId);
        return LOS_NOK;
    }

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        if (inspectId == g_inspect[index].inspectId) {
            g_inspect[index].status = inspectStu;
            break;
        }
    }

    if (index == LOS_INSPECT_BUFF) {
        printf("\ninspectId = [%d] not find.\n", inspectId);
        return LOS_NOK;
    } else {
        return LOS_OK;
    }
}

STATIC VOID RunDemoById(UINT32 id)
{
    UINT32 ret;

    ret = g_inspect[id].InspectFunc();
    do {
        if ((g_inspect[id].status == LOS_INSPECT_STU_SUCCESS) && (ret == LOS_OK)) {
            printf("Kernel %s demo finished.\n\n", g_inspect[id].name);
            g_enCurStatus = LOS_INSPECT_STU_SUCCESS;
        } else if (g_inspect[id].status == LOS_INSPECT_STU_ERROR) {
            g_enCurStatus = LOS_INSPECT_STU_ERROR;
            printf("Inspect %s error, g_demoInspectErrCnt = [%d]\n\n",
                   g_inspect[id].name, g_demoInspectErrCnt);
        }
        (VOID)LOS_TaskDelay(DELAY_INTERVAL);
    } while ((g_inspect[id].status == LOS_INSPECT_STU_START) || (g_enCurStatus != g_inspect[id].status));
}

/*****************************************************************************
 Function    : InspectByID
 Description : Inspect function By ID
 Input       : InspectId  inspectId
 Output      : None
 Return      : LOS_NOK/LOS_OK
 *****************************************************************************/
UINT32 InspectByID(InspectId inspectId)
{
    UINT32 index;

    if (inspectId >= LOS_INSPECT_BUFF) {
        printf("\ninspectId = [%d] failed.\n", inspectId);
        return LOS_NOK;
    }

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        if (inspectId == g_inspect[index].inspectId) {
            if (g_inspect[index].InspectFunc == NULL) {
                printf("inspectId = [%d] failed, inspectFunc is null.\n\n", inspectId);
                return LOS_NOK;
            }

            RunDemoById(index);
            break;
        }
    }
    if (index == LOS_INSPECT_BUFF) {
        return LOS_NOK;
    } else {
        return g_enCurStatus ? LOS_NOK : LOS_OK;
    }
}

/*****************************************************************************
 Function    : KernelInspectTaskEntry
 Description : Inspect Task Deal
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
STATIC VOID KernelInspectTaskEntry(VOID)
{
    UINT32 ret;
    UINT32 index;
    g_demoInspectErrCnt = 0;

    /* output a message on hyperterminal using printf function */
    LOS_TaskDelay(DELAY_INTERVAL);
    printf("\nLos inspect start.\n");

    for (index = 0; index < LOS_INSPECT_BUFF; index++) {
        ret = InspectByID((InspectId)index);
        if (ret != LOS_OK) {
            g_demoInspectErrCnt++;
        }
    }

    printf("Inspect completed, g_demoInspectErrCnt = %d.\n\n", g_demoInspectErrCnt);
}

/*****************************************************************************
 Function    : KernelDemoInspectEntry
 Description : Create Inspect task
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
VOID KernelInspectDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)KernelInspectTaskEntry;
    taskInitParam.pcName = "KernelInspectTask";
    taskInitParam.usTaskPrio = TASK_PRIOR;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Kernel inspect Demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
