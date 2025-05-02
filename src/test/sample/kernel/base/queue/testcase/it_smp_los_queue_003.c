/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Queue Test Case
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

#include "it_los_queue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID TaskF01(VOID)
{
    UINT32 ret;
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 1, g_testCount);

    LOS_AtomicInc(&g_testCount);
    ret = LOS_QueueRead(g_testQueueID01, &g_testQBuff2, 8, LOS_WAIT_FOREVER); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 4, g_testCount); // g_testCount equal 4

    LOS_AtomicInc(&g_testCount);

    TestDumpCpuid();
    return;
}

static VOID TaskF02(VOID)
{
    UINT32 ret;

    LOS_AtomicInc(&g_testCount);
    ret = LOS_QueueRead(g_testQueueID01, &g_testQBuff2, 8, LOS_WAIT_FOREVER); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 2, g_testCount);  // 2 other core  get the sem first

    TestDumpCpuid();
    LOS_AtomicInc(&g_testCount);
    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret, otherCpuid;
    TSK_INIT_PARAM_S testTask;

    g_testCount = 0;

    ret = TestQueueCreate("Q1", 1, &g_testQueueID01, 0, 8); // maxMsgSize: 8
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    otherCpuid = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    TEST_TASK_PARAM_INIT_AFFI(testTask, "it_sem_033_task2", TaskF02,
        TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(otherCpuid));
    ret = LOS_TaskCreate(&g_testTaskID02, &testTask);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    TestAssertBusyTaskDelay(LOOP, 1);
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);

    TestBusyTaskDelay(10); // 10 let the task f02 todo pend

    ret = OS_TCB_FROM_TID(g_testTaskID02)->taskStatus;
    ICUNIT_GOTO_NOT_EQUAL(ret & OS_TASK_STATUS_PEND, 0, ret, EXIT);

    TEST_TASK_PARAM_INIT_AFFI(testTask, "it_sem_033_task1", TaskF01,
        TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
    ret = LOS_TaskCreate(&g_testTaskID01, &testTask);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT); // g_testCount equal 2.

    LOS_TaskLock();

    ret = LOS_QueueWrite(g_testQueueID01, &g_testQBuff1, 8, LOS_WAIT_FOREVER); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    TestAssertBusyTaskDelay(LOOP, 3); // delay 3
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT1); // g_testCount equal 3.

    ret = LOS_QueueWrite(g_testQueueID01, &g_testQBuff1, 8, LOS_WAIT_FOREVER); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT1); // 3 not schedule in current cpu cause tasklock

    LOS_AtomicInc(&g_testCount);

    LOS_TaskUnlock();

    ICUNIT_GOTO_EQUAL(g_testCount, 5, g_testCount, EXIT); // g_testCount equal 5.

EXIT:
    (VOID)LOS_TaskDelete(g_testTaskID01);
    (VOID)LOS_TaskDelete(g_testTaskID02);
    ret = LOS_QueueDelete(g_testQueueID01);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    return LOS_OK;
EXIT1:
    LOS_TaskUnlock();
    (VOID)LOS_TaskDelete(g_testTaskID01);
    (VOID)LOS_TaskDelete(g_testTaskID02);
    ret = LOS_QueueDelete(g_testQueueID01);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    return LOS_OK;
}

/**
 * @ingroup TEST_QUE
 * @par TestCase_Number
 * ItSmpLosQueue003
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Queue smp function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Read the queue in the running task of the current core.write queue in the lock task.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItSmpLosQueue003(VOID)
{
    TEST_ADD_CASE("ItSmpLosQueue003", TestCase, TEST_LOS, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
