/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: MISC Test Case
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

#include "it_los_misc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_ARCH_FPU_ENABLE
#define COUNTOF(a) (sizeof(a) / sizeof((a)[0]))

static int FloatThread(FLOAT *arg)
{
    FLOAT *val = arg;
    UINT32 i, j;

    FLOAT a[16];

    /* do a bunch of work with floating point to test context switching */
    a[0] = *val;
    for (i = 1; i < COUNTOF(a); i++) {
        a[i] = a[i - 1] * 1.01f;
    }

    for (i = 0; i < 1000000; i++) { // run 1000000 times.
        a[0] += 0.001f;
        for (j = 1; j < COUNTOF(a); j++) {
            a[j] += a[j - 1] * 0.00001f;
        }
    }

    *val = a[COUNTOF(a) - 1];

    return LOS_OK;
}

static UINT32 TestCase(VOID)
{
    /* test lazy fpu load on separate thread */
    FLOAT val[8];
    UINT32  ret;
    UINT32 taskid;
    UINT32 i;
    TSK_INIT_PARAM_S task1 = {0};
    FLOAT val_result[8] = {
        47822.828125, 69934.515625, 92046.062500, 114157.406250, \
        136270.421875, 158378.812500, 180492.656250, 202600.265625
    };

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)FloatThread;
    task1.uwStackSize  = TASK_STACK_SIZE_TEST * 5; // 5 is coefficients
    task1.pcName       = "Tsk_Float";
    task1.usTaskPrio   = 10; // taskPrio is 10.
    task1.uwResved     = LOS_TASK_STATUS_DETACHED;

    for (i = 0; i < 8; i++) { // run 8 times.
        val[i] = i;
#ifndef LOSCFG_OBSOLETE_API
        task1.pArgs = (VOID *)&val[i];
#else
        task1.auwArgs[0] = (UINTPTR)&val[i];
#endif
        ret = LOS_TaskCreate(&taskid, &task1);
        ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    }

    TestExtraTaskDelay(5 * LOSCFG_BASE_CORE_TICK_PER_SECOND); // 5 sec for smp , float_thread may not bu finished.
    for (i = 0; i < 8; i++) { // run 8 times.
		/* float precision is 0.000001 */
        ICUNIT_ASSERT_WITHIN_EQUAL(val_result[i], val_result[i] - 0.000001, val_result[i] + 0.000001, val_result[i]);
    }

    return LOS_OK;
}
#endif

/**
 * @ingroup TEST_MISC
 * @par TestCase_Number
 * ItLosFloat001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Test that floating-point calculation is correct.
 * @par TestCase_Pretreatment_Condition
 * CPU support Floating Point Computing
 * @par TestCase_Test_Steps
 * step1: Test Floating Point Addition
 * @par TestCase_Expected_Result
 * 1. Each addition returns the correct result.
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosFloat001(VOID)
{
#ifdef LOSCFG_ARCH_FPU_ENABLE
    TEST_ADD_CASE("ItLosFloat001", TestCase, TEST_LOS, TEST_MISC, TEST_LEVEL0, TEST_FUNCTION);
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
