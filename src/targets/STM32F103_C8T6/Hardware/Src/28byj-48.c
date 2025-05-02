/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: 28byj stepper Implementation
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

#include "sys_init.h"
#include "demo_entry.h"
#include "gpio.h"
#include "pca9685.h"

#define CYCLE_ANGLE         360
#define STEPPER_ONE_CYCLE   64
#define APP_TASK_PRIORITY   5
#define CYCLE_STEP          8
#define STEP_ANGLE          5.625

VOID NanoStepperRotating(UINT32 target, UINT32 time, BOOL dir)
{
    UINT8 *work = NULL;

    UINT8 CCW[CYCLE_STEP] = {0x08, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09}; //Counterclockwise beat
    UINT8 CW[CYCLE_STEP] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08}; //Clockwise beat

    if(dir == TRUE){
        work = CCW;
    } else {
        work = CW;
    }

    for(int cycle = 0; cycle < target; cycle++){
        for(int j = 0; j < CYCLE_STEP; j++){
            for(int i = 0; i < CYCLE_STEP; i++){
                switch (work[i]) {
                    case 0x08:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        break;
                    case 0x0c:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        break;
                    case 0x04:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        break;
                    case 0x06:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
                        break;
                    case 0x02:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
                        break;
                    case 0x03:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                        break;
                    case 0x01:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                        break;
                    case 0x09:
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        break;
                }

                LOS_Mdelay(2); // delay 2 ms, minimum response time

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
            }
        }
    }
}

VOID MotorRotation(UINT32 angle, UINT32 time, BOOL dir)
{
    NanoStepperRotating(angle * STEPPER_ONE_CYCLE / CYCLE_ANGLE, time, dir);
}

STATIC UINT32 NanoStepper_Task(VOID)
{
    UINT32 ret = LOS_OK;

    while (1) {
        MotorRotation(90, 400, TRUE); // test, Rotate 90 degrees clockwise in 400 ms
        LOS_TaskDelay(1000); // delay 1s
        MotorRotation(90, 400, FALSE); // test, Rotate 90 degrees counterclockwise in 400 ms
    }
    return ret;
}

INT32 NanoStepperTaskCreate(VOID)
{
    UINT32 ret = LOS_OK;
    UINT32 taskID = 0;
    TSK_INIT_PARAM_S taskInitParam;

    memset(&taskInitParam, 0, sizeof(TSK_INIT_PARAM_S));
    taskInitParam.usTaskPrio = APP_TASK_PRIORITY;
    taskInitParam.pcName = "NanoStepper_Task";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)NanoStepper_Task;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    ret = LOS_TaskCreate(&taskID, &taskInitParam);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    return ret;
}
