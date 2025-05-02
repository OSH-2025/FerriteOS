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

VOID* g_pool = NULL;
VOID* g_poolSlab = NULL;
LOS_MEM_POOL_STATUS  g_memStartStatus;
LOS_MEM_POOL_STATUS  g_memEndStatus;

VOID MEM_INIT_SLAB(VOID)
{
    UINT32 uwRet;

    g_poolSlab = LOS_MemAlloc(OS_SYS_MEM_ADDR, TEST_MEM_SIZE_SMALL);
    if (g_poolSlab == NULL) {
        dprintf("alloc failed , mem testcase would be failed!!!!\n");
    }

    uwRet = LOS_MemInit(g_poolSlab, TEST_MEM_SIZE_SMALL);
    if (uwRet != LOS_OK) {
        dprintf("init failed, mem testcase would be failed!!!!\n");
    }
}

VOID MEM_FREE_SLAB(VOID)
{
    VOID *p1 = NULL;
    UINT32 uwRet;

    p1 = memset(g_poolSlab, 0, TEST_MEM_SIZE_SMALL);
    ICUNIT_ASSERT_EQUAL_VOID(p1, g_poolSlab, p1);
#ifdef LOSCFG_MEM_MUL_POOL
    uwRet = LOS_MemDeInit(g_poolSlab);
    if (uwRet != LOS_OK) {
        dprintf("Deinit failed , mem testcase may be wrong!!!\n");
    }
#endif
    uwRet = LOS_MemFree(OS_SYS_MEM_ADDR, g_poolSlab);
    if (uwRet != LOS_OK) {
        dprintf("free failed,a leak mem is born!!!!\n");
    }
}

VOID MEM_START(VOID)
{
    g_pool = LOS_MemAlloc(OS_SYS_MEM_ADDR, TEST_MEM_SIZE);
    if (g_pool == NULL) {
        dprintf("alloc failed , mem testcase would be failed!!!!\n");
    }
}

VOID MEM_END(VOID)
{
    UINT32 uwRet;
    uwRet = LOS_MemFree(OS_SYS_MEM_ADDR, g_pool);
    if (uwRet != LOS_OK) {
        dprintf("free failed,a leak mem is born!!!!\n");
    }
}

VOID MEM_INIT(VOID)
{
    UINT32 uwRet;
    uwRet = LOS_MemInit(g_pool, TEST_MEM_SIZE);
    if (uwRet != LOS_OK) {
        dprintf("init failed, mem testcase would be failed!!!!\n");
    }
}

VOID MEM_FREE(VOID)
{
    VOID *p1 = NULL;
    p1 = memset(g_pool, 0, TEST_MEM_SIZE);
    ICUNIT_ASSERT_EQUAL_VOID(p1, g_pool, p1);
#ifdef LOSCFG_MEM_MUL_POOL
    UINT32 uwRet;
    uwRet = LOS_MemDeInit(g_pool);
    if (uwRet != LOS_OK) {
        dprintf("Deinit failed , mem testcase may be wrong!!!\n");
    }
#endif
}

VOID MEM_START_GetMemInfo(VOID *pool)
{
    UINT32 uwRet;

    uwRet = LOS_MemInfoGet(pool, &g_memStartStatus);
    ICUNIT_ASSERT_EQUAL_VOID(uwRet, LOS_OK, uwRet);

    return;
}

VOID MEM_END_GetMemInfo(VOID *pool)
{
    UINT32 uwRet;

    uwRet = LOS_MemInfoGet(pool, &g_memEndStatus);
    ICUNIT_ASSERT_EQUAL_VOID(uwRet, LOS_OK, uwRet);

    return;
}

VOID MEM_INFO_CHECK(VOID)
{
    UINT32 ret = LOS_OK;

    if ((g_memStartStatus.uwTotalUsedSize != g_memEndStatus.uwTotalUsedSize) || \
        (g_memStartStatus.uwTotalFreeSize != g_memEndStatus.uwTotalFreeSize)) {
        dprintf("start totalUsedSize:0x%x. \n", g_memStartStatus.uwTotalUsedSize);
        dprintf("end totalUsedSize:0x%x. \n", g_memEndStatus.uwTotalUsedSize);
        dprintf("start totalFreeSize:0x%x. \n", g_memStartStatus.uwTotalFreeSize);
        dprintf("end totalFreeSize:0x%x. \n", g_memEndStatus.uwTotalFreeSize);

        ret = LOS_NOK;
    }

    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    return;
}

/* calculate 2��exp�η� */
UINT32 CalPow(UINT32 exp)
{
    UINT32 pw = 1;
    pw <<= exp;
    if (pw < sizeof(VOID*)) {
        pw = sizeof(VOID*);
    }
    return pw;
}

/**
This API is to find the highest one bit of TEST_MEM_SIZE that is set and return the bit index
**/
UINT32 TestMemPoolSizeHighBitGet(UINT32 bitmap)
{
    return LOS_HighBitGet(bitmap);
}

VOID ItSuiteLosMem(VOID)
{
    ItLosMem001();
    ItLosMem002();
#ifdef LOSCFG_KERNEL_MEM_BESTFIT
    ItLosMem003(); // 0 LOS_MemTotalUsedGet
    ItLosMem004(); // 0 LOS_MemLastUsedGet
#ifndef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
    ItLosMem005(); // 0 LOS_MemUsedBlksGet
    ItLosMem006(); // 0 LOS_MemTaskIdGet
#endif
    ItLosMem007(); // 0  LOS_MemFreeBlksGet
#endif
    ItLosMem008(); // 0 LOS_MemIntegrityCheck
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
    ItLosMem009(); // 0 LOS_MemCheckLevelGet LOS_MemCheckLevelSet
    ItLosMem010(); // 0 LOS_MemNodeSizeCheck
#endif
#ifdef LOSCFG_KERNEL_MEM_BESTFIT
    ItLosMem011(); // 0 LOS_MemInfoGet  // bestfitlittle: struct size is different with tlsf
    ItLosMem012(); // 0  LOS_MemPoolSizeGet
    ItLosMem013(); //  0 LOS_MemFreeNodeShow
#endif
#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
    ItLosMem014();
#endif
#ifdef LOS_MEM_SLAB
    ItLosMemSlab001();
    ItLosMemSlab002();
    ItLosMemSlab003();
    ItLosMemSlab004();
    ItLosMemSlab005();
#endif
#ifdef LOS_MEM_SLINK
#ifdef LOSCFG_MEM_MUL_MODULE
    ItLosMemSlink001();
#endif
    ItLosMemSlink001();
#endif
}
