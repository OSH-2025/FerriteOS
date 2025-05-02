/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: VIC(Vectored Interrupt Controller) for C-Sky HeadFile
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

#ifndef _VIC_H
#define _VIC_H

#include "los_hwi.h"
#include "los_hwi_pri.h"
#include "asm/platform.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_hwi
 * Highest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_HIGHEST
#define OS_HWI_PRIO_HIGHEST         0
#endif

/**
 * @ingroup los_hwi
 * Lowest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_LOWEST
#define OS_HWI_PRIO_LOWEST          (LOSCFG_HWI_PRIO_LIMIT)
#endif

/**
 * @ingroup los_hwi
 * Count of CK802-Core system interrupt vector.
 */
#define OS_SYS_VECTOR_CNT           32

/**
 * @ingroup los_hwi
 * AIRCR register priority group parameter .
 */
#define OS_CSKY_AIRCR_PRIGROUP      4
#define PSR_VEC_OFFSET              16U

#define VIC_REG                     ((VIC_TYPE *) 0xE000E100UL)

typedef struct {
    UINT32 ISER[4U];
    UINT32 RESERVED0[12U];
    UINT32 IWER[4U];
    UINT32 RESERVED1[12U];
    UINT32 ICER[4U];
    UINT32 RESERVED2[12U];
    UINT32 IWDR[4U];
    UINT32 RESERVED3[12U];
    UINT32 ISPR[4U];
    UINT32 RESERVED4[12U];
    UINT32 ISSR[4U];
    UINT32 RESERVED5[12U];
    UINT32 ICPR[4U];
    UINT32 RESERVED6[12U];
    UINT32 ICSR[4U];
    UINT32 RESERVED7[12U];
    UINT32 IABR[4U];
    UINT32 RESERVED8[60U];
    UINT32 IPR[32U];
    UINT32 RESERVED9[480U];
    UINT32 ISR;
    UINT32 IPTR;
    UINT32 TSPEND;
    UINT32 TSABR;
    UINT32 TSPR;
} VIC_TYPE;

/* hardware interrupt entry */
extern VOID IrqEntryCsky(VOID);

/* Reset handle entry */
extern VOID Reset_Handler(VOID);

extern VOID osPendSV(VOID);

extern VOID IrqEntry(VOID);
extern LITE_OS_SEC_TEXT VOID OsTickEntry(VOID);
/*
 * @ingroup los_hwi
 * Set interrupt vector table.
 */
#define OsSetVector(num, vector) do {                               \
    g_hwiVec[num + OS_SYS_VECTOR_CNT] = (HWI_PROC_FUNC)IrqEntryV7M; \
    g_hwiForm[num + OS_SYS_VECTOR_CNT].pfnHook = vector;            \
} while (0)

extern HWI_PROC_FUNC g_hwiVec[LOSCFG_PLATFORM_HWI_LIMIT + OS_SYS_VECTOR_CNT];

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _VIC_H */
