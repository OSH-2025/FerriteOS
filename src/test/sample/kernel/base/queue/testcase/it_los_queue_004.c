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
    CHAR aucBuff1[QUEUE_SHORT_BUFFER_LENTH] = "LiteOS";

    LOS_AtomicInc(&g_testCount);

    ret = LOS_QueueWrite(g_testQueueID01, &aucBuff1, 8, LOOP); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);

    return;
}

static VOID TaskF02(VOID)
{
    UINT32 ret;
    CHAR aucBuff2[QUEUE_SHORT_BUFFER_LENTH] = "";

    LOS_AtomicInc(&g_testCount);

    ret = LOS_QueueRead(g_testQueueID01, &aucBuff2, 8, 0); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);

    return;
}
static UINT32 TestCase(VOID)
{
    UINT32 ret;
    CHAR aucBuff1[8] = "LiteOS"; // len: 8

    TSK_INIT_PARAM_S stTask1 = {0};
    TSK_INIT_PARAM_S stTask2 = {0};

    stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    stTask1.pcName       = "TskName1";
    stTask1.uwStackSize  = TASK_STACK_SIZE_TEST;
    stTask1.usTaskPrio   = 22; // 22: pri

    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    stTask2.pcName       = "TskName2";
    stTask2.uwStackSize  = TASK_STACK_SIZE_TEST;
    stTask2.usTaskPrio   = 21; // pri: 21

    g_testCount = 0;

    ret = TestQueueCreate("Q1", 1, &g_testQueueID01, 0, sizeof(UINTPTR));
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_QueueWrite(g_testQueueID01, &aucBuff1, 8, LOOP);  // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_TaskCreate(&g_testTaskID01, &stTask1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    TestExtraTaskDelay(TEST_TASKDELAY_2TICK);

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT1);
    LOS_AtomicInc(&g_testCount);

    ret = LOS_TaskCreate(&g_testTaskID02, &stTask2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
    TestExtraTaskDelay(TEST_TASKDELAY_2TICK);

    ICUNIT_GOTO_EQUAL(g_testCount, 5, g_testCount, EXIT2); // g_testCount equal 5.
EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT:
    ret = LOS_QueueDelete(g_testQueueID01);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_QUE
 * @par TestCase_Number
 * ItLosQueue004
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Queue API function test.
 * @par TestCase_Pretreatment_Condition
 * System Support Queues.
 * @par TestCase_Test_Steps
 * Read and write the queue in the two task.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosQueue004(VOID)
{
    TEST_ADD_CASE("ItLosQueue004", TestCase, TEST_LOS, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
