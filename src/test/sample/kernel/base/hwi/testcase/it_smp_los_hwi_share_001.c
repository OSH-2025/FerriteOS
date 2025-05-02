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

#if defined(LOSCFG_SHARED_IRQ) && defined(LOSCFG_KERNEL_SMP)
static  HWI_IRQ_PARAM_S  dev1, dev2;

static VOID HWIF01(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    LOS_AtomicInc(&g_testCount);
}

static VOID HWIF02(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    LOS_AtomicAdd(&g_testCount, 0xff);
}

static VOID TaskF01(VOID)
{
    UINT32 ret;

    TestDumpCpuid();
    ret = LOS_HwiEnable(HWI_NUM_TEST);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    TestHwiTrigger(HWI_NUM_TEST);

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    HWI_PRIOR_T  hwiPrio = 3; //  3: hwi pri
    TSK_INIT_PARAM_S stTetask;

    g_testCount = 0;

    dev1.pDevId = (VOID *)1;
    dev1.swIrq = HWI_NUM_TEST;
    ret = LOS_HwiCreate(HWI_NUM_TEST, hwiPrio, IRQF_SHARED, (HWI_PROC_FUNC)HWIF01, &dev1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dev2.pDevId = (VOID *)2; // 2: ded id
    dev2.swIrq = HWI_NUM_TEST;
    ret = LOS_HwiCreate(HWI_NUM_TEST, hwiPrio, IRQF_SHARED, (HWI_PROC_FUNC)HWIF02, &dev2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    TestDumpCpuid();

    TEST_TASK_PARAM_INIT_AFFI(stTetask, "ItSmpLosHwiShare001", TaskF01,
                              TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
    ret = LOS_TaskCreate(&g_testTaskID01, &stTetask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    (VOID)LOS_TaskDelay(10); // delay 10

    ICUNIT_GOTO_EQUAL(g_testCount, 0x100, g_testCount, EXIT2);
EXIT2:
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT1:
    ret = LOS_HwiDelete(HWI_NUM_TEST, &dev2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
EXIT:
    ret = LOS_HwiDelete(HWI_NUM_TEST, &dev1);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_HWI
* @par TestCase_Number
* ItSmpLosHwiShare001
* @par TestCase_TestCase_Type
* Function test
* @brief Test Sharing Interrupt.
* @par TestCase_Pretreatment_Condition
* The system supports shared interrupts.
* @par TestCase_Test_Steps
* step1: create hwi which is binded two devices, and create high-pri task1;\n
* step2: enable hwi and then trigger the hwi;\n
* @par TestCase_Expected_Result
* Both devices successfully responded to the interrupt.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItSmpLosHwiShare001(VOID)
{
    TEST_ADD_CASE("ItSmpLosHwiShare001", TestCase, TEST_LOS, TEST_HWI, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
