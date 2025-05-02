/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: iCunit Test Frame
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

#include "ostest.h"
#include "console.h"

#if defined(LOSCFG_KERNEL_MEM_BESTFIT) || defined(LOSCFG_KERNEL_MEM_BESTFIT_LITTLE)
#include "los_multipledlinkhead_pri.h"
#include "los_memory_pri.h"
#include "los_memory_internal.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

UINT32 g_testTskHandle;

#ifdef LOSCFG_KERNEL_MEM_BESTFIT
int g_minMemPoolSize = OS_MEM_MIN_POOL_SIZE;
#elif defined LOSCFG_KERNEL_MEM_BESTFIT_LITTLE
int g_minMemPoolSize = sizeof(LosMemPoolInfo);
#endif

LOS_MEM_POOL_STATUS g_sysMemStatus = {0};

VOID TestDispinfo(VOID)
{
    unsigned int memused = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
    dprintf("memused = 0x%x\n", memused);
    LOS_MemInfoGet(OS_SYS_MEM_ADDR, &g_sysMemStatus);
}

VOID TestHwiClearAll(VOID)
{
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST0);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST1);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST2);
    TEST_DEV_HWI_CLEAR(HWI_NUM_TEST3);
}

VOID TestKernelBase(VOID)
{
#ifdef LOSCFG_TEST_KERNEL_BASE
#ifdef LOSCFG_TEST_KERNEL_BASE_TASK
    ItSuiteLosTask();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_HWI
    ItSuiteLosHwi();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_SWTMR
    ItSuiteLosSwtmr();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_IPC_QUEUE
    ItSuiteLosQueue();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_IPC_EVENT
    ItSuiteLosEvent();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_IPC_MUX
    ItSuiteLosMux();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_IPC_SEM
    ItSuiteLosSem();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_MEM
#ifdef LOSCFG_KERNEL_MEMBOX
    ItSuiteLosMembox();
#endif
    ItSuiteLosMem();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_ATOMIC
    ItSuiteLosAtomic();
#endif
#ifdef LOSCFG_TEST_KERNEL_BASE_MISC
    ItSuiteLosMisc();
#endif
#endif
}

VOID TestTaskEntry(UINT32 uwParam1, UINT32 uwParam2, UINT32 uwParam3, UINT32 uwParam4)
{
    dprintf("\t\n --- Test start--- \n");

#ifdef LOSCFG_TEST
    ICunitInit();
    TestDispinfo();
    TestHwiClearAll();
    TestKernelBase();
    ICunitPrintReport();
#endif
    dprintf("\t\n --- Test End--- \n");
}

UINT32 OsTestInit(VOID)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S osTaskInitParam = {0};
    osTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TestTaskEntry;
    osTaskInitParam.uwStackSize  = LOSCFG_TEST_TASK_STACK_SIZE;
    osTaskInitParam.pcName       = "IT_TST_INI";
    osTaskInitParam.usTaskPrio   = TASK_PRIO_TEST;
    osTaskInitParam.uwResved   = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    osTaskInitParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(0);
#endif
    uwRet = LOS_TaskCreate(&g_testTskHandle, &osTaskInitParam);
    return uwRet;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
