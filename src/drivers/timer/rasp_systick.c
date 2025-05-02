/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: timer
 * Author: Huawei LiteOS Team
 * Create: 2021-07-01
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

#include "asm/platform.h"
#include "los_hwi.h"
#include "los_tick_pri.h"
#include "los_hwi_pri.h"

#define OS_CYCLE_PER_TICK (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

LITE_OS_SEC_BSS UINT32     g_cyclesPerTick;

VOID HalClockInit(VOID)
{
    UINT32 ret = LOS_HwiCreate(ARM_TIMER_INI, 0, 0, OsTickHandler, 0);
    if (ret != 0) {
        printf("ret of LOS_HwiCreate = %#x\n", ret);
    }
    return;
}

VOID HalClockStart(VOID)
{
    ARMTIMER_INFO * timer = ARMTIMER_REG_BASE;
    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    g_cyclesPerTick = LOSCFG_BASE_CORE_TICK_PER_SECOND;

    /* systime=250000000 */
    timer->preDivider = (OS_SYS_CLOCK / OS_SYS_US_PER_SECOND - 1);

    timer->reload   = 0;
    timer->load     = 0;
    timer->IRQClear = 0;
    timer->control  = 0;
    timer->reload   = LOSCFG_BASE_CORE_TICK_PER_SECOND;
    timer->load     = LOSCFG_BASE_CORE_TICK_PER_SECOND;

    /* 23-bit counter, enable interrupt, enable timer */
    timer->control = (1 << 1) | (1 << 5) | (1 << 7);

    UINT32 ret = LOS_HwiEnable(ARM_TIMER_INI);
    if (ret != 0) {
        PRINTK("LOS_HwiEnable failed. ret = %#x\n", ret);
    }
}

UINT64 HalClockGetCycles(VOID)
{
    UINT64 cycle;
    UINT32 hiCycle;
    UINT32 hiCycleTemp;
    UINT32 lowCycle;
    SYSTEMTIMER_INFO *stimer = SYSTEMTIMER_REG_BASE;

    do {
        hiCycle = stimer->CHI;
        lowCycle = stimer->CLO;
        hiCycleTemp = stimer->CHI;
    } while (hiCycle != hiCycleTemp);

    /* hiCycle => [32:63] lowCycle => [0:31] */
    cycle = ((((UINT64)hiCycle) << 32) | lowCycle) * (OS_SYS_CLOCK / OS_SYS_US_PER_SECOND);
    return cycle;
}

UINT64 HalClockGetSysTickCycles(VOID)
{
    return HalClockGetCycles();
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + (UINT64)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

UINT64 hi_sched_clock(VOID)
{
    return LOS_CurrNanosec();
}

/* just stub api for tickless */
UINT32 HalClockGetTickTimerCycles(VOID)
{
    return 0;
}

/* just stub api for tickless */
VOID HalClockTickTimerReload(UINT32 cycles)
{
    (VOID)cycles;
}

VOID HalTickCheckAndAdjust(UINT32 ticks)
{
    UINT32 remainingCycles = HalGetRemainingCycles(&ticks);
    UINT32 sleepCycles = (ticks - 1) * OS_CYCLE_PER_TICK;
    HalClockTickTimerReload(sleepCycles + remainingCycles);
}