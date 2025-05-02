/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Swtmr Test Case
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
 
#include "it_los_swtmr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID SwtmrF01(VOID)
{
    LOS_AtomicInc(&g_testCount);
}

static VOID TaskF01(VOID)
{
    UINT32 ret;

    ret = LOS_SwtmrCreate(g_testPeriod, LOS_SWTMR_MODE_ONCE, (SWTMR_PROC_FUNC)SwtmrF01, &g_usSwTmrID, 0);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    ret = LOS_SwtmrStart(g_usSwTmrID);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    return;
}

static UINT32 TestCase(VOID)
{
    TSK_INIT_PARAM_S testTask;
    UINT32 ret;

    g_testCount = 0;
    g_testPeriod = 10; // Swtmr interval is 10.

    TEST_TASK_PARAM_INIT_AFFI(testTask, "smp_swtmr_tsk001", TaskF01,
        TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
    ret = LOS_TaskCreate(&g_testTaskID01, &testTask);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    (VOID)LOS_TaskDelay(g_testPeriod + 1);

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT);

    /* do stop and delete */
    ret = LOS_SwtmrStop(g_usSwTmrID);
    ICUNIT_GOTO_NOT_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_SwtmrDelete(g_usSwTmrID);
    ICUNIT_GOTO_NOT_EQUAL(ret, LOS_OK, ret, EXIT);

EXIT:
    (VOID)LOS_SwtmrDelete(g_usSwTmrID);
    (VOID)LOS_TaskDelete(g_testTaskID01);

    return LOS_OK;
}

/**
 * @ingroup TEST_SWTMR
 * @par TestCase_Number
 * ItSmpLosSwtmr001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Swtmr API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * step1: Create a once swtmr in the running task of the current core and start the swtmr.\n
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItSmpLosSwtmr001(VOID)
{
    TEST_ADD_CASE("ItSmpLosSwtmr001", TestCase, TEST_LOS, TEST_SWTMR, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
