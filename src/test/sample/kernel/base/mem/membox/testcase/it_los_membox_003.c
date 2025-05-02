/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Membox Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "it_los_membox.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_KERNEL_MEMBOX_STATIC
static UINT32 TestCase(VOID)
{
    int i;
    UINT32 ret, count, note1, note2;
    VOID *p0 = NULL;
    VOID *p1 = NULL;
    VOID *p2 = NULL;

    MEMBOX_START();
    g_blkSize = 0x200;
    MEMBOX_INIT();
    count  = MBOX_BLK_NUM / 2; /* count 2 is half block number that membox can alloc */
    note1 = count - 2; /* note1 2 */
    note2 = count - 1;

    for (i = 0; i < count; i++) {
        p0 = LOS_MemboxAlloc(g_boxAddr);
        ICUNIT_GOTO_NOT_EQUAL(p0, NULL, p0, EXIT);
        if (i == note1) {
            p1 = p0;
        }
        if (i == note2) {
            p2 = p0;
        }
    }

    ICUNIT_GOTO_EQUAL(((UINTPTR)p2 - (UINTPTR)p1), (LOS_MEMBOX_ALLIGNED(g_blkSize) + OS_MEMBOX_NODE_HEAD_SIZE), \
        ((UINTPTR)p2 - (UINTPTR)p1), EXIT);

    ret = LOS_MemboxFree(g_boxAddr, p1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_MemboxFree(g_boxAddr, p2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    p0 = LOS_MemboxAlloc(g_boxAddr);
    ICUNIT_GOTO_EQUAL(p0, p2, p0, EXIT);

    p0 = LOS_MemboxAlloc(g_boxAddr);
    ICUNIT_GOTO_EQUAL(p0, p1, p0, EXIT);

EXIT:
    MEMBOX_FREE();
    MEMBOX_END();
    return LOS_OK;
}

/**
 * @ingroup TEST_MEM
 * @par TestCase_Number
 * ItLosMembox003
 * @par TestCase_TestCase_Type
 * Function test
 * @brief After the memory in the Membox is free, next alloc memory will return it first.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: Alloc two memory in middle of the membox;free it;alloc it again.
 * @par TestCase_Expected_Result
 * 1. the last free memory will alloc first.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMembox003(VOID)
{
    TEST_ADD_CASE("ItLosMembox003", TestCase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */