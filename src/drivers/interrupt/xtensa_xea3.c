/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: LiteOS hwi Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-28
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

#include <xtensa/hal.h>
#include <xtensa/hal-certified.h>
#include <xtensa/intctrl.h>
#include "los_hwi_pri.h"
#include "los_hwi.h"
#include "los_sched_pri.h"
#include "los_task_pri.h"
#if (LOSCFG_KERNEL_TRACE == YES)
#include "los_trace.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef struct {
    HWI_PROC_FUNC pfnHook;
    HWI_ARG_T     uwParam;
} HwiIrqProcFunc;

/* g_mainHwiForm: Need to maintain continuity with the interrupt stack space in memory,
  variables defined in the assembly file */
extern HwiIrqProcFunc g_mainHwiForm[LOSCFG_PLATFORM_HWI_LIMIT];
STATIC UINT32 g_curIrqNum = 0;
STATIC HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT];

UINT32 HalIrqGetIntNum(VOID)
{
    return XT_RER(ICREG_HIGHESTNUM);
}

LITE_OS_SEC_TEXT_INIT VOID IrqEntryXea3(HWI_ARG_T arg)
{
    UINT32 hwiIndex;
    (VOID)arg;

    /* no need restore for xea3 */
    (VOID)LOS_IntLock();

    hwiIndex = HalIrqGetIntNum();
    if (hwiIndex >= LOSCFG_PLATFORM_HWI_LIMIT) {
        PRINT_ERR("irqNum %d error, check!", hwiIndex);
        return;
    }
    g_curIrqNum = hwiIndex;
    xthal_window_spill();
    OsIntHandle(hwiIndex, &g_hwiForm[hwiIndex]);
}

UINT32 ArchIrqPending(HWI_HANDLE_T irq)
{
    UINTPTR uvIntSave;

    if (!HWI_NUM_VALID(irq)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    uvIntSave = LOS_IntLock();
    xthal_interrupt_trigger(irq);
    LOS_IntRestore(uvIntSave);
    return LOS_OK;
}

UINT32 ArchIrqUnmask(HWI_HANDLE_T irq)
{
    UINTPTR uvIntSave;

    if (!HWI_NUM_VALID(irq)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    uvIntSave = LOS_IntLock();
    xthal_interrupt_enable(irq);
    LOS_IntRestore(uvIntSave);
    return LOS_OK;
}

UINT32 ArchIrqMask(HWI_HANDLE_T irq)
{
    UINTPTR uvIntSave;

    if (!HWI_NUM_VALID(irq)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    uvIntSave = LOS_IntLock();
    xthal_interrupt_disable(irq);
    LOS_IntRestore(uvIntSave);
    return LOS_OK;
}

UINT32 HalCurIrqGet(VOID)
{
    return g_curIrqNum;
}

UINT32 ArchIrqClear(HWI_HANDLE_T hwiNum)
{
    UINTPTR uvIntSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    uvIntSave = LOS_IntLock();
    xthal_interrupt_clear(hwiNum);
    LOS_IntRestore(uvIntSave);

    return LOS_OK;
}

STATIC HwiHandleInfo *ArchIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

STATIC const HwiControllerOps g_xea3Ops = {
    .triggerIrq     = ArchIrqPending,
    .clearIrq       = ArchIrqClear,
    .enableIrq      = ArchIrqUnmask,
    .disableIrq     = ArchIrqMask,
    .getCurIrqNum   = HalCurIrqGet,
    .getHandleForm  = ArchIrqGetHandleForm,
};

LITE_OS_SEC_TEXT_INIT VOID ArchIrqInit(VOID)
{
    UINT32 i;

    /* init handle form */
    for (i = 0; i < LOSCFG_PLATFORM_HWI_LIMIT; i++) {
        g_mainHwiForm[i].pfnHook = (HWI_PROC_FUNC)IrqEntryXea3;
        g_mainHwiForm[i].uwParam = i;
        LOS_HwiDisable(i);
    }

    /* register interrupt controller's operations */
    OsHwiControllerReg(&g_xea3Ops);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
