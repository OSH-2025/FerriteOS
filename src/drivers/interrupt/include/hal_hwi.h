/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Hwi Implementation HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#ifndef _HAL_HWI_H
#define _HAL_HWI_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(LOSCFG_CORTEX_M_NVIC)
#define OS_USER_HWI_MIN                 15
#else
#define OS_USER_HWI_MIN                 0
#endif

#if defined(LOSCFG_PLATFORM_HIFIVE1_REV1_B01)
#define OS_USER_HWI_MAX                 (OS_HWI_MAX_NUM - 1)
#else
#define OS_USER_HWI_MAX                 (LOSCFG_PLATFORM_HWI_LIMIT - 1)
#endif

#define HWI_NUM_VALID(num)              (((num) >= OS_USER_HWI_MIN) && ((num) <= OS_USER_HWI_MAX))

extern VOID ArchIrqInit(VOID);
extern VOID HalIrqInitPercpu(VOID);
extern UINT32 ArchIrqMask(UINT32 vector);
extern UINT32 ArchIrqUnmask(UINT32 vector);
extern UINT32 ArchIrqPending(UINT32 vector);
extern UINT32 ArchIrqClear(UINT32 vector);
extern CHAR *HalIrqVersion(VOID);
extern UINT32 HalCurIrqGet(VOID);
extern UINT32 HalIrqSetPrio(UINT32 vector, UINT8 priority);
#ifdef LOSCFG_KERNEL_SMP
extern UINT32 HalIrqSendIpi(UINT32 target, UINT32 ipi);
extern UINT32 HalIrqSetAffinity(UINT32 vector, UINT32 cpuMask);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _HAL_HWI_H */
