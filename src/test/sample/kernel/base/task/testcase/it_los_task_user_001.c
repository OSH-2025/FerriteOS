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
    g_testCount++;

    return;
}

static VOID TaskF02(VOID)
{
    g_testCount++;

    return;
}

static UINT32 Testcase(VOID)
{
    UINT32 ret;
    VOID *topOfStack = NULL;
    TSK_INIT_PARAM_S task1 = {0};
    TSK_INIT_PARAM_S task2 = {0};
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task1.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1.pcName       = "TskUser001A";
    task1.usTaskPrio   = TASK_PRIO_TEST - 1;

    task2.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    task2.uwStackSize  = TASK_STACK_SIZE_TEST;
    task2.pcName       = "TskUser001B";
    task2.usTaskPrio   = TASK_PRIO_TEST - 2; // TASK_PRIO_TEST - 2 has higher priority than TASK_PRIO_TEST

    g_testCount = 0;

    topOfStack = LOS_MemAllocAlign(OS_SYS_MEM_ADDR, TASK_STACK_SIZE_TEST, OS_TASK_STACK_ADDR_ALIGN);
    ICUNIT_ASSERT_NOT_EQUAL(topOfStack, NULL, 0);

    ret = LOS_TaskCreateStatic(&g_testTaskID01, &task1, topOfStack);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_TaskCreate(&g_testTaskID02, &task2);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT); // value 2
    ICUNIT_GOTO_NOT_EQUAL(topOfStack, NULL, topOfStack, EXIT);

EXIT:
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, topOfStack);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_TASK
 * @par TestCase_Number
 * ItLosTaskUser001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Task API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * 1.Dynamic memalloc topOfStack and size is equal to TASK_STACK_SIZE_TEST;
 * 2.user define the topOfStack.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */
 
VOID ItLosTaskUser001(VOID)
{
    TEST_ADD_CASE("ItLosTaskUser001", Testcase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
