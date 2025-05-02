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

#ifdef LOSCFG_BASE_CORE_SWTMR
static VOID SwtmrF01(VOID)
{
    UINT32 ret;
    CHAR aucBuffer1[] = "MiniOS";

    ret = LOS_QueueWrite(g_testQueueID01, &aucBuffer1, 8, 0); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);
}

static VOID SwtmrF02(VOID)
{
    UINT32 ret;
    CHAR aucBuffer2[] = "LiteOS";

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 1, g_testCount);

    ret = LOS_QueueRead(g_testQueueID01, &aucBuffer2, 8, 0); // bufferSize: 8
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);
}
static UINT32 TestCase(VOID)
{
    UINT32 ret;
    CHAR aucBuffer2[] = "LiteOS";
    UINT16 swtmrId1, swtmrId2;

    g_testCount = 0;

    ret = LOS_SwtmrCreate(4, LOS_SWTMR_MODE_ONCE, (SWTMR_PROC_FUNC)SwtmrF01, &swtmrId1, 0xffff); // interval: 4
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_SwtmrCreate(8, LOS_SWTMR_MODE_ONCE, (SWTMR_PROC_FUNC)SwtmrF02, &swtmrId2, 0xffff); // interval: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT0);

    ret = TestQueueCreate("Q1", 1, &g_testQueueID01, 0, sizeof(UINTPTR));
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    ret = LOS_SwtmrStart(swtmrId1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    ret = LOS_SwtmrStart(swtmrId2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    (VOID)LOS_TaskDelay(5); // delay 5 ticks
    (VOID)LOS_TaskDelay(5); // delay 5 ticks

    ret = LOS_QueueRead(g_testQueueID01, &aucBuffer2, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_QUEUE_ISEMPTY, ret, EXIT2);

EXIT2:
    ret = LOS_QueueDelete(g_testQueueID01);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
EXIT1:
    (VOID)LOS_SwtmrDelete(swtmrId2);
EXIT0:
    (VOID)LOS_SwtmrDelete(swtmrId1);

    return LOS_OK;
}

/**
 * @ingroup TEST_QUE
 * @par TestCase_Number
 * ItLosQueue006
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Queue API function test.
 * @par TestCase_Pretreatment_Condition
 * System Support Queues.
 * @par TestCase_Test_Steps
 * Reads and writes queue separately in the swtmr.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItLosQueue006(VOID)
{
    TEST_ADD_CASE("ItLosQueue006", TestCase, TEST_LOS, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
