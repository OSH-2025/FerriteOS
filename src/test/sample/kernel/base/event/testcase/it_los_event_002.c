/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Event Test Case
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
 
#include "it_los_event.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID TaskF01(VOID)
{
    UINT32 ret;

    g_testCount++;

    ret = LOS_EventRead(&g_pevent, 0x1, LOS_WAITMODE_AND, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, 0x1, ret);
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 3, g_testCount); // g_testCount equal 3

    g_testCount++;

    ret = LOS_EventRead(&g_pevent, 0x11, LOS_WAITMODE_AND, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, g_pevent.uwEventID, ret);
    ICUNIT_ASSERT_EQUAL_VOID(g_pevent.uwEventID, 0x11, g_pevent.uwEventID);

    g_testCount++;

    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;
    memset(&task1, 0, sizeof(TSK_INIT_PARAM_S));
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task1.pcName       = "EventTsk02";
    task1.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1.usTaskPrio   = TASK_PRIO_TEST - 2; // TASK_PRIO_TEST - 2 has higher priority than TASK_PRIO_TEST
    task1.uwResved     = LOS_TASK_STATUS_DETACHED;

    g_testCount = 0;

    ret = LOS_EventInit(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_EventRead(&g_pevent, 0x1, LOS_WAITMODE_AND, 1); // timeout 1 tick
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_EVENT_READ_TIMEOUT, ret, EXIT);

    ret = LOS_EventRead(&g_pevent, 0x1, LOS_WAITMODE_AND, 1); // timeout 1 tick
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_EVENT_READ_TIMEOUT, ret, EXIT);

    ret = LOS_EventRead(&g_pevent, 0x1, LOS_WAITMODE_AND, 2); // timeout 2 tick
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_EVENT_READ_TIMEOUT, ret, EXIT);

    ret = LOS_TaskCreate(&g_testTaskID01, &task1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
    TestExtraTaskDelay(2); // delay 2 ticks

    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT1);

    g_testCount++;

    ret = LOS_EventWrite(&g_pevent, 0x10);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT1);  // g_testCount equal 2

    g_testCount++;

    ret = LOS_EventWrite(&g_pevent, 0x1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
    TestExtraTaskDelay(2); // delay 2 ticks

    ICUNIT_GOTO_EQUAL(g_testCount, 5, g_testCount, EXIT1); // g_testCount equal 5

EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT:
    ret = LOS_EventDestroy(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_EVENT
* @par TestCase_Number
* ItLosEvent002
* @par TestCase_TestCase_Type
* Function test
* @brief Test event read and write.
* @par TestCase_Pretreatment_Condition
* System Support Events.
* @par TestCase_Test_Steps
* step1: init event;\n
* step2: create high-pri task1 and write event;\n
* step3: task1 read event ;\n
* @par TestCase_Expected_Result
* task1 read event successfully after testcacse task writes the event.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosEvent002(VOID)
{
    TEST_ADD_CASE("ItLosEvent002", TestCase, TEST_LOS, TEST_EVENT, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
