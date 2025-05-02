/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: memcpy
 * Author: Huawei LiteOS Team
 * Create: 2021-01-28
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

#define destin  x0
#define source  x1
#define cnt     x2
#define destTmp x3
#define tmp1    x4
#define tmpw1   w4
#define tmp2    x5
#define tmpw2   w5
#define tmp3    x6
#define tmp4    x7
#define tmp5    x9
#define tmp6    x10

.global memcpy
.type memcpy,%function
memcpy:
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
    stp     x4, lr, [SP,#-16]!
    stp     x3, x2, [SP,#-16]!
    stp     x1, x0, [SP,#-16]!

    mov     x3, #1 /* notice OsMemSysNodeCheck this is memcpy */
    bl      OsMemSysNodeCheck
    cmp     x0, #0 /* if OsMemSysNodeCheck return LOS_OK, do memcpy as usual */
    ble     1f
    ldp     x1, x0, [SP], #16
    ldp     x3, x2, [SP], #16
    ldp     x4, lr, [SP], #16
    mov     x0, #0 /* if OsMemSysNodeCheck return LOS_NOK, memcpy return NULL */
    ret

1:
    ldp     x1, x0, [SP], #16
    ldp     x3, x2, [SP], #16
    ldp     x4, lr, [SP], #16
#endif

.macro  copy_less_than_16_bytes n branch
    tbz     \n, #3, 1f
    ldr     tmp1, [source], #8
    str     tmp1, [destTmp], #8
1:
    tbz     \n, #2, 2f
    ldr     tmpw1, [source], #4
    str     tmpw1, [destTmp], #4
2:
    tbz     \n, #1, 3f
    ldrh    tmpw1, [source], #2
    strh    tmpw1, [destTmp], #2
3:
    tbz     \n, #0, \branch
    ldrb    tmpw1, [source], #1
    strb    tmpw1, [destTmp], #1
.endm

    mov     destTmp, destin
    cmp     cnt, #16
    b.lt    .Lcopy_less_than_16_bytes

    cmp     cnt, #32
    b.lt    .Lcopy_less_than_32_bytes
    neg     tmp3, destTmp
    ands    tmp3, tmp3, #15 /* calc the up-aligned unaligned cnt. */
    b.eq    5f
    copy_less_than_16_bytes n=tmp3 branch=4f /* copy the unasigned cnt. */
4:
    sub     cnt, cnt, tmp3 /* update the cnt. */
5:
    cmp     cnt, #128
    b.lt    .Lcopy_32_bytes_loop
    mov     tmp4, #128
    udiv    tmp5, cnt, tmp4 /* tmp5 is the quotient. */
    msub    tmp6, tmp5, tmp4, cnt /* tmp6 = cnt - tmp5 * tmp4, tmp6 is the remainder. */

    .p2align 4
.Lcopy_128_bytes_loop:
    subs    tmp5, tmp5, #1
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    b.ne    .Lcopy_128_bytes_loop
    mov     cnt, tmp6

.Lcopy_less_than_128_bytes:
    cmp     cnt, #16
    b.lt    .Lcopy_less_than_16_bytes
    cmp     cnt, #32
    b.lt    .Lcopy_less_than_32_bytes

    .p2align 4
.Lcopy_32_bytes_loop:
    subs    cnt, cnt, #32
    b.lt    6f
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    b.gt    .Lcopy_32_bytes_loop
    b       .Lreturn
6:
    add     cnt, cnt, #32
    tbz     cnt, #4, .Lcopy_less_than_16_bytes

    .p2align 4
.Lcopy_less_than_32_bytes:
    ldp     tmp1, tmp2, [source], #16
    stp     tmp1, tmp2, [destTmp], #16
    sub     cnt, cnt, #16

.Lcopy_less_than_16_bytes:
    copy_less_than_16_bytes n=cnt branch=.Lreturn

.Lreturn:
    ret

.size memcpy, .-memcpy
