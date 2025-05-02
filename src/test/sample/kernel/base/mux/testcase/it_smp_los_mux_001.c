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

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 0, g_testCount);
    LOS_AtomicInc(&g_testCount);

    ret = LOS_MuxPend(g_mutexTest, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);

    TestAssertBusyTaskDelay(LOOP, 3); // wait for flag equal 3
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT); // g_testCount equal 3

    ret =  LOS_MuxPost(g_mutexTest);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);

    TestDumpCpuid();
    return;
EXIT:
    LOS_MuxPost(g_mutexTest);
    return;
}

static VOID TaskF02(VOID)
{
    UINT32 ret;

    TestAssertBusyTaskDelay(LOOP, 2); // flag 2
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 2, g_testCount); // g_testCount assert equal 2
    ret = LOS_MuxDelete(g_mutexTest);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_ERRNO_MUX_PENDED, ret);
    LOS_AtomicInc(&g_testCount);
    TestDumpCpuid();
    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret, otherCpuid;
    TSK_INIT_PARAM_S testTask;

    g_testCount = 0;

    ret = LOS_MuxCreate(&g_mutexTest);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    otherCpuid = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    TEST_TASK_PARAM_INIT_AFFI(testTask, "ItSmpLosMux001_task2", TaskF02,
                              TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(otherCpuid)); // other cpu
    ret = LOS_TaskCreate(&g_testTaskID02, &testTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    TEST_TASK_PARAM_INIT_AFFI(testTask, "ItSmpLosMux001_task1", TaskF01,
                              TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(ArchCurrCpuid())); // current cpu
    ret = LOS_TaskCreate(&g_testTaskID01, &testTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    TestAssertBusyTaskDelay(LOOP, 4); // flag 4
    ICUNIT_GOTO_EQUAL(g_testCount, 4, g_testCount, EXIT2); // g_testCount equal 4

EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT:
    (VOID)LOS_MuxDelete(g_mutexTest);
    return LOS_OK;
}

/**
 * @ingroup TEST_MUX
 * @par TestCase_Number
 * ItSmpLosMux001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Mux smp API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Create a MUX in the running task of the current core,
 * and then delete the MUX in other cores after the MUX in the current core is pending.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItSmpLosMux001(VOID)
{
    TEST_ADD_CASE("ItSmpLosMux001", TestCase, TEST_LOS, TEST_MUX, TEST_LEVEL0, TEST_FUNCTION);
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
