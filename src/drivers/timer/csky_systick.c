/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: C-Sky timer Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-28
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
#include "hal_hwi.h"
#include "los_hwi.h"
#include "los_tick_pri.h"
#include "los_hwi_pri.h"
#include "vic.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct {
    UINT32 CTRL;
    UINT32 LOAD;
    UINT32 VAL;
    UINT32 CALIB;
} CORE_TIM_TYPE;

#define OS_CYCLE_PER_TICK    (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

#define TICK_INTR_CHECK      (0x4000000UL)
#define CORE_TIM_BASE        (0xE000E010UL)
#define SysTick              ((CORE_TIM_TYPE *)CORE_TIM_BASE)

#define CORETIM_ENABLE       (1UL << 0)
#define CORETIM_INTMASK      (1UL << 1)
#define CORETIM_SOURCE       (1UL << 2)
#define CORETIM_MODE         (1UL << 16)

#define TIM_INT_NUM          1 /* SysTick_IRQn + OS_SYS_VECOTRE */

LITE_OS_SEC_BSS UINT32 g_cyclesPerTick;

LITE_OS_SEC_TEXT VOID CskyTickHandler(VOID) {
    /* Need to read SysTick->CTRL to clear the COUNTFLAG of CTRL,
    otherwise it will keep entering systick interrupt */
    if (SysTick->CTRL) {
        OsTickHandler();
    }
}

VOID HalClockInit(VOID)
{
    UINT32 ret = LOS_HwiCreate(TIM_INT_NUM, 0, 0, CskyTickHandler, 0);
    if (ret != 0) {
        PRINTK("ret of LOS_HwiCreate = %#x\n", ret);
    }
}

VOID HalClockStart(VOID)
{
    if ((OS_SYS_CLOCK == 0) || (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    g_cyclesPerTick = OS_CYCLE_PER_TICK;

    SysTick->LOAD = (OS_CYCLE_PER_TICK - 1);
    SysTick->VAL = 0;
    SysTick->CTRL |= (CORETIM_SOURCE | CORETIM_ENABLE | CORETIM_INTMASK);

    VIC_REG->IWER[0] = 0x1 << TIM_INT_NUM;
    UINT32 ret = LOS_HwiEnable(TIM_INT_NUM);
    if (ret != 0) {
        PRINTK("LOS_HwiEnable failed. ret = %#x\n", ret);
    }
}


UINT64 HalClockGetCycles(VOID)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;

    intSave = LOS_IntLock();

    swTick = LOS_TickCountGet();
    hwCycle = SysTick->VAL;
    if (SysTick->CTRL & CORETIM_MODE) {
        swTick++;
    }
    cycle = (swTick * g_cyclesPerTick) + (g_cyclesPerTick - hwCycle);
    LOS_IntRestore(intSave);

    return cycle;
}

UINT64 HalClockGetSysTickCycles(VOID)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 hwCycle;
    UINT32 intSave;
    UINT32 systickLoad;
    UINT32 systickCur;

    intSave = LOS_IntLock();

    swTick = LOS_TickCountGet();

    systickLoad = SysTick->LOAD;
    systickCur = SysTick->VAL;
    hwCycle = systickLoad - systickCur;

    if (SysTick->CTRL & CORETIM_MODE) {
        swTick++;
    }

    cycle = swTick * systickLoad + hwCycle;

    LOS_IntRestore(intSave);

    return cycle;
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + (UINT64)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile("nop");
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

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
