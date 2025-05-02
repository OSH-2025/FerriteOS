/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-09-14
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

#ifndef _MAIN_H
#define _MAIN_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define WDT_WRWITE_MASK                0x50d83aa1U
#define RTC_CNTL                       0x3ff48000U
#define RTC_WDT_PROTECT                0x3ff480a4U
#define RTC_WDT_CFG0                   0x3ff4808cU
#define RTC_WDT_FEED                   0x3ff480a0U
#define TIMER_GROUP0                   0x3ff5F000U
#define WDT_TIMER_GROUP0_CNTL          0x3ff5F048U
#define TIMER_GROUP1                   0x3ff60000U
#define WDT_TIMER_GROUP1_CNTL          0x3ff60048U
#define REG32_READ(reg)                (*(volatile UINT32*) (reg))
#define REG32_WRITE(reg, value)        (*(volatile UINT32*) (reg) = (value))
#define RTC_CNTL_WDT_FEED_BIT          (31)
#define RTC_CNTL_WDT_FLASHBOOT_MOD_EN  (10)

extern char _sbss, _ebss, __init_stack_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MAIN_H */
