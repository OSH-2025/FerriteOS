/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exception Operations HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-09-07
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

#include "los_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_STACK_INIT          0xCACACACA
#define OS_STACK_MAGIC_WORD    0xCCCCCCCC

#define SAVED_REGA_NUM         16
#define SAVED_REGF_NUM         16
#define BASE_AREA_REG_NUM      4

typedef struct {
    UINT32 pc;
    UINT32 ps;
    UINT32 regA[SAVED_REGA_NUM];
    UINT32 sar;
    UINT32 excCause;
    UINT32 excVaddr;
#ifdef LOSCFG_XTENSA_LOOPS
    UINT32 lbeg;
    UINT32 lend;
    UINT32 lcount;
#endif
#ifdef LOSCFG_ARCH_FPU_ENABLE
    UINT32 temp;
    UINT16 cpenable;
    UINT16 cpstored;
    UINT32 fcr;
    UINT32 fsr;
    UINT32 regF[SAVED_REGF_NUM];
#endif
#ifdef LOSCFG_XTENSA_WINDOWSPILL
    UINT32 res[BASE_AREA_REG_NUM];
#endif
} EXC_CONTEXT_S;

STATIC INLINE VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("SYSCALL");
}

#define VECTOR_START           _init_start
#define INIT_VECTOR_START      ((UINTPTR)&VECTOR_START)
extern CHAR *VECTOR_START;

/* *
 * @ingroup los_arch_interrupt
 * Maximum number of used hardware interrupts.
 */
#ifndef OS_HWI_MAX_NUM
#define OS_HWI_MAX_NUM         LOSCFG_PLATFORM_HWI_LIMIT
#endif

/* *
 * @ingroup los_arch_interrupt
 * Highest priority of a hardware interrupt.
 */
#ifndef OS_HWI_PRIO_HIGHEST
#define OS_HWI_PRIO_HIGHEST    0
#endif

#define OS_EXC_IN_INIT         0
#define OS_EXC_IN_TASK         1
#define OS_EXC_IN_HWI          2

/* *
 * @ingroup  los_arch_interrupt
 * Define the type of a hardware interrupt vector table function.
 */
typedef VOID (**HWI_VECTOR_FUNC)(void);

/* *
 * @ingroup los_arch_interrupt
 * Count of Xtensa system interrupt vector.
 */
#define OS_SYS_VECTOR_CNT      0

/* *
 * @ingroup los_arch_interrupt
 * Count of Xtensa interrupt vector.
 */
#define OS_VECTOR_CNT          (OS_SYS_VECTOR_CNT + OS_HWI_MAX_NUM)

/**
 * @ingroup los_exc
 * Exception information structure
 *
 * Description: Exception information saved when an exception is triggered on the Xtensa platform.
 *
 */
typedef struct TagExcInfo {
    UINT16 phase;
    UINT16 type;
    UINT32 faultAddr;
    UINT32 thrdPid;
    UINT16 nestCnt;
    UINT16 reserved;
    EXC_CONTEXT_S *context;
} ExcInfo;

VOID ArchExcInit(VOID);
VOID HakSpillWindow(VOID);
VOID ArchBackTrace(VOID);
VOID ArchBackTraceWithSp(const VOID *stackPointer);

extern UINT32 g_curNestCount;
extern ExcInfo g_excInfo;
extern CHAR _text_start;
extern CHAR _text_end;
extern CHAR _iram_text_start;
extern CHAR _iram_text_end;
extern CHAR __text_start;
extern CHAR __text_end;
extern CHAR __init_stack_s;
extern CHAR __init_stack_e;

#define MAX_INT_INFO_SIZE      (8 + 0x164)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_EXCEPTION_H */
