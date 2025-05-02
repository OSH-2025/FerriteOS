/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */
 
#include "it_los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_BASE_CORE_TIMESLICE
static VOID TaskF02(VOID const *argument)
{
    g_timesliceTestCount++;
    g_testCount++;

    return;
}

static VOID TaskF01(VOID const *argument)
{
    UINT32 intSave;

    intSave = LOS_IntLock();

    g_timesliceTestCount1 = TestTickCountGet();

    LOS_IntRestore(intSave);

    while (1) {
        dprintf("");
        intSave = LOS_IntLock();
        if ((g_timesliceTestCount1 + LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT + 1) < TestTickCountGet()) {
            LOS_IntRestore(intSave);
            break;
        }
        LOS_IntRestore(intSave);
    }
    ICUNIT_ASSERT_EQUAL_VOID(g_timesliceTestCount, 1, g_timesliceTestCount);

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    g_timesliceTestCount1 = 0;
    g_timesliceTestCount = 0;
    g_testCount = 0;

    LOS_TaskLock();

    TSK_INIT_PARAM_S task;

    task.pfnTaskEntry = (TSK_ENTRY_FUNC) TaskF01;
    task.pcName       = "TimesTsk001A";
    task.uwStackSize  = TASK_STACK_SIZE_TEST * 4; // size:4
    task.usTaskPrio   = TASK_PRIO_TEST - 1;
    task.uwResved     = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    task.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif

    ret = LOS_TaskCreate(&g_testTaskID01, &task);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    task.pfnTaskEntry = (TSK_ENTRY_FUNC) TaskF02;
    task.pcName       = "TimesTsk001B";
    task.uwStackSize  = TASK_STACK_SIZE_TEST;
    task.usTaskPrio   = TASK_PRIO_TEST - 1;
    task.uwResved     = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    task.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif

    ret = LOS_TaskCreate(&g_testTaskID02, &task);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    LOS_TaskUnlock();

    (VOID)LOS_TaskDelay(5); // delay 5 ticks

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT2);
EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT1:
    LOS_TaskUnlock();
    (VOID)LOS_TaskDelete(g_testTaskID01);

    return LOS_OK;
}

/**
 * @ingroup TEST_TASK
 * @par TestCase_Number
 * ItLosTaskTimeslice001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Task API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Task rotation with the same priority.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosTaskTimeslice001(VOID)
{
    TEST_ADD_CASE("ItLosTaskTimeslice001", TestCase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
