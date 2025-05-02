/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: User Task Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-14
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

#include "los_task_pri.h"
#include "demo_entry.h"

#ifndef LOSCFG_PLATFORM_ESP32_QEMU
#define LED_TASK_PRIORITY   7
#define LED_TASK_STACK_SIZE 0x500

#define GPIO_OUT_REG        ((volatile UINT32 *)0x3FF44004) /* GPIO 0-31 output enable register */
#define GPIO_ENABLE_REG     ((volatile UINT32 *)0x3FF44020) /* GPIO 0-31 output register */

VOID LedGpio(VOID)
{
    UINT32 flag = 0;
    *(GPIO_ENABLE_REG) |= (1 << 2); /* GPIO 2 output enable */
    while (1) {
        if (flag == 0) {
            *(GPIO_OUT_REG) |= (1 << 2); /* GPIO 2 output 1 */
        } else {
            *(GPIO_OUT_REG) &= ~(1 << 2); /* GPIO 2 output 0 */
        }
        flag = ~flag;
        (VOID)LOS_TaskDelay(1000); /* 1000: delay 1000ms */
    }
    *(GPIO_ENABLE_REG) &= ~(1 << 2); /* GPIO 2 output disable */
}

VOID LedTask(VOID)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S taskInitParam = {0};

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LedGpio;
    taskInitParam.uwStackSize = LED_TASK_STACK_SIZE;
    taskInitParam.pcName = "LedTask";
    taskInitParam.usTaskPrio = LED_TASK_PRIORITY;
    ret = LOS_TaskCreate(&taskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create led task failed.\n");
    }
}
#endif

VOID app_init(VOID)
{
    printf("app init!\n");
#ifndef LOSCFG_PLATFORM_ESP32_QEMU
    LedTask();
#endif
    DemoEntry();
}
