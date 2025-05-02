/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Low Level Impelmentations Headfile
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

#ifndef _ARCH_TASK_H
#define _ARCH_TASK_H

#ifndef __ASSEMBLER__
#include "los_typedef.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LOSCFG_STACK_POINT_ALIGN_SIZE  4

#define OS_HW_AR0_SAVE_OFFSET          16
#define OS_HW_AR1_SAVE_OFFSET          12
#define TASK_CONTEXT_REG_NUM           12        /* 12 is the registers a4~a15 */

/* Offset of TaskContext field */
#define REG_OFF_AR0      (-16)
#define REG_OFF_AR1      (REG_OFF_AR0 + 4)
#define REG_OFF_AR2      (REG_OFF_AR1 + 4)
#define REG_OFF_AR3      (REG_OFF_AR2 + 4)
#define REG_OFF_PC       (REG_OFF_AR3 + 4)
#define REG_OFF_PS       (REG_OFF_PC + 4)
#define REG_OFF_EXCCAUSE (REG_OFF_PS + 4)
#define REG_OFF_BR       (REG_OFF_EXCCAUSE + 4)
#define REG_OFF_LBEG     (REG_OFF_BR + 4)
#define REG_OFF_LEND     (REG_OFF_LBEG + 4)
#define REG_OFF_LCOUNT   (REG_OFF_LEND + 4)
#define REG_OFF_SAR      (REG_OFF_LCOUNT + 4)
#define REG_OFF_AR4      (REG_OFF_SAR + 4)
#define REG_OFF_AR5      (REG_OFF_AR4 + 4)
#define REG_OFF_AR6      (REG_OFF_AR5 + 4)
#define REG_OFF_AR7      (REG_OFF_AR6 + 4)
#define REG_OFF_AR8      (REG_OFF_AR7 + 4)
#define REG_OFF_AR9      (REG_OFF_AR8 + 4)
#define REG_OFF_AR10     (REG_OFF_AR9 + 4)
#define REG_OFF_AR11     (REG_OFF_AR10 + 4)
#define REG_OFF_AR12     (REG_OFF_AR11 + 4)
#define REG_OFF_AR13     (REG_OFF_AR12 + 4)
#define REG_OFF_AR14     (REG_OFF_AR13 + 4)
#define REG_OFF_AR15     (REG_OFF_AR14 + 4)
#define REG_STRUCT_SIZE  (REG_OFF_AR15 + 4)      /* REG_STRUCT_SIZE = sizeof(TaskContext) */

#define REG_CONTEXT_SIZE               (REG_STRUCT_SIZE + 32)
#define FASTSWITCH_REG_CONTEXT_SIZE    (REG_OFF_BR + 4 + 16)

#ifndef __ASSEMBLER__
/* used for all task's stack init */
typedef struct {
    UINT32 pc;
    UINT32 ps;
    UINT32 excCause;
    UINT32 br;
    UINT32 lbeg;
    UINT32 lend;
    UINT32 lcount;
    UINT32 sar;
    UINT32 arg[TASK_CONTEXT_REG_NUM]; /* 12: registers a4~a15 */
} TaskContext;

typedef struct {
    VOID *runTask;
    VOID *newTask;
} LosTask;
extern LosTask g_losTask;

STATIC INLINE VOID *ArchCurrTaskGet(VOID)
{
    return g_losTask.newTask;
}

STATIC INLINE VOID ArchCurrTaskSet(VOID *val)
{
    g_losTask.newTask = val;
}

extern VOID *OsTaskStackInit(UINT32 taskId, UINT32 stackSize, VOID *topStack);
#endif /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
