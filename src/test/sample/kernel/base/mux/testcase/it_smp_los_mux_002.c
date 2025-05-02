/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Mux Test Case
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

#include "it_los_mux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#ifdef LOSCFG_KERNEL_SMP
static VOID TaskF01(VOID)
{
    UINT32 ret;

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 2, g_testCount); // g_testCount assert equal 2
    LOS_AtomicInc(&g_testCount);

    ret = LOS_MuxPend(g_mutexTest, 5); // timeout 5
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_ERRNO_MUX_TIMEOUT, ret);

    ICUNIT_GOTO_EQUAL(g_testCount, 4, g_testCount, EXIT); // g_testCount equal 4
    LOS_AtomicInc(&g_testCount); // g_testCount equal 5
    TestDumpCpuid();
    return;
EXIT:
    (VOID)LOS_TaskDelete(g_testTaskID01);
    return;
}

static VOID TaskF02(VOID)
{
    UINT32 ret;

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 0, g_testCount);
    LOS_AtomicInc(&g_testCount); // g_testCount equal 1

    ret = LOS_MuxPend(g_mutexTest, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);
    TestDumpCpuid();

    (VOID)LOS_TaskDelay(10); // delay 10 ticks
    ICUNIT_GOTO_EQUAL(g_testCount, 5, g_testCount, EXIT); // g_testCount equal 5
EXIT:
    ret = LOS_MuxPost(g_mutexTest);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
    LOS_AtomicInc(&g_testCount);  // g_testCount equal 6
    return;
}

static VOID HwiF01(VOID)
{
    UINT32 ret;

    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT); // g_testCount equal 3
    ret = LOS_MuxPost(g_mutexTest);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_ERRNO_MUX_PEND_INTERR, ret);
    LOS_AtomicInc(&g_testCount);
    TestDumpCpuid();
    return;
EXIT:
    (VOID)LOS_HwiDelete(HWI_NUM_TEST, NULL);
    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret, otherCpuid;
    TSK_INIT_PARAM_S stTestTask;

    g_testCount = 0;

    ret = LOS_MuxCreate(&g_mutexTest);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    otherCpuid = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    ret = LOS_HwiCreate(HWI_NUM_TEST, 1, 0, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT0);

    ret = LOS_HwiEnable(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
#ifdef LOSCFG_KERNEL_SMP
    HalIrqSetAffinity(HWI_NUM_TEST, CPUID_TO_AFFI_MASK(otherCpuid)); // other cpu
#endif
    TEST_TASK_PARAM_INIT_AFFI(stTestTask, "ItSmpLosMux002_task1", TaskF02,
                              TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(otherCpuid)); // other cpu
    ret = LOS_TaskCreate(&g_testTaskID02, &stTestTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    TestAssertBusyTaskDelay(100, 2); // timeout 100 flag 2
    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT2); // g_testCount equal 2

    TEST_TASK_PARAM_INIT_AFFI(stTestTask, "ItSmpLosMux002_task2", TaskF01,
                              TASK_PRIO_TEST + 1, CPUID_TO_AFFI_MASK(ArchCurrCpuid())); // current cpu
    ret = LOS_TaskCreate(&g_testTaskID01, &stTestTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    (VOID)LOS_TaskDelay(1); // TaskF01 run
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT); // g_testCount equal 3

    TestHwiTrigger(HWI_NUM_TEST);

    (VOID)LOS_TaskDelay(20);  // delay 20 ticks
    ICUNIT_GOTO_EQUAL(g_testCount, 6, g_testCount, EXIT); // g_testCount equal 6

    ret = LOS_MuxDelete(g_mutexTest);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

EXIT:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT1:
    (VOID)LOS_HwiDelete(HWI_NUM_TEST, NULL);
EXIT0:
    (VOID)LOS_MuxDelete(g_mutexTest);
    return LOS_OK;
}

/**
 * @ingroup TEST_MUX
 * @par TestCase_Number
 * ItSmpLosMux002
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Mux smp API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * After the MUX is created, pend the MUX in the running task of the another core,
 * After post the MUX in the interrupt of another core, pend the mux on the current core,
 * At last post the mux on another core ,deletes the MUX on the current core
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItSmpLosMux002(VOID)
{
    TEST_ADD_CASE("ItSmpLosMux002", TestCase, TEST_LOS, TEST_MUX, TEST_LEVEL0, TEST_FUNCTION);
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
