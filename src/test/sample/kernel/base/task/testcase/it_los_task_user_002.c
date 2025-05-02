/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Test Case
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
 
#include "it_los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_TASK_STATIC_ALLOCATION
static VOID TaskF01(VOID)
{
    UINT32 ret;

    g_testCount++;

    ret = LOS_TaskSuspend(g_testTaskID01);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 2, g_testCount); // g_testCount assert equal 2
    g_testCount++;
}

static UINT32 Testcase(VOID)
{
    UINT32 ret;
    VOID *topOfStack = NULL;
    TSK_INIT_PARAM_S task = {0};
    task.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task.usTaskPrio   = TASK_PRIO_TEST - 5; // TASK_PRIO_TEST - 5 has higher priority than TASK_PRIO_TEST
    task.pcName       = "TskUser002A";
    task.uwStackSize  = TASK_STACK_SIZE_TEST;
#ifdef LOSCFG_KERNEL_SMP
    task.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
#if LOSCFG_COMPAT_POSIX
    task.uwResved = LOS_TASK_STATUS_DETACHED;
#endif

    g_testCount = 0;

    topOfStack = LOS_MemAllocAlign(OS_SYS_MEM_ADDR, TASK_STACK_SIZE_TEST, OS_TASK_STACK_ADDR_ALIGN);
    ICUNIT_ASSERT_NOT_EQUAL(topOfStack, NULL, topOfStack);

    ret = LOS_TaskCreateStatic(&g_testTaskID01, &task, topOfStack);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    g_testCount++;

    ret = LOS_TaskResume(g_testTaskID01);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    g_testCount++;

    ICUNIT_GOTO_EQUAL(g_testCount, 4, g_testCount, EXIT1); // g_testCount assert equal 4

EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT:
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, topOfStack);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_TASK
 * @par TestCase_Number
 * ItLosTaskUser002
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_TaskResume API test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * 1.Create taskA, and suspend it.
 * 2.Resume taskA in testcase.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItLosTaskUser002(VOID)
{
    TEST_ADD_CASE("ItLosTaskUser002", Testcase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */