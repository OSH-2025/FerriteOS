/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tinyframe Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-28
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

#include "tinyframe_demo.h"
#include <stdio.h>
#include "los_task.h"
#include "TinyFrame.h"
#include "utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TINYFRAME_TASK_PRIORITY    6
#define TINYFRAME_TASK_STACK_SIZE  0x1000

STATIC UINT32 g_demoTaskId;
TinyFrame *g_tfDemo;

TF_Result IdListenerDemo(TinyFrame *tf, TF_Msg *msg)
{
    printf("IdListener demo\n");
    dumpFrameInfo(msg);
    return TF_CLOSE;
}

VOID TF_WriteImpl(TinyFrame *tf, const UINT8 *buff, UINT32 len)
{
    printf("--------------------\n");
    printf("\033[32mTF_WriteImpl - sending frame:\033[0m\n");

    dumpFrame(buff, len);
 
    // Send it back as if we received it
    TF_Accept(tf, buff, len);
}
 
TF_Result GenericListenerDemo(TinyFrame *tf, TF_Msg *msg)
{
    printf("GenericListener demo\n");
    dumpFrameInfo(msg);
    return TF_STAY;
}

STATIC VOID MasterDemo(VOID)
{
    bool ret;
    const CHAR *sendStr = "liteos";
    const CHAR *queryStr = "summer";
    g_tfDemo = TF_Init(TF_MASTER);
    if (g_tfDemo == NULL) {
        printf  ("Tinyframe init failed.\n");
        return;
    }
    ret = TF_AddGenericListener(g_tfDemo, GenericListenerDemo);
    if (!ret) {
        printf("Tinyframe add generic listener error.\n");
        return;
    }

    ret = TF_SendSimple(g_tfDemo, 1, (pu8)sendStr, (TF_LEN)(strlen(sendStr) + 1));
    if (!ret) {
        printf("Tinyframe send simple error.\n");
        return;
    }

    ret = TF_QuerySimple(g_tfDemo, 1, (pu8)queryStr, (TF_LEN)strlen(queryStr), IdListenerDemo, 0);
    if (!ret) {
        printf("TinyFrame query simple error.\n");
        return;
    }
}

STATIC VOID TfSimpleDemo(VOID)
{
    TF_Msg msg;
    bool ret;
    const CHAR *longStr = "welcome to liteos community.";
    const CHAR *tempStr = "Hello TinyFrame";

    // Set up the TinyFrame library, 1 = master, 0 = slave
    g_tfDemo = TF_Init(TF_MASTER);
    ret = TF_AddGenericListener(g_tfDemo, GenericListenerDemo);
    if (!ret) {
        printf("Tinyframe add generic listener failed.\n");
        return;
    }

    TF_ClearMsg(&msg);
    msg.type = 1;
    msg.data = (pu8)tempStr;
    msg.len = (TF_LEN)(strlen(tempStr) + 1);
    TF_Send(g_tfDemo, &msg);
    if (!ret) {
        printf("Tinyframe send failed.\n");
        return;
    }

    TF_ClearMsg(&msg);
    msg.type = 1;
    msg.data = (pu8)longStr;
    // add the null byte
    msg.len = (TF_LEN)(strlen(longStr) + 1);
    TF_Send(g_tfDemo, &msg);
    if (!ret) {
        printf("Tinyframe send failed.\n");
    }
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Tinyframe demo task start to run.\n");
    MasterDemo();
    TfSimpleDemo();
    printf("Tinyframe demo task finished.\n");
}

VOID TinyframeDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = TINYFRAME_TASK_PRIORITY;
    taskInitParam.pcName = "TinyframeDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TINYFRAME_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create tinyframe demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
