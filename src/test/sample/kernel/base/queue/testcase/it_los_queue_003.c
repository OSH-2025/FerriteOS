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

static UINT32 TestCase(VOID)
{
    UINT32 ret, index;
    CHAR aucBuff1[QUEUE_SHORT_BUFFER_LENTH] = "LiteOS";
    CHAR aucBuff2[QUEUE_SHORT_BUFFER_LENTH] = "IOTOS";
    CHAR aucBuff3[8] = "TEST"; // len: 8
    UINTPTR aucBuff4, aucBuff5, aucBuff6;

    ret = TestQueueCreate("Q1", 3, &g_testQueueID01, 0, sizeof(UINTPTR)); // len: 3
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_QueueWrite(g_testQueueID01, aucBuff1, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueWrite(g_testQueueID01, aucBuff2, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueWrite(g_testQueueID01, &aucBuff3, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueRead(g_testQueueID01, &aucBuff4, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueRead(g_testQueueID01, &aucBuff5, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueRead(g_testQueueID01, &aucBuff6, 8, 0); // bufferSize: 8
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    for (index = 0; index < 8; index++) { // 8
        ICUNIT_GOTO_EQUAL(*((char *)aucBuff4 + index), aucBuff1[index], *((char *)aucBuff4 + index), EXIT);
        ICUNIT_GOTO_EQUAL(*((char *)aucBuff5 + index), aucBuff2[index], *((char *)aucBuff5 + index), EXIT);
        ICUNIT_GOTO_EQUAL(*((char *)aucBuff6 + index), aucBuff3[index], *((char *)aucBuff6 + index), EXIT);
    }

EXIT:
    ret = LOS_QueueDelete(g_testQueueID01);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_QUE
 * @par TestCase_Number
 * ItLosQueue003
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Read/Write Sequence and Message Content Accuracy Test.
 * @par TestCase_Pretreatment_Condition
 * System Support Queues.
 * @par TestCase_Test_Steps
 * Verify the correctness of the read/write sequence and message content.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItLosQueue003(VOID)
{
    TEST_ADD_CASE("ItLosQueue003", TestCase, TEST_LOS, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
