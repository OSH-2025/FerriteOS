/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Ai Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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

#include "ai_demo.h"
#include "los_task.h"

#ifdef LOSCFG_DEMOS_AI_MNIST
#include "mnist_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AI_MNIST_QUANT
#include "mnist_quant_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AI_MOBILENET
#include "mobilenet_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AI_RESNET
#include "resnet_demo.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define AI_TASK_PRIORITY     5
#define AI_DEMO_TASK_SIZE    0x6000
STATIC UINT32 g_demoTaskId;

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Ai demo task start to run.\n");
#ifdef LOSCFG_DEMOS_AI_MNIST
    mnist_demo();  // 84kb
#endif /* LOSCFG_DEMOS_AI_MNIST */

#ifdef LOSCFG_DEMOS_AI_MNIST_QUANT
    mnist_quant_demo(); // 47kb
#endif /* LOSCFG_DEMOS_AI_MNIST_QUANT */

#ifdef LOSCFG_DEMOS_AI_MOBILENET
    mobilenet_demo(); // 170kb
#endif /* LOSCFG_DEMOS_AI_MOBILENET */

#ifdef LOSCFG_DEMOS_AI_RESNET
    resnet_demo(); // 275kb
#endif /* LOSCFG_DEMOS_AI_RESNET */
    printf("Ai demo task finished.\n");
}

VOID AiDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = AI_TASK_PRIORITY;
    taskInitParam.pcName = "AiDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = AI_DEMO_TASK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Creat ai demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
