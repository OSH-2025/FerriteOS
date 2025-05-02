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
#ifdef LOSCFG_KERNEL_SMP
static VOID TaskF01(VOID)
{
    UINT32 ret;

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 0, g_testCount);
    LOS_AtomicInc(&g_testCount);

    ret = LOS_EventRead(&g_pevent, 0x11, LOS_WAITMODE_AND, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, g_pevent.uwEventID, ret);
    ICUNIT_ASSERT_EQUAL_VOID(g_pevent.uwEventID, 0x11, g_pevent.uwEventID);

    LOS_AtomicInc(&g_testCount);
    TestDumpCpuid();

    return;
}


static UINT32 TestCase(VOID)
{
    UINT32 ret, otherCpuId;
    TSK_INIT_PARAM_S stTestTask;

    g_testCount = 0;

    ret = LOS_EventInit(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    otherCpuId = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    TEST_TASK_PARAM_INIT_AFFI(stTestTask, "ItSmpLosEvent001_task1", TaskF01,
        TASK_PRIO_TEST - 1, CPUID_TO_AFFI_MASK(otherCpuId)); // other cpu
    ret = LOS_TaskCreate(&g_testTaskID01, &stTestTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    TestAssertBusyTaskDelay(10, 1); // delay max 10 ticks wait for flag equal 1

    TestBusyTaskDelay(2); // delay 2 ticks

    ret = OS_TCB_FROM_TID(g_testTaskID01)->taskStatus;
    ICUNIT_GOTO_NOT_EQUAL((ret & OS_TASK_STATUS_PEND), 0, ret, EXIT0);

    ret = LOS_EventDestroy(&g_pevent);
    ICUNIT_GOTO_EQUAL(ret, LOS_ERRNO_EVENT_SHOULD_NOT_DESTORY, ret, EXIT0);
    ICUNIT_GOTO_EQUAL(g_testCount, 1, g_testCount, EXIT0);
EXIT0:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT:
    ret = LOS_EventDestroy(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_EVENT
* @par TestCase_Number
* ItSmpLosEvent001
* @par TestCase_TestCase_Type
* Function test
* @brief Test destroy non-empty events.
* @par TestCase_Pretreatment_Condition
* System Support Events.
* @par TestCase_Test_Steps
* step1: init event;\n
* step2: create high-pri task1, then destroy event;\n
* step3: task1 read event;\n
* @par TestCase_Expected_Result
* destroy event failed.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItSmpLosEvent001(VOID)
{
    TEST_ADD_CASE("ItSmpLosEvent001", TestCase, TEST_LOS, TEST_EVENT, TEST_LEVEL0, TEST_FUNCTION);
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
