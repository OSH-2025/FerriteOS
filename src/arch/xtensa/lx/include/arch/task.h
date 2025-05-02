/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Operations HeadFile
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
 * --------------------------------------------------------------------------- */

/**
 * @defgroup los_task
 * @ingroup kernel
 */

#ifndef _ARCH_TASK_H
#define _ARCH_TASK_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern VOID *g_newTask;

#define LOSCFG_STACK_POINT_ALIGN_SIZE 16
#define SAVED_REG_NUM                 8
#define SPILL_WINDOW_SIZE             8

#define SAVED_REGA_NUM                16
#define SAVED_REGF_NUM                16
#define BASE_AREA_REG_NUM             4

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
} TaskContext;

STATIC INLINE VOID *ArchCurrTaskGet(VOID)
{
    return g_newTask;
}

STATIC INLINE VOID ArchCurrTaskSet(VOID *val)
{
    g_newTask = val;
}

extern VOID *OsTaskStackInit(UINT32 taskId, UINT32 uwStackSize, VOID *pTopStack);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_TASK_H */
