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

#ifndef _IT_LOS_QUEUE_H
#define _IT_LOS_QUEUE_H

#include "los_atomic.h"
#include "los_queue.h"
#include "los_queue_pri.h"
#include "los_swtmr.h"
#include "ostest.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define QUEUE_SHORT_BUFFER_LENTH            12
#define QUEUE_STANDARD_BUFFER_LENTH        50
#define QUEUE_BASE_NUM                        3
#define QUEUE_BASE_MSGSIZE                8

#define LOOP 100

#ifdef __LP64__
#define PER_ADDED_VALUE     8
#else
#define PER_ADDED_VALUE     4
#endif

extern struct LOS_SPEC_CONTROL_HEADER g_stAllocatorCfg;
extern CHAR g_testQBuff1[QUEUE_SHORT_BUFFER_LENTH];
extern CHAR g_testQBuff2[QUEUE_SHORT_BUFFER_LENTH];
extern UINT32 g_testQueueCreateMode;
extern UINT32 TestQueueCreate(CHAR *queueName, UINT16 len, UINT32 *queueID,
                                    UINT32 flags, UINT16 maxMsgSize);

#ifdef LOSCFG_QUEUE_STATIC_ALLOCATION
#define TEST_QUEUE_BUFF_SIZE 0x3000

extern UINT8 g_testQueueBuf1[TEST_QUEUE_BUFF_SIZE];
extern UINT8 g_testQueueBuf2[TEST_QUEUE_BUFF_SIZE];
extern UINT8 g_testQueueBuf3[TEST_QUEUE_BUFF_SIZE];
extern UINT8 g_testQueueBuf4[TEST_QUEUE_BUFF_SIZE];
extern UINT32 Test_QueueCreateStatic(CHAR *queueName, UINT16 len, UINT32 *queueID,
                                           UINT32 flags, UINT16 maxMsgSize);
#endif

extern VOID ItLosQueue001(VOID);
extern VOID ItLosQueue002(VOID);
extern VOID ItLosQueue003(VOID);
extern VOID ItLosQueue004(VOID);
extern VOID ItLosQueue005(VOID);
extern VOID ItLosQueue006(VOID);
#ifdef LOS_MEM_TLSF
#else
extern VOID ItLosQueue007(VOID);
#endif
extern VOID ItLosQueueHead001(VOID);
extern VOID ItLosQueueHead002(VOID);
#ifdef LOSCFG_KERNEL_SMP
extern VOID ItSmpLosQueue001(VOID);
extern VOID ItSmpLosQueue002(VOID);
extern VOID ItSmpLosQueue003(VOID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _IT_LOS_QUEUE_H */