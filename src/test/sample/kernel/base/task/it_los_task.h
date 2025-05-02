/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Task Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

#ifndef _IT_LOS_TASK_H
#define _IT_LOS_TASK_H

#include "ostest.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define STACK_SIZE 0x800

extern volatile UINT64 g_timesliceTestCount1;
extern volatile INT32 g_timesliceTestCount;
extern UINT8 g_testUserTaskStack[STACK_SIZE];
extern UINT32 OsShellCmdTaskCntGet(VOID);

extern VOID ItLosTask001(VOID);
extern VOID ItLosTask002(VOID);
extern VOID ItLosTask003(VOID);
extern VOID ItLosTask004(VOID);
extern VOID ItLosTask005(VOID);
extern VOID ItLosTask006(VOID);
extern VOID ItLosTask007(VOID);
extern VOID ItLosTask008(VOID);
extern VOID ItLosTask009(VOID);
extern VOID ItLosTask010(VOID);
extern VOID ItLosTask011(VOID);
#ifdef LOSCFG_BASE_CORE_TIMESLICE
extern VOID ItLosTaskTimeslice001(VOID);
#endif
#ifdef LOSCFG_TASK_STATIC_ALLOCATION
extern VOID ItLosTaskUser001(VOID);
extern VOID ItLosTaskUser002(VOID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _IT_LOS_TASK_H */