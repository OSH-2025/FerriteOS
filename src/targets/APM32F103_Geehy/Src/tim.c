/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Timer Init Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-23
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
#include "apm32f10x_tmr.h"
#include "apm32f10x_misc.h"
#include "apm32f10x_rcm.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define TIMER3_RELOAD 50000

VOID ApmMiniTim3Init(VOID)
{
    TMR_BaseConfig_T tmrBaseConfigStruct;
    
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR3);
    
     /** Set clockDivision = 1 */
    tmrBaseConfigStruct.clockDivision = TMR_CLOCK_DIV_1;
     /** Up-counter */
    tmrBaseConfigStruct.countMode = TMR_COUNTER_MODE_UP;
     /** Set divider = 7199.So TMR3 clock freq ~= 72000/(7199 + 1) = 10kHZ */
    tmrBaseConfigStruct.division = 7199;
     /** Set counter = 49999 + 1 */
    tmrBaseConfigStruct.period = 49999;
     /** Repetition counter = 0x0 */
    tmrBaseConfigStruct.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR3, &tmrBaseConfigStruct);
    
    TMR_Enable(TMR3);
}

VOID Tim3IrqHandler(VOID)
{
    if (TMR_ReadIntFlag(TMR3, TMR_INT_UPDATE) == SET) {
        TMR_ClearIntFlag(TMR3, TMR_INT_UPDATE);
    }
}

VOID TimInit(VOID)
{
    ApmMiniTim3Init();
}

VOID TimerHwiCreate(VOID)
{
    UINT32 ret;

    ret = LOS_HwiCreate(TIM_IRQ, 0, 0, Tim3IrqHandler, 0); // 16: cortex-m irq num shift
    if (ret != 0) {
        printf("ret of TIM3 LOS_HwiCreate = %#x\n", ret);
        return;
    }
    TMR_EnableInterrupt(TMR3, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR3_IRQn, 0, 0);
}

UINT64 GetTimerCycles(VOID)
{
    STATIC UINT64 bacCycle = 0;
    STATIC UINT64 cycleTimes = 0;
    UINT64 cycles;
    UINT64 swCycles = TMR3->CNT;

    if (swCycles < bacCycle) {
        cycleTimes++;
    }
    bacCycle = swCycles;
    if (cycleTimes > 0xffff) {
        return 0;
    }
    cycles = swCycles + cycleTimes * TIMER3_RELOAD;

    return cycles;
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
