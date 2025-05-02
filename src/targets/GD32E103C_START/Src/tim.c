/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Timer Driver Initialization Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-24
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

#include "tim.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define TIMER3_RELOAD    50000
#define TIMER3_PRESCALER 12000

VOID Timer3Init(UINT32 prescaler, UINT32 period)
{
    timer_parameter_struct timerInitStruct;

    rcu_periph_clock_enable(RCU_TIMER3);

    timer_deinit(TIMER3);
    timerInitStruct.prescaler = prescaler;
    timerInitStruct.period = period;
    timerInitStruct.alignedmode = TIMER_COUNTER_EDGE;
    timerInitStruct.counterdirection = TIMER_COUNTER_UP;
    timerInitStruct.clockdivision = TIMER_CKDIV_DIV1;
    timerInitStruct.repetitioncounter = 0;
    timer_init(TIMER3, &timerInitStruct);
    timer_enable(TIMER3);
}

VOID Tim3IrqHandler(VOID)
{
    if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    }
}

VOID TimInit(VOID)
{
    Timer3Init(TIMER3_PRESCALER - 1, TIMER3_RELOAD - 1);
}

VOID TimerHwiCreate(VOID)
{
    UINT32 ret;

    ret = LOS_HwiCreate(TIM_IRQ, 0, 0, Tim3IrqHandler, 0);
    if (ret != 0) {
        printf("ret of TIM3 LOS_HwiCreate = %#x\n", ret);
        return;
    }
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    timer_enable(TIMER3);
}

UINT64 GetTimerCycles(VOID)
{
    static UINT64 bacCycle;
    static UINT64 cycleTimes;
    UINT64 swCycles = (UINT64)timer_counter_read(TIMER3);
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

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */