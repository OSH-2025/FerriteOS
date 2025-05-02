/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-10-28
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

#include "los_typedef.h"
#include "uart.h"

#ifndef _USART_H
#define _USART_H

#define REG32_READ(reg)                   (*(volatile UINT32*) (reg))
#define REG32_WRITE(reg, value)           (*(volatile UINT32*) (reg) = (value))
#define REG32_MASK_CLEAR(reg, mask)       (REG32_WRITE(reg, (REG32_READ(reg) & (~(mask)))))
#define REG32_MASK_SET(reg, mask)         (REG32_WRITE(reg, (REG32_READ(reg) | (mask))))
#define FUN_WPU_BIT                       (0x1 << 7)
#define IO_MUX_BASE                       (0x60000800)
#define IO_MUX_UART0_TX_REG               (IO_MUX_BASE + 0x18)
#define IO_MUX_UART0_RX_REG               (IO_MUX_BASE + 0x14)
#define IO_MUX_FUNC_MASK                  (0x13 << 4)
#define UART_BAUDRATE_DEFAULT             (115200)
#define UART_8BITS_DATA                   (0x3)
#define UART_FIFO_BUFFER                  (128)

#define IO_MUX_FUNCTION0                  (0)
#define IO_MUX_FUNCTION1                  (1)
#define IO_MUX_FUNCTION2                  (2)
#define IO_MUX_FUNCTION3                  (3)
#define IO_MUX_FUNCTION4                  (4)
#define IO_MUX_FUNCTION5                  (5)

#define UART_REG_BASE                     (0x60000000)
#define UART_STATUS_REG                   (UART_REG_BASE + 0x1C)
#define TXFIFO_CNT_MASK                   (0xFF)
#define TXFIFP_CNT_SHIFT                  (16)
#define UART_FIFO_REG                     (UART_REG_BASE)
#define UART_INTENA_REG                   (UART_REG_BASE + 0xC)
#define UART_INTCLR_REG                   (UART_REG_BASE + 0x10)
#define RXFIFO_TOUT_INT_MASK              (1 << 8)
#define RXFIFO_FULL_INT_MASK              (1)

extern UartControllerOps g_genericUart;

#endif /* _USART_H */
