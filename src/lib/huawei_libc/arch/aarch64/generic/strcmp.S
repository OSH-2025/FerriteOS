/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: strcmp
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
#define result      w0

#define data1       x2
#define data1w      w2
#define data2       x3
#define data2w      w3
#define val_ff      x5
#define val_7f      x6
#define tmp1        x7
#define tmp2        x9

.global strcmp
.type strcmp,%function
strcmp:
.macro find_zero_or_different_bytes d1 d2 branch
        and     tmp1, \d1, val_7f
        orr     tmp2, \d1, val_7f
        add     tmp1, tmp1, val_7f
        orr     tmp1, tmp1, tmp2
        /* if the byte is not null byte, the corresponding byte is 0xff in tmp1, otherwise, it is 7f. */
        cmp     tmp1, val_ff
        b.ne    \branch
#ifndef __AARCH64EB__
        rev     \d1, \d1
        rev     \d2, \d2
#endif
        cmp     \d1, \d2
        b.ne    2f
.endm

.macro init
        mov     val_7f, #0x7f7f7f7f7f7f7f7f
        mov     val_ff, #-1 /* val_7f and val_ff are used to compute the NUL-byte detection. */
.endm
        .p2align 6
        cmp     src1, src2
        b.eq    .return_0 /* if src1 == src2, return 0. */
        eor     tmp1, src1, src2
        tst     tmp1, #7 /* src1 and src2 is not aligned and their unaligned size is not same, then byte-wise compare. */
        b.ne    .compare_1_byte_by_loop

        init /* here, src1 and src2 are aligned, or their unaligned size is same. */
        neg     tmp1, src1
        ands    tmp1, tmp1, #7 /* calc the up-aligned unaligned size. */
        b.eq    .Laligned8_loop

1:
        ldrb    data1w, [src1], #1
        ldrb    data2w, [src2], #1
        cmp     data1w, 0
        b.eq    .return_value /* if data1w is 0, end comparison. */
        cmp     data1w, data2w
        b.ne    .return_value /* if data1w and data2w are not equal, end comparison. */
        subs    tmp1, tmp1, #1
        b.ne    1b /* align the src1. */

.Laligned8_loop:
        ldr     data1, [src1], #8
        ldr     data2, [src2], #8
        find_zero_or_different_bytes d1=data1 d2=data2 branch=.return_88
        b       .Laligned8_loop

2:
        cset    result, ne
        cneg    result, result, lo
        ret

.return_88:
        sub     src1, src1, #8 /* reback the address. */
        sub     src2, src2, #8

.compare_1_byte_by_loop:
        ldrb    data1w, [src1], #1
        ldrb    data2w, [src2], #1
        cmp     data1w, #0
        ccmp    data1w, data2w, #0, ne
        b.eq    .compare_1_byte_by_loop
        sub     result, data1w, data2w  
        ret

.return_0:
        mov     result, #0
        ret

.return_value:
        sub     result, data1w, data2w 
        ret

.size strcmp, .-strcmp
