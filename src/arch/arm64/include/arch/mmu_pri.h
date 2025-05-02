/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Arm64 Mmu Inner HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2019-01-01
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

#ifndef _ARCH_MMU_PRI_H
#define _ARCH_MMU_PRI_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MMU_1K 0x400
#define MMU_4K 0x1000
#define MMU_16K 0x4000
#define MMU_64K 0x10000
#define MMU_1M 0x100000
#define MMU_2M 0x200000
#define MMU_1G 0x40000000
#define MMU_4G 0x100000000UL

#define SHIFT_1K 10
#define SHIFT_4K 12
#define SHIFT_16K 14
#define SHIFT_64K 16
#define SHIFT_1M 20
#define SHIFT_2M 21
#define SHIFT_1G 30

extern VOID OsNoCachedRemap(UINTPTR physAddr, size_t size);
extern VOID OsCachedRemap(UINTPTR physAddr, size_t size);

extern VOID OsBlockMapsSet(UINT64 flags, UINT64 start, UINT64 end);
extern VOID OsBlockMapsInit(UINT64 flags, UINT64 start, UINT64 end);

extern VOID OsSysSecPteInit(UINTPTR startAddr, UINTPTR len, UINT64 flag);
extern VOID OsAppSecPteInit(UINTPTR startAddr, UINTPTR len, UINT64 flag);
extern VOID ArchCodeProtect(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_MMU_PRI_H */
