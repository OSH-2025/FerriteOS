/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: arm control.
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

#include "los_mp_pri.h"
#include "los_hwi_pri.h"
#include "los_task_pri.h"
#include "interrupt_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ARM_IRQ_NUM             8
#define VC_IRQ_NUM              32
#define GPU_IRQ_NUM             32
#define MORE_PENDING_BIT0       8
#define MORE_PENDING_BIT1       9
#define ARM_IRQ_PEND_MASK       0x3FF
#define ARM_IRQ_MASK            0xFF
#define VC_IRQ_MASK             0xFFFFFFFF
#define GPU_IRQ_MASK            0xFFFFFFFF
#define MAILBOX_NUM_PER_CORE    4

STATIC UINT32 g_curIrqNum = 0;

LITE_OS_SEC_BSS HwiHandleInfo g_hwiForm[LOSCFG_PLATFORM_HWI_LIMIT] = { 0 };

LITE_OS_SEC_TEXT_MINOR VOID IrqEntryArmControl(VOID)
{
    UINT32 hwiIndex;

    hwiIndex = HalCurIrqGet();
    if (hwiIndex == ARM_IRQ_MASK) {
        return;
    }

    ArchIrqClear(hwiIndex);
    OsIntHandle(hwiIndex, &g_hwiForm[hwiIndex]);
    return;
}

UINT32 ArchIrqUnmask(UINT32 hwiNum)
{
    INTERRUPTS_INFO *irq = IRQ_REG_BASE;
    MAILBOXES_INFO *coreirq = CORE_MAILBOX_REG_BASE;
    UINT32 currCpuid = ArchCurrCpuid();

    if (hwiNum < ARM_IRQ_NUM) {
        irq->enableBasicIRQs |= (1 << hwiNum);
    } else if ((hwiNum < VC_IRQ_NUM) && (hwiNum >= ARM_IRQ_NUM)) {
        irq->enableIRQs[0] |= (1 << hwiNum);
    } else if ((hwiNum >= VC_IRQ_NUM) && (hwiNum < CNTPS_IRQ)) {
        hwiNum = hwiNum % GPU_IRQ_NUM;
        irq->enableIRQs[1] |= (1 << hwiNum);
    } else if ((hwiNum >= CNTPS_IRQ) && (hwiNum <= CNTV_IRQ)) {
        coreirq->CoreTimeIRQ[currCpuid] |= (1 << (hwiNum - CNTPS_IRQ));
    } else if ((hwiNum >= MAILBOX0_IRQ) && (hwiNum <= MAILBOX3_IRQ)) {
        coreirq->IRQControl[currCpuid] |= (1 << (hwiNum - MAILBOX0_IRQ));
    } else {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    return LOS_OK;
}

UINT32 ArchIrqMask(HWI_HANDLE_T hwiNum)
{
    INTERRUPTS_INFO *irq = IRQ_REG_BASE;
    MAILBOXES_INFO *coreirq = CORE_MAILBOX_REG_BASE;
    UINT32 currCpuid = ArchCurrCpuid();

    if (hwiNum < ARM_IRQ_NUM) {
        irq->disableBasicIRQs |= (1 << hwiNum);
    } else if ((hwiNum < VC_IRQ_NUM) && (hwiNum >= ARM_IRQ_NUM)) {
        irq->disableIRQs[0] |= (1 << hwiNum);
    } else if ((hwiNum >= VC_IRQ_NUM) && (hwiNum < CNTPS_IRQ))  {
        hwiNum = hwiNum % GPU_IRQ_NUM;
        irq->disableIRQs[1] |= (1 << hwiNum);
    } else if ((hwiNum >= CNTPS_IRQ) && (hwiNum <= CNTV_IRQ)) {
        coreirq->CoreTimeIRQ[currCpuid] &= ~(1 << (hwiNum - CNTPS_IRQ));
    } else if ((hwiNum >= MAILBOX0_IRQ) && (hwiNum <= MAILBOX3_IRQ)) {
        coreirq->IRQControl[currCpuid] &= ~(1 << (hwiNum - MAILBOX0_IRQ));
    } else {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }
    return LOS_OK;
}

UINT32 HalCurIrqGet(VOID)
{
    UINT32 value;
    UINT32 irqNum = ARM_IRQ_MASK;
    INTERRUPTS_INFO *irq = IRQ_REG_BASE;
    MAILBOXES_INFO *coreIrq = CORE_MAILBOX_REG_BASE;
    UINT32 currCpuid = ArchCurrCpuid();

    value = irq->basicPending & ARM_IRQ_PEND_MASK;
    if (value) {
        if (value & (1 << MORE_PENDING_BIT0)) {
            value = irq->pending[0];
            irqNum = FFS(value) - 1;
        } else if (value & (1 << MORE_PENDING_BIT1)) {
            value = irq->pending[1];
            irqNum = FFS(value) + VC_IRQ_NUM - 1;
        } else {
            value &= ARM_IRQ_MASK;
            irqNum = FFS(value) - 1;
        }
        g_curIrqNum = irqNum;
    } else {
        value = coreIrq->IRQSource[currCpuid];
        if (value &= ARM_IRQ_MASK) {
            irqNum = FFS(value) - 1;
            irqNum += CNTPS_IRQ;
            g_curIrqNum = irqNum;
        }
    }
    return irqNum;
}

UINT32 ArchIrqClear(UINT32 hwiNum)
{
    UINT32 currCpuid = ArchCurrCpuid();
    ARMTIMER_INFO *timer = ARMTIMER_REG_BASE;
    MAILBOXES_INFO *coreIrq = CORE_MAILBOX_REG_BASE;
    UINT32 value;
    if (!HWI_NUM_VALID(hwiNum)) {
        printf("HWI_NUM_VALID\n");
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    if (hwiNum == ARM_TIMER_INI) {
        timer->IRQClear = 0;
    } else if ((hwiNum >= MAILBOX0_IRQ) && (hwiNum <= MAILBOX3_IRQ)) {
        value = coreIrq->readClear[MAILBOX_NUM_PER_CORE * currCpuid + hwiNum - MAILBOX0_IRQ];
        coreIrq->readClear[MAILBOX_NUM_PER_CORE * currCpuid + hwiNum - MAILBOX0_IRQ] = value;
    }
    return LOS_OK;
}

UINT32 ArchIrqPending(UINT32 hwiNum)
{
    UINT32 intSave;
    UINT32 currCpuid = ArchCurrCpuid();
    MAILBOXES_INFO *coreIrq = CORE_MAILBOX_REG_BASE;

    if (!HWI_NUM_VALID(hwiNum)) {
        return LOS_ERRNO_HWI_NUM_INVALID;
    }

    intSave = LOS_IntLock();
    if ((hwiNum >= MAILBOX0_IRQ) && (hwiNum <= MAILBOX3_IRQ)) {
        coreIrq->writeSet[MAILBOX_NUM_PER_CORE * currCpuid + hwiNum - MAILBOX0_IRQ] = 1;
    }
    LOS_IntRestore(intSave);
    return LOS_OK;
}

CHAR *HalIrqVersion(VOID)
{
    return "arm control";
}

HwiHandleInfo *ArchIrqGetHandleForm(HWI_HANDLE_T hwiNum)
{
    if (!HWI_NUM_VALID(hwiNum)) {
        return NULL;
    }

    return &g_hwiForm[hwiNum];
}

#ifdef LOSCFG_KERNEL_SMP
UINT32 HalIrqSendIpi(UINT32 target, UINT32 ipi)
{
    UINT32 i;
    MAILBOXES_INFO *coreIrq = CORE_MAILBOX_REG_BASE;

    for (i = 0; i < LOSCFG_KERNEL_CORE_NUM; i++) {
        if (target & (1 << i)) {
            /* per cpu 4 mailbox */
            coreIrq->writeSet[i * 4 + MAILBOX1_IRQ - MAILBOX0_IRQ] = (1 << ipi);
        }
    }
    return LOS_OK;
}
#endif

STATIC const HwiControllerOps g_armControlOps = {
    .enableIrq      = ArchIrqUnmask,
    .disableIrq     = ArchIrqMask,
    .getCurIrqNum   = HalCurIrqGet,
    .getIrqVersion  = HalIrqVersion,
    .getHandleForm  = ArchIrqGetHandleForm,
    .handleIrq      = IrqEntryArmControl,
    .clearIrq       = ArchIrqClear,
    .triggerIrq     = ArchIrqPending,
#ifdef LOSCFG_KERNEL_SMP
    .sendIpi            = HalIrqSendIpi,
    .setIrqCpuAffinity  = NULL,
#endif
};

#ifdef LOSCFG_KERNEL_SMP
UINT32 HalSmpIrqInit(VOID)
{
    UINT32 ret;

    /* register inter-processor interrupt */
    ret = LOS_HwiCreate(MAILBOX0_IRQ, OS_HWI_PRIO_LOWEST, 0, OsMpWakeHandler, 0);
    if (ret != LOS_OK) {
        return ret;
    }
    ret = LOS_HwiCreate(MAILBOX1_IRQ, OS_HWI_PRIO_LOWEST, 0, OsMpScheduleHandler, 0);
    if (ret != LOS_OK) {
        return ret;
    }
    ret = LOS_HwiCreate(MAILBOX2_IRQ, OS_HWI_PRIO_LOWEST, 0, OsMpScheduleHandler, 0);
    if (ret != LOS_OK) {
        return ret;
    }
#ifdef LOSCFG_KERNEL_SMP_CALL
    ret = LOS_HwiCreate(MAILBOX3_IRQ, OS_HWI_PRIO_LOWEST, 0, OsMpFuncCallHandler, 0);
    if (ret != LOS_OK) {
        return ret;
    }
#endif
    return LOS_OK;
}
#endif

VOID HalIrqInitPercpu(VOID)
{
    INTERRUPTS_INFO * irq = IRQ_REG_BASE;
    /* mask all of interrupts */
    irq->disableBasicIRQs = ARM_IRQ_MASK;
    irq->disableIRQs[0] = VC_IRQ_MASK;
    irq->disableIRQs[1] = GPU_IRQ_MASK;

#ifndef LOSCFG_ARCH_ARM_AARCH64
    asm ("mrc p15, #0, r1, c1, c0, #0\n"
         "bic r1, #(1 << 13)\n"
         "ldr r0, =system_vectors\n"
         "mcr p15, #0, r0, c12, c0, #0\n"
         "dsb\n");
#endif
}

VOID ArchIrqInit(VOID)
{
    HalIrqInitPercpu();
    /* register interrupt controller's operations */
    OsHwiControllerReg(&g_armControlOps);
#ifdef LOSCFG_KERNEL_SMP
    UINT32 ret = HalSmpIrqInit();
    if (ret != LOS_OK) {
        PRINT_ERR("HalSmpIrqInit failed, ret:0x%x\n", ret);
    }
#endif
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
