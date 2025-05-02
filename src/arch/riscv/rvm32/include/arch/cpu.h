/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: CPU Operations HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2020-04-18
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

/**
 * @defgroup cpu
 * @ingroup kernel
 */

#ifndef _ARCH_CPU_H
#define _ARCH_CPU_H

#ifndef __ASSEMBLER__
#include "los_typedef.h"
#endif

#ifdef __riscv64
#define LREG                    ld
#define SREG                    sd
#define FLREG                   fld
#define FSREG                   fsd
#define REGBYTES                8
#else
#define LREG                    lw
#define SREG                    sw
#define FLREG                   flw
#define FSREG                   fsw
#define REGBYTES                4
#endif

#ifdef LOSCFG_ARCH_FPU_ENABLE
#define INT_SIZE_ON_STACK       (68 * REGBYTES)
#else
#define INT_SIZE_ON_STACK       (32 * REGBYTES)
#endif

/* risc-v mstatus Register */
#define LOS_MSTATUS_MIE         0x00000008
#define LOS_MSTATUS_MPIE        0x00000080
#define LOS_MSTATUS_MPP         0x00001800
#define LOS_MSTATUS_FS          0x00006000
#ifdef LOSCFG_ARCH_FPU_ENABLE
#define LOSCFG_MSTATUS_M        (LOS_MSTATUS_MPP | LOS_MSTATUS_MPIE | LOS_MSTATUS_FS)
#else
#define LOSCFG_MSTATUS_M        (LOS_MSTATUS_MPP | LOS_MSTATUS_MPIE)
#endif
#define UINT32_CUT_MASK         0xFFFFFFFF
#define UINT8_CUT_MASK          0xFF

#ifndef __ASSEMBLER__
STATIC INLINE UINT32 ArchCurrCpuid(VOID)
{
    return 0;
}

STATIC INLINE UINT32 ArchSPGet(VOID)
{
    UINT32 val;
    __asm__ volatile("mv %0, sp" : "=r"(val));
    return val;
}

STATIC INLINE UINTPTR GetSP(VOID)
{
    UINTPTR spSave;
    __asm__ __volatile__("mv %0, sp" : "=r"(spSave));
    return spSave;
}

STATIC INLINE const CHAR *ArchCpuInfo(VOID)
{
    return "unknown";
}
#endif  /* __ASSEMBLER__ */

#endif
