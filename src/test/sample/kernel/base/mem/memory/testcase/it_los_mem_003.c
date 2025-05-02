/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Memory Test Case
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

#include "it_los_mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 TestCase(VOID)
{
    UINT32 ret, tempSize;
    UINT32 memUsed0, memUsed1, memUsed2;
    UINT32 size = 0x100;
    VOID *p, *p1;

    MEM_START();
    MEM_INIT();

    memUsed2 = memUsed0 = LOS_MemTotalUsedGet(g_pool);

    p = LOS_MemAlloc((VOID *)g_pool, size);
    ICUNIT_GOTO_NOT_EQUAL(p, NULL, p, EXIT);

    memUsed1 = LOS_MemTotalUsedGet(g_pool);

    tempSize = memUsed1 - memUsed0;
#ifdef LOS_MEM_TLSF
    ICUNIT_GOTO_EQUAL(tempSize, size + HEAD_SIZE, tempSize, EXIT);
#else
    ICUNIT_GOTO_EQUAL(tempSize, size + sizeof(LOS_MEM_DYN_NODE), tempSize, EXIT);
#endif

    memUsed0 = LOS_MemTotalUsedGet(g_pool);

    p1 = LOS_MemAlloc((VOID *)g_pool, size);
    ICUNIT_GOTO_NOT_EQUAL(p, NULL, p, EXIT);

    memUsed1 = LOS_MemTotalUsedGet(g_pool);
    tempSize = memUsed1 - memUsed0;
    ICUNIT_GOTO_EQUAL(tempSize, size + HEAD_SIZE, tempSize, EXIT);

    ret = LOS_MemFree((VOID *)g_pool, p);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_MemFree((VOID *)g_pool, p1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memUsed1 = LOS_MemTotalUsedGet(g_pool);
    ICUNIT_GOTO_EQUAL(memUsed1, memUsed2, memUsed1, EXIT);

EXIT:
    MEM_FREE();
    MEM_END();
    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMem003
 * @par TestCase_TestCase_Type
 * Function test
 * @brief alloc a memory with a length of size by LOS_MemAlloc will use size + sizeof(LOS_MEM_DYN_NODE) bytes.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: alloc a memory with a length of size by LOS_MemAlloc and check it physical memory usage.
 * @par TestCase_Expected_Result
 * 1.it physical memory usage equal size + sizeof(LOS_MEM_DYN_NODE) bytes.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMem003(VOID)
{
    TEST_ADD_CASE("ItLosMem003", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
