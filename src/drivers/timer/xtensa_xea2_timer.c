/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Hardware Tick Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-12
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
#include "asm/interrupt_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define CPU_CYCLE_LOW_BITS       32
#define CPU_CYCLE_LOW_BITS_MASK  0xFFFFFFFFU
LITE_OS_SEC_BSS UINT32 g_cyclesPerTick;

static inline VOID OS_HWTMR_ADJUST_CCOMPARE(UINT32 hwTmrNum, UINT32 newCompareVal)
{
    if (hwTmrNum == OS_HWI_TIMER0) {
        __asm__ __volatile__("wsr %0, ccompare0; rsync":: "a"(newCompareVal));
    } else if (hwTmrNum == OS_HWI_TIMER1) {
        __asm__ __volatile__("wsr %0, ccompare1; rsync":: "a"(newCompareVal));
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

LITE_OS_SEC_TEXT_INIT UINT64 HalClockGetCycles(VOID)
{
    return HalClockGetCurrCyclesBase(g_tickCount[ArchCurrCpuid()]);
}

VOID HalUpdateTimerCmpVal(VOID)
{
    UINT32 oldComp;
    UINT32 diff;
    UINT32 val;
    UINT32 count = 0;

    do {
        __asm__ __volatile__("rsr %0, ccompare0" : "=a"(oldComp) :);
        __asm__ __volatile__("wsr %0, ccompare0; rsync" : : "a"(oldComp + g_cyclesPerTick));
        __asm__ __volatile__("rsr %0, ccount" : "=a"(val) :);
        /* must handle overflow for 32-bits counter */
        diff = (val >= oldComp) ? (val - oldComp) : ((0xFFFFFFFF - oldComp) + val);
    } while ((diff > g_cyclesPerTick) && (count++ < 30)); /* 30: max loop times, avoid dead loop */
}

VOID HalTickEntry(VOID)
{
    OsTickHandler();
    HalUpdateTimerCmpVal();
}

VOID HalClockInit(VOID)
{
    UINT32 ret;
    SET_SYS_CLOCK(OS_SYS_CLOCK);
    g_cyclesPerTick = (GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND);
    ret = LOS_HwiCreate(OS_TICK_INT_NUM, 0, 0, (HWI_PROC_FUNC)HalTickEntry, NULL);
    if (ret != LOS_OK) {
    	PRINT_ERR("%s %d ret:0x%x\n", __FUNCTION__, __LINE__, ret);
    }
}

VOID HalClockStart(VOID)
{
    UINT32 ccount;

    __asm__ __volatile__("rsr %0, ccount" : "=a"(ccount) :);
    OS_HWTMR_ADJUST_CCOMPARE(OS_TICK_INT_NUM, (ccount + g_cyclesPerTick));

    (VOID)ArchIrqUnmask(OS_TICK_INT_NUM);
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 cycles = (UINT64)usecs * GET_SYS_CLOCK() / OS_SYS_US_PER_SECOND;
    UINT64 deadline = HalClockGetCycles() + cycles;

    while (HalClockGetCycles() < deadline) {
        __asm__ __volatile__("nop");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
