/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: memcmp
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
#define src1        x0
#define src2        x1
#define cnt         x2
#define result      w0
#define prezero     x6
#define diff        x7

#define data1       x3
#define data1w      w3
#define data2       x4
#define data2w      w4
#define tmp1        x5

.p2align 6
.global memcmp
.type memcmp,%function
memcmp:
        cbz     cnt, .return_0
        cmp     src1, src2 /* if two adresses are same, return 0. */
        b.eq    .return_0

        cmp     cnt, #8 /* if cnt is less 8 bytes, badly compare. */
        b.mi    .compare_1_byte_by_loop

        neg     tmp1, src1
        ands    tmp1, tmp1, #7 /* calc the up-aligned unaligned cnt. */
        b.eq    .compare_8_bytes_by_loop

        sub     cnt, cnt, tmp1
1: /* align the unaligned bytes. */
        ldrb    data1w, [src1], #1
        ldrb    data2w, [src2], #1
        cmp     data1w, data2w
        b.ne    .return_value
        subs    tmp1, tmp1, #1
        b.hi    1b
        cmp     cnt, #8
        b.mi    .compare_1_byte_by_loop

        .p2align 4
.compare_8_bytes_by_loop:
        ldr     data1, [src1], 8
        ldr     data2, [src2], 8
        subs    cnt, cnt, 8
        ccmp    data1, data2, 0, hi  /* NZCV = 0b0000. */
        b.eq    .compare_8_bytes_by_loop
        /* if cnt < 0, there is less than 8-bytes left. 
           if cnt > 0, there is different character between data1 and data2. 
           if cnt == 0, need to compare data1 and data2. */
        cmp     cnt, #0
        b.lt    3f
#ifndef __AARCH64EB__
        rev     data1, data1
        rev     data2, data2
#endif
        cmp     data1, data2
        b.eq    .return_0
        b.lt    2f
        mov     result, #1
        ret
2:
        mov     result, #-1
        ret
3:
        add     cnt, cnt, #8
        sub     src1, src1, #8 /* reback the address. */
        sub     src2, src2, #8

.compare_1_byte_by_loop:
        ldrb    data1w, [src1], #1
        ldrb    data2w, [src2], #1
        subs    cnt, cnt, #1
        ccmp    data1w, data2w, 0, ne
        b.eq    .compare_1_byte_by_loop
        b       .return_value

.return_0:
        mov     result, #0
        ret

.return_value:
        sub     result, data1w, data2w
        ret

.size memcmp, .-memcmp
