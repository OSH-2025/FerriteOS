/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Memory Test Case
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

#include "los_list.h"
#include "it_los_misc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 Testcase(VOID)
{
    BOOL result;
    TestCB *dListNodeTemp;
    TestCB dListNode01, dListNode02, dListNode03, dListNode04, dListNode05, dListNode06;

    dListNode01.uwValue = 1; // 1: value
    dListNode02.uwValue = 2; // 2: value
    dListNode03.uwValue = 3; // 3: value
    dListNode04.uwValue = 4; // 4: value
    dListNode05.uwValue = 5; // 5: value
    dListNode06.uwValue = 6; // 6: value

    g_testCount = 0;

    LOS_ListInit(&g_testList);
    ICUNIT_ASSERT_EQUAL(g_testList.pstNext, &g_testList, g_testList.pstNext);

    LOS_ListAdd(&g_testList, &dListNode01.stTestList);
    LOS_ListTailInsert(&g_testList, &dListNode02.stTestList);
    LOS_ListTailInsert(&g_testList, &dListNode03.stTestList);

    dListNodeTemp = (TestCB *)LOS_DL_LIST_ENTRY(g_testList.pstNext->pstNext->pstNext, TestCB, stTestList);
    ICUNIT_ASSERT_EQUAL(dListNodeTemp->uwValue, 3, dListNodeTemp->uwValue); // 3: value

    dListNodeTemp = (TestCB *)LOS_DL_LIST_FIRST(g_testList.pstNext);
    ICUNIT_ASSERT_EQUAL(dListNodeTemp->uwValue, 2, dListNodeTemp->uwValue); // 2: value

    dListNodeTemp = (TestCB *)LOS_DL_LIST_LAST(g_testList.pstNext->pstNext);
    ICUNIT_ASSERT_EQUAL(dListNodeTemp->uwValue, 1, dListNodeTemp->uwValue);

    LOS_ListTailInsert(&g_testList, &dListNode05.stTestList);

    // Add node04 between node03 and node05.
    LOS_ListHeadInsert(&dListNode03.stTestList, &dListNode04.stTestList);
    dListNodeTemp = (TestCB *)LOS_DL_LIST_ENTRY(g_testList.pstNext->pstNext->pstNext->pstNext, TestCB, stTestList);
    ICUNIT_ASSERT_EQUAL(dListNodeTemp->uwValue, 4, dListNodeTemp->uwValue); // 4: value

    LOS_ListTailInsert(&g_testList, &dListNode06.stTestList);
    dListNodeTemp = (TestCB *)LOS_DL_LIST_ENTRY(&dListNode06.stTestList, TestCB, stTestList);
    ICUNIT_ASSERT_EQUAL(dListNodeTemp->uwValue, 6, dListNodeTemp->uwValue); // 6: value

    LOS_ListDelete((LOS_DL_LIST*)(&dListNode06));
    // After node06 is deleted, node05 is the last node.
    dListNodeTemp = (TestCB *)LOS_DL_LIST_FIRST(&dListNode05.stTestList);
    ICUNIT_ASSERT_NOT_EQUAL(dListNodeTemp->uwValue, 6, dListNodeTemp->uwValue); // 6: value

    result = LOS_ListEmpty(&g_testList);
    ICUNIT_ASSERT_EQUAL(result, FALSE, result);

    return LOS_OK;
}

/**
 * @ingroup los_list test
 * @par TestCase_Number
 * ItLosList001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_ListInit/LOS_ListAdd/LOS_ListTailInsert/LOS_ListHeadInsert/LOS_ListDelete/LOS_ListEmpty and
 * @ LOS_DL_LIST_ENTRY、LOS_DL_LIST_FIRST、LOS_DL_LIST_LAST test.
 * @par TestCase_Pretreatment_Condition
 * NA.
 * @par TestCase_Test_Steps
 * step1: init double double link list;\n
 * step2: add node to double link list, and then delete node from double link list;\n
 * @par TestCase_Expected_Result
 * 1.init double double link list successfully;\n
 * 2.add and delete node from double link list successfully;\n
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosList001(VOID)
{
    TEST_ADD_CASE("ItLosList001", Testcase, TEST_LOS, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
