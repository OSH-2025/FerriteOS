/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description: LiteOS Perf Armv8 Pmu Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2017-01-01
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
 *---------------------------------------------------------------------------*/

#include "armv8_pmu_pri.h"
#include "perf_pmu_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC HwPmu g_armv8Pmu;

STATIC INLINE UINT32 Armv8PmcrRead(VOID)
{
    return (UINT32)AARCH64_SYSREG_READ(pmcr_el0);
}

STATIC INLINE VOID Armv8PmcrWrite(UINT32 value)
{
    value &= ARMV8_PMCR_MASK;
    AARCH64_SYSREG_WRITE(pmcr_el0, value);
    ISB();
}

STATIC INLINE UINT32 Armv8PmuOverflowed(UINT32 pmovsr)
{
    return pmovsr & ARMV8_OVERFLOWED_MASK;
}

STATIC INLINE UINT32 Armv8PmuCntOverflowed(UINT32 pmovsr, UINT32 index)
{
    return pmovsr & ARMV8_CNT2BIT(ARMV8_IDX2CNT(index));
}

STATIC INLINE UINT32 Armv8CntValid(UINT32 index)
{
    return index <= ARMV8_IDX_MAX_COUNTER;
}

STATIC INLINE VOID Armv8PmuSelCnt(UINT32 index)
{
    UINT32 counter = ARMV8_IDX2CNT(index);
    AARCH64_SYSREG_WRITE(pmselr_el0, counter);
    ISB();
}

STATIC INLINE VOID Armv8PmuSetCntPeriod(UINT32 index, UINT32 period)
{
    if (!Armv8CntValid(index)) {
        PRINT_ERR("CPU writing wrong counter %u\n", index);
    } else if (index == ARMV8_IDX_CYCLE_COUNTER) {
        const UINT64 value64 = ARMV8_PERIOD_MASK | period;
        AARCH64_SYSREG_WRITE(pmccntr_el0, value64);
        PRINT_DEBUG("%llx\n", AARCH64_SYSREG_READ(pmccntr_el0));
    } else {
        Armv8PmuSelCnt(index);
        AARCH64_SYSREG_WRITE(pmxevcntr_el0, period);
    }
}

STATIC INLINE VOID Armv8BindEvt2Cnt(UINT32 index, UINT32 value)
{
    Armv8PmuSelCnt(index);
    value &= ARMV8_EVTYPE_MASK;
    AARCH64_SYSREG_WRITE(pmxevtyper_el0, value);
}

STATIC INLINE VOID Armv8EnableCnt(UINT32 index)
{
    UINT32 counter = ARMV8_IDX2CNT(index);
    AARCH64_SYSREG_WRITE(pmcntenset_el0, ARMV8_CNT2BIT(counter));
}

STATIC INLINE VOID Armv8DisableCnt(UINT32 index)
{
    UINT32 counter = ARMV8_IDX2CNT(index);
    AARCH64_SYSREG_WRITE(pmcntenclr_el0, ARMV8_CNT2BIT(counter));
}

STATIC INLINE VOID Armv8EnableCntInterrupt(UINT32 index)
{
    UINT32 counter = ARMV8_IDX2CNT(index);
    AARCH64_SYSREG_WRITE(pmintenset_el1, ARMV8_CNT2BIT(counter));
}

STATIC INLINE VOID Armv8DisableCntInterrupt(UINT32 index)
{
    UINT32 counter = ARMV8_IDX2CNT(index);
    AARCH64_SYSREG_WRITE(pmintenclr_el1, ARMV8_CNT2BIT(counter));
    ISB();
    AARCH64_SYSREG_WRITE(pmovsclr_el0, ARMV8_CNT2BIT(counter));
    ISB();
}

STATIC INLINE UINT32 Armv8PmuGetOverflowStatus(VOID)
{
    UINT32 flags;

    asm volatile("mrs %0, pmovsclr_el0" : "=r" (flags));
    flags &= ARMV8_FLAG_MASK;
    asm volatile("msr pmovsclr_el0, %0" :: "r" (flags));

    return flags;
}

STATIC VOID Armv8EnableEvent(Event *event)
{
    UINT32 cnt = event->counter;

    if (!Armv8CntValid(cnt)) {
        PRINT_ERR("CPU enabling wrong PMNC counter IRQ enable %u\n", cnt);
        return;
    }

    if (event->period == 0) {
        PRINT_INFO("event period value not valid, counter: %u\n", cnt);
        return;
    }
    /*
     * Enable counter and interrupt, and set the counter to count
     * the event that we're interested in.
     */
    UINT32 intSave = LOS_IntLock();

    Armv8DisableCnt(cnt);
    Armv8BindEvt2Cnt(cnt, event->eventId);
    Armv8EnableCntInterrupt(cnt);
    Armv8EnableCnt(cnt);

    LOS_IntRestore(intSave);

    PRINT_DEBUG("enabled event: %u cnt: %u\n", event->eventId, cnt);
}

STATIC VOID Armv8DisableEvent(Event *event)
{
    UINT32 cnt = event->counter;

    if (!Armv8CntValid(cnt)) {
        PRINT_ERR("CPU enabling wrong PMNC counter IRQ enable %u\n", cnt);
        return;
    }

    /*
     * Disable counter and interrupt
     */
    UINT32 intSave = LOS_IntLock();
    Armv8DisableCnt(cnt);
    Armv8DisableCntInterrupt(cnt);
    LOS_IntRestore(intSave);
}

STATIC INLINE VOID Armv8StartAllCnt(VOID)
{
    PRINT_DEBUG("starting pmu...\n");

    UINT32 value = Armv8PmcrRead() | ARMV8_PMCR_E;
    if (g_armv8Pmu.cntDivided) {
        value |= ARMV8_PMCR_D;
    } else {
        value &= ~ARMV8_PMCR_D;
    }

    Armv8PmcrWrite(value);
    LOS_HwiEnable(LOS_PMU_IRQ_NR);
}

STATIC INLINE VOID Armv8StopAllCnt(VOID)
{
    PRINT_DEBUG("stopping pmu...\n");
    /* Disable all counters */
    Armv8PmcrWrite(Armv8PmcrRead() & ~ARMV8_PMCR_E);

    LOS_HwiDisable(LOS_PMU_IRQ_NR);
}

STATIC INLINE VOID Armv8ResetAllCnt(VOID)
{
    UINT32 index;
    for (index = ARMV8_IDX_CYCLE_COUNTER; index < ARMV8_IDX_MAX_COUNTER; index++) {
        Armv8DisableCnt(index);
        Armv8DisableCntInterrupt(index);
    }

    /* Initialize & Reset PMNC: C and P bits and D bits */
    UINT32 reg = ARMV8_PMCR_P | ARMV8_PMCR_C | (g_armv8Pmu.cntDivided ? ARMV8_PMCR_D : 0);
    Armv8PmcrWrite(reg);
}

STATIC VOID Armv8SetEventPeriod(Event *event)
{
    if (event->period != 0) {
        PRINT_INFO("counter: %u, period: 0x%x\n", event->counter, event->period);
        Armv8PmuSetCntPeriod(event->counter, PERIOD_CALC(event->period));
    }
}

STATIC INLINE UINTPTR Armv8ReadEventCnt(Event* event)
{
    UINT64 value = 0;
    UINT32 index = event->counter;

    if (!Armv8CntValid(index)) {
        PRINT_ERR("reading wrong counter %u\n", index);
    } else if (index == ARMV8_IDX_CYCLE_COUNTER) {
        value = AARCH64_SYSREG_READ(pmccntr_el0);
        value &= ARMV8_READ_MASK;
    } else {
        Armv8PmuSelCnt(index);
        value = AARCH64_SYSREG_READ(pmxevcntr_el0);
    }

    if (value < PERIOD_CALC(event->period)) {
        if (Armv8PmuCntOverflowed(Armv8PmuGetOverflowStatus(), event->counter)) {
            value += event->period;
        }
    } else {
        value -= PERIOD_CALC(event->period);
    }

    return value;
}

STATIC const UINT32 g_armv8Map[] = {
    [PERF_COUNT_HW_CPU_CYCLES]          = ARMV8_PMUV3_PERF_HW_CLOCK_CYCLES,
    [PERF_COUNT_HW_INSTRUCTIONS]        = ARMV8_PMUV3_PERF_HW_INSTR_EXECUTED,
    [PERF_COUNT_HW_DCACHE_REFERENCES]   = ARMV8_PMUV3_PERF_HW_L1_DCACHE_ACCESS,
    [PERF_COUNT_HW_DCACHE_MISSES]       = ARMV8_PMUV3_PERF_HW_L1_DCACHE_REFILL,
    [PERF_COUNT_HW_ICACHE_REFERENCES]   = ARMV8_PMUV3_PERF_HW_L1_ICACHE_ACCESS,
    [PERF_COUNT_HW_ICACHE_MISSES]       = ARMV8_PMUV3_PERF_HW_L1_ICACHE_REFILL,
    [PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = ARMV8_PMUV3_PERF_HW_PC_WRITE,
    [PERF_COUNT_HW_BRANCH_MISSES]       = ARMV8_PMUV3_PERF_HW_PC_BRANCH_MIS_PRED,
};

UINT32 Armv8PmuMapEvent(UINT32 eventType, BOOL reverse)
{
    if (!reverse) {  /* common event to armv8 real event */
        if (eventType < ARRAY_SIZE(g_armv8Map)) {
            return g_armv8Map[eventType];
        }
        return eventType;
    } else {        /* armv8 real event to common event */
        UINT32 i;
        for (i = 0; i < ARRAY_SIZE(g_armv8Map); i++) {
            if (g_armv8Map[i] == eventType) {
                return i;
            }
        }
        return PERF_HW_INVAILD_EVENT_TYPE;
    }
}

STATIC VOID Armv8PmuIrqHandler(VOID)
{
    UINT32 index;
    PerfRegs regs;

    PerfEvent *events = &(g_armv8Pmu.pmu.events);
    UINT32 eventNum = events->nr;

    /* Get and reset the IRQ flags */
    UINT32 pmovsr = Armv8PmuGetOverflowStatus();
    if (!Armv8PmuOverflowed(pmovsr)) {
        return;
    }

    (VOID)memset_s(&regs, sizeof(PerfRegs), 0, sizeof(PerfRegs));
    OsPerfFetchIrqRegs(&regs);

    Armv8StopAllCnt();

    for (index = 0; index < eventNum; index++) {
        Event *event = &(events->per[index]);
        /*
         * We have a single interrupt for all counters. Check that
         * each counter has overflowed before we process it.
         */
        if (!Armv8PmuCntOverflowed(pmovsr, event->counter) || (event->period == 0)) {
            continue;
        }

        Armv8PmuSetCntPeriod(event->counter, PERIOD_CALC(event->period));

        OsPerfUpdateEventCount(event, event->period);
        OsPerfHandleOverFlow(event, &regs);
    }
    Armv8StartAllCnt();
}

UINT32 OsGetPmuMaxCounter(VOID)
{
    return ARMV8_IDX_MAX_COUNTER;
}

UINT32 OsGetPmuCycleCounter(VOID)
{
    return ARMV8_IDX_CYCLE_COUNTER;
}

UINT32 OsGetPmuCounter0(VOID)
{
    return ARMV8_IDX_COUNTER0;
}

STATIC HwPmu g_armv8Pmu = {
    .canDivided = TRUE,
    .enable     = Armv8EnableEvent,
    .disable    = Armv8DisableEvent,
    .start      = Armv8StartAllCnt,
    .stop       = Armv8StopAllCnt,
    .clear      = Armv8ResetAllCnt,
    .setPeriod  = Armv8SetEventPeriod,
    .readCnt    = Armv8ReadEventCnt,
    .mapEvent   = Armv8PmuMapEvent,
};

UINT32 OsHwPmuInit(VOID)
{
    UINT32 ret;
    ret = LOS_HwiCreate(LOS_PMU_IRQ_NR, 0, 0, Armv8PmuIrqHandler, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("pmu %d irq handler register failed\n", LOS_PMU_IRQ_NR);
        return ret;
    }

    ret = OsPerfHwInit(&g_armv8Pmu);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
