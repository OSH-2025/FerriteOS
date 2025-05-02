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

#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
static UINT32 TestCase(VOID)
{
    UINT32 ret;
    UINT32 size;
    VOID *p[SLAB_MEM_COUNT + 1] = {0};
    UINT32 cfg[SLAB_MEM_COUNT] = {1024, 512, 256, 16}; /* alloc memory COUNT 1024, 512, 256, 16 */

    LOS_SlabSizeCfg(cfg, SLAB_MEM_COUNT);

    MEM_INIT_SLAB();
    MEM_START_GetMemInfo(g_poolSlab);

    p[0] = LOS_MemAlloc(g_poolSlab, 0x10); // 0:the first of array p
    ICUNIT_ASSERT_NOT_EQUAL(p[0], NULL, p[0]);

    memset(p[0], 1, 0x10);

    p[1] = LOS_MemAlloc(g_poolSlab, 0x20); // 1:the second of array p
    ICUNIT_ASSERT_NOT_EQUAL(p[1], NULL, p[1]);

    memset(p[1], 1, 0x20);

    p[2] = LOS_MemAlloc(g_poolSlab, 0x40);    // 2:the third of array p
    ICUNIT_ASSERT_NOT_EQUAL(p[2], NULL, p[2]); // 2:the third of array p

    memset(p[2], 1, 0x40); // 2:the third of array p

    p[3] = LOS_MemAlloc(g_poolSlab, 0x80);    // 3:the fourth of array p
    ICUNIT_ASSERT_NOT_EQUAL(p[3], NULL, p[3]); // 3:the fourth of array p

    memset(p[3], 1, 0x80); // 3:the fourth of array p

    p[4] = LOS_MemAlloc(g_poolSlab, 0x0); // 4:the fifth of array p
    ICUNIT_ASSERT_EQUAL(p[4], NULL, p[4]); // 4:the fifth of array p

    for (int i = 0; i < 4; i++) { // 4:the num of alloc memory success
        ret = LOS_MemFree(g_poolSlab, p[i]);
        ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    }

    MEM_END_GetMemInfo(g_poolSlab);
    MEM_INFO_CHECK();
    MEM_FREE_SLAB();

    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMem014
 * @par TestCase_TestCase_Type
 * Function test
 * @brief in memory mode LOSCFG_KERNEL_MEM_SLAB_EXTENTION,alloc memory succeeded.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: alloc memory by LOS_MemAlloc in memory mode LOSCFG_KERNEL_MEM_SLAB_EXTENTION.
 * @par TestCase_Expected_Result
 * 1.LOS_MemAlloc return expected result.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMem014(VOID)
{
    TEST_ADD_CASE("ItLosMem014", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
