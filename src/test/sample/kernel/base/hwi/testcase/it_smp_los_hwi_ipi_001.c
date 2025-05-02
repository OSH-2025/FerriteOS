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

#ifdef LOSCFG_KERNEL_SMP
static UINT32  targetCpuid;

static VOID HwiF01(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST_IPI);

    LOS_AtomicInc(&g_testCount);
    TestDumpCpuid();
    ICUNIT_ASSERT_EQUAL_VOID(ArchCurrCpuid(), targetCpuid, ArchCurrCpuid());
}

static VOID  TaskF01(VOID)
{
    PRINT_DEBUG("targetCpuid = %d\n", targetCpuid);

    HalIrqSendIpi(targetCpuid + 1, HWI_NUM_TEST_IPI);

    return;
}

static UINT32 TestCase(VOID)
{
    TSK_INIT_PARAM_S testTask;
    UINT32 ret, testid, otherCpuid;

    g_testCount = 0;

    targetCpuid  = 0;

    ret = LOS_HwiCreate(HWI_NUM_TEST_IPI, 1, 0, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_HwiEnable(HWI_NUM_TEST_IPI);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    otherCpuid = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    targetCpuid = otherCpuid;  // other cpu

    TEST_TASK_PARAM_INIT_AFFI(testTask, "ItSmpLosHwiIpi001", TaskF01,
                              TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(otherCpuid));
    ret = LOS_TaskCreate(&testid, &testTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    (VOID)LOS_TaskDelay(10); // delay 10 ticks.

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);
EXIT:
    ret = LOS_HwiDelete(HWI_NUM_TEST_IPI, NULL);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_HWI
* @par TestCase_Number
* ItSmpLosHwiIpi001
* @par TestCase_TestCase_Type
* Function test
* @brief Triggering Inter-Core Interrupt Test.
* @par TestCase_Pretreatment_Condition
* The system supports inter-core interrupts.
* @par TestCase_Test_Steps
* step1: core0 create and enable hwi, then create high-pri task1 which  is binded to core1;
* step2: task1 trigger to other core;
* @par TestCase_Expected_Result
* Other core response the hwi successfully.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItSmpLosHwiIpi001(VOID)
{
    TEST_ADD_CASE("ItSmpLosHwiIpi001", TestCase, TEST_LOS, TEST_HWI, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
