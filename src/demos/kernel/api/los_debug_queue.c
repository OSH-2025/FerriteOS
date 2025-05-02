/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Kernel Test Queue Demo Implementation
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

#if LOS_KERNEL_DEBUG_QUEUE
#include "los_debug_queue.h"
#include "los_task.h"
#include "los_queue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HI_TASK_PRIOR   4
#define LO_TASK_PRIOR   5
#define DELAY_INTERVAL  1000
#define QUEUE_SIZE      LOS_KERNEL_QUEUE_SIZE
#define QUEUE_MSGLEN    LOS_KERNEL_QUEUE_MESSAGELEN

STATIC UINT32 g_demoTask1Id;
STATIC UINT32 g_demoTask2Id;
STATIC UINT32 g_demoQueueId;
STATIC BOOL g_demoDoneFlag = FALSE;

STATIC VOID SendTaskEntry(VOID)
{
    UINT32 ret;
    CHAR sendMsgBuf[] = LOS_KERNEL_QUEUE_MESSAGE;
    UINT32 msgLen = sizeof(sendMsgBuf);
    INT32 i;

    for (i = 0; i < LOS_KERNEL_QUEUE_CYCLE_TIMES; i++) {
        ret = LOS_QueueWriteCopy(g_demoQueueId, sendMsgBuf, msgLen, LOS_KERNEL_QUEUE_OVERTIME);
        if (ret != LOS_OK) {
            printf("Send message failed, error: %x.\n", ret);
        } else {
            printf("Send message ok, message: %s\n", sendMsgBuf);
        }
        LOS_TaskDelay(LOS_KERNEL_QUEUE_SENDINTERVAL);
    }
}

STATIC VOID ReceiveTaskEntry(VOID)
{
    UINT32 ret;
    CHAR readBuf[QUEUE_MSGLEN] = {0};
    UINT32 readLen = QUEUE_MSGLEN;
    QUEUE_INFO_S queueInfo;
    INT32 i;

    for (i = 0; i < LOS_KERNEL_QUEUE_CYCLE_TIMES; i++) {
        readLen = QUEUE_MSGLEN;
        ret = LOS_QueueReadCopy(g_demoQueueId, readBuf, &readLen, LOS_KERNEL_QUEUE_OVERTIME);
        if (ret != LOS_OK) {
            printf("Receive message failed, error: %x.\n", ret);
        } else {
            printf("Receive message : %s.\n", (CHAR *)readBuf);
        }

        LOS_QueueInfoGet(g_demoQueueId, &queueInfo);
        printf("\tQueue size : %u\n\tCurrent queue usage : %u\n", QUEUE_SIZE, queueInfo.usReadableCnt);
        LOS_TaskDelay(LOS_KERNEL_QUEUE_RECEIVEINTERVAL);
    }

    LOS_QueueDelete(g_demoQueueId);
    printf("Delete the queue successfully.\n");
    g_demoDoneFlag = TRUE;
}

UINT32 QueueDebug(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    printf("\nKernel debug queue start to run.\n");
    LOS_TaskLock();
    /* create task */
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)SendTaskEntry;
    taskInitParam.pcName = "DebugQueueSendTask";
    taskInitParam.usTaskPrio = LO_TASK_PRIOR;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTask1Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create queue sending Task failed.\n");
        LOS_TaskUnlock();
        return LOS_NOK;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)ReceiveTaskEntry;
    taskInitParam.pcName = "DebugQueueReceiveTask";
    taskInitParam.usTaskPrio = HI_TASK_PRIOR;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTask2Id, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create queue receiving Task failed.\n");
        if (LOS_OK != LOS_TaskDelete(g_demoTask1Id)) {
            printf("Delete queue sending Task failed.\n");
        }
        LOS_TaskUnlock();
        return LOS_NOK;
    }

    /* create queue */
    ret = LOS_QueueCreate("demoQueue", QUEUE_SIZE, &g_demoQueueId, 0, QUEUE_MSGLEN);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();
        printf("Create the queue failed, error : %x.\n", ret);
        return ret;
    }
    printf("Create the queue successfully.\n");
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
