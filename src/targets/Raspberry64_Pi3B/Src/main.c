/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main Process Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-08-04
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

#include "canary.h"
#include "los_task_pri.h"
#include "mmu.h"
#include "mmu_pri.h"
#include "los_atomic.h"
#include "los_swtmr_pri.h"
#include "arch/cache.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SPIN_TABLE_BASE 0xD8

VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = (UINTPTR)LOS_HEAP_ADDR_END;
}

extern VOID reset_vector(VOID);

VOID MmuSectionMap(VOID)
{
    UINT64 flag;
     /* device */
    flag = MMU_INITIAL_MAP_DEVICE | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsInit(flag, 0, MMU_4G);

     /* normal cache */
    flag = MMU_PTE_CACHE_RW_FLAGS | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsSet(flag, 0, g_sys_mem_addr_end - 1);
}

VOID OsSystemInfo(VOID)
{
    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n\n"
                  "LiteOS Kernel Version : %s\n"
                  "Processor   : %s"
#ifdef LOSCFG_KERNEL_SMP
                  " * %d\n"
                  "Run Mode    : SMP\n"
#else
                  "\n"
                  "Run Mode    : UP\n"
#endif
                  "build time  : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING,
                  LOS_CpuInfo(),
#ifdef LOSCFG_KERNEL_SMP
                  LOSCFG_KERNEL_SMP_CORE_NUM,
#endif
                  __DATE__, __TIME__);
}

INT32 main(VOID)
{
#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());
    OsSystemInfo();
    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

#ifdef LOSCFG_KERNEL_SMP
    UINT8 coreId;
    for (coreId = 1; coreId < LOSCFG_KERNEL_CORE_NUM; coreId++) {
        *(((UINTPTR *)SPIN_TABLE_BASE) + coreId) = (UINTPTR)reset_vector;
        ArchDCacheCleanByAddr((UINTPTR)SPIN_TABLE_BASE, \
                              (UINTPTR)(SPIN_TABLE_BASE+LOSCFG_KERNEL_CORE_NUM * sizeof(UINTPTR)));
        asm("SEV");
    }

    (VOID)LOS_HwiEnable(MAILBOX0_IRQ);
    (VOID)LOS_HwiEnable(MAILBOX1_IRQ);
    (VOID)LOS_HwiEnable(MAILBOX2_IRQ);
#ifdef LOSCFG_KERNEL_SMP_CALL
    (VOID)LOS_HwiEnable(MAILBOX3_IRQ);
#endif
#endif

    OsStart();

    return LOS_OK;
}

#ifdef LOSCFG_KERNEL_SMP
LITE_OS_SEC_TEXT_INIT INT32 secondary_cpu_start(VOID)
{
    OsCurrTaskSet(OsGetMainTask());
    HalIrqInitPercpu();
    (VOID)LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);
    (VOID)LOS_HwiEnable(MAILBOX0_IRQ);
    (VOID)LOS_HwiEnable(MAILBOX1_IRQ);
    (VOID)LOS_HwiEnable(MAILBOX2_IRQ);
#ifdef LOSCFG_KERNEL_SMP_CALL
    (VOID)LOS_HwiEnable(MAILBOX3_IRQ);
#endif
#ifdef LOSCFG_BASE_CORE_SWTMR
    (VOID)OsSwtmrInit();
#endif
    (VOID)OsIdleTaskCreate();
    OsStart();

    return LOS_OK;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
