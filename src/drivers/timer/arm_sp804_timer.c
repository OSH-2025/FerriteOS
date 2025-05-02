/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Timer Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "time_pri.h"
#include "los_tick_pri.h"
#include "los_hwi.h"
#include "hisoc/sys_ctrl.h"
#include "los_swtmr.h"
#ifdef LOSCFG_ARCH_RISCV
#include "arch/regs.h"
#include "riscv_interrupt.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_SMP
#error "hisoc timer does not suppot on SMP mode!"
#endif

#define TIMER_ENABLE_BIT           7
#define TIMER_COUNTING_MODE_BIT    6
#define TIMER_INTERRUPT_ENABLE_BIT 5
#define TIMER_SIZE_SELECT_BIT      1

#define READ_TIMER (*(volatile UINT32 *)(TIMER_TICK_REG_BASE + TIMER_VALUE))

STATIC UINT32 g_timeStamp __attribute__((section(".data.init")));
STATIC UINT32 g_lastDec __attribute__((section(".data.init")));

#ifdef LOSCFG_ARM_SP804_ADVANCED
#define TIMER_FREQUENCY_DIV_BIT    3
#define MS_PER_S   1000

STATIC volatile UINT64 g_schedClockNanosec = 0;
STATIC volatile UINT64 g_schedClockCycle = 0;
STATIC volatile UINT32 g_timeClkLast = 0;

STATIC UINT16 g_swtmrId;
#endif /* LOSCFG_ARM_SP804_ADVANCED */

VOID HalClockTickTimerReload(UINT32 cycles)
{
    UINT32 cyclesPerTick;
    cyclesPerTick = GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    WRITE_UINT32(cycles, TIMER_TICK_REG_BASE + TIMER_LOAD);
    WRITE_UINT32(cyclesPerTick, TIMER_TICK_REG_BASE + TIMER_BGLOAD);

    /* int clear */
    WRITE_UINT32(1, TIMER_TICK_REG_BASE + TIMER_INT_CLR);
    dsb(); /* Make sure pending bit of int source clear */
    (void)ArchIrqClear(NUM_HAL_INTERRUPT_TIMER);
}

VOID ResetTimerMasked(VOID)
{
    g_lastDec = READ_TIMER;
    g_timeStamp = 0;
}

UINT32 GetTimerMasked(VOID)
{
    UINT32 now = READ_TIMER;

    if (g_lastDec >= now) {
        /* not roll back */
        g_timeStamp += g_lastDec - now;
    } else {
        /* rollback */
        g_timeStamp += g_lastDec + (GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND) - now;
    }

    g_lastDec = now;
    return g_timeStamp;
}

UINT32 ArchTimerRollback(VOID)
{
    UINT32 flag;

    READ_UINT32(flag, TIMER_TICK_REG_BASE + TIMER_RIS);
    return flag;
}

UINT32 HalClockGetTickTimerCycles(VOID)
{
    UINT32 temp;

    READ_UINT32(temp, TIMER_TICK_REG_BASE + TIMER_VALUE);
    return temp;
}

#ifdef LOSCFG_ARM_SP804_ADVANCED

UINT32 TimeClockRead(VOID)
{
    UINT32 value;

    READ_UINT32(value, TIMER_TIME_REG_BASE + TIMER_VALUE);
    value = TIMER_MAXLOAD - value;

    return value;
}

/* this func is start timer2 for start time */
VOID HalClockInitStart(VOID)
{
    UINT32 temp;

#ifndef NO_SC_CTRL
    /*
     * enable timer2 here,
     * but only time0 is used for system clock.
     */
    READ_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
    temp |= TIMER2_ENABLE;
    WRITE_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
#endif

    /* init the timestamp and lastdec value */
    ResetTimerMasked();

    /* disable timer2 */
    WRITE_UINT32(0x0, TIMER2_REG_BASE + TIMER_CONTROL);
    /* set init value as period */
    WRITE_UINT32(TIMER_MAXLOAD, TIMER2_REG_BASE + TIMER_LOAD);

    /*
     * Timing mode: 32bits [bit 1 set as 1]
     * ticking with 1/256 clock frequency [bit 3 set as 1, bit 2 set as 0]
     * timer mode periodic [bit 6 set as 1]
     * timer enabled [bit 7 set as 1]
     */
    temp = (1U << TIMER_ENABLE_BIT) | (1U << TIMER_COUNTING_MODE_BIT) |
           (1U << TIMER_FREQUENCY_DIV_BIT) | (1U << TIMER_SIZE_SELECT_BIT);
    WRITE_UINT32(temp, TIMER2_REG_BASE + TIMER_CONTROL);
}

UINT32 GetTimer2Value(VOID)
{
    UINT32 temp;

    READ_UINT32(temp, TIMER2_REG_BASE + TIMER_VALUE);
    return temp;
}

/* get the system ms clock since the system start */
UINT32 HiGetMsClock(VOID)
{
    const UINT32 t32 = TIMER_MAXLOAD - GetTimer2Value();
    UINT64 t64 = (UINT64)t32 << 0x8; /* Timer2 is divided by 256, left shift 8 to recover sys clock */
    UINT64 temp = OS_TIME_TIMER_CLOCK / MS_PER_S;

    return (UINT32)(t64 / temp);
}

STATIC UINT32 UpdateTimeClk(UINT32 *timeClk)
{
    UINT32 timeClkNow;

    timeClkNow = TimeClockRead();
    if (timeClkNow >= g_timeClkLast) {
        *timeClk = timeClkNow - g_timeClkLast;
    } else {
        *timeClk = timeClkNow + (TIMER_MAXLOAD - g_timeClkLast);
    }
    return timeClkNow;
}

STATIC UINT64 GetSchedClock(VOID)
{
    UINT32 timeClk;

    (VOID)UpdateTimeClk(&timeClk);

    return (g_schedClockNanosec + (UINT64)CYCLE_TO_NS(timeClk));
}

STATIC UINT64 SchedClock(VOID)
{
    UINT32 intSave;
    UINT64 timeClk64;

    intSave = LOS_IntLock();
    timeClk64 = GetSchedClock();
    LOS_IntRestore(intSave);

    return timeClk64;
}

STATIC VOID AdvacneTimerInit(VOID)
{
    UINT32 temp;
#ifndef NO_SC_CTRL
    /*
     * enable time here,
     */
    READ_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
    temp |= TIMER_TIME_ENABLE;
    WRITE_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
#endif

    /* init the timestamp and lastdec value */
    ResetTimerMasked();

    /* disable timer1 */
    WRITE_UINT32(0x0, TIMER_TIME_REG_BASE + TIMER_CONTROL);
    /* set init value as period */
    WRITE_UINT32(0xffffffff, TIMER_TIME_REG_BASE + TIMER_LOAD);

    /*
     * Timing mode: 32bits [bit 1 set as 1]
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * timer mode periodic [bit 6 set as 1]
     * timer enabled [bit 7 set as 1]
     */
    temp = (1U << TIMER_ENABLE_BIT) | (1U << TIMER_COUNTING_MODE_BIT) | (1U << TIMER_SIZE_SELECT_BIT);
    WRITE_UINT32(temp, TIMER_TIME_REG_BASE + TIMER_CONTROL);
}

UINT64 hi_sched_clock(VOID)
{
    return SchedClock();
}

VOID UpdateSchedClock(VOID)
{
    UINT32 intSave;

    intSave = LOS_IntLock();
    UINT32 timeClk;
    g_timeClkLast = UpdateTimeClk(&timeClk);
    g_schedClockCycle += (UINT64)timeClk;
    g_schedClockNanosec += (UINT64)CYCLE_TO_NS(timeClk);
    OsAdjTime();
    LOS_IntRestore(intSave);
}

VOID SchedClockSwtmr(VOID)
{
    UINT32 ret;
    ret = LOS_SwtmrCreate(SCHED_CLOCK_INTETRVAL_TICKS, LOS_SWTMR_MODE_PERIOD,
                          (SWTMR_PROC_FUNC)UpdateSchedClock, &g_swtmrId, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("LOS_SwtmrCreate error %u\n", ret);
        return;
    }
    ret = LOS_SwtmrStart(g_swtmrId);
    if (ret != LOS_OK) {
        PRINT_ERR("LOS_SwtmrStart error %u\n", ret);
        return;
    }
}

UINT64 HalClockGetCycles(VOID)
{
    UINT32 timeClk;
    UINT32 intSave;
    UINT64 cycle;

    intSave = LOS_IntLock();
    (VOID)UpdateTimeClk(&timeClk);
    cycle = g_schedClockCycle + (UINT64)timeClk;
    LOS_IntRestore(intSave);

    return cycle;
}

#else /* !LOSCFG_ARM_SP804_ADVANCED */

STATIC VOID AdvacneTimerInit(VOID)
{
}

VOID SchedClockSwtmr(VOID)
{
}

UINT64 HalClockGetCycles(VOID)
{
    UINT64 swTick;
    UINT64 cycle;
    UINT32 intSta;
    UINT32 hwCycle;
    UINT32 intSave;
    UINT32 period = GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    intSave = LOS_IntLock();
    swTick = LOS_TickCountGet();

    READ_UINT32(hwCycle, TIMER_TICK_REG_BASE + TIMER_VALUE);

    READ_UINT32(intSta, TIMER_TICK_REG_BASE + TIMER_RIS);
    if (((intSta & 0x0000001) != 0)) {                      /* check whether the systick interruption occurs */
        swTick++;
        READ_UINT32(hwCycle, TIMER_TICK_REG_BASE + TIMER_VALUE);
    }
    cycle = (swTick * period) + (period - hwCycle);
    LOS_IntRestore(intSave);

    return cycle;
}

#endif /* LOSCFG_ARM_SP804_ADVANCED */

LITE_OS_SEC_TEXT VOID HalTickEntry(VOID)
{
    /* int clear */
    WRITE_UINT32(1, TIMER_TICK_REG_BASE + TIMER_INT_CLR);
#ifdef LOSCFG_ARCH_RISCV
    dsb(); /* Make sure pending bit of int source clear for riscv SoC */
#endif
#ifdef LOSCFG_PLATFORM_BSP_RISCV_PLIC
    (void)ArchIrqClear(NUM_HAL_INTERRUPT_TIMER);
#endif

    OsTickHandler();
}

VOID HalClockInit(VOID)
{
    AdvacneTimerInit();
    (void)LOS_HwiCreate(NUM_HAL_INTERRUPT_TIMER, OS_HWI_PRIO_LOWEST, 0, HalTickEntry, 0);
}

VOID HalClockEnable(VOID)
{
    UINT32 val;
    val = GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* timer enabled [bit 7 set as 1] */
    val |= 1U << TIMER_ENABLE_BIT;
    WRITE_UINT32(val, TIMER_TICK_REG_BASE + TIMER_CONTROL);
}

VOID HalClockDisable(VOID)
{
    UINT32 val;
    val = GET_UINT32(TIMER_TICK_REG_BASE + TIMER_CONTROL);
    /* timer disabled [bit 7 set as 0] */
    val &= ~(1U << TIMER_ENABLE_BIT);
    WRITE_UINT32(val, TIMER_TICK_REG_BASE + TIMER_CONTROL);
}

/* Delay for some number of micro-seconds */
UINT32 GetTimer(UINT32 base)
{
    return GetTimerMasked() - base;
}

VOID HalClockFreqWrite(UINT32 freq)
{
    (VOID)freq;
    PRINT_WARN("hisoc timer does not support setting frequency\n");
}

LITE_OS_SEC_TEXT_INIT VOID HalClockStart(VOID)
{
    UINT32 temp;
    UINT32 period = GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND;

#ifndef NO_SC_CTRL
    /*
     * enable time0 here,
     * but only time0 is used for system clock.
     */
    READ_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
    temp |= TIMER_TICK_ENABLE;
    WRITE_UINT32(temp, SYS_CTRL_REG_BASE + REG_SC_CTRL);
#endif

    /* disable first */
    WRITE_UINT32(0x0, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* set init value as period */
    WRITE_UINT32(period, TIMER_TICK_REG_BASE + TIMER_LOAD);

    /*
     * Timing mode: 32bits [bit 1 set as 1]
     * ticking with 1/1 clock frequency [bit 3 set as 0, bit 2 set as 0]
     * interrupt enabled [bit 5 set as 1]
     * timer mode periodic [bit 6 set as 1]
     */
    temp = (1U << TIMER_COUNTING_MODE_BIT) | (1U << TIMER_INTERRUPT_ENABLE_BIT) | (1U << TIMER_SIZE_SELECT_BIT);
    WRITE_UINT32(temp, TIMER_TICK_REG_BASE + TIMER_CONTROL);

    /* clock start */
    (void)ArchIrqUnmask(NUM_HAL_INTERRUPT_TIMER);
    HalClockEnable();

    /* start adjusting swtmer */
    SchedClockSwtmr();

    g_cycle2NsScale = ((double)OS_SYS_NS_PER_SECOND / ((long)OS_TIME_TIMER_CLOCK));
}

VOID HalDelayUs(UINT32 usecs)
{
    UINT64 tmo = LOS_CurrNanosec() + (UINT64)usecs * OS_SYS_NS_PER_US;

    while (LOS_CurrNanosec() < tmo) {
        __asm__ volatile ("nop");
    }
}

UINT32 HalGetRemainingCycles(UINT32 *ticks)
{
    UINT32 cycles;
    HalClockDisable();
    cycles = HalClockGetTickTimerCycles();
    /* If the tick interrupt has arrived, update tick count and sleep ticks */
    if (ArchTimerRollback() != 0) {
        if (ticks != NULL) {
            (*ticks)--;
        }
        cycles = HalClockGetTickTimerCycles();
    }
    return cycles;
}

VOID HalTickCheckAndAdjust(UINT32 ticks)
{
    UINT32 period = GET_SYS_CLOCK() / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    UINT32 remainingCycles = HalGetRemainingCycles(&ticks);
    UINT64 sleepCycles = (UINT64)(ticks - 1) * period;
    HalClockTickTimerReload((UINT32)(sleepCycles + remainingCycles));
    HalClockEnable();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
