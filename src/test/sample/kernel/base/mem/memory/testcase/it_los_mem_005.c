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
    UINT32 memUseBlk;
    UINT32 size = 0x100;
    VOID *p = NULL;

    MEM_START();

    MEM_INIT();

    memUseBlk = LOS_MemUsedBlksGet(g_pool);
#ifdef LOS_MEM_TLSF
#ifdef LOSCFG_KERNEL_MEM_SPEC
    ICUNIT_GOTO_EQUAL(memUseBlk, 1 + LOS_SLAB_USED_CNT + g_uwSpecMemClassCnt,
                      memUseBlk, EXIT);
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 1 + LOS_SLAB_USED_CNT, memUseBlk, EXIT);
#endif
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 1, memUseBlk, EXIT);
#endif

    p = LOS_MemAlloc((VOID *)g_pool, size);
    ICUNIT_GOTO_NOT_EQUAL(p, NULL, p, EXIT);

    memUseBlk = LOS_MemUsedBlksGet(g_pool);
#ifdef LOS_MEM_TLSF
#ifdef LOSCFG_KERNEL_MEM_SPEC
    ICUNIT_GOTO_EQUAL(memUseBlk, 2 + LOS_SLAB_USED_CNT + g_uwSpecMemClassCnt, // memUseBlk equal 2.
                      memUseBlk, EXIT);
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 2 + LOS_SLAB_USED_CNT, memUseBlk, EXIT); // memUseBlk equal 2.
#endif
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 2, memUseBlk, EXIT); // memUseBlk equal 2.
#endif

    ret = LOS_MemFree((VOID *)g_pool, p);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    memUseBlk = LOS_MemUsedBlksGet(g_pool);
#ifdef LOS_MEM_TLSF
#ifdef LOSCFG_KERNEL_MEM_SPEC
    ICUNIT_GOTO_EQUAL(memUseBlk, 1 + LOS_SLAB_USED_CNT + g_uwSpecMemClassCnt,
                      memUseBlk, EXIT);
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 1 + LOS_SLAB_USED_CNT, memUseBlk, EXIT);
#endif
#else
    ICUNIT_GOTO_EQUAL(memUseBlk, 1, memUseBlk, EXIT);
#endif

EXIT:
    MEM_FREE();
    MEM_END();
    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMem005
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_MemUsedBlksGet API test.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: alloc memory and get memory used block by LOS_MemUsedBlksGet.
 * @par TestCase_Expected_Result
 * 1.LOS_MemUsedBlksGet return expected result in different memory mode.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMem005(VOID)
{
    TEST_ADD_CASE("ItLosMem005", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
