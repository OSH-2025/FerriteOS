/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main Process Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-01
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
#include "uart.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = (UINTPTR)LOS_HEAP_ADDR_END;
}

static VOID CpuInit(VOID)
{
    __asm__ (
    "msr	cpsr_c, %1\n\t"
    "mov	sp,     %0\n\t"
    "msr	cpsr_c, %3\n\t"
    "mov	sp,     %2\n\t"
    "msr	cpsr_c, %5\n\t"
    "mov	sp,     %4\n\t"
    "msr	cpsr_c, %7\n\t"
    "mov	sp,     %6\n\t"
    "msr	cpsr_c, %8\n\t"
    :
    : "r" (__irq_stack_top),
      "I" (PSR_F_BIT | PSR_I_BIT | CPSR_IRQ_MODE),
      "r" (__abt_stack_top),
      "I" (PSR_F_BIT | PSR_I_BIT | CPSR_ABT_MODE),
      "r" (__undef_stack_top),
      "I" (PSR_F_BIT | PSR_I_BIT | CPSR_UNDEF_MODE),
      "r" (__fiq_stack_top),
      "I" (PSR_F_BIT | PSR_I_BIT | CPSR_FIQ_MODE),
      "I" (PSR_F_BIT | PSR_I_BIT | CPSR_SVC_MODE)
      : "r14");
}

INT32 main(VOID)
{
#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    BoardConfig();
    CpuInit();
    uart_early_init();

    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n"
                  "\nLiteOS Kernel Version : %s\n"
                  "build date : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING, __DATE__, __TIME__);

    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    OsStart();

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
