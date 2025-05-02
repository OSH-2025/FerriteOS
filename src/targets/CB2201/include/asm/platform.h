/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Platform HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-04-02
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

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "los_typedef.h"
#include "stdarg.h"
#include "uart.h"

#include "interrupt_config.h"
#include "memmap_config.h"
#include "csi_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_SYS_CLOCK              24000000
#define PMU_REG_BASE              (0x40002000UL)
#define RTC_CLOCK_SEL_MASK        (0x1U << 0)
#define PMU_CLOCK_SEL_MASK        (0x1U << 1)

typedef struct {
    UINT32 LPCR;             /* Offset: 0x000 (R/W) low power control register */
    UINT32 MCLKSEL;          /* Offset: 0x004 (R/W) MCLK select register */
    UINT32 CRCR;             /* Offset: 0x008 (R/W) clock ratio control register */
    UINT32 CGCR;             /* Offset: 0x00c (R/W) clock gate control register */
    UINT32 CGSR;             /* Offset: 0x010 (R/W) clock gate status register */
    UINT32 CLKDSENR;         /* Offset: 0x014 (R/W) clock disable register */
    UINT32 CLKSTBR;          /* Offset: 0x018 (R/W) clock stable register */
    UINT32 CLKSTBST;         /* Offset: 0x01c (R/W) clock stable interrupt statue register */
    UINT32 CLKSTBMK;         /* Offset: 0x020 (R/W) clock stable interrupt mask register */
    UINT32 CSSCR;            /* Offset: 0x024 (R/W) clock source stable counter register */
    UINT32 DFCC;             /* Offset: 0x028 (R/W) dynamic frequence conversion control register */
    UINT32 PCR;              /* Offset: 0x02c (R/W) pll control register */
    UINT32 PLTR;             /* Offset: 0x030 (R/W) pll lock timer register */
    UINT32 SWHRC;            /* Offset: 0x034 (R/W) software HRST control register */
    UINT32 SWHRD;            /* Offset: 0x038 (R/W) software HRST duration register */
    UINT32 SWPRC;            /* Offset: 0x03c (R/W) software PRST control register */
    UINT32 SWPRD;            /* Offset: 0x040 (R/W) software PRST duration register */
    UINT32 SWRE;             /* Offset: 0x044 (R/W) software reset enable register */
    UINT32 BOOTSEL;          /* Offset: 0x048 (R/W) boot selection register */
    UINT32 SCGCR;            /* Offset: 0x04c (R/W) security clock gate control register */
    UINT32 SCGSR;            /* Offset: 0x050 (R/W) security clock gate status register */
} PMU_REG;

typedef enum {
    IH_CLK       = 0,
    EH_CLK       = 1,
    IL_CLK       = 2,
    EL_CLK       = 3
} CLK_SRC;

#ifdef LOSCFG_PLATFORM_OSAPPINIT
extern VOID app_init(VOID);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _PLATFORM_H */

