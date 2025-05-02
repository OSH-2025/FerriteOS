/* ----------------------------------------------------------------------------
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: Gpio Define HeadFile
* Author: Huawei LiteOS Team
* Create: 2021-10-24
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1.Redistributions of source code must retain the above copyright notice, this list of
* conditions and the following disclaimer.
* 2.Redistributions in binary form must reproduce the above copyright notice, this list
* of conditions and the following disclaimer in the documentation and/or other materials
* provided with the distribution.
* 3.Neither the name of the copyright holder nor the names of its contributors may be used
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
*--------------------------------------------------------------------------- */
#ifndef __GPIO_DEF_H__
#define __GPIO_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define GPIO_REG_BASE (0x3f200000)
// GPIO FUNCTION SELECT
#define GPIO_FUNC_SEL_0 (GPIO_REG_BASE)
#define GPIO_FUNC_SEL_1 (GPIO_REG_BASE+0x4)
#define GPIO_FUNC_SEL_2 (GPIO_REG_BASE+0x8)
#define GPIO_FUNC_SEL_3 (GPIO_REG_BASE+0xc)
#define GPIO_FUNC_SEL_4 (GPIO_REG_BASE+0x10)
#define GPIO_FUNC_SEL_5 (GPIO_REG_BASE+0x14)
#define GPIO_FUNC_SEL_BITS (3)
#define GPIO_FUNC_SEL_MASK (0x7)
// GPIO PIN OUTPUT SET
#define GPIO_OUTPUT_SET_0 (GPIO_REG_BASE+0x1C)
#define GPIO_OUTPUT_SET_1 (GPIO_REG_BASE+0x20)
// GPIO PIN OUTPUT CLEAR
#define GPIO_OUTPUT_CLR_0 (GPIO_REG_BASE+0x28)
#define GPIO_OUTPUT_CLR_0 (GPIO_REG_BASE+0x28)
// GPIO PIN LEVEL
#define GPIO_LEVEL_0 (GPIO_REG_BASE+0x34)
#define GPIO_LEVEL_1 (GPIO_REG_BASE+0x38)
// GPIO PIN EVENT DETECT STATUS
#define GPIO_EVENT_DETECT_STATUS_0 (GPIO_REG_BASE+0x40)
#define GPIO_EVENT_DETECT_STATUS_1 (GPIO_REG_BASE+0x44)
// GPIO RISING EDGE DETECT ENABLE REGISTERS
#define GPIO_RISING_EDGE_DETECT_EN_0 (GPIO_REG_BASE+0x4C)
#define GPIO_RISING_EDGE_DETECT_EN_1 (GPIO_REG_BASE+0x50)
// GPIO FALLING EDGE DETECT ENABLE REGISTERS
#define GPIO_FALLING_EDGE_DETECT_EN_0 (GPIO_REG_BASE+0x58)
#define GPIO_FALLING_EDGE_DETECT_EN_1 (GPIO_REG_BASE+0x5C)
// GPIO PIN HIGH DETECT ENABLE
#define GPIO_HIGH_DETECT_EN_0 (GPIO_REG_BASE+0x64)
#define GPIO_HIGH_DETECT_EN_1 (GPIO_REG_BASE+0x68)
// GPIO PIN LOW DETECT ENABLE
#define GPIO_LOW_DETECT_EN_0 (GPIO_REG_BASE+0x70)
#define GPIO_LOW_DETECT_EN_1 (GPIO_REG_BASE+0x74)
// GPIO PIN ASYNC. RISING EDGE DETECT
#define GPIO_ASYNC_RISING_EDGE_DETECT_EN_0 (GPIO_REG_BASE+0x7C)
#define GPIO_ASYNC_RISING_EDGE_DETECT_EN_1 (GPIO_REG_BASE+0x80)
// GPIO PIN ASYNC. FALLING EDGE DETECT
#define GPIO_ASYNC_FALLING_EDGE_DETECT_EN_0 (GPIO_REG_BASE+0x88)
#define GPIO_ASYNC_FALLING_EDGE_DETECT_EN_1 (GPIO_REG_BASE+0x8C)
// GPIO PIN PULL-UP/DOWN ENABLE
#define GPIO_PULL_UPDOWN_EN (GPIO_REG_BASE+0x94)
#define GPIO_PULL_UPDOWN_EN_MASK  (0x3)
// GPIO PIN PULL-UP/DOWN ENABLE CLOCK
#define GPIO_PULL_UPDOWN_EN_CLK_0 (GPIO_REG_BASE+0x98)
#define GPIO_PULL_UPDOWN_EN_CLK_1 (GPIO_REG_BASE+0x9C)

// GPIO FUNCTION
#define GPIO_FUNC_MASK   (0x111)
#define GPIO_FUNC_INPUT  (0x000)
#define GPIO_FUNC_OUTPUT (0x001)
#define GPIO_FUNC_ALT0   (0x100)
#define GPIO_FUNC_ALT1   (0x101)
#define GPIO_FUNC_ALT2   (0x110)
#define GPIO_FUNC_ALT3   (0x111)
#define GPIO_FUNC_ALT4   (0x011)
#define GPIO_FUNC_ALT5   (0x010)

enum GPIO_PIN{
    GPIO_0 = 0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,

    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15,
    GPIO_16,
    GPIO_17,
    GPIO_18,
    GPIO_19,

    GPIO_20,
    GPIO_21,
    GPIO_22,
    GPIO_23,
    GPIO_24,
    GPIO_25,
    GPIO_26,
    GPIO_27,
    GPIO_28,
    GPIO_29,

    GPIO_30,
    GPIO_31,
    GPIO_32,
    GPIO_33,
    GPIO_34,
    GPIO_35,
    GPIO_36,
    GPIO_37,
    GPIO_38,
    GPIO_39,

    GPIO_40,
    GPIO_41,
    GPIO_42,
    GPIO_43,
    GPIO_44,
    GPIO_45,
    GPIO_46,
    GPIO_47,
    GPIO_48,
    GPIO_49,

    GPIO_50,
    GPIO_51,
    GPIO_52,
    GPIO_53,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif //__GPIO_DEF_H__ 
