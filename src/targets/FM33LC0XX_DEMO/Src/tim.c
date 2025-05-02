/* *
 * *****************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 * of the TIM instances.
 * *****************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 * opensource.org/licenses/BSD-3-Clause
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------ */
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "los_hwi.h"

#define TIMER3_RELOAD 50000
/* USER CODE END 0 */


/* TIM3 init function */
void MX_TIM3_Init(void)
{
    LL_GPTIM_InitTypeDef        InitStructer;

    InitStructer.Prescaler               = 7200 - 1;                        // 分频系数7200
    InitStructer.CounterMode           = LL_GPTIM_COUNTER_DIRECT_UP;        // 向上计数
    InitStructer.Autoreload            = 50000 - 1;                         // 自动重装载值50000
    InitStructer.ClockDivision           = LL_GPTIM_CLOCKDIVISION_DIV1;     // 死区和滤波设置
    InitStructer.AutoreloadState       = DISABLE;                           // 自动重装载禁止preload
    LL_GPTIM_Init(GPTIM0, &InitStructer);

    NVIC_DisableIRQ(GPTIM0_IRQn);
    NVIC_SetPriority(GPTIM0_IRQn, 0);        // 中断优先级配置
    NVIC_EnableIRQ(GPTIM0_IRQn);

    LL_GPTIM_ClearFlag_UPDATE(GPTIM0);      // 清除计数器中断标志位

    LL_GPTIM_EnableIT_UPDATE(GPTIM0);       // 开启计数器中断

//    LL_GPTIM_EnableCounter(GPTIM0);       // 使能定时器

}

void GPTIM0_IRQHandler(void)
{
    if (( LL_GPTIM_IsEnabledIT_UPDATE(GPTIM0) == SET ) &&(LL_GPTIM_IsActiveFlag_UPDATE(GPTIM0)==SET))
    {
        LL_GPTIM_ClearFlag_UPDATE(GPTIM0);
    }
}


/* *
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    GPTIM0_IRQHandler();
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

VOID TimInit(VOID)
{
    MX_TIM3_Init();
}

VOID TimerHwiCreate(VOID)
{
    UINT32 ret;

    ret = LOS_HwiCreate(TIM_IRQ, 0, 0, TIM3_IRQHandler, 0); // 16: cortex-m irq num shift
    if (ret != 0) {
        printf("ret of TIM3 LOS_HwiCreate = %#x\n", ret);
        return;
    }
    LL_GPTIM_EnableCounter(GPTIM0);
}

UINT64 GetTimerCycles(VOID)
{
    static UINT64 bacCycle;
    static UINT64 cycleTimes;
    UINT64 swCycles = LL_GPTIM_GetCounter(GPTIM0);

    if (swCycles < bacCycle) {
        cycleTimes++;
    }
    bacCycle = swCycles;
    return swCycles + cycleTimes * TIMER3_RELOAD;
}

TimControllerOps g_cpupTimerOps = {
    .timInit = TimInit,
    .timHwiCreate = TimerHwiCreate,
    .timGetTimerCycles = GetTimerCycles
};
/* USER CODE END 1 */

/* *********************** (C) COPYRIGHT STMicroelectronics *****END OF FILE*** */
