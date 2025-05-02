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
    UINT32 ret;
    UINT32 size = 0x100;
    VOID *p;

    p = LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
    ICUNIT_ASSERT_NOT_EQUAL(p, NULL, p);

    ret = LOS_MemTaskIdGet(p);
    ICUNIT_GOTO_EQUAL(ret, OsCurrTaskGet()->taskId, ret, EXIT0);

EXIT0:
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, p);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_MemTaskIdGet(p);
    ICUNIT_ASSERT_EQUAL(ret, OS_INVALID, ret);

    size = 0x1;
    p = LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
    ICUNIT_ASSERT_NOT_EQUAL(p, NULL, p);

    ret = LOS_MemTaskIdGet(p);
    ICUNIT_GOTO_EQUAL(ret, OsCurrTaskGet()->taskId, ret, EXIT1);

EXIT1:
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, p);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_MemTaskIdGet(p);
    ICUNIT_ASSERT_EQUAL(ret, OS_INVALID, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMem006
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_MemTaskIdGet API test.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: when alloc memory,obtain the ID of the task that uses the memory by LOS_MemTaskIdGet.
 * @par TestCase_Expected_Result
 * 1.LOS_MemTaskIdGet will return expected result.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMem006(VOID)
{
    TEST_ADD_CASE("ItLosMem006", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
