/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main Process Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-19
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

#include "usart.h"
#include "canary.h"
#include "los_task_pri.h"
#include "los_atomic.h"
#include "los_swtmr_pri.h"
#include "mmu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = (UINTPTR)LOS_HEAP_ADDR_END;
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

VOID CpuInit(VOID)
{
    UINT32 coreId;
    coreId = ArchCurrCpuid();
    __asm__ (
    "msr cpsr_c, %1\n\t"
    "mov sp, %0\n\t"
    "msr cpsr_c, %3\n\t"
    "mov sp, %2\n\t"
    "msr cpsr_c, %5\n\t"
    "mov sp, %4\n\t"
    "msr cpsr_c, %7\n\t"
    "mov sp, %6\n\t"
    "msr cpsr_c, %8\n\t"
    :
    : "r" (__irq_stack_top - OS_EXC_IRQ_STACK_SIZE * coreId),
    "I" (PSR_F_BIT | PSR_I_BIT | CPSR_IRQ_MODE),
    "r" (__abt_stack_top - OS_EXC_ABT_STACK_SIZE * coreId),
    "I" (PSR_F_BIT | PSR_I_BIT | CPSR_ABT_MODE),
    "r" (__undef_stack_top - OS_EXC_UNDEF_STACK_SIZE * coreId),
    "I" (PSR_F_BIT | PSR_I_BIT | CPSR_UNDEF_MODE),
    "r" (__fiq_stack_top - OS_EXC_FIQ_STACK_SIZE * coreId),
    "I" (PSR_F_BIT | PSR_I_BIT | CPSR_FIQ_MODE),
    "I" (PSR_F_BIT | PSR_I_BIT | CPSR_SVC_MODE)
    : "r14");
}

Atomic g_cpuNum = 1;
extern void reset_vector(void);
extern UINT8 g_firstPageTable[MMU_16K];

#ifdef LOSCFG_APC_ENABLE
VOID MmuSectionInit(VOID)
{
    INT32 ret;
    UINT32 ttbBase;
     /* First clear all TT entries - ie Set them to Faulting */
    ret = memset_s((VOID *)(UINT8 *)g_firstPageTable, MMU_16K, 0, MMU_16K);
    if (ret != 0) {
        return;
    }
    ttbBase = (UINT32)g_firstPageTable;

    /* Set all mem 4G as uncacheable & rw first */
    X_MMU_SECTION(0, 0, (MMU_4G >> SHIFT_1M),
                  UNCACHEABLE, UNBUFFERABLE, ACCESS_RW, NON_EXECUTABLE, D_CLIENT);

    X_MMU_SECTION(0, 0, ((AUX_BASE - 1) >> SHIFT_1M),
                  CACHEABLE, BUFFERABLE, ACCESS_RW, EXECUTABLE, D_CLIENT);
}
#endif

INT32 main(VOID)
{
#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    CpuInit();
    uart_early_init();
    BoardConfig();
    OsSystemInfo();
    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
#ifdef LOSCFG_KERNEL_SMP
    MAILBOXES_INFO *mailbox = CORE_MAILBOX_REG_BASE;
    UINT8 coreId;
    for (coreId = 1; coreId < LOSCFG_KERNEL_CORE_NUM; coreId++) {
        /* per cpu 4 mailbox */
        mailbox->writeSet[coreId * 4 + MAILBOX3_IRQ - MAILBOX0_IRQ] = (UINT32)reset_vector;
        __asm__ volatile ("sev");
    }
    while (LOS_AtomicRead(&g_cpuNum) < LOSCFG_KERNEL_CORE_NUM) {
        asm volatile("wfe");
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
extern VOID HalIrqInitPercpu(VOID);
LITE_OS_SEC_TEXT_INIT INT32 secondary_cpu_start(VOID)
{
    OsCurrTaskSet(OsGetMainTask());
    CpuInit();

    /* increase cpu counter */
    LOS_AtomicInc(&g_cpuNum);
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

