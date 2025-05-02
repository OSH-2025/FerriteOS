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

static VOID HwiF01(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    g_testCount++;

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret, intSave;
    HWI_PRIOR_T hwiPrio = 3; // pri:3
    HWI_MODE_T mode    = 0;

    g_testCount = 0;
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);

    ret = LOS_HwiCreate(HWI_NUM_TEST, hwiPrio, mode, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_HwiEnable(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);

    intSave = LOS_IntLock();

    TestHwiTrigger(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(g_testCount, 0, g_testCount, EXIT1);

    LOS_IntRestore(intSave);
    (VOID)LOS_TaskDelay(5); /* wait 5 ticks for hwi trigger */
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);

EXIT1:
    LOS_IntRestore(intSave);
EXIT:
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
EXIT2:
    (VOID)LOS_HwiDelete(HWI_NUM_TEST, NULL);
    return LOS_OK;
}

/**
* @ingroup TEST_HWI
* @par TestCase_Number
* ItLosHwi001
* @par TestCase_TestCase_Type
* Function test
* @brief Test whether the interrupt responds after the interrupt is disabled..
* @par TestCase_Pretreatment_Condition
* System supports interrupt.
* @par TestCase_Test_Steps
* step1: create and enable hwi;\n
* step2: disable interrupt, and then trigger the hwi, and then enable hwi;\n
* @par TestCase_Expected_Result
* 1. The interrupt does not respond after the interrupt is disabled;\n
* 2. The interrupt responds only after the interrupt is recovered.\n
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosHwi001(VOID)
{
    TEST_ADD_CASE("ItLosHwi001", TestCase, TEST_LOS, TEST_HWI, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
