/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exception Operations HeadFile
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
 * ---------------------------------------------------------------------------- */

#ifndef _ARCH_EXCEPTION_H
#define _ARCH_EXCEPTION_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Initial bit32 stack value. */
#define OS_STACK_INIT       0xCACACACA
/* Bit32 stack top magic number. */
#define OS_STACK_MAGIC_WORD 0xCCCCCCCC
#define OS_MAX_BACKTRACE    10
#define OS_TRAP_STACK_SIZE  500

typedef enum {
    OS_EXC_TYPE_RESET       = 0,
    OS_EXC_TYPE_TRAP        = 16,
    OS_EXC_TYPE_SOFT        = 22
} ExcInfoType;

typedef UINT32 (*EXC_INFO_SAVE_CALLBACK)(UINT32, VOID*);

#define OS_EXC_IN_INIT                  0
#define OS_EXC_IN_TASK                  1
#define OS_EXC_IN_HWI                   2

/**
 * @ingroup los_exc
 * the struct of register files
 *
 * description: the register files that saved when exception triggered
 *
 */
typedef struct tagExcContext {
    UINT32 R0;
    UINT32 R1;
    UINT32 R2;
    UINT32 R3;
    UINT32 R4;
    UINT32 R5;
    UINT32 R6;
    UINT32 R7;
    UINT32 R8;
    UINT32 R9;
    UINT32 R10;
    UINT32 R11;
    UINT32 R12;
    UINT32 R13;
    UINT32 R14;
    UINT32 R15;
    UINT32 EPSR;
    UINT32 EPC;
} ExcContext;

typedef VOID (* EXC_PROC_FUNC)(UINT32, ExcContext *);
VOID OsExcHandleEntry(const ExcContext *excSp, UINT32 faultAddr);
/**
 * @ingroup  los_hwi
 * @brief: Exception initialization.
 *
 * @par Description:
 * This API is used to configure the exception function vector table.
 *
 * @attention:
 * <ul><li>None.</li></ul>
 *
 * @param None.
 *
 * @retval: None
 * @par Dependency:
 * <ul><li>los_hwi.h: the header file that contains the API declaration.</li></ul>
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
VOID OsExcInit(VOID);

typedef struct tagExcInfo {
    UINT16 phase;              /* Phase in which an exception occurs:
                                * 0 means that the exception occurred during initialization,
                                * 1 means that the exception occurred in the task,
                                * 2 means that the exception occurred in the interrupt.
                                */
    UINT16 type;               /* Exception type */
    UINT32 faultAddr;          /* The wrong access address when the exception occurred */
    UINT32 idx;
    UINT16 nestCnt;            /* Count of nested exception */
    UINT16 reserved;           /* Reserved for alignment */
    ExcContext *context;       /* Hardware context when an exception occurs */
} ExcInfo;

extern UINT32 g_curNestCount;
extern CHAR g_trapstackbase[OS_TRAP_STACK_SIZE];
VOID HandleEntry(VOID);

/**
 * @ingroup los_exc
 * @brief Kernel task backtrace function.
 *
 * @par Description:
 * Backtrace function that prints task call stack information traced from the input task.
 * @attention
 * <ul>
 * <li>The input taskID should be valid.</li>
 * </ul>
 *
 * @param  taskID [IN] Type #UINT32 Task ID.
 *
 * @retval #None.
 *
 * @par Dependency:
 * los_exc.h: the header file that contains the API declaration.
 * @see None.
 * @since Huawei LiteOS V100R001C00
 */
VOID OsTaskBackTrace(UINT32 taskId);
VOID ArchExcInit(VOID);

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("doze");
}

STATIC INLINE UINTPTR ArchGetSp(VOID)
{
    UINTPTR regSp;

    __asm__ __volatile__("mov %0, sp\n" : "=r"(regSp));

    return regSp;
}

STATIC INLINE UINTPTR ArchGetIntSp(VOID)
{
    UINTPTR regPsp;

    __asm__ __volatile__("mfcr %0, cr<15, 1>" : "=r"(regPsp));

    return regPsp;
}

VOID ArchBackTrace(VOID);
VOID ArchBackTraceWithSp(const VOID *stackPointer);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */