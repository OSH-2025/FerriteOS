/* ----------------------------------------------------------------------------
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: LedLink Demo Implementation
* Author: Huawei LiteOS Team
* Create: 2021-10-24
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1.Redistributions of source code must retain the above copyright notice, this list of
* conditions and the following disclaimer.
* 2.Redistributions in binary form must reproduce the above copyright notice, this list
* of conditions and the following disclaimer in the documentation and/or other materials
* provided with the distribution.
* 3.Neither the name of the copyright holder nor the names of its contributors may be used
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
*--------------------------------------------------------------------------- */
#include "gpio.h"
#include "stdio.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID LedInit(UINT8 ledPin)
{
    gpio_func_select(ledPin, GPIO_FUNC_OUTPUT);
}

VOID LedOn(UINT8 ledPin)
{
    gpio_output_set(ledPin);
}

VOID LedOff(UINT8 ledPin)
{
    gpio_output_clr(ledPin);
}

VOID LedTest(void)
{
    UINT8 ledPin = GPIO_5;
    LedInit(ledPin);
    while (true) {
        printf("led(%d): on\n", ledPin);
        LedOn(ledPin);
        LOS_TaskDelay(1000); // delay time 1000ms
        printf("led(%d): off\n", ledPin);
        LedOff(ledPin);
        LOS_TaskDelay(1000); // delay time 1000ms
    }
}

VOID LedDemo(VOID)
{
    UINT32 ret;
    UINT32 ledTaskId;
    TSK_INIT_PARAM_S taskInitParam;

    printf("Led Demo on gpio pin:%d\n", GPIO_5);
    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }

    taskInitParam.usTaskPrio = 6;  // set task priority to 6
    taskInitParam.pcName = "LedDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LedTest;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ret = LOS_TaskCreate(&ledTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create agenttiny demo task failed.\n");
    }
}
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
