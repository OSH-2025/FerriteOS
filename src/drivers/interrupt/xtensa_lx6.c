/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS Hwi Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-10
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

#include "xtensa_lx6.h"
#include "los_hwi_pri.h"
#include "los_task_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HWI_NUM_SIX   6

LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT];
extern VOID HalUpdateTimerCmpVal(VOID);

UINT32 ArchIrqPending(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    __asm__ __volatile__("wsr %0, intset; rsync" : : "a"(0x1U << hwiNum));

    return LOS_OK;
}

UINT32 ArchIrqUnmask(HWI_HANDLE_T hwiNum)
{
    UINT32 ier;

    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    __asm__ __volatile__("rsr %0, intenable" : "=a"(ier) : : "memory");
    __asm__ __volatile__("wsr %0, intenable; rsync" : : "a"(ier | ((UINT32)0x1U << hwiNum)));

    return LOS_OK;
}

UINT32 ArchIrqMask(HWI_HANDLE_T hwiNum)
{
    UINT32 ier;

    if (!HWI_NUM_VALID(hwiNum)) {
        return OS_ERRNO_HWI_NUM_INVALID;
    }

    __asm__ __volatile__("rsr %0, intenable" : "=a"(ier) : : "memory");
    __asm__ __volatile__("wsr %0, intenable; rsync" : : "a"(ier & ~((UINT32)0x1U << hwiNum)));

    return LOS_OK;
}

__attribute__((section(".iram1"))) VOID ArchInterrupt(VOID)
{
    UINT32 hwiIndex;

    hwiIndex = ArchCurIrqGet();
    ArchIrqClear(hwiIndex);
    OsIntHandle(hwiIndex, &g_hwiForm[hwiIndex]);
    if (hwiIndex == HWI_NUM_SIX) {
        HalUpdateTimerCmpVal();
    }
    if (OsTaskProcSignal() != 0) {
        OsSchedPreempt();
    }
}

UINT32 ArchIrqClear(HWI_HANDLE_T vector)
{
    if (!HWI_NUM_VALID(vector)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    __asm__ __volatile__("wsr %0, intclear; rsync" : : "a"(0x1U << vector));

    return LOS_OK;
}

UINT32 ArchCurIrqGet(VOID)
{
    UINT32 ier;
    UINT32 intenable;
    UINT32 intSave;

    __asm__ __volatile__("rsr %0, interrupt" : "=a"(ier) : : "memory");
    __asm__ __volatile__("rsr %0, intenable" : "=a"(intenable) : : "memory");

    intSave = ier & intenable;

    return  __builtin_ffs(intSave) - 1;
}

STATIC HwiHandleInfo *ArchIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

STATIC const HwiControllerOps g_lx6Ops = {
    .triggerIrq     = ArchIrqPending,
    .clearIrq       = ArchIrqClear,
    .enableIrq      = ArchIrqUnmask,
    .disableIrq     = ArchIrqMask,
    .getHandleForm  = ArchIrqGetHandleForm,
};

VOID ArchIrqInit(VOID)
{
    for (UINT32 i = 0; i < OS_HWI_MAX_NUM; i++) {
        ArchIrqMask(i);
    }
    asm volatile ("wsr %0, vecbase" : : "r"(INIT_VECTOR_START));
    OsHwiControllerReg(&g_lx6Ops);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
