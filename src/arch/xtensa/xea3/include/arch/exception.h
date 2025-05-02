/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Aarch32 Exception HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2018-08-01
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

/** @defgroup los_exc Exception handling
 *  @ingroup kernel
 */
#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#include "los_typedef.h"
#include "arch/interrupt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Initial bit32 stack value. */
#define OS_STACK_INIT       0xCACACACA
/* Bit32 stack top magic number. */
#define OS_STACK_MAGIC_WORD 0xCCCCCCCC

typedef struct {
    UINT32 AR0; // AR0-AR7
    UINT32 AR1;
    UINT32 AR2;
    UINT32 AR3;
    UINT32 AR4;
    UINT32 AR5;
    UINT32 AR6;
    UINT32 AR7;
    UINT32 RESERVED;
    UINT32 LCOUNT;
    UINT32 LEND;
    UINT32 LBEG;
    UINT32 EPC;
    UINT32 PS;
    UINT32 EXCCAUSE;
    UINT32 EXCVADDR;
    UINT32 AR8; // AR8-AR15
    UINT32 AR9;
    UINT32 AR10;
    UINT32 AR11;
    UINT32 AR12;
    UINT32 AR13;
    UINT32 AR14;
    UINT32 AR15;
} ExcContext;

enum BackTraceType {
    OS_NORMAL_TASK_BT = 0,
    OS_INTEXC_TASK_BT = 1,
    OS_IMPOSSIBLE_BT  = 2,
};

#define OS_EXC_SHOW_MEM_BYTES                  16
#define OS_EXC_MAX_MEM_INFO_SIZE               (OS_EXC_SHOW_MEM_BYTES * 2 + OS_EXC_SHOW_MEM_BYTES)

/**
 * @ingroup los_exc
 * @brief the default exception handler
 */
extern VOID OsExcDefaultHandler(VOID);

extern VOID LOS_TasksDump(VOID);

#define ArchGetFp()

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    (VOID)ArchIntLock();
    while (1) {
    }
}

VOID ArchBackTraceWithSp(const VOID *stackPointer);
VOID ArchBackTrace(VOID);
VOID ArchExcInit(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _ARCH_EXCEPTION_H */
