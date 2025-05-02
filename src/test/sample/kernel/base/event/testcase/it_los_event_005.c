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

static VOID HwiF01(VOID)
{
    UINT32 ret;

    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);

    LOS_AtomicInc(&g_testCount);

    ret = LOS_EventWrite(&g_pevent, 0x11);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    LOS_AtomicInc(&g_testCount);
}

static VOID TaskF01(VOID)
{
    UINT32 ret;

    LOS_AtomicInc(&g_testCount);

    ret = LOS_EventRead(&g_pevent, 0x11, LOS_WAITMODE_AND, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, g_pevent.uwEventID, ret);
    ICUNIT_ASSERT_EQUAL_VOID(ret, 0x11, ret);

#if !defined(LOSCFG_KERNEL_SMP)
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 5, g_testCount); // g_testCount equal 5
#endif
    LOS_AtomicInc(&g_testCount);
}

static VOID TaskF02(VOID)
{
    UINT32 ret;

    LOS_AtomicInc(&g_testCount);

    ret = LOS_EventRead(&g_pevent, 0x11, LOS_WAITMODE_AND, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, g_pevent.uwEventID, ret);
    ICUNIT_ASSERT_EQUAL_VOID(ret, 0x11, ret);

#if !defined(LOSCFG_KERNEL_SMP)
    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 6, g_testCount); // g_testCount equal 6
#endif
    LOS_AtomicInc(&g_testCount);
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;

    memset(&task1, 0, sizeof(TSK_INIT_PARAM_S));
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task1.pcName       = "EvtTsk05A";
    task1.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1.usTaskPrio   = TASK_PRIO_TEST - 2; // TASK_PRIO_TEST - 2 has higher priority than TASK_PRIO_TEST
    task1.uwResved     = LOS_TASK_STATUS_DETACHED;

    g_testCount = 0;

    ret = LOS_EventInit(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    ret = LOS_TaskCreate(&g_testTaskID01, &task1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT0);

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    task1.pcName       = "EvtTsk05B";
    task1.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1.usTaskPrio   = TASK_PRIO_TEST - 1;

    ret = LOS_TaskCreate(&g_testTaskID02, &task1);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);

    TestExtraTaskDelay(2); // delay 2 ticks

    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT2); // g_testCount equal 2
    LOS_AtomicInc(&g_testCount);

    ret = LOS_HwiCreate(HWI_NUM_TEST, 0, 0, HwiF01, 0);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);
#ifdef LOSCFG_KERNEL_SMP
    HalIrqSetAffinity(HWI_NUM_TEST, CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
#endif

    ret = LOS_HwiEnable(HWI_NUM_TEST);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT3);

    TestHwiTrigger(HWI_NUM_TEST);

    (VOID)LOS_TaskDelay(5); // delay 5 ticks
    ICUNIT_GOTO_EQUAL(g_testCount, 7, g_testCount, EXIT3); //  g_testCount equal 7

EXIT3:
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    ret = LOS_HwiDelete(HWI_NUM_TEST, NULL);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT2);
EXIT2:
    (VOID)LOS_TaskDelete(g_testTaskID02);
EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID01);
EXIT0:
    ret = LOS_EventDestroy(&g_pevent);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    return LOS_OK;
}

/**
* @ingroup TEST_EVENT
* @par TestCase_Number
* ItLosEvent005
* @par TestCase_TestCase_Type
* Function test
* @brief Test event read and write.
* @par TestCase_Pretreatment_Condition
* System Support Events.
* @par TestCase_Test_Steps
* step1: init event;\n
* step2: create high-pri task1 and task2, and create hwi;\n
* step3: task1 and task2 read event;\n
* step4: write event in hwi;\n
* @par TestCase_Expected_Result
* task1 and task2 read event successfully after hwi writes the event.
* @par TestCase_Level
* Level 0
* @par TestCase_Automated
* true
* @par TestCase_Remark
* null
*/

VOID ItLosEvent005(VOID)
{
    TEST_ADD_CASE("ItLosEvent005", TestCase, TEST_LOS, TEST_EVENT, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
