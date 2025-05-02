/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Hardware Tick Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-10
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
#include "asm/interrupt_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_CYCLE_PER_TICK  (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)

LITE_OS_SEC_BSS UINT32 g_cyclesPerTick;

UINT32 GetCcount(VOID)
{
    UINT32 intSave;
    __asm__ __volatile__("rsr %0, ccount" : "=a"(intSave) :);
    return intSave;
}

UINT32 ResetCcount(VOID)
{
    __asm__ __volatile__("wsr %0, ccount; rsync" : :"a"(0));
    return LOS_OK;
}

UINT32 GetCcompare(VOID)
{
    UINT32 intSave;
    __asm__ __volatile__("rsr %0, ccompare0" : "=a"(intSave) :);
    return intSave;
}

UINT32 SetCcompare(UINT32 newCompareVal)
{
    __asm__ __volatile__("wsr %0, ccompare0; rsync" : : "a"(newCompareVal));
    return LOS_OK;
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

    ccount = GetCcount();
    ccompare = GetCcompare();
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

VOID HalUpdateTimerCmpVal(VOID)
{
    SetCcompare(OS_CYCLE_PER_TICK);
    ResetCcount();
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
    ret = LOS_HwiCreate(OS_TICK_INT_NUM, 0, 0, (HWI_PROC_FUNC)HalTickEntry, NULL);
    if (ret != LOS_OK) {
    	PRINT_ERR("%s %d ret:0x%x\n", __FUNCTION__, __LINE__, ret);
    }
}

VOID HalClockStart(VOID)
{
    if ((OS_SYS_CLOCK == 0) || (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    g_cyclesPerTick = OS_CYCLE_PER_TICK;
    SetCcompare(g_cyclesPerTick);
    ResetCcount();
#ifdef LOSCFG_COMPILER_XTENSA_LX6
    __asm__ __volatile__("wsr %0, ccompare1; rsync" : : "a"(0));
    __asm__ __volatile__("wsr %0, ccompare2; rsync" : : "a"(0));
#endif
    UINT32 ret = LOS_HwiEnable(OS_TICK_INT_NUM);
    if (ret != 0) {
        PRINTK("LOS_HwiEnable failed. ret = %#x\n", ret);
    }
    (VOID)ArchIrqUnmask(OS_TICK_INT_NUM);
}

UINT64 HalClockGetCycles(VOID)
{
    return HalClockGetCurrCyclesBase(g_tickCount[ArchCurrCpuid()]);
}

UINT64 HalClockGetSysTickCycles(VOID)
{
	return 0;
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
