/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Timer Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_api_timer.h"
#include "los_sys.h"
#include "los_swtmr.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DELAY_INTERVAL1     200
#define DELAY_INTERVAL2     1000
#define SWTMR_INTERVAL1     1000
#define SWTMR_INTERVAL2     100

STATIC UINT32 g_demoTimerCount1 = 0;
STATIC UINT32 g_demoTimerCount2 = 0;
STATIC UINT16 g_demoswtmrId1;
STATIC UINT16 g_demoswtmrId2;

STATIC VOID Timer1Callback(UINT32 arg)
{
    UINT32 tickLast1;

    g_demoTimerCount1++;
    tickLast1 = (UINT32)LOS_TickCountGet();
    printf("LOS_TickCountGet g_demoTimerCount1 = %d.\n", g_demoTimerCount1);
    printf("LOS_TickCountGet tickLast1 = %d.\n", tickLast1);
}

STATIC VOID Timer2Callback(UINT32 arg)
{
    UINT32 ret;
    UINT32 tickLast2;

    tickLast2 = (UINT32)LOS_TickCountGet();
    g_demoTimerCount2++;
    printf("LOS_TickCountGet g_demoTimerCount2 = %d.\n", g_demoTimerCount2);
    printf("LOS_TickCountGet tickLast2 = %d.\n", tickLast2);
    ret = InspectStatusSetById(LOS_INSPECT_TIMER, LOS_INSPECT_STU_SUCCESS);
    if (ret != LOS_OK) {
        printf("Set inspect status failed.\n");
    }
}

STATIC VOID RunSwtmr1(VOID)
{
    UINT32 ret;

    ret = LOS_SwtmrStart(g_demoswtmrId1);
    if (ret != LOS_OK) {
        printf("Start software timer1 failed.\n");
    } else {
        printf("Start software timer1 successfully.\n");
    }
    (VOID)LOS_TaskDelay(DELAY_INTERVAL1);

    ret = LOS_SwtmrStop(g_demoswtmrId1);
    if (ret != LOS_OK) {
        printf("Stop software timer1 failed.\n");
    } else {
        printf("Stop software timer1 successfully.\n");
    }
    ret = LOS_SwtmrStart(g_demoswtmrId1);
    if (ret != LOS_OK) {
        printf("Start software timer1 failed.\n");
    }
    (VOID)LOS_TaskDelay(DELAY_INTERVAL2);

    /* the timer that mode is once, kernel will delete it automatically when timer is timeout */
    ret = LOS_SwtmrDelete(g_demoswtmrId1);
    if (ret != LOS_OK) {
        printf("Delete software timer1 failed.\n");
    } else {
        printf("Delete software timer1 successfully.\n");
    }
}

STATIC VOID RunSwtmr2(VOID)
{
    UINT32 ret;

    ret = LOS_SwtmrStart(g_demoswtmrId2);
    if (ret != LOS_OK) {
        printf("Start software timer2 failed.\n");
    } else {
        printf("Start software timer2 successfully.\n");
    }
    (VOID)LOS_TaskDelay(DELAY_INTERVAL2);

    ret = LOS_SwtmrStop(g_demoswtmrId2);
    if (ret != LOS_OK) {
        printf("Stop software timer2 failed.\n");
    }
    ret = LOS_SwtmrDelete(g_demoswtmrId2);
    if (ret != LOS_OK) {
        printf("Delete software timer2 failed.\n");
    }
}

UINT32 SwTimerDemo(VOID)
{
    UINT32 ret;

    printf("Kernel swtimer demo start to run.\n");

    /* create software timer */
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    ret = LOS_SwtmrCreate(SWTMR_INTERVAL1, LOS_SWTMR_MODE_ONCE, (SWTMR_PROC_FUNC)Timer1Callback,
                          &g_demoswtmrId1, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    ret = LOS_SwtmrCreate(SWTMR_INTERVAL1, LOS_SWTMR_MODE_ONCE, (SWTMR_PROC_FUNC)Timer1Callback,
                          &g_demoswtmrId1, 1);
#endif
    if (ret != LOS_OK) {
        printf("Create software timer1 failed.\n");
    } else {
        printf("Create software timer1 successfully.\n");
    }

#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
    ret = LOS_SwtmrCreate(SWTMR_INTERVAL2, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)Timer2Callback,
                          &g_demoswtmrId2, 1, OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE);
#else
    ret = LOS_SwtmrCreate(SWTMR_INTERVAL2, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)Timer2Callback,
                          &g_demoswtmrId2, 1);
#endif
    if (ret != LOS_OK) {
        printf("Create software timer2 failed.\n");
    } else {
        printf("Create software timer2 successfully.\n");
    }

    /* run software timer */
    RunSwtmr1();
    RunSwtmr2();

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
