/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: los_config system configuration items
 * Author: Huawei LiteOS Team
 * Create: 2018-05-14
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
 * @defgroup Special Register Fields and Values
 * @ingroup kernel
 */

#ifndef _ARCH_REGS_H
#define _ARCH_REGS_H

/* PS register -- imprecise exception */
#define SPREG_PS_DEPC_SHIFT                                  4
#define SPREG_PS_DEPC_MASK                                   0x00000004
#define SPREG_PS_DEPC                                        SPREG_PS_DEPC_MASK
/* PS register -- interrupt part */
#define SPREG_PS_DI_SHIFT                                    3
#define SPREG_PS_DI_MASK                                     0x00000008
#define SPREG_PS_DI                                          SPREG_PS_DI_MASK
#define SPREG_PS_DI_DEPC                                     0x0000000C
/* PS register -- stack part */
#define SPREG_PS_STACK_SHIFT                                 5
#define SPREG_PS_STACK_MASK                                  0x000000E0
#define SPREG_PS_STACK_INTERRUPT                             0x00000000    /* 0, exec level 4 -- Interrupt handler */
#define SPREG_PS_STACK_CROSS                                 0x00000020    /* 1, exec level 4 -- Interrupt handler */
#define SPREG_PS_STACK_IDLE                                  0x00000040    /* 2 */
#define SPREG_PS_STACK_KERNEL                                0x00000060    /* 3, exec level 4 -- Exception Handler */
#define SPREG_PS_STACK_PAGE                                  0x000000E0    /* 7 */
#define SPREG_PS_STACK_FIRSTINT                              0x00000080    /* 4, exec level 4 -- Application Code */
#define SPREG_PS_STACK_FIRSTKER                              0x000000A0    /* 5, exec level 4 -- Application Code */
/* PS register -- entry no rotate */
#define SPREG_PS_ENTRYNR_SHIFT                               22
#define SPREG_PS_ENTRYNR_MASK                                0x00400000
#define SPREG_PS_ENTRYNR                                     SPREG_PS_ENTRYNR_MASK

/* Exccause Register  -- cause */
#define SPREG_EXCCAUSE_CAUSE_SHIFT                           0
#define SPREG_EXCCAUSE_CAUSE_BITS                            4
#define SPREG_EXCCAUSE_CAUSE_MASK                            0x0000000F

/**
 * @ingroup Execute level of core
 */
#define EXEC_LEVEL_APPLICATION_CODE                          2
#define EXEC_LEVEL_EXCEPTION_HANDLER                         3
#define EXEC_LEVEL_INTERRUPT_HANDLER                         4
#define EXEC_LEVEL_NON_INTERRUPTIBLE                         5

/**
 * @ingroup Schedule Flag stored on Task Context
 */
#define OS_SCHED_FLAG_TASKPREEMT                             4 /* Task Preemted through LOS_Schedule */

/**
 * @ingroup Stack Frame Save Area Fields Define
 */
#define STKFR_SA_OFF_A0                                   0
#define STKFR_SA_OFF_A1                                   4
#define STKFR_SA_OFF_A2                                   8
#define STKFR_SA_OFF_A3                                   12
#define STKFR_SA_OFF_A4                                   16
#define STKFR_SA_OFF_A5                                   20
#define STKFR_SA_OFF_A6                                   24
#define STKFR_SA_OFF_A7                                   28
#define STKFR_SA_SIZE                                     32

/**
 * @ingroup Context Fields Define
 */
#define CONTEXT_OFF_RESERVED                              0
#define CONTEXT_OFF_LCOUNT                                4
#define CONTEXT_OFF_LEND                                  8
#define CONTEXT_OFF_LBEG                                  12
#define CONTEXT_OFF_EPC                                   16
#define CONTEXT_OFF_PC                                    16 /* reuse with EPC */
#define CONTEXT_OFF_PS                                    20
#define CONTEXT_OFF_SCHED_FLAG                            24 /* reuse with exccause */
#define CONTEXT_OFF_EXCCAUSE                              24
#define CONTEXT_OFF_EXCVADDR                              28
#define CONTEXT_OFF_A8                                    32
#define CONTEXT_OFF_A9                                    36
#define CONTEXT_OFF_A10                                   40
#define CONTEXT_OFF_A11                                   44
#define CONTEXT_OFF_A12                                   48
#define CONTEXT_OFF_A13                                   52
#define CONTEXT_OFF_A14                                   56
#define CONTEXT_OFF_A15                                   60
#define CONTEXT_SIZE                                      96

#ifdef YES
#undef YES
#endif
#define YES                    1

#ifdef NO
#undef NO
#endif
#define NO                     0

#endif /* _ARCH_REGS_H */
