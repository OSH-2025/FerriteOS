/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hardware Tick Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2018-05-12
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

#include <xtensa/core-macros.h>
#include "los_tick_pri.h"
#include "los_config.h"
#include "los_hwi.h"
#include "asm/platform.h"
#include "asm/interrupt_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 g_ticksPerSec;
UINT32 g_cyclePerSec;
UINT32 g_cyclesPerTick;

#define OS_HWTMR_GET_NEXT_TICK_CYCLES(uwCyclePerTick) (xthal_get_ccount() + (uwCyclePerTick))

STATIC INLINE VOID HalSetCCompareValue(UINT32 hwTmrNum, UINT32 newCompareVal)
{
    if (hwTmrNum == OS_HWI_TIMER0) {
        xthal_set_ccompare(0, newCompareVal);
    } else if (hwTmrNum == OS_HWI_TIMER1) {
        xthal_set_ccompare(1, newCompareVal);
    }
}

STATIC INLINE UINT64 HalClockGetCurrCyclesBase(UINT64 swTick)
{
    UINT64 cycle;
    UINT32 ccount;
    UINT32 ccompare;
    UINT32 hwCycle;
    UINTPTR intSave;
    UINT32 cyclesPerTick;

    intSave = LOS_IntLock();

    ccount = xthal_get_ccount();
    ccompare = xthal_get_ccompare(0);
    cyclesPerTick = GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    if (ccompare < ccount) {
        if (ccompare >= cyclesPerTick) {
            ccompare += cyclesPerTick;
            swTick++;
            hwCycle = ccompare - ccount;
        } else {
            hwCycle = (0xFFFFFFFF - ccount) + ccompare;
        }
    } else {
        hwCycle = ccompare - ccount;
    }
    cycle = (((swTick) * cyclesPerTick) + (UINT64)(cyclesPerTick - hwCycle));

    LOS_IntRestore(intSave);

    return cycle;
}

VOID HalTickEntry(VOID)
{
    UINT32 nextTickCycles = OS_HWTMR_GET_NEXT_TICK_CYCLES(g_cyclesPerTick);

    (VOID)LOS_HwiClear(OS_TICK_INT_NUM);
    HalSetCCompareValue(OS_TICK_INT_NUM, nextTickCycles);
    OsTickHandler();
}

VOID HalClockInit(VOID)
{
    SET_SYS_CLOCK(OS_SYS_CLOCK);
    g_ticksPerSec = LOSCFG_BASE_CORE_TICK_PER_SECOND;
    g_cyclePerSec = GET_SYS_CLOCK();
    g_cyclesPerTick = (GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND);

    return;
}

VOID HalClockStart(VOID)
{
    UINT32 ret;
    UINTPTR intSave;
    UINT32 nextTickCycles;

    intSave = LOS_IntLock();
    nextTickCycles = OS_HWTMR_GET_NEXT_TICK_CYCLES(g_cyclesPerTick);
    HalSetCCompareValue(OS_TICK_INT_NUM, nextTickCycles);
    ret = LOS_HwiCreate(OS_TICK_INT_NUM, 0, 0, HalTickEntry, (UINT32)NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("error : Create Timer Interrupt Failed.\n");
        LOS_IntRestore(intSave);
        return;
    }
    ret = LOS_HwiEnable(OS_TICK_INT_NUM);
    if (ret != LOS_OK) {
        PRINT_ERR("error : Enable Timer Interrupt Failed.\n");
    }
    LOS_IntRestore(intSave);
    return;
}


VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + (UINT64)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

LITE_OS_SEC_TEXT_INIT UINT64 HalClockGetCycles(VOID)
{
    return HalClockGetCurrCyclesBase(g_tickCount[ArchCurrCpuid()]);
}

LITE_OS_SEC_TEXT_MINOR UINT64 HalClockGetCurrCycles(VOID)
{
    return HalClockGetCurrCyclesBase(0);
}

#ifdef LOSCFG_KERNEL_TICKLESS
LITE_OS_SEC_TEXT_MINOR VOID HalClockTickTimerReload(UINT32 cycles)
{
    UINT32 nextTickCycles = OS_HWTMR_GET_NEXT_TICK_CYCLES(cycles);

    (VOID)LOS_HwiClear(OS_TICK_INT_NUM);
    HalSetCCompareValue(OS_TICK_INT_NUM, nextTickCycles);
}

VOID HalTickReload(VOID)
{
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

