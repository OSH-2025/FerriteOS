/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: memset
 * Author: Huawei LiteOS Team
 * Create: 2021-01-25
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

#define src     x0
#define val     x1
#define valw    w1
#define cnt     x2
#define tmpSrc  x6

.global memset
.type memset,%function
memset:
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
    stp     lr, x4, [SP,#-16]!
    stp     x2, x3, [SP,#-16]!
    stp     x0, x1, [SP,#-16]!

    mov     x3, #0 /* notice OsMemSysNodeCheck this is memset. */
    bl      OsMemSysNodeCheck
    cmp     x0, #0 /* if OsMemSysNodeCheck return LOS_OK, do memset as usual. */
    ble     1f
    ldp     x0, x1, [SP], #16
    ldp     x2, x3, [SP], #16
    ldp     lr, x4, [SP], #16
    mov     x0, #0 /* if OsMemSysNodeCheck return LOS_NOK, memset return NULL. */
    ret

1:  ldp     x0, x1, [SP], #16
    ldp     x2, x3, [SP], #16
    ldp     lr, x4, [SP], #16
#endif

    mov     tmpSrc, src /* Preserve return value. */
    dup     v0.16B, valw
    mov     val, v0.D[0]
    cmp     cnt, #16
    b.lt    .Lset_less_than_16_bytes
    cmp     cnt, #128
    b.lt    .Lset_less_than_128_bytes

/* set 128-bytes in loop. */
    .p2align 4
.Lset_128_bytes_loop:
    subs    cnt, cnt, #128
    b.lt    2f /* if cnt is less than 128, go to 2f. */
    stp     q0, q0, [tmpSrc], #32
    stp     q0, q0, [tmpSrc], #32
    stp     q0, q0, [tmpSrc], #32
    stp     q0, q0, [tmpSrc], #32
    b.gt    .Lset_128_bytes_loop
    b       .Lreturn /* here, cnt is 0. */
2:
    add     cnt, cnt, #128

.Lset_less_than_128_bytes:
    cmp     cnt, #16
    b.lt    .Lset_less_than_16_bytes
    cmp     cnt, #32
    b.lt    .Lset_less_than_32_bytes

/* set 32-bytes in loop. */
    .p2align 4
.Lset_32_bytes_loop:
    subs    cnt, cnt, #32
    b.lt    3f /* if cnt is less than 32, go to 3f. */
    stp     q0, q0, [tmpSrc], #32
    b.gt    .Lset_32_bytes_loop
    b       .Lreturn /* here, cnt is 0. */
3:
    add     cnt, cnt, #32
    tbz     cnt, #4, .Lset_less_than_16_bytes

.Lset_less_than_32_bytes:
    str     q0, [tmpSrc], #16
    sub     cnt, cnt, #16

/* set 0~15-bytes. */
.Lset_less_than_16_bytes:
    tbz     cnt, #3, 4f /* if the third bit of cnt is 0, go to 4f. */
    str     val, [tmpSrc], #8
4:
    tbz     cnt, #2, 5f
    str     valw, [tmpSrc], #4
5:
    tbz     cnt, #1, 6f
    strh    valw, [tmpSrc], #2
6:
    tbz     cnt, #0, .Lreturn
    strb    valw, [tmpSrc], #1
.Lreturn:
    ret

.size memset,.-memset

