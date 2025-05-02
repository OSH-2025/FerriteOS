/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Kernel Extended PERF Demo
 * Author: Huawei LiteOS Team
 * Create: 2021-08-19
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

#include "perf_demo.h"
#include "los_perf.h"
#include "los_task.h"
#include "los_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PERF_TASK_PRIORITY 25
#define PERF_TASK_STACKSIZE 0x10000
UINT32 g_arry[100] = {0};
UINT32 g_arry_2[100] = {0};

STATIC VOID OsPrintBuff(const CHAR *buf, UINT32 num)
{
    UINT32 i = 0;
    printf("num: ");
    for (i = 0; i < num; i++) {
        printf("%02d", i);
    }
    printf("\n");
    printf("hex: ");
    for (i = 0; i < num; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");
}

__attribute__((noinline)) VOID foo1(VOID)
{
    for (UINT32 i = 0; i < 10000; i++) {
        g_arry[i % 100] = i * i;
    }
}

__attribute__((noinline)) VOID bar(VOID)
{
    for (UINT32 i = 0; i < 10000; i++) {
        g_arry_2[i % 100] = i * i;
    }
}

__attribute__((noinline)) VOID boo(VOID)
{
    for (UINT32 i = 0; i < 10000; i++) {
        foo1();
    }
    for (UINT32 i = 0; i < 20000; i++) {
        bar();
    }
}

__attribute__((noinline)) VOID test(VOID)
{
    boo();
}

STATIC VOID perfTestTimedEvent(VOID)
{
    UINT32 ret;

    PerfConfigAttr attr = {
        .eventsCfg = {
            .type = PERF_EVENT_TYPE_TIMED,
            .events = {
                [0] = {PERF_COUNT_CPU_CLOCK, 1},
            },
            .eventsNr = 1,
            .predivided = 0,            /* cycle counter increase every 64 cycles */
        },
        .taskIds = {0},
        .taskIdsNr = 0,                 /* instrument the whole system by default */
        .needSample = 0,                //1,
        .sampleType = 0xFFFFFFFF,       //PERF_RECORD_IP,//0
    };

    ret = LOS_PerfConfig(&attr);
    if (ret != LOS_OK) {
        PRINT_ERR("perf config error %u\n", ret);
        return;
    }

    LOS_PerfStart(0);
    test();
    LOS_PerfStop();

    CHAR buf[0x200] = {0};
    UINT32 len;
    len = LOS_PerfDataRead(buf, 0x200); /* get sample data */
    OsPrintBuff(buf, len);              /* print data */
}

VOID perfTestSwEvent(VOID)
{
    UINT32 ret;
    PerfConfigAttr attr = {
        .eventsCfg = {
            .type = PERF_EVENT_TYPE_SW,
            .events = {
                [0] = {PERF_COUNT_SW_TASK_SWITCH, 1},
                [1] = {PERF_COUNT_SW_IRQ_RESPONSE, 1},
            },
            .eventsNr = 2,
            .predivided = 0,            /* cycle counter increase every 64 cycles */
        },
        .taskIds = {0},
        .taskIdsNr = 0,                 /* instrument the whole system by default */
        .needSample = 0,
        .sampleType = 0,
    };

    ret = LOS_PerfConfig(&attr);
    if (ret != LOS_OK) {
        PRINT_ERR("perf config error %u\n", ret);
        return;
    }

    LOS_PerfStart(1);
    LOS_TaskDelay(5);                   /* let task switch event happen */
    LOS_TaskDelay(5);                   /* let task switch event happen */
    LOS_TaskDelay(5);                   /* let task switch event happen */
    LOS_PerfStop();
}

VOID perfTestHwEvent(VOID)
{
    UINT32 ret;
    CHAR buf[0x200] = {0};
    UINT32 len;

    PerfConfigAttr attr = {
        .eventsCfg = {
            .type = PERF_EVENT_TYPE_HW,
            .events = {
                [0] = {PERF_COUNT_HW_CPU_CYCLES, 0xFFFF},
                [1] = {PERF_COUNT_HW_BRANCH_INSTRUCTIONS, 0xFFFFFF00},
            },
            .eventsNr = 2,
            .predivided = 1,            /* cycle counter increase every 64 cycles */
        },
        .taskIds = {0},
        .taskIdsNr = 0,                 /* instrument the whole system by default */
        .needSample = 0,
        .sampleType = PERF_RECORD_IP | PERF_RECORD_CALLCHAIN,
    };

    ret = LOS_PerfConfig(&attr);
    if (ret != LOS_OK) {
        PRINT_ERR("perf config error %u\n", ret);
        return;
    }

    printf("------------count mode------------\n");
    LOS_PerfStart(1);
    test();
    LOS_PerfStop();

    printf("------------sample mode------------\n");
    attr.needSample = 1;
    LOS_PerfConfig(&attr);
    LOS_PerfStart(1);
    test();
    LOS_PerfStop();

    len = LOS_PerfDataRead(buf, 0x200); /* get sample data */
    OsPrintBuff(buf, len);              /* print data */
}

STATIC VOID DemoTaskEntry(VOID)
{
    perfTestSwEvent();
    perfTestTimedEvent();
    return;
    perfTestHwEvent();
}

VOID PerfDemoTask(VOID)
{
    UINT32 uwRet = 0;
    UINT32 tid;
    TSK_INIT_PARAM_S os_task_init_param = {0};

    os_task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    os_task_init_param.uwStackSize = PERF_TASK_STACKSIZE;
    os_task_init_param.pcName = "IT_TST_INI";
    os_task_init_param.usTaskPrio = PERF_TASK_PRIORITY;
    os_task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
#if (LOSCFG_KERNEL_SMP == YES)
    os_task_init_param.usCpuAffiMask = CPUID_TO_AFFI_MASK(0);
#endif

    uwRet = LOS_TaskCreate(&tid, &os_task_init_param);
    if (0 != uwRet) {
        printf("perf test task creat failed\n");
    }
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
