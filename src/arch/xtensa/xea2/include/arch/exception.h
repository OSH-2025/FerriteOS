/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Exception HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-01-01
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
 * @defgroup los_exc Exception
 * @ingroup kernel
 */

#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#ifndef __ASSEMBLER__
#include "los_typedef.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_MAX_BACKTRACE        100

#define OS_STACK_INIT           0xCACACACA
#define OS_STACK_MAGIC_WORD     0xCCCCCCCC

#define OS_EXC_STAGE_INIT_VALUE 0xFFFFFFFF
#define OS_EXC_STAGE_INIT       0
#define OS_EXC_STAGE_TASK       1
#define OS_EXC_STAGE_IRQ        2
#define OS_EXC_STAGE_EXC        3

/* Offset of ExcContext's field */
#define EXC_REG_OFF_EXCVADDR 0
#define EXC_REG_OFF_AR0      (EXC_REG_OFF_EXCVADDR + 4)
#define EXC_REG_OFF_AR1      (EXC_REG_OFF_AR0 + 4)
#define EXC_REG_OFF_AR2      (EXC_REG_OFF_AR1 + 4)
#define EXC_REG_OFF_AR3      (EXC_REG_OFF_AR2 + 4)
#define EXC_REG_OFF_EPC1     (EXC_REG_OFF_AR3 + 4)
#define EXC_REG_OFF_PS       (EXC_REG_OFF_EPC1 + 4)
#define EXC_REG_OFF_EXCCAUSE (EXC_REG_OFF_PS + 4)
#define EXC_REG_OFF_BR       (EXC_REG_OFF_EXCCAUSE + 4)
#define EXC_REG_OFF_LBEG     (EXC_REG_OFF_BR + 4)
#define EXC_REG_OFF_LEND     (EXC_REG_OFF_LBEG + 4)
#define EXC_REG_OFF_LCOUNT   (EXC_REG_OFF_LEND + 4)
#define EXC_REG_OFF_SAR      (EXC_REG_OFF_LCOUNT + 4)
#define EXC_REG_OFF_AR4      (EXC_REG_OFF_SAR + 4)
#define EXC_REG_OFF_AR5      (EXC_REG_OFF_AR4 + 4)
#define EXC_REG_OFF_AR6      (EXC_REG_OFF_AR5 + 4)
#define EXC_REG_OFF_AR7      (EXC_REG_OFF_AR6 + 4)
#define EXC_REG_OFF_AR8      (EXC_REG_OFF_AR7 + 4)
#define EXC_REG_OFF_AR9      (EXC_REG_OFF_AR8 + 4)
#define EXC_REG_OFF_AR10     (EXC_REG_OFF_AR9 + 4)
#define EXC_REG_OFF_AR11     (EXC_REG_OFF_AR10 + 4)
#define EXC_REG_OFF_AR12     (EXC_REG_OFF_AR11 + 4)
#define EXC_REG_OFF_AR13     (EXC_REG_OFF_AR12 + 4)
#define EXC_REG_OFF_AR14     (EXC_REG_OFF_AR13 + 4)
#define EXC_REG_OFF_AR15     (EXC_REG_OFF_AR14 + 4)
#define EXC_REG_SIZE         (EXC_REG_OFF_AR15 + 4)

#ifndef __ASSEMBLER__
typedef struct {
    UINT32 ar0;
    UINT32 ar1;
    UINT32 ar2;
    UINT32 ar3;
    UINT32 epc;
    UINT32 ps;
    UINT32 excCause;
    UINT32 br;
    UINT32 lbeg;
    UINT32 lend;
    UINT32 lcount;
    UINT32 sar;
    UINT32 ar4;
    UINT32 ar5;
    UINT32 ar6;
    UINT32 ar7;
    UINT32 ar8;
    UINT32 ar9;
    UINT32 ar10;
    UINT32 ar11;
    UINT32 ar12;
    UINT32 ar13;
    UINT32 ar14;
    UINT32 ar15;
} IntContext;

typedef struct {
    UINT32 excAddr;
    UINT32 ar0;
    UINT32 ar1;
    UINT32 ar2;
    UINT32 ar3;
    UINT32 epc;
    UINT32 ps;
    UINT32 excCause;
    UINT32 br;
    UINT32 lbeg;
    UINT32 lend;
    UINT32 lcount;
    UINT32 sar;
    UINT32 ar4;
    UINT32 ar5;
    UINT32 ar6;
    UINT32 ar7;
    UINT32 ar8;
    UINT32 ar9;
    UINT32 ar10;
    UINT32 ar11;
    UINT32 ar12;
    UINT32 ar13;
    UINT32 ar14;
    UINT32 ar15;
} ExcContext;

typedef struct {
    UINT16 phase;           /* Exception occurrence phase: 0 indicates that the exception occurs during
                             * initialization, 1 indicates that the exception occurs during task, 2
                             * indicates that the exception occurs during interrupt, and 3 indicates that
                             * the exception occurs during exception.
                             */
    UINT16 type;            /* Type of exception, refer to no. 1-19 listed above for exceptions */
    UINT32 faultAddr;       /* A precise address access error indicates the error access address where
                             * the exception occurred.
                             */
    UINT32 thrdPid;         /* An exception occurs in the interrupt, indicating the interrupt number. An
                             * exception occurs in the task, indicating the task id, or 0xffffffff if it
                             * occurs during initialization
                             */
    UINT16 nestCnt;         /* The number of nested exceptions, currently only support the first time the
                             * exception into the implementation of the registered hook function
                             */
} ExcInfo;

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("SYSCALL");
}

extern VOID *g_cpOwner;
extern VOID ArchExcInit(VOID);
typedef VOID (* EXC_PROC_FUNC)(UINT32 excType, const ExcContext *excBufAddr);
extern UINT32 ArchSetExcHook(EXC_PROC_FUNC excHook);
extern EXC_PROC_FUNC ArchGetExcHook(VOID);
extern VOID ArchDumpRegsAroundMem(const UINT32 *registers, UINT32 start, UINT32 end, UINT32 offset);
extern VOID ArchBackTrace(VOID);
extern VOID ArchBackTraceWithSp(const VOID *stackPointer);
extern UINT32 ArchBackTraceGet(UINTPTR sp, UINT32 retAddr, UINTPTR *callChain, UINT32 maxDepth);

typedef VOID (*NMI_PROC_FUNC)(const VOID *excBufAddr);
extern UINT32 ArchSetNMIHook(NMI_PROC_FUNC nmiHook);
extern NMI_PROC_FUNC ArchGetNMIHook(VOID);

#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */
