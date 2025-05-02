/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Misc Test Case
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

#include "it_los_misc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static UINT32 TestCase(VOID)
{
    UINT32 mask;
    UINT32 maskBak;
    UINT16 pos;
    UINT16 start;

    for (pos = 0; pos <= 0x1F; pos++) {
        mask = 0xFFFFFFFF;
        maskBak = 0;
        LOS_BitmapClr(&mask, pos);
        maskBak = ~(1 << pos);
        ICUNIT_ASSERT_EQUAL(mask, maskBak, mask);
    }

    /* The part greater than 0x31 is ignored. */
    for (start = 0x20; start < 0xFFE0; start += 0x20) {
        for (pos = start; pos <= (start + 0x1F); pos++) {
            mask = 0xFFFFFFFF;
            maskBak = 0;
            LOS_BitmapClr(&mask, pos);
            maskBak = ~(1 << (pos % 0x20));
            ICUNIT_ASSERT_EQUAL(mask, maskBak, mask);
        }
    }

    /* LOS_BitmapClr will not change the bit not set. */
    maskBak = 0;
    for (pos = 0; pos <= 0x1F; pos++) {
        mask = 0;
        LOS_BitmapClr(&mask, pos);
        ICUNIT_ASSERT_EQUAL(mask, maskBak, mask);
    }
    return LOS_OK;
}

/**
 * @ingroup TEST_MISC
 * @par TestCase_Number
 * ItLosBitmap001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief LOS_BitmapClr API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Cyclically clear each bit.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosBitmap001(VOID)
{
    TEST_ADD_CASE("ItLosBitmap001", TestCase, TEST_LOS, TEST_MISC, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
