/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: User Task Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-23
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

#include "main.h"
#include "demo_entry.h"
#include "los_task_pri.h"

#define TASK_DELAY 1000

STATIC UINT32 LedTask(VOID)
{
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
        LOS_TaskDelay(TASK_DELAY);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        LOS_TaskDelay(TASK_DELAY);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
        LOS_TaskDelay(TASK_DELAY);
    }
    return 0;
}

STATIC UINT32 LedTaskCreate(VOID)
{
    INT32 ret;
    UINT32 taskId = 0;
    TSK_INIT_PARAM_S ledTaskParam;

    ret = memset_s(&ledTaskParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }
    ledTaskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LedTask;
    ledTaskParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ledTaskParam.pcName = "ledTask";
    ledTaskParam.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    ledTaskParam.uwResved = LOS_TASK_STATUS_DETACHED;
    return LOS_TaskCreate(&taskId, &ledTaskParam);
}

VOID app_init(VOID)
{
    printf("app init!\n");
    (VOID)LedTaskCreate();
    DemoEntry();
}
