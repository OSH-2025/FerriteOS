/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: VIC(Nested Vectored Interrupt Controller) for C-Sky Implementation
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
 * --------------------------------------------------------------------------- */

#include "vic.h"
#include "los_hwi_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PRI_OFF_PER_INT  8
#define PRI_PER_REG      4
#define PRI_OFF_IN_REG   6
#define PRI_BITS         2
#define PRI_HI           0
#define PRI_LOW          7
#define MASK_8_BITS      0xFF
#define MASK_32_BITS     0xFFFFFFFF
#define RESET_INT_NUM    0
#define TRAP0_INT_NUM    22
#define BYTES_OF_128_INT 4
#define TIM_INT_NUM      1

STATIC UINT32 g_curIrqNum = 0;

LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT] = { 0 };

LITE_OS_SEC_DATA_VEC HWI_PROC_FUNC g_hwiVec[LOSCFG_PLATFORM_HWI_LIMIT + OS_SYS_VECTOR_CNT];

VOID IrqEntryCsky(VOID)
{
    UINT32 hwiIndex = ArchGetPsr();
    hwiIndex = (hwiIndex >> PSR_VEC_OFFSET) & MASK_8_BITS;
    g_curIrqNum = hwiIndex - OS_SYS_VECTOR_CNT;

    OsIntHandle(g_curIrqNum, &g_hwiForm[g_curIrqNum]);

    if (OsTaskProcSignal() != 0) {
        OsSchedPreempt();
    }
}

UINT32 ArchIrqUnmask(UINT32 hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    VIC_REG->ISER[hwiNum / OS_SYS_VECTOR_CNT] = (UINT32)(1UL << (hwiNum % OS_SYS_VECTOR_CNT));
#ifdef CONFIG_SYSTEM_SECURE
    VIC_REG->ISSR[hwiNum / OS_SYS_VECTOR_CNT] = (UINT32)(1UL << (hwiNum % OS_SYS_VECTOR_CNT));
#endif
    LOS_IntRestore(intSave);

    return LOS_OK;
}

UINT32 HalIrqSetPriority(UINT32 hwiNum, UINT8 priority)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    if (!HWI_PRI_VALID(priority)) {
        return OS_ERRNO_HWI_PRIO_INVALID;
    }

    intSave = LOS_IntLock();
    VIC_REG->IPR[hwiNum / PRI_PER_REG] |= ((priority << PRI_OFF_IN_REG) << (hwiNum % PRI_PER_REG) * PRI_OFF_PER_INT);
    LOS_IntRestore(intSave);

    return LOS_OK;
}

UINT32 ArchIrqMask(HWI_HANDLE_T hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    VIC_REG->ICER[hwiNum / OS_SYS_VECTOR_CNT] = (UINT32)(1UL << (hwiNum % OS_SYS_VECTOR_CNT));
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 ArchIrqPending(UINT32 hwiNum)
{
    UINT32 intSave;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    VIC_REG->ISPR[hwiNum / OS_SYS_VECTOR_CNT] = (UINT32)(1UL << (hwiNum % OS_SYS_VECTOR_CNT));
    LOS_IntRestore(intSave);
    return LOS_OK;
}

UINT32 ArchIrqClear(UINT32 hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    VIC_REG->ICPR[hwiNum / OS_SYS_VECTOR_CNT] = (UINT32)(1UL << (hwiNum % OS_SYS_VECTOR_CNT));
    return LOS_OK;
}

UINT32 HalCurIrqGet(VOID)
{
    return g_curIrqNum;
}

CHAR *HalIrqVersion(VOID)
{
    return "VIC";
}

HwiHandleInfo *ArchIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

STATIC const HwiControllerOps g_nvicOps = {
    .triggerIrq     = ArchIrqPending,
    .enableIrq      = ArchIrqUnmask,
    .disableIrq     = ArchIrqMask,
    .setIrqPriority = HalIrqSetPriority,
    .getCurIrqNum   = HalCurIrqGet,
    .getIrqVersion  = HalIrqVersion,
    .getHandleForm  = ArchIrqGetHandleForm,
    .clearIrq       = ArchIrqClear,
};

VOID ArchIrqInit(VOID)
{
    UINT32 i;

    for (i = OS_SYS_VECTOR_CNT; i < (LOSCFG_PLATFORM_HWI_LIMIT + OS_SYS_VECTOR_CNT); i++) {
        g_hwiVec[i] = (HWI_PROC_FUNC)IrqEntry;
    }
    g_hwiVec[RESET_INT_NUM] = (HWI_PROC_FUNC)Reset_Handler;
    g_hwiVec[TRAP0_INT_NUM] = (HWI_PROC_FUNC)osPendSV;

    /* register interrupt controller's operations */
    OsHwiControllerReg(&g_nvicOps);

    ArchSetVbr((UINT32) & (g_hwiVec));
    for (int i = 0; i < BYTES_OF_128_INT; i++) {
        VIC_REG->IABR[i] = 0x0;
        VIC_REG->ICPR[i] = MASK_32_BITS;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
