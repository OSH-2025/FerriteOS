/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: CPU Register Defines Headfile
 * Author: Huawei LiteOS Team
 * Create: 2021-01-14
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

#ifndef _ARCH_REGS_H
#define _ARCH_REGS_H

#include <xtensa/corebits.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_XTENSA_HIFI3Z
#define OS_INTLEVEL_EXCM                 4  /* cannot be higher than debug level */
#endif

#define OS_PS_WOE                        0x40000
#define OS_PS_CALLINC(n)                 (((n) & 0x3) << 16) /* n = 0..3 */
#define OS_PS_OWB(n)                     (((n) & 0xf) << 8)  /* n = 0..15 (or 0..7) */
#define OS_PS_RING(n)                    (((n) & 0x3) << 6)  /* n = 0..3 */
#define OS_PS_UM                         0x20
#define OS_PS_EXCM                       0x10
#define OS_PS_INTLEVEL(n)                ((n) & 0xf)         /* n = 0..15 */
#define OS_PS_OWB_SHIFT                  8
#define OS_PS_OWB_BITS                   4
#define OS_PS_VECTOR_VALUE               (OS_PS_WOE | OS_PS_CALLINC(1) | OS_PS_INTLEVEL(OS_INTLEVEL_EXCM))

/* redefine xea2 exccause num:[0, 39] */
#define OS_EXCCAUSE_LEVEL1_INT           EXCCAUSE_LEVEL1INTERRUPT
#define OS_EXCCAUSE_INT                  OS_EXCCAUSE_LEVEL1_INT
#define OS_EXCCAUSE_ALLOCA_CAUSE         EXCCAUSE_ALLOCA
#define OS_EXCCAUSE_COPROCESSOR_1        EXCCAUSE_CP1_DISABLED /* Access to Coprocessor 1 when disabled */
/* self-defined exccause info */
#define OS_EXCCAUSE_SWITCH               64
#define OS_EXCCAUSE_INITIAL              65
#define OS_EXCCAUSE_ECCERROR             68
#define OS_EXCCAUSE_NMIERROR             69
#define OS_EXCCAUSE_RESET_REENTRY        70
#define OS_EXCCAUSE_DOUBLEERROR          71
#define OS_EXCCAUSE_FATALERROR           72
#define OS_EXCCAUSE_STACKOVERFLOW        73
#define OS_EXCCAUSE_ECC_CORRECTABLEERROR 77

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_REGS_H */
