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

#ifdef LOS_MEM_SLAB
static UINT32 TestCase(VOID)
{
    UINT32 ret;
    UINT32 alloc_size = 16; // alloc size is 16
    VOID *p[200] = {NULL}; // Array size: 200
    UINT32 blockNum = LOS_SLAB_DEFAULT_ALLOCATOR_SIZE / alloc_size;
    UINT32 freeBlks;
    // UINT32 bucketNum = ((UINT32)TEST_MEM_SIZE / LOS_SLAB_DEFAULT_ALLOCATOR_SIZE) >> 1;
    VOID *p0;
    int i = 0;

    MEM_START();
    MEM_INIT();

    // case 1:alloc once, result:used blk 1, free blk 15, used bucket 1, free
    // buckets bucketNum - 1.
    p0 = LOS_MemAlloc(g_pool, alloc_size);
    ICUNIT_GOTO_NOT_EQUAL(p0, NULL, 0, EXIT);
    p[i] = p0;

    ret = LOS_SlabUsedBlksGet(g_pool, alloc_size);
    ICUNIT_GOTO_EQUAL(1, ret, ret, EXIT);

    ret = LOS_SlabfreeBlksGet(g_pool, alloc_size);
    freeBlks = blockNum - 1;
    ICUNIT_GOTO_EQUAL(freeBlks, ret, ret, EXIT);

    // case 2:alloc 127 times, result:used blk 128, free blk 0, used bucket 8,
    // free buckets bucketNum - 8.
    for (i = 1; i < LOS_SLAB_DEFAULT_ALLOCATOR_SIZE / alloc_size; i++) {
        p0 = LOS_MemAlloc(g_pool, alloc_size);
        ICUNIT_GOTO_NOT_EQUAL(p0, NULL, 0, EXIT);
        p[i] = p0;
    }

    ret = LOS_SlabUsedBlksGet(g_pool, alloc_size);
    ICUNIT_GOTO_EQUAL(i, ret, ret, EXIT);

    ret = LOS_SlabfreeBlksGet(g_pool, alloc_size);
    freeBlks = blockNum - ((i - 1) % blockNum) - 1;
    ICUNIT_GOTO_EQUAL(freeBlks, ret, ret, EXIT);

    // case 3:free 127 times, result:used blk 1, free blk 15, used bucket 1, free
    // buckets bucketNum - 1.
    i = i - 1;
    for (; i > 0; i--) {
        ret = LOS_MemFree(g_pool, p[i]);
        ICUNIT_GOTO_EQUAL(ret, LOS_OK, 0, EXIT);
    }

    ret = LOS_SlabUsedBlksGet(g_pool, alloc_size);
    ICUNIT_GOTO_EQUAL(1, ret, ret, EXIT);

    ret = LOS_SlabfreeBlksGet(g_pool, alloc_size);
    freeBlks = blockNum - 1;
    ICUNIT_GOTO_EQUAL(freeBlks, ret, ret, EXIT);

    // case 3:free once, result:used blk 0, free blk 16, used bucket 1, free
    // buckets bucketNum - 1.(first create bucket do not destroy)
    ret = LOS_MemFree(g_pool, p[i]);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, 0, EXIT);

    ret = LOS_SlabUsedBlksGet(g_pool, alloc_size);
    ICUNIT_GOTO_EQUAL(0, ret, ret, EXIT);

    ret = LOS_SlabfreeBlksGet(g_pool, alloc_size);
    ICUNIT_GOTO_EQUAL(blockNum, ret, ret, EXIT);

EXIT:
    MEM_FREE();
    MEM_END();
    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMemSlab001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Test Slab interface LOS_SlabUsedBlksGet/LOS_SlabfreeBlksGet
 * @par TestCase_Pretreatment_Condition
 * Device support Slab.
 * @par TestCase_Test_Steps
 * step1: Invoke the LOS_SlabUsedBlksGet/LOS_SlabfreeBlksGet interface.
 * @par TestCase_Expected_Result
 * 1.LOS_SlabUsedBlksGet/LOS_SlabfreeBlksGet return expected result.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMemSlab001(VOID)
{
    TEST_ADD_CASE("ItLosMemSlab001", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
