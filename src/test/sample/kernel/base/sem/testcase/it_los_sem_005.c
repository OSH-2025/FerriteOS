/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sem Test Case
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
 
#include "it_los_sem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID TaskF01(VOID)
{
    UINT32 ret;

    g_testCount++;

    ret = LOS_SemPend(g_usSemID, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    g_testCount++;

    return;
}

static VOID TaskF02(VOID)
{
    UINT32 ret;

    ICUNIT_TRACK_EQUAL(g_testCount, 1, g_testCount);

    LOS_TaskLock();

    ret = LOS_SemPost(g_usSemID);
    ICUNIT_TRACK_EQUAL(ret, LOS_OK, ret);
#ifndef LOSCFG_KERNEL_SMP
    ICUNIT_TRACK_EQUAL(g_testCount, 1, g_testCount);
#endif

    LOS_TaskUnlock();
#ifndef LOSCFG_KERNEL_SMP
    ICUNIT_TRACK_EQUAL(g_testCount, 1, g_testCount);
#endif
    g_testCount++;

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S testTask = {0};
    testTask.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    testTask.pcName = "SemTsk005A";
    testTask.uwStackSize = TASK_STACK_SIZE_TEST;
    testTask.usTaskPrio = TASK_PRIO_TEST - 1;
    testTask.uwResved = LOS_TASK_STATUS_DETACHED;

    g_testCount = 0;

    ret = LOS_SemCreate(0, &g_usSemID);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_TaskCreate(&g_testTaskID01, &testTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    TestExtraTaskDelay(TEST_TASKDELAY_2TICK);

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT1);

    testTask.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    testTask.pcName = "SemTsk005B";
    testTask.usTaskPrio = TASK_PRIO_TEST - 0x2;  // TASK_PRIO_TEST - 2 has higher priority than TASK_PRIO_TEST

    ret = LOS_TaskCreate(&g_testTaskID02, &testTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    TestExtraTaskDelay(TEST_TASKDELAY_2TICK);

    ICUNIT_GOTO_EQUAL(g_testCount, 0x3, g_testCount, EXIT2);
EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT:
    ret = LOS_SemDelete(g_usSemID);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_SEM
* @par TestCase_Number
* ItLosSem005
* @par TestCase_TestCase_Type
* Function test
* @brief Test interface LOS_SemCreate/LOS_SemPost.
* @par TestCase_Pretreatment_Condition
* System-supported semaphore.
* @par TestCase_Test_Steps
* step1: create sem(init value:0);\n
* step2: create high-pri task1 and task2(more high-pri), then pend sem in task1;\n
* step3: post sem in task2;\n
* @par TestCase_Expected_Result
* task1 wait for sem, until sem is post in task2;\n
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosSem005(VOID)
{
    TEST_ADD_CASE("ItLosSem005", TestCase, TEST_LOS, TEST_SEM, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */