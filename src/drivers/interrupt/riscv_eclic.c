/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description : LiteOS Interrupt Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-28
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

#include "los_hwi_pri.h"
#include "riscv_encoding.h"
#include "los_task_pri.h"
#include "eclic.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC UINT32 g_curIrqNum = 0;

LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT];

LITE_OS_SEC_DATA_VEC HWI_PROC_FUNC g_hwiVec[LOSCFG_PLATFORM_HWI_LIMIT] = {
    (HWI_PROC_FUNC)_start,              /* [0] reset */
    (HWI_PROC_FUNC)0,                   /* [1] Reserved */
    (HWI_PROC_FUNC)0,                   /* [2] Reserved */
    (HWI_PROC_FUNC)IrqEntryRiscv,       /* [3] CLIC_INT_SFT */
    (HWI_PROC_FUNC)0,                   /* [4] Reserved */
    (HWI_PROC_FUNC)0,                   /* [5] Reserved */
    (HWI_PROC_FUNC)0,                   /* [6] Reserved */
    (HWI_PROC_FUNC)IrqEntryRiscv,       /* [7] CLIC_INT_TMR */
    (HWI_PROC_FUNC)0,                   /* [8] Reserved */
    (HWI_PROC_FUNC)0,                   /* [9] Reserved */
    (HWI_PROC_FUNC)0,                   /* [10] Reserved */
    (HWI_PROC_FUNC)0,                   /* [11] Reserved */
    (HWI_PROC_FUNC)0,                   /* [12] Reserved */
    (HWI_PROC_FUNC)0,                   /* [13] Reserved */
    (HWI_PROC_FUNC)0,                   /* [14] Reserved */
    (HWI_PROC_FUNC)0,                   /* [15] Reserved */
    (HWI_PROC_FUNC)0,                   /* [16] Reserved */
    (HWI_PROC_FUNC)IrqEntryRiscv,       /* [17] CLIC_INT_BWEI */
    (HWI_PROC_FUNC)IrqEntryRiscv,       /* [18] CLIC_INT_PMOVI */
};

LITE_OS_SEC_TEXT_MINOR VOID IrqEntryRiscv(VOID)
{
    UINT32 hwiIndex;

    hwiIndex = (read_csr(mcause)) & 0xff;;
    g_curIrqNum = hwiIndex;

    if (hwiIndex == CLIC_INT_SFT) {
        *(volatile UINT32 *)(TIMER_CTRL_ADDR + TIMER_MSIP) = 0;
    }
    if (hwiIndex == CLIC_INT_TMR) {
        *(UINT64 *) (TIMER_CTRL_ADDR + TIMER_MTIME) = 0;
    }

    OsIntHandle(hwiIndex, &g_hwiForm[hwiIndex]);

    if (OsTaskProcSignal() != 0) {
        OsSchedPreempt();
    }
}

UINT32 EclicIrqUnmask(UINT32 hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    intSave = LOS_IntLock();
    eclic_enable_interrupt(hwiNum);
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 EclicIrqSetPriority(UINT32 hwiNum, UINT8 priority)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    if (!HWI_PRI_VALID(priority)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }
    
    intSave = LOS_IntLock();
    eclic_set_irq_priority(hwiNum, priority);
    LOS_IntRestore(intSave);

    return LOS_OK;
}

UINT32 EclicIrqMask(HWI_HANDLE_T hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    if (hwiNum == CLIC_INT_SFT) {
        *(volatile UINT32 *)(TIMER_CTRL_ADDR + TIMER_MSIP) = 0;
    }
    if (hwiNum == CLIC_INT_TMR) {
        *(UINT64 *) (TIMER_CTRL_ADDR + TIMER_MTIME) = 0;
    }
    eclic_disable_interrupt(hwiNum);
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 EclicIrqPending(UINT32 hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    intSave = LOS_IntLock();
    if (hwiNum == CLIC_INT_SFT) {
        *(volatile UINT32 *)(TIMER_CTRL_ADDR + TIMER_MSIP) = 1;
    } else {
        eclic_set_pending(hwiNum);
    }
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 EclicCurIrqGet(VOID)
{
    g_curIrqNum = (read_csr(mcause)) & 0xff;
    return g_curIrqNum;
}

CHAR *EclicIrqVersion(VOID)
{
    return "ECLIC";
}

HwiHandleInfo *EclicIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if ((hwiNum > LOSCFG_PLATFORM_HWI_LIMIT) || (hwiNum < OS_USER_HWI_MIN)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

STATIC const HwiControllerOps g_eclicOps = {
    .triggerIrq     = EclicIrqPending,
    .enableIrq      = EclicIrqUnmask,
    .disableIrq     = EclicIrqMask,
    .setIrqPriority = EclicIrqSetPriority,
    .getCurIrqNum   = EclicCurIrqGet,
    .getIrqVersion  = EclicIrqVersion,
    .getHandleForm  = EclicIrqGetHandleForm,
};

VOID ArchIrqInit(VOID)
{
    UINT32 i;

    for (i = OS_SYS_VECTOR_CNT; i < LOSCFG_PLATFORM_HWI_LIMIT; i++) {
        g_hwiVec[i] = (HWI_PROC_FUNC)IrqEntryRiscv;
    }
    asm volatile ("csrw 0x307, %0" :: "r"(g_hwiVec));
    eclic_init(ECLIC_NUM_INTERRUPTS);
    eclic_set_nlbits(ECLIC_GROUP_LEVEL2_PRIO2);
    OsHwiControllerReg(&g_eclicOps);
    eclic_mode_enable();
    set_csr(mstatus, MSTATUS_MIE);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
