/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: ARMv6 Or ARMv7 JMP Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

.syntax unified
#if defined(LOSCFG_ARCH_ARM_V6M)
.arch armv6-m
#elif defined(LOSCFG_ARCH_CORTEX_M33)
.arch armv8-m.main
#else
.arch armv7e-m
#endif
.thumb

.section .text
.thumb

.type longjmp, %function
.global longjmp
longjmp:
#if defined(LOSCFG_ARCH_ARM_V6M)
    adds    r0, #16
    ldmia   r0!, {r3-r7}

    mov     r8, r3
    mov     r9, r4
    mov     r10, r5
    mov     r11, r6
    mov     lr, r7

    subs    r0, #36
    ldmia   r0!, {r4-r7}

    adds    r0, #20
    ldr     r3, [r0]
    mov     sp, r3
#else
    ldmia   r0!, {r4-r11,lr}
#if !defined(LOSCFG_ARCH_CORTEX_M3) && !defined(LOSCFG_ARCH_ARM_V6M)
    vldmia  r0!, {d8-d15}
#endif
    ldr     sp, [r0]
#endif
    mov     r0, r1
    cmp     r1, #0
    bne     1f
#if defined(LOSCFG_ARCH_ARM_V6M)
    movs    r0, #1
#else
    mov     r0, #1
#endif
    1:
    bx      lr

.type setjmp, %function
.global setjmp
setjmp:
#if defined(LOSCFG_ARCH_ARM_V6M)
    stmia   r0!, {r4-r7}

    mov     r3, r8
    mov     r4, r9
    mov     r5, r10
    mov     r6, r11
    mov     r7, lr
    stmia   r0!, {r3 - r7}

    mov     r3, sp
    str     r3, [r0]
    movs    r0, #0
#else
    stmia   r0!, {r4-r11,lr}
#if !defined(LOSCFG_ARCH_CORTEX_M3) && !defined(LOSCFG_ARCH_ARM_V6M)
    vstmia r0!, {d8-d15}
#endif
    str     sp, [r0]
    mov     r0, #0
#endif
    bx      lr

