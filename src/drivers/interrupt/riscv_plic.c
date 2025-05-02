/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description : PLIC(Platform-Level Interrupt Controller) for RISCV Implemention.
 * Author: Huawei LiteOS Team
 * Create : 2021-05-13
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
 * ---------------------------------------------------------------------------- */

#include "plic.h"
#include "los_hwi_pri.h"
#include "los_task_pri.h"
#include "arch/regs.h"
#include "metal/machine/platform.h"
#include "metal/drivers/riscv_plic0.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CLINT_SOFT_IRQ (LOSCFG_PLATFORM_HWI_LIMIT)
#define CLINT_TIME_IRQ (LOSCFG_PLATFORM_HWI_LIMIT + 1)
#define MAX_PRIORITY   7

STATIC UINT32 g_curIrqNum = 0;
LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[OS_HWI_MAX_NUM];

UINT32 PlicIrqUnmask(UINT32 hwiNum)
{
    UINT32 intSave;
    UINT32 current;
    UINT32 controlBase = METAL_RISCV_PLIC0_C000000_BASE_ADDRESS;
    if (hwiNum >= OS_HWI_MAX_NUM) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    if (hwiNum ==  CLINT_SOFT_IRQ) {
        SET_CSR(mie, LOS_MIP_MSIP);
    } else if (hwiNum == CLINT_TIME_IRQ) {
        SET_CSR(mie, LOS_MIP_MTIP);
    } else {
        SET_CSR(mie, LOS_MIP_MEIP);
        /* (0-31)==>offset(0) (32-63)==>offset(4) */
        current = *(UINT32 volatile *)(controlBase + METAL_RISCV_PLIC0_ENABLE_BASE + hwiNum * sizeof(UINT32) / 32);
        /* (0-31)==>offset(0) (32-63)==>offset(4) */
        *(UINT32 volatile *)(controlBase + METAL_RISCV_PLIC0_ENABLE_BASE + hwiNum * sizeof(UINT32) / 32) =
                                                                 current | (1 << (hwiNum & METAL_PLIC_SOURCE_MASK));
    }

    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 PlicIrqSetPriority(UINT32 hwiNum, UINT8 priority)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    if (!HWI_PRI_VALID(priority)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    if (hwiNum < LOSCFG_PLATFORM_HWI_LIMIT) {
        *(volatile UINT32 *)(METAL_RISCV_PLIC0_C000000_BASE_ADDRESS +
                            METAL_RISCV_PLIC0_PRIORITY_BASE +
                            (hwiNum << METAL_PLIC_SOURCE_PRIORITY_SHIFT)) = priority;
    }
    return LOS_OK;
}

UINT32 PlicIrqMask(HWI_HANDLE_T hwiNum)
{
    UINT32 intSave;
    UINT32 current;
    UINT32 controlBase = METAL_RISCV_PLIC0_C000000_BASE_ADDRESS;
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    if (hwiNum == CLINT_SOFT_IRQ) {
        CLEAR_CSR(mie, LOS_MIP_MSIP);
    } else if (hwiNum == CLINT_TIME_IRQ) {
        CLEAR_CSR(mie, LOS_MIP_MTIP);
    } else {
        /* (0-31)==>offset(0) (32-63)==>offset(4) */
        current = *(UINT32 volatile *)(controlBase + METAL_RISCV_PLIC0_ENABLE_BASE + hwiNum * 4 / 32);
        /* (0-31)==>offset(0) (32-63)==>offset(4) */
        *(UINT32 volatile *)(controlBase + METAL_RISCV_PLIC0_ENABLE_BASE + hwiNum * 4 / 32) =
                                                    current & ~(1 << (hwiNum & METAL_PLIC_SOURCE_MASK));
    }
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 PlicIrqPending(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    if (hwiNum == CLINT_SOFT_IRQ) {
        SET_CSR(mip, LOS_MIP_MSIP);
    } else if (hwiNum == CLINT_TIME_IRQ) {
        SET_CSR(mie, LOS_MIP_MTIP);
    } else {
        SET_CSR(mie, LOS_MIP_MEIP);
    }
    return LOS_OK;
}

UINT32 PlicGetClaim(VOID)
{
    return *(UINT32 volatile *)(METAL_RISCV_PLIC0_C000000_BASE_ADDRESS +
                              METAL_RISCV_PLIC0_CONTEXT_BASE +
                              METAL_RISCV_PLIC0_CONTEXT_CLAIM);
}

VOID PlicGetComplete(UINT32 hwiNum)
{
    if (hwiNum >= LOSCFG_PLATFORM_HWI_LIMIT) {
        return;
    }
    *(UINT32 volatile *)(METAL_RISCV_PLIC0_C000000_BASE_ADDRESS +
                         METAL_RISCV_PLIC0_CONTEXT_BASE +
                         METAL_RISCV_PLIC0_CONTEXT_CLAIM) = hwiNum;
}

UINT32 PlicCurIrqGet(VOID)
{
    UINT32 irqNum;

    irqNum = (READ_CSR(mcause)) & 0xFF;
    if (irqNum == LOS_IRQ_M_SOFT) {
        irqNum = CLINT_SOFT_IRQ;
    } else if (irqNum == LOS_IRQ_M_TIMER) {
        irqNum = CLINT_TIME_IRQ;
    } else if (irqNum == LOS_IRQ_M_EXT) {
        irqNum = PlicGetClaim();
    }
    g_curIrqNum = irqNum;
    return irqNum;
}
VOID PlicSetThreshold(UINT32 threshold)
{
    *(UINT32 volatile *)(METAL_RISCV_PLIC0_C000000_BASE_ADDRESS + METAL_RISCV_PLIC0_CONTEXT_BASE) = threshold;
}

CHAR *PlicIrqVersion(VOID)
{
    return "PLIC";
}

HwiHandleInfo *PlicIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if ((hwiNum >= OS_HWI_MAX_NUM) || (hwiNum < OS_USER_HWI_MIN)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

LITE_OS_SEC_TEXT_MINOR VOID IrqEntryRiscv(VOID)
{
    UINT32 hwiIndex;

    hwiIndex = PlicCurIrqGet();
    if (hwiIndex == CLINT_SOFT_IRQ) {
        *(volatile UINT32 *)(METAL_RISCV_CLINT0_2000000_BASE_ADDRESS) = 0;
    } else if (hwiIndex == CLINT_TIME_IRQ) {
        *(volatile UINT64 *) (METAL_RISCV_CLINT0_2000000_BASE_ADDRESS + METAL_RISCV_CLINT0_MTIME) = 0;
    }

    OsIntHandle(hwiIndex, &g_hwiForm[hwiIndex]);
    PlicGetComplete(hwiIndex);
}

STATIC const HwiControllerOps g_plicOps = {
    .triggerIrq     = PlicIrqPending,
    .enableIrq      = PlicIrqUnmask,
    .disableIrq     = PlicIrqMask,
    .setIrqPriority = PlicIrqSetPriority,
    .getCurIrqNum   = PlicCurIrqGet,
    .getIrqVersion  = PlicIrqVersion,
    .getHandleForm  = PlicIrqGetHandleForm,
};

VOID ArchIrqInit(VOID)
{
    UINT32 i;

    asm volatile ("la t0, TrapVector\t\ncsrw mtvec, t0");
    OsHwiControllerReg(&g_plicOps);
    *(UINT64 *)(METAL_RISCV_PLIC0_C000000_BASE_ADDRESS + METAL_RISCV_PLIC0_ENABLE_BASE) = 0;

    for (i = 1; i < LOSCFG_PLATFORM_HWI_LIMIT; i++) {
        if (PlicIrqSetPriority(i, 1) != LOS_OK) {
            return;
        }
    }

    PlicSetThreshold(0);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
