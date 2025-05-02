/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-07-19
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

#ifndef _USART_H
#define _USART_H

#include "los_typedef.h"
#include "uart.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UART_RXREADY_FLAG   0x1
#define UART_TXEMPTY_FLAG   0x20

#define AUX_BASE        (0x3F000000)
#define AUX_REG_BASE    (AUX_BASE + 0x215000)
#define AUX_ENABLES     (AUX_REG_BASE + 0x4)
#define MINI_UART       ((MINIUART_INFO *)(AUX_REG_BASE + 0x40))
#define GPIO_REG_BASE   ((GPIO_INFO *)(AUX_BASE + 0x200000))

typedef struct TagGpioInfo {
    volatile UINT32 GPFSEL[6];      /* GPIO Function Select x */
    volatile UINT32 RES0;           /* Reserved */
    volatile UINT32 GPSET[2];       /* GPIO Pin Output Set x */
    volatile UINT32 RES1;           /* Reserved */
    volatile UINT32 GPCLR[2];       /* GPIO Pin Output Clear x */
    volatile UINT32 RES2;           /* Reserved */
    volatile UINT32 GPLEV[2];       /* GPIO Pin Level x */
    volatile UINT32 RES3;           /* Reserved */
    volatile UINT32 GPEDS[2];       /* GPIO Pin Event Detect Status x */
    volatile UINT32 RES4;           /* Reserved */
    volatile UINT32 GPREN[2];       /* GPIO Pin Rising Edge Detect Enable x */
    volatile UINT32 RES5;           /* Reserved */
    volatile UINT32 GPFEN[2];       /* GPIO Pin Falling Edge Detect Enable x */
    volatile UINT32 RES6;           /* Reserved */
    volatile UINT32 GPHEN[2];       /* GPIO Pin High Detect Enable x */
    volatile UINT32 RES7;           /* Reserved */
    volatile UINT32 GPLEN[2];       /* GPIO Pin Low Detect Enable 0 */
    volatile UINT32 RES8;           /* Reserved */
    volatile UINT32 GPAREN[2];      /* GPIO Pin Async. Rising Edge Detect x */
    volatile UINT32 RES9;           /* Reserved */
    volatile UINT32 GPAFEN[2];      /* GPIO Pin Async. Falling Edge Detect x */
    volatile UINT32 RES10;          /* Reserved */
    volatile UINT32 GPPUD;          /* GPIO Pin Pull-up/down Enable */
    volatile UINT32 GPPUDCLK[2];    /* GPIO Pin Pull-up/down Enable Clock x */
} GPIO_INFO;

typedef struct TagMiniUartInfo {
    volatile UINT32 IO;         /* Mini Uart I/O Data 8 */
    volatile UINT32 IIR;        /* Mini Uart Interrupt Identify */
    volatile UINT32 IER;        /* Mini Uart Interrupt Enable */
    volatile UINT32 LCR;        /* Mini Uart Line Control */
    volatile UINT32 MCR;        /* Mini Uart Modem Control */
    volatile UINT32 LSR;        /* Mini Uart Line Status */
    volatile UINT32 MSR;        /* Mini Uart Modem Status */
    volatile UINT32 SCRATCH;    /* Mini Uart Scratch */
    volatile UINT32 CNTL;       /* Mini Uart Extra Control */
    volatile UINT32 STAT;       /* Mini Uart Extra Status */
    volatile UINT32 BAUD;       /* Mini Uart Baudrate */
} MINIUART_INFO;

extern UartControllerOps g_genericUart;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _USART_H */

