/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart Init Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-13
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

#include "usart.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UART_INT_RAW_REG           ((volatile UINT32 *)0x3FF40004)
#define UART_INT_ENA_REG           ((volatile UINT32 *)0x3FF4000C)
#define UART_INT_CLR_REG           ((volatile UINT32 *)0x3FF40010)
#define UART_CONF1_REG             ((volatile UINT32 *)0x3FF40024)
#define PRO_UART_INTR_MAP_REG      ((volatile UINT32 *)0x3FF0018C)
#define PRO_INTR_STATUS_REG_1_REG  ((volatile UINT32 *)0x3FF000F0)

extern int uart_tx_one_char(uint8_t ch);
extern int uart_rx_one_char(uint8_t *ch);

VOID UartPutc(CHAR c)
{
    (VOID)uart_tx_one_char(c);
}

STATIC VOID UartWriteChar(const CHAR c)
{
    UartPutc(c);
}

STATIC UINT8 UartReadChar(VOID)
{
    uint8_t ch;

    (VOID)uart_rx_one_char(&ch);

    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
    *(UART_INT_CLR_REG) |= 0x1; /* 0x1: clear UART_RXFIFO_FULL_INT interrupt */
    (VOID)LOS_HwiClear(NUM_HAL_INTERRUPT_UART);
}

STATIC INT32 UartHwi(VOID)
{
    UINT32 reg;
    UINT32 ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, UartHandler, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s, %d, uart interrupt created failed, ret = %x.\n", __FILE__, __LINE__, ret);
    } else {
        *(UART_INT_ENA_REG) |= 0x1; /* 0x1: UART_RXFIFO_FULL_INT interrupt enable bit */
        *(UART_INT_ENA_REG) |= (1 << 4); /* 1 << 4: UART_FRM_ERR_INT interrupt enable bit */
        reg = *(UART_CONF1_REG);
        reg = (reg & (~0x7f)) | 0x1;
        *UART_CONF1_REG = reg;
        *(PRO_UART_INTR_MAP_REG) = NUM_HAL_INTERRUPT_UART;
        (VOID)LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);
    }
    return (INT32)ret;
}

UartControllerOps g_genericUart = {
    .uartInit = NULL,
    .uartWriteChar = UartWriteChar,
    .uartReadChar = UartReadChar,
    .uartHwiCreate = UartHwi,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

