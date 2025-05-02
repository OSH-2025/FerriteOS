/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Timer Implementation
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
#include "arch/regs.h"

#ifdef LOSCFG_PLATFORM_GD32VF103V_EVAL
#include "eclic.h"
#include "n200_timer.h"
#include "n200_func.h"
#endif

#ifdef LOSCFG_PLATFORM_HIFIVE1_REV1_B01
#include "plic.h"
#include "metal/machine/platform.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

LITE_OS_SEC_BSS UINT32     g_cyclesPerTick;

#ifdef LOSCFG_PLATFORM_HIFIVE1_REV1_B01
#define MTIMER_INT_NUM      (LOSCFG_PLATFORM_HWI_LIMIT + 1)
#define OS_CYCLE_PER_TICK   (LFROSC_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define MTIMER_CTRL_BASE    METAL_RISCV_CLINT0_2000000_BASE_ADDRESS
#define MTIMERCMP_OFFSET    METAL_RISCV_CLINT0_MTIMECMP_BASE
#define MTIMER_OFFSET       METAL_RISCV_CLINT0_MTIME
#elif defined(LOSCFG_PLATFORM_GD32VF103V_EVAL)
#define QUAD_DIVIDING       4
#define MTIMER_INT_NUM      CLIC_INT_TMR
#define OS_CYCLE_PER_TICK   (OS_SYS_CLOCK / LOSCFG_BASE_CORE_TICK_PER_SECOND / QUAD_DIVIDING)
#define MTIMER_CTRL_BASE    TIMER_CTRL_ADDR
#define MTIMERCMP_OFFSET    TIMER_MTIMECMP
#define MTIMER_OFFSET       TIMER_MTIME
#endif

UINT64 HalClockGetSysTickCycles(VOID)
{
    return HalClockGetCycles();
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

VOID HalClockInit(VOID)
{
    UINT32 ret = LOS_HwiCreate(MTIMER_INT_NUM, 0, 0, OsTickHandler, 0);
    if (ret != 0) {
        PRINTK("ret of LOS_HwiCreate = %#x\n", ret);
    }
}

UINT64 HalClockGetCycles(VOID)
{
    UINT64 cycle;
    UINT32 hiCycle;
    UINT32 hiCycleTemp;
    UINT32 lowCycle;
    UINT32 intSave;

    intSave = LOS_IntLock();
    do {
        hiCycle = READ_CSR(mcycleh);
        lowCycle = READ_CSR(mcycle);
        hiCycleTemp = READ_CSR(mcycleh);
    } while (hiCycle != hiCycleTemp);

    /* hiCycle => [32:63] lowCycle => [0:31] */
    cycle = (((UINT64)hiCycle) << 32) | lowCycle;
    g_tickCount[ArchCurrCpuid()] = cycle * LOSCFG_BASE_CORE_TICK_PER_SECOND / OS_SYS_CLOCK;
    LOS_IntRestore(intSave);

    return cycle;
}

VOID HalClockStart(VOID)
{
    if ((OS_SYS_CLOCK == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND == 0) ||
        (LOSCFG_BASE_CORE_TICK_PER_SECOND > OS_SYS_CLOCK)) {
        return;
    }

    g_cyclesPerTick = OS_CYCLE_PER_TICK;

    *(UINT64 *)(MTIMER_CTRL_BASE + MTIMERCMP_OFFSET) = OS_CYCLE_PER_TICK;
    UINT32 ret = LOS_HwiEnable(MTIMER_INT_NUM);
    if (ret != 0) {
        PRINTK("LOS_HwiEnable failed. ret = %#x\n", ret);
    }
    *(UINT64 *)(MTIMER_CTRL_BASE + MTIMER_OFFSET) = 0;
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + (UINT64)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
