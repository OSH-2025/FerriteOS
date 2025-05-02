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
    UINT32 ret;
    UINT32 queueID;
    CHAR   aucBuff1[QUEUE_SHORT_BUFFER_LENTH] = "abc";
    CHAR   aucBuff2[QUEUE_SHORT_BUFFER_LENTH] = "";
    UINT32 readSize;

    ret = TestQueueCreate("Q1", 3, &queueID, 0, QUEUE_SHORT_BUFFER_LENTH); // len: 3
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_QueueWriteCopy(queueID, &aucBuff1, QUEUE_SHORT_BUFFER_LENTH, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memset(aucBuff2, 0, QUEUE_SHORT_BUFFER_LENTH);
    ret = LOS_QueueRead(queueID, &aucBuff2, QUEUE_SHORT_BUFFER_LENTH, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_QueueWriteCopy(queueID, &aucBuff1, QUEUE_SHORT_BUFFER_LENTH + PER_ADDED_VALUE, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG, ret, EXIT);

    memset(aucBuff2, 0, QUEUE_SHORT_BUFFER_LENTH);
    ret = LOS_QueueRead(queueID, &aucBuff2, QUEUE_SHORT_BUFFER_LENTH, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_QUEUE_ISEMPTY, ret, EXIT);

    ret = LOS_QueueWrite(queueID, &aucBuff1, QUEUE_SHORT_BUFFER_LENTH - 1, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memset(aucBuff2, 0, QUEUE_SHORT_BUFFER_LENTH);
    ret = LOS_QueueRead(queueID, &aucBuff2, QUEUE_SHORT_BUFFER_LENTH, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memset(aucBuff2, 0, QUEUE_SHORT_BUFFER_LENTH);
    ret = LOS_QueueRead(queueID, &aucBuff2, QUEUE_SHORT_BUFFER_LENTH, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_QUEUE_ISEMPTY, ret, EXIT);

    ret = LOS_QueueWrite(queueID, &aucBuff1, 0, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memset(aucBuff2, 0, QUEUE_SHORT_BUFFER_LENTH);
    readSize = 0;
    ret = LOS_QueueReadCopy(queueID, &aucBuff2, &readSize, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_QUEUE_READSIZE_IS_INVALID, ret, EXIT);

EXIT:
    ret = LOS_QueueDelete(queueID);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_QUE
 * @par TestCase_Number
 * ItLosQueue007
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_QueueWriteCopy function test.
 * @par TestCase_Pretreatment_Condition
 * System Support Queues.
 * @par TestCase_Test_Steps
 * LOS_QueueWriteCopy input parameter test of the third parameter.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItLosQueue007(VOID)
{
    TEST_ADD_CASE("ItLosQueue007", TestCase, TEST_LOS, TEST_QUE, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
