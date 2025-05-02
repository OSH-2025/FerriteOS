/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Hwi Test Case
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
 
#include "it_los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID HwiF03(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST1);
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 2, g_testCount); // g_testCount euqal 2.
    g_testCount++;

    return;
}

static VOID HwiF02(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST3);
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 1, g_testCount);
    g_testCount++;

    return;
}

static VOID HwiF01(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    g_testCount++;

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32       ret;
    HWI_PRIOR_T  hwiPrio = 1;
    HWI_MODE_T   mode    = 0;

    g_testCount = 0;

    ret = LOS_HwiCreate(HWI_NUM_TEST, hwiPrio, mode, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_HwiEnable(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    ret = LOS_HwiCreate(HWI_NUM_TEST3, hwiPrio, mode, (HWI_PROC_FUNC)HwiF02, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    ret = LOS_HwiEnable(HWI_NUM_TEST3);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    ret = LOS_HwiCreate(HWI_NUM_TEST1, hwiPrio, mode, (HWI_PROC_FUNC)HwiF03, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    ret = LOS_HwiEnable(HWI_NUM_TEST1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT3);

    TestHwiTrigger(HWI_NUM_TEST);
    TestHwiTrigger(HWI_NUM_TEST3);
    TestHwiTrigger(HWI_NUM_TEST1);

    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT4); // g_testCount equal 3.

EXIT4:
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST3);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST1);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
EXIT3:
    ret = LOS_HwiDelete(HWI_NUM_TEST1, NULL);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);
EXIT2:
    ret = LOS_HwiDelete(HWI_NUM_TEST3, NULL);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
EXIT1:
    ret = LOS_HwiDelete(HWI_NUM_TEST, NULL);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_HWI
* @par TestCase_Number
* ItLosHwi002
* @par TestCase_TestCase_Type
* Function test
* @brief Test the interrupt response sequence of different priorities.
* @par TestCase_Pretreatment_Condition
* System supports interrupt.
* @par TestCase_Test_Steps
* step1: create three hwi;\n
* step2: trigger these three hwi at the same time;\n
* @par TestCase_Expected_Result
* The system preferentially responds to high-priority interrupts.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosHwi002(VOID)
{
    TEST_ADD_CASE("ItLosHwi002", TestCase, TEST_LOS, TEST_HWI, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
