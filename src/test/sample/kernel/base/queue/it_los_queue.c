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

UINT32 g_testQueueCreateMode = OS_QUEUE_ALLOC_DYNAMIC;
CHAR g_testQBuff1[QUEUE_SHORT_BUFFER_LENTH] = "LiteOS";
CHAR g_testQBuff2[QUEUE_SHORT_BUFFER_LENTH] = "";

#ifdef LOSCFG_QUEUE_STATIC_ALLOCATION
__attribute((aligned (LOSCFG_STACK_POINT_ALIGN_SIZE)))
UINT8 g_testQueueBuf1[TEST_QUEUE_BUFF_SIZE];
__attribute((aligned (LOSCFG_STACK_POINT_ALIGN_SIZE)))
UINT8 g_testQueueBuf2[TEST_QUEUE_BUFF_SIZE];
__attribute((aligned (LOSCFG_STACK_POINT_ALIGN_SIZE)))
UINT8 g_testQueueBuf3[TEST_QUEUE_BUFF_SIZE];
__attribute((aligned (LOSCFG_STACK_POINT_ALIGN_SIZE)))
UINT8 g_testQueueBuf4[TEST_QUEUE_BUFF_SIZE];
#endif

VOID ItSuiteLosQueueBase(VOID)
{
    ItLosQueue001();
    ItLosQueue002();
    ItLosQueue003();
    ItLosQueue004();
    ItLosQueue005();
    ItLosQueue006();
#ifdef LOS_MEM_TLSF
#else
    ItLosQueue007();
#endif
    ItLosQueueHead001();
    ItLosQueueHead002();
#ifdef LOSCFG_KERNEL_SMP
    ItSmpLosQueue001();
    ItSmpLosQueue002();
    ItSmpLosQueue003();
#endif

/* restore default test environment */
#ifdef LOSCFG_KERNEL_SMP
    HalIrqSetAffinity(HWI_NUM_TEST, 1);
#endif
}

#ifdef LOSCFG_QUEUE_STATIC_ALLOCATION
UINT32 Test_QueueCreateStatic(CHAR *queueName, UINT16 len, UINT32 *queueID,
                              UINT32 flags, UINT16 maxMsgSize)
{
    UINT32 ret;
    if (queueID == &g_testQueueID01) {
        ret = LOS_QueueCreateStatic(queueName, len, queueID, flags, maxMsgSize,
                                    g_testQueueBuf1, TEST_QUEUE_BUFF_SIZE);
    } else if (queueID == &g_testQueueID02) {
        ret = LOS_QueueCreateStatic(queueName, len, queueID, flags, maxMsgSize,
                                    g_testQueueBuf2, TEST_QUEUE_BUFF_SIZE);
    } else if (queueID == &g_testQueueID03) {
        ret = LOS_QueueCreateStatic(queueName, len, queueID, flags, maxMsgSize,
                                    g_testQueueBuf3, TEST_QUEUE_BUFF_SIZE);
    } else {
        ret = LOS_QueueCreateStatic(queueName, len, queueID, flags, maxMsgSize,
                                    g_testQueueBuf4, TEST_QUEUE_BUFF_SIZE);
    }
    return ret;
}
#endif

UINT32 TestQueueCreate(CHAR *queueName, UINT16 len, UINT32 *queueID,
                        UINT32 flags, UINT16 maxMsgSize)
{
    if (g_testQueueCreateMode == OS_QUEUE_ALLOC_DYNAMIC) {
        return LOS_QueueCreate(queueName, len, queueID, flags, maxMsgSize);
    } else {
#ifdef LOSCFG_QUEUE_STATIC_ALLOCATION
        return Test_QueueCreateStatic(queueName, len, queueID, flags, maxMsgSize);
#endif
    }
    return LOS_NOK;
}

VOID ItSuiteLosQueueCreateDynamic(VOID)
{
    g_testQueueCreateMode = OS_QUEUE_ALLOC_DYNAMIC;
    ItSuiteLosQueueBase();
}

VOID ItSuiteLosQueueCreateStatic(VOID)
{
    g_testQueueCreateMode = OS_QUEUE_ALLOC_STATIC;
    ItSuiteLosQueueBase();
    g_testQueueCreateMode = OS_QUEUE_ALLOC_DYNAMIC;
}

VOID ItSuiteLosQueue(VOID)
{
    ItSuiteLosQueueCreateDynamic();
#ifdef LOSCFG_QUEUE_STATIC_ALLOCATION
    ItSuiteLosQueueCreateStatic();
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
