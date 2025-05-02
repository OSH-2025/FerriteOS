/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Agriculture Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-20
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

#include "e53_ia1/E53_IA1.h"
#include "lcd.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TASK_PRIORITY           7
#define TASK_STACK_SIZE         0x1000
#define REFRESH_TIME            1000    //ms
#define AUTO_CONTROL_TIME       60      //s
#define MAX_BUFLEN              50

STATIC UINT32 g_demoTaskId;

STATIC VOID LcdShowAgriculture(E53IA1DataType *data)
{
    INT32 ret0, ret1, ret2, ret3, ret4;
    CHAR sData[5][MAX_BUFLEN] = {0};
    ret0 = snprintf_s(sData[0], MAX_BUFLEN, MAX_BUFLEN - 1, "Lux : %.2f", data->lux);
    ret1 = snprintf_s(sData[1], MAX_BUFLEN, MAX_BUFLEN - 1, "Temperature : %.2f", data->temperature);
    ret2 = snprintf_s(sData[2], MAX_BUFLEN, MAX_BUFLEN - 1, "Humidity : %.2f", data->humidity);
    ret3 = snprintf_s(sData[3], MAX_BUFLEN, MAX_BUFLEN - 1, "Light : %s", GetLightStatus() ? " ON" : "OFF");
    ret4 = snprintf_s(sData[4], MAX_BUFLEN, MAX_BUFLEN - 1, "Motor : %s", GetMoterStatus() ? " ON" : "OFF");
    if ((ret0 < 0) || (ret1 < 0) || (ret2 < 0) || (ret3 < 0) || (ret4 < 0)) {
        return;
    }
    /* param1,parma2 : starting point coordinates
       param3 : width of string
       param4 : height of string
       param5 : size of string
       param6 : string display
    */
    LCD_ShowString(20, 20, 240, 16, 16, sData[0]);
    LCD_ShowString(20, 60, 240, 16, 16, sData[1]);
    LCD_ShowString(20, 100, 240, 16, 16, sData[2]);
    LCD_ShowString(20, 140, 240, 16, 16, sData[3]);
    LCD_ShowString(20, 180, 240, 16, 16, sData[4]);
}

VOID AutoControl(E53IA1DataType *data)
{
    //20 : when lux < 20, set light on.
    if (data->lux < 20) {
        SetLightStatus(ON);
    } else {
        SetLightStatus(OFF);
    }
    //30 : when temperature > 30, set moter on.
    if (data->temperature > 30) {
        SetMoterStatus(ON);
    } else {
        SetMoterStatus(OFF);
    }
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 count = 0;
    E53IA1Init();
    KeyInit();
    LCD_Clear(WHITE);
    while (1) {
        E53IA1DataType data = {0};
        LOS_TaskDelay(REFRESH_TIME);
        E53IA1Read(&data);
        if (count > AUTO_CONTROL_TIME) {
            AutoControl(&data);
            count = 0;
        }
        LcdShowAgriculture(&data);
        count++;
    }
}

VOID AgricultureDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TASK_STACK_SIZE;
    taskInitParam.pcName = "AgricultureDemoTask";
    taskInitParam.usTaskPrio = TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create agriculture demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
