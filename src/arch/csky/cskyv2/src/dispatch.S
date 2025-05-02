/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: C-sky Dispatch Implementation
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
 * --------------------------------------------------------------------------- */

#define VIC_TSPDR                 0XE000EC08

.section .text
.align  2
.type OsStartToRun, %function
.global OsStartToRun
OsStartToRun:
    lrw    r1, g_oldTask
    lrw    r2, g_runTask

    st.w   r0, (r1)
    st.w   r0, (r2)

    ldw    sp, (r0)

    ldw    r0, (sp, 64)
    mtcr   r0, epc
    ldw    r0, (sp, 60)
    mtcr   r0, epsr
    ldw    r15, (sp, 56)
    ldm    r0-r13, (sp)

    rte

.align  2
.type OsTaskSchedule, %function
.global OsTaskSchedule
OsTaskSchedule:
    subi   sp, 68
    stm    r0-r13, (sp)
    stw    r15, (sp, 56)
    mfcr   r2, psr
    stw    r2, (sp, 60)
    stw    r15, (sp, 64)
    stw    sp, (r1)

.align  2
.type osPendSV, %function
.global osPendSV
osPendSV:
    psrset  ee

TaskSwitch:
    lrw    r3, g_oldTask
    lrw    r2, g_runTask
    ldw    r2, (r2)
    stw    r2, (r3)

    ldw    sp, (r0)
    ldw    r0, (sp, 64)
    mtcr   r0, epc
    ldw    r0, (sp, 60)
    mtcr   r0, epsr
    ldw    r15, (sp, 56)
    ldm    r0-r13, (sp)
    addi   sp, 68

    rte


