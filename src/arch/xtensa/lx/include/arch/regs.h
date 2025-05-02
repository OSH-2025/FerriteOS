/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: CPU Register Defines Headfile
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

/**
 * @defgroup Special Register Fields and Values
 * @ingroup kernel
 */

#ifndef _ARCH_REGS_H
#define _ARCH_REGS_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PS_UM_MASK                               0x00000020
#define SPREG_PS_DI_MASK                         0x00000008
#define SPREG_PS_STACK_CROSS                     0x00000020

/**
 * @ingroup Schedule Flag stored on Task Context
 */
#define OS_SCHED_FLAG_TASKPREEMT                 4 /* Task Preemted through LOS_Schedule */

/**
 * @ingroup Context Fields Define
 */
#define CONTEXT_OFF_EPC                          (0)
#define CONTEXT_OFF_PC                           (0)
#define CONTEXT_OFF_PS                           (4)
#define CONTEXT_OFF_A0                           (8)
#define CONTEXT_OFF_A1                           (12)
#define CONTEXT_OFF_A2                           (16)
#define CONTEXT_OFF_A3                           (20)
#define CONTEXT_OFF_A4                           (24)
#define CONTEXT_OFF_A5                           (28)
#define CONTEXT_OFF_A6                           (32)
#define CONTEXT_OFF_A7                           (36)
#define CONTEXT_OFF_A8                           (40)
#define CONTEXT_OFF_A9                           (44)
#define CONTEXT_OFF_A10                          (48)
#define CONTEXT_OFF_A11                          (52)
#define CONTEXT_OFF_A12                          (56)
#define CONTEXT_OFF_A13                          (60)
#define CONTEXT_OFF_A14                          (64)
#define CONTEXT_OFF_A15                          (68)
#define CONTEXT_OFF_SAR                          (72)
#define CONTEXT_OFF_EXCCAUSE                     (76)
#define CONTEXT_OFF_EXCVADDR                     (80)
#define CONTEXT_COMMON_SIZE                      (84)

#ifdef LOSCFG_XTENSA_LOOPS
#define CONTEXT_OFF_LBEG                         (84)
#define CONTEXT_OFF_LEND                         (88)
#define CONTEXT_OFF_LCOUNT                       (92)
#define CONTEXT_LOOPS_SIZE                       (12)
#else 
#define CONTEXT_LOOPS_SIZE                       (0)
#endif

#define LEVEL1_TINRERRUPT_CAUSE                  (4)

#ifdef LOSCFG_XTENSA_WINDOWSPILL
#define ALLOC_CAUSE                              (5)
#define LOWBIT_CALLNMODE                         (30)
#define HIGHBIT_CALLNMODE                        (31)
#define PS_OWB_SHIFT                             (8)
#define CONTEXT_WINDOWSPILL_SIZE                 (16)
#define INDEX_OF_ARGS0                           (6)
#define WINDOWSTARTBITS	                         (16)	/* window start reg bits */
#define WINDOWBASEBITS	                         (4)	/* window base reg bits */
#define WINDOWSTARTMASK	                         ((1 << WINDOWSTARTBITS) - 1)
#define WOE_ENABLE                               (0x40000)
#define BIT_CALLINC                              (16)
#else
#define CONTEXT_WINDOWSPILL_SIZE                 (0)
#define INDEX_OF_ARGS0                           (2)
#endif

#ifdef LOSCFG_ARCH_FPU_ENABLE
#define CONTEXT_OFF_TMP0                         (96)
#define CONTEXT_OFF_CPENABLE                     (100)
#define CONTEXT_OFF_CPSTORED                     (102)
#define CONTEXT_OFF_FCR                          (104)
#define CONTEXT_OFF_FSR                          (108)
#define CONTEXT_OFF_F0                           (112)
#define CONTEXT_OFF_F1                           (116)
#define CONTEXT_OFF_F2                           (120)
#define CONTEXT_OFF_F3                           (124)
#define CONTEXT_OFF_F4                           (128)
#define CONTEXT_OFF_F5                           (132)
#define CONTEXT_OFF_F6                           (136)
#define CONTEXT_OFF_F7                           (140)
#define CONTEXT_OFF_F8                           (144)
#define CONTEXT_OFF_F9                           (148)
#define CONTEXT_OFF_F10                          (152)
#define CONTEXT_OFF_F11                          (156)
#define CONTEXT_OFF_F12                          (160)
#define CONTEXT_OFF_F13                          (164)
#define CONTEXT_OFF_F14                          (168)
#define CONTEXT_OFF_F15                          (172)
#define CONTEXT_FPU_SIZE                         (80)
#else    /* LOSCFG_ARCH_FPU_ENABLE */
#define CONTEXT_FPU_SIZE                         (0)
#endif   /* !LOSCFG_ARCH_FPU_ENABLE */

#define CONTEXT_SIZE                             (CONTEXT_FPU_SIZE + CONTEXT_LOOPS_SIZE + \
                                                  CONTEXT_COMMON_SIZE + CONTEXT_WINDOWSPILL_SIZE)
#define EXCCAUSE_LEVEL1INTERRUPT                 4
#define XTENSA_LOGREG_NUM                        16
#define INDEX_OF_SP                              1
#define LEVEL_MASK                               0xf
#define INT_MASK                                 5
#define LEVEL1_INT_MASK                          1
#define USER_VECTOR_MODE                         0x20
#define LEVEL1                                   1
#define LEVEL2                                   2
#define LEVEL3                                   3
#define LEVEL4                                   4
#define LEVEL5                                   5

#ifdef LOSCFG_PLATFORM_ESP8266
#define LOAD_STORE_ERROR_MASK                    (0x700F)
#define STOTE16_OPCODE                           (0x5002)
#define STOTE8_OPCODE                            (0x4002)
#define LOAD16_OPCODE                            (0x1002)
#define LOAD8_OPCODE                             (0x2)
#define LOADSTOREEXP_REG_INDEX                   (0x7)
#define LOADSTOREEXP_TABLEENTRY_SIZE             (0x8)
#endif

#ifdef YES
#undef YES
#endif
#define YES                                      1

#ifdef NO
#undef NO
#endif
#define NO                                       0

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_REGS_H */
