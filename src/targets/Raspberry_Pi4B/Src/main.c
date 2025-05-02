/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main
 * Author: Huawei LiteOS Team
 * Create: 2021-07-27
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

#include "los_task_pri.h"
#include "arch/canary.h"
#include "los_swtmr_pri.h"
#include "los_config.h"
#include "los_printf.h"
#include "los_atomic.h"
#include "mmu_pri.h"
#include "gic_common.h"
#include "platform.h"
#include "mmu.h"
#include "los_memory.h"

#define SPIN_TABLE_BASE 0xD8

Atomic g_ncpu = 1;

VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = (UINTPTR)&__LOS_HEAP_ADDR_END__;
}

 /*
 *   device:
 *       access without cache;
 *       not support out-of-order execution;
 *       not support Instruction pre read;
 *       not support non-aligned access
 *   normal:
 *       access cacheability(cache or non-cache);
 *       support out-of-order execution;
 *       support Instruction pre read;
 *       support non-aligned access
 */
VOID MmuSectionMap(VOID)
{
    UINT64 flag;
     /* device */
    flag = MMU_INITIAL_MAP_DEVICE | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsInit(flag, 0, DDR_MEM_SIZE);
    /* normal cache */
    flag = MMU_PTE_CACHE_RW_FLAGS | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsSet(flag, 0, g_sys_mem_addr_end - 1);
}

LITE_OS_SEC_TEXT_INIT void osSystemInfo(VOID)
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
                  "GIC Rev     : %s\n"
                  "build time  : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING,
                  LOS_CpuInfo(),
#ifdef LOSCFG_KERNEL_SMP
                  LOSCFG_KERNEL_SMP_CORE_NUM,
#endif
                  HalIrqVersion(), __DATE__, __TIME__);
}

LITE_OS_SEC_TEXT_INIT int secondary_cpu_start(VOID)
{
    int cpuId;
    cpuId = ArchCurrCpuid();

    OsCurrTaskSet(OsGetMainTask());

    /* increase cpu counter */
    LOS_AtomicInc(&g_ncpu);

    /* store each core's hwid */
    CPU_MAP_SET(cpuId, OsHwIDGet());
    HalIrqInitPercpu();
#ifdef LOSCFG_BASE_CORE_SWTMR
    (VOID)OsSwtmrInit();
#endif
    (VOID)OsIdleTaskCreate();
    OsStart();

    while (1) {
        __asm volatile("wfi");
    }
}

LITE_OS_SEC_TEXT_INIT VOID release_secondary_cores(VOID)
{
    for (UINT32 i = 1; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        *(((UINTPTR *)SPIN_TABLE_BASE) + i) = (UINTPTR)SYS_MEM_BASE;
        flush_dcache((UINTPTR)(SPIN_TABLE_BASE + sizeof(UINTPTR) * i), \
                     (UINTPTR)(SPIN_TABLE_BASE + sizeof(UINTPTR) * (i + 1)));
        __asm volatile("sev");
    }
}

LITE_OS_SEC_TEXT_INIT int main(VOID)
{
    UINT32 ret;

#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    /* system and chip info */
    osSystemInfo();

    PRINTK("\nmain core booting up...\n");

    ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

#ifdef LOSCFG_KERNEL_SMP
    PRINTK("releasing %u secondary cores\n", LOSCFG_KERNEL_SMP_CORE_NUM - 1);
    release_secondary_cores();
#endif

    CPU_MAP_SET(0, OsHwIDGet());

    OsStart();

    while (1) {
        __asm volatile("wfi");
    }
}
