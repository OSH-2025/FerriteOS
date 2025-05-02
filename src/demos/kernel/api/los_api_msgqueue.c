/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Message Queue Demo Implementation
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

#include "los_api_msgqueue.h"
#include "los_task.h"
#include "los_queue.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define API_MSG_NUM         5
#define BUFF_OVERWRITE_NUM  2
#define QUEUE_MSGLEN        24
#define QUEUE_SIZE          5
#define DELAY_INTERVAL1     50
#define DELAY_INTERVAL2     500
#define TASK_PRIOR          9

STATIC UINT32 g_demoTask1Id;
STATIC UINT32 g_demoTask2Id;
STATIC UINT32 g_demoQueue;
STATIC CHAR g_demoBuf[] = "test is message x";

/* task1 send data */
STATIC VOID SendTaskEntry(UINT32 param)
{
    UINT32 i = 0;
    UINT32 ret;
    UINT32 len = sizeof(g_demoBuf);

    while (i < API_MSG_NUM) {
        g_demoBuf[len - BUFF_OVERWRITE_NUM] = '0' + i;
        i++;

        /* write g_demoBuf data to queue */
        ret = LOS_QueueWrite(g_demoQueue, g_demoBuf, len, 0);
        if (ret != LOS_OK) {
            printf("Send message failed, error: %x.\n", ret);
        }

        LOS_TaskDelay(DELAY_INTERVAL1);
    }
}

/* task2 Receive data */
STATIC VOID ReceiveTaskEntry(UINT32 param)
{
    UINT32 ret;
    UINTPTR readBuf = 0;
    UINT32 msgCount = 0;

    while (1) {
        /* read data from queue to readbuf */
        ret = LOS_QueueRead(g_demoQueue, &readBuf, QUEUE_MSGLEN, 0);
        if (ret != LOS_OK) {
            printf("Receive message failed, error: %x.\n", ret);
            break;
        } else {
            printf("Receive message : %s.\n", readBuf);
            msgCount++;
        }

        (VOID)LOS_TaskDelay(DELAY_INTERVAL1);
    }

    /* delete queue */
    if (LOS_QueueDelete(g_demoQueue) != LOS_OK) {
        printf("Delete the queue failed.\n");
    }
    printf("Delete the queue successfully.\n");

    if (API_MSG_NUM == msgCount) {
        ret = InspectStatusSetById(LOS_INSPECT_MSG, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
    } else {
        ret = InspectStatusSetById(LOS_INSPECT_MSG, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
    }
}

UINT32 MsgQueueDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("Kernel message queue demo start to run.\n");

    /* create queue */
    ret = LOS_QueueCreate("queue", QUEUE_SIZE, &g_demoQueue, 0, QUEUE_MSGLEN);
    if (ret != LOS_OK) {
        printf("Create the queue failed.\n");
        return LOS_NOK;
    }
    printf("Create the queue successfully.\n");

    LOS_TaskLock();
    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)SendTaskEntry;
    taskInitParam.usTaskPrio = TASK_PRIOR;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.pcName = "MsgQueueDemoSendTask";
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    taskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_demoTask1Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create queue sending Task failed.\n");
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ReceiveTaskEntry;
    taskInitParam.pcName = "MsgQueueDemoReceiveTask";
    ret = LOS_TaskCreate(&g_demoTask2Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create queue receiving Task failed.\n");
        if (LOS_OK != LOS_TaskDelete(g_demoTask1Id)) {
            printf("Delete queue sending Task failed.\n");
        }
        return LOS_NOK;
    }

    LOS_TaskUnlock();
    LOS_TaskDelay(DELAY_INTERVAL2);

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
