/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: iCunit Test Frame
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
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

#include "test_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

volatile INT32 g_testCount;
UINT32 g_testTaskID01;
UINT32 g_testTaskID02;
UINT32 g_testTaskID03;

UINT32 g_usSemID;
UINT32 g_mutexTest;
UINT32 g_testPeriod;
UINT16 g_usSwTmrID;
UINT32 g_testQueueID01;
UINT32 g_testQueueID02;
UINT32 g_testQueueID03;
EVENT_CB_S g_pevent;

UINT32 TaskCountGetTest(VOID)
{
    UINT32 uwLoop;
    UINT32 uwTaskCnt = 0;
    UINT32 uwIntSave;
    LosTaskCB *pstTaskCB = (LosTaskCB *)NULL;

    uwIntSave = LOS_IntLock();
    for (uwLoop = 0; uwLoop < g_taskMaxNum; uwLoop++) {
        pstTaskCB = (((LosTaskCB *)g_taskCBArray) + uwLoop);
        if (pstTaskCB->taskStatus & OS_TASK_STATUS_UNUSED) {
            continue;
        }
        uwTaskCnt++;
    }
    (VOID)LOS_IntRestore(uwIntSave);
    return uwTaskCnt;
}

VOID TestHwiTrigger(unsigned int irq)
{
    LOS_HwiTrigger(irq);

#ifdef LOSCFG_ARCH_RISCV
    TaskHoldCycles(10); // delay 10 cycles wait for device hwi trigger
#endif
    dsb();
    dsb();
}

VOID TestExtraTaskDelay(UINT32 uwTick)
{
#ifdef LOSCFG_KERNEL_SMP
    // trigger task schedule may occor on another core
    // needs adding delay and checking status later
    LOS_TaskDelay(uwTick);
#else
    // do nothing
#endif
}

UINT64 TestTickCountGet(VOID)
{
    /* not use LOS_TickCountGet for now,
       cause every timer is not match with others.
       use cpu0 timer instead.
     */
    return g_tickCount[0];
}

UINT64 TestTickCountByCurrCpuid(VOID)
{
    return g_tickCount[ArchCurrCpuid()];
}

/*
 * different from calling LOS_TaskDelay,
 * this func will not yeild this task to another one.
 */
VOID TestBusyTaskDelay(UINT32 tick)
{
    UINT32 cpuid = ArchCurrCpuid();
    UINT64 runtime = TestTickCountByCurrCpuid() + tick;
    while (1) {
        if (runtime <= TestTickCountByCurrCpuid()) {
            if (cpuid != ArchCurrCpuid()) {
                dprintf("error: yeild to another core when busy delay!\n");
            }
            break;
        }
        wfi();
    }
}

VOID TestAssertBusyTaskDelay(UINT32 timeout, UINT32 flag)
{
    UINT64 runtime = TestTickCountGet() + timeout;
    while (1) {
        if ((runtime <= TestTickCountGet()) || (g_testCount == flag)) {
            break;
        }
        wfi();
    }
}

VOID TaskHoldCycles(UINT64 cycles)
{
    UINT32 cntHi;
    UINT32 cntLow;
    UINT64 curCycles;
    LOS_GetCpuCycle(&cntHi, &cntLow);

    curCycles = ((UINT64)cntHi << 32) + cntLow; // 32 bit.
    cycles += curCycles;
    while (curCycles < cycles) {
        LOS_GetCpuCycle(&cntHi, &cntLow);
        curCycles = ((UINT64)cntHi << 32) + cntLow; // 32 bit.
    }
    return;
}

VOID TestDumpCpuid(VOID)
{
#ifdef LOSCFG_KERNEL_SMP
    PRINT_DEBUG("%d,cpuid = %d\n", LOS_CurTaskIDGet(), ArchCurrCpuid());
#endif
    return;
}

UINT32 MemGetFreeSize(VOID *pool)
{
    LOS_MEM_POOL_STATUS stStatus = {0};
    LOS_MemInfoGet(pool, &stStatus);

    return stStatus.uwTotalFreeSize;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
