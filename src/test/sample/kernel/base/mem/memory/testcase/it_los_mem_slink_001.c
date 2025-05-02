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

#ifdef LOSCFG_MEM_MUL_MODULE
#ifdef LOS_MEM_SLINK
static UINT32 TestCase(VOID)
{
    UINT32 ret, retTmpFirst, retTmp, i, count;
    UINT32 size = 0x1;
    VOID *p[800] = {NULL}; // Array size: 800
    UINT32 itemAlign = sizeof(VOID *);
    UINT32 itemSize;
    UINT32 itemLevel;

#ifdef LOS_MEM_SLAB
    struct LOS_SLAB_ALLOCATOR_CFG_INFO stCfg;
    memset(&stCfg, 0, sizeof(stCfg));
    LOS_SlabAllocatorCfgRegister(g_pool, &stCfg);
#endif

    MEM_INIT();
    retTmpFirst = LOS_MemMusedGet(MODULE0);

label:
    itemSize = (size + (itemAlign - 1)) & ~(itemAlign - 1);
    itemSize += sizeof(LOS_SLINK_NODE);
    itemLevel = OS_SLINK_CLASS_LEVEL_GET(itemSize);
    itemSize = OS_SLINK_CLASS_SIZE_GET(itemLevel);

    count = (LOS_SLINK_BLOCK_BOUNDARY - sizeof(LOS_SLINK_BLOCK_NODE)) / itemSize;
    for (i = 0; i < count; i++) {
        p[i] = LOS_MemMalloc(g_pool, size, MODULE0);
        ICUNIT_GOTO_NOT_EQUAL(p[i], NULL, 0, EXIT);
        memset(p[i], 1, size);
    }
    retTmp = LOS_MemMusedGet(MODULE0) - retTmpFirst;
    ICUNIT_GOTO_EQUAL(retTmp, (LOS_SLINK_BLOCK_BOUNDARY - ((LOS_SLINK_BLOCK_BOUNDARY - sizeof(LOS_SLINK_BLOCK_NODE)) \
        % itemSize)), retTmp, EXIT);
    for (i = 0; i < count; i++) {
        ret = LOS_MemMfree(g_pool, p[i], MODULE0);
        ICUNIT_GOTO_EQUAL(ret, LOS_OK, 0, EXIT);
    }
    retTmp = LOS_MemMusedGet(MODULE0) - retTmpFirst;
    ICUNIT_GOTO_EQUAL(retTmp, 0, retTmp, EXIT);
    size += 1;
    if (itemSize >= 64) // max itemSize is 64
        goto EXIT;
    goto label;

EXIT:
    MEM_FREE();
    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMemSlink001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Test interface LOS_MemMusedGet/LOS_MemMalloc/LOS_MemMfree when device support LOS_MEM_SLINK.
 * @par TestCase_Pretreatment_Condition
 * Device support LOS_MEM_SLINK.
 * @par TestCase_Test_Steps
 * step1: Invoke the LOS_MemMusedGet/LOS_MemMalloc/LOS_MemMfree interface.
 * @par TestCase_Expected_Result
 * 1.LOS_MemMusedGet/LOS_MemMalloc/LOS_MemMfree return expected result.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMemSlink001(VOID)
{
    TEST_ADD_CASE("ItLosMemSlink001", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
