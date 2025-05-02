/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Intermit Implementation
 * Author: Huawei LiteOS Team
 * Create: 2019-08-01
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
.arch armv7-m
.thumb
.global g_regBackupBuf

.section .text.OsSRSaveRegister
.weak OsSRSaveRegister
.type OsSRSaveRegister, %function
.global OsSRSaveRegister
OsSRSaveRegister:
    .fnstart
    .cantunwind
        push {r4,r5}
        ldr r5, =g_regBackupBuf
        str r0, [r5]

        adds r5, r5, #4
        str r1, [r5]

        adds r5, r5, #4
        str r2, [r5]

        adds r5, r5, #4
        str r3, [r5]

        mov r0, r5
        pop {r4,r5}

        adds r0, r0, #4
        str r4, [r0]

        adds r0, r0, #4
        str r5, [r0]

        mov r5, r0
        adds r5, r5, #4
        str r6, [r5]

        adds r5, r5, #4
        str r7, [r5]

        adds r5, r5, #4
        str r8, [r5]

        adds r5, r5, #4
        str r9, [r5]

        adds r5, r5, #4
        str r10, [r5]

        adds r5, r5, #4
        str r11, [r5]

        adds r5, r5, #4
        str r12, [r5]

        adds r5, r5, #4
        mrs r4, apsr
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, ipsr
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, epsr
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, primask
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, basepri
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, basepri_max
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, faultmask
        str r4, [r5]

        adds r5, r5, #4
        mrs r4, control
        str r4, [r5]

        adds r5, r5, #4
        mov r4, sp
        str r4, [r5]

        adds r5, r5, #4
        mov r4, lr
        str r4, [r5]

        adds r5, r5, #4
        mov r4, lr
        str r4, [r5]
        mov pc, lr
        .fnend

.section .text.OsSRRestoreRegister
.weak OsSRRestoreRegister
.type OsSRRestoreRegister, %function
.global OsSRRestoreRegister
OsSRRestoreRegister:
    .fnstart
    .cantunwind
        ldr r0, =g_regBackupBuf

        adds r0, r0, #8
        ldr r2, [r0], #4
        ldr r3, [r0], #4
        ldr r4, [r0], #4
        ldr r5, [r0], #4
        ldr r6, [r0], #4
        ldr r7, [r0], #4
        ldr r8, [r0], #4
        ldr r9, [r0], #4
        ldr r10, [r0], #4
        ldr r11, [r0], #4
        ldr r12, [r0], #4

        ldr r1, [r0], #4
        msr apsr, r1

        ldr r1, [r0], #4
        msr ipsr, r1

        ldr r1, [r0], #4
        msr epsr, r1

        ldr r1, [r0], #4
        msr primask, r1

        ldr r1, [r0], #4
        msr basepri, r1

        ldr r1, [r0], #4
        msr basepri_max, r1

        ldr r1, [r0], #4
        msr faultmask, r1

        ldr r1, [r0], #4
        msr control, r1

        ldr sp, [r0], #4
        ldr lr, [r0], #4

        push {r2}
        ldr r2, =g_regBackupBuf
        ldr r0, [r2], #4
        ldr r1, [r2]

        pop {r2}
        mov pc, lr
        .fnend
