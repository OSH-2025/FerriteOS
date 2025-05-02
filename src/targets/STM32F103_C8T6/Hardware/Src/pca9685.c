/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: pca9685 Implementation
 * Author: Huawei LiteOS Team
 * Create: 2022-02-10
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

#include "pca9685.h"
#include "los_task.h"

#define APP_TASK_PRIORITY   5

VOID Pca9685Write(UINT8 addr,UINT8 data)
{
    HAL_I2C_Mem_Write(&hi2c1, PCA9685_ADDR, addr, 1, &data, 1, 100);
}

UINT8 Pca9685Read(UINT8 addr)
{
    UINT8 data;

    HAL_I2C_Mem_Read(&hi2c1, PCA9685_ADDR, addr, 1, &data, 1, 100);

    return data;
}

VOID Pca9685Reset(VOID)
{
    Pca9685Write(PCA9685_MODE, 0x0);
}

VOID Pca9685SetFreq(FLOAT freq)
{
    UINT8 prescale;
    UINT8 oldMode;
    UINT8 newMode;
    double prescaleVal;

    /* refresh_rate = EXTCLK / (4096 * (prescale + 1)) */
    prescaleVal = 25000000; // EXTCLK
    prescaleVal /= 4096;
    prescaleVal /= freq;
    prescaleVal -= 1;
    prescale = (UINT32)prescaleVal;

    LOS_TaskLock();
    oldMode = Pca9685Read(PCA9685_MODE);
    newMode = (oldMode & 0x7F) | 0x10; // 0x10:bit4: sleep Mode control
    Pca9685Write(PCA9685_MODE, newMode);
    Pca9685Write(PCA9685_PRECALER, prescale);
    Pca9685Write(PCA9685_MODE, oldMode);

    HAL_Delay(20);
    Pca9685Write(PCA9685_MODE, oldMode | 0xa1); // 0xa1: bit7:RESTART bit0:ALLCALL
    LOS_TaskUnlock();
}

VOID Pca9685SetPwm(UINT8 num, UINT32 on, UINT32 off)
{
    Pca9685Write(LED0_ON_L + 4 * num, on);
    Pca9685Write(LED0_ON_H + 4 * num, on >> 8);
    Pca9685Write(LED0_OFF_L + 4 * num, off);
    Pca9685Write(LED0_OFF_H + 4 * num, off >> 8);
}

STATIC UINT32 Pca9685TestTask(VOID)
{
    UINT32 ret = LOS_OK;

    Pca9685Reset();
    Pca9685SetFreq(50); // pwm output : 50hz

    while (1) {
        Pca9685SetPwm(0,0,100); // test num 100
        LOS_TaskDelay(1000); // delay 1s
        Pca9685SetPwm(0,0,500); // test num 500
        LOS_TaskDelay(1000); // delay 1s
    }
    return ret;
}

INT32 Pca9685DemoTaskCreate(VOID)
{
    UINT32 ret = LOS_OK;
    UINT32 taskID = 0;
    TSK_INIT_PARAM_S taskInitParam;

    memset(&taskInitParam, 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.usTaskPrio = APP_TASK_PRIORITY;
    taskInitParam.pcName = "Pca9685TestTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Pca9685TestTask;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ret = LOS_TaskCreate(&taskID, &taskInitParam);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    return ret;
}
