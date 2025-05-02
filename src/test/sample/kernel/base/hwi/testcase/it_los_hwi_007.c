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
#if defined(LOSCFG_ARCH_INTERRUPT_PREEMPTION) && !defined(LOSCFG_ARCH_RISCV)
static HWI_IRQ_PARAM_S g_stHwiArg;

static VOID HwiF01(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST3);
    g_testCount++;

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret,  intSave;
    HWI_PRIOR_T hwiPrio = 0;

    g_testCount = 0;

#ifdef LOSCFG_SHARED_IRQ
    HWI_MODE_T mode = IRQF_SHARED;
#else
    HWI_MODE_T mode = 0;
#endif

    g_stHwiArg.pDevId = (VOID *)1;
    g_stHwiArg.swIrq = HWI_NUM_TEST3;
    ret = LOS_HwiCreate(HWI_NUM_TEST3, hwiPrio, mode, (HWI_PROC_FUNC)HwiF01, &g_stHwiArg);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_HwiEnable(HWI_NUM_TEST3);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    TestHwiTrigger(HWI_NUM_TEST3);
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT2);

    intSave = LOS_IntLock();

    TestHwiTrigger(HWI_NUM_TEST3);
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT3);

    ret = LOS_HwiDelete(HWI_NUM_TEST3, &g_stHwiArg);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT3);

    LOS_IntRestore(intSave);

    TestHwiTrigger(HWI_NUM_TEST3);
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT3);

EXIT3:
    LOS_IntRestore(intSave);
EXIT2:
    LOS_HwiClear(HWI_NUM_TEST3);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST3);
EXIT1:
    ret = LOS_HwiDelete(HWI_NUM_TEST3, &g_stHwiArg);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_HWI
* @par TestCase_Number
* ItLosHwi007
* @par TestCase_TestCase_Type
* Function test
* @brief Disable Interrupt Test.
* @par TestCase_Pretreatment_Condition
* System supports interrupt.
* @par TestCase_Test_Steps
* step1: create and enable hwi, then trigger hwi;\n
* step2: disable interrupt, and then trigger the hwi;\n
* step3: trigger hwi after delete the hwi;\n
* @par TestCase_Expected_Result
* 1. system cannot respone interrupt after disable interrupt;\n
* 2. system cannot respone interrupt after delete interrupt;\n
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosHwi007(VOID)
{
    TEST_ADD_CASE("ItLosHwi007", TestCase, TEST_LOS, TEST_HWI, TEST_LEVEL0, TEST_FUNCTION);
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
