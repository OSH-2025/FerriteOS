/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart Init Implementation
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

#include "usart.h"
#include "los_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID UartWriteChar(const CHAR c)
{
    UINT32 txFifoCnt;
    do {
        txFifoCnt = (REG32_READ(UART_STATUS_REG) >> TXFIFP_CNT_SHIFT) & TXFIFO_CNT_MASK;
    } while (txFifoCnt == (UART_FIFO_BUFFER - 1));
    if (c == 0) {
        return;
    }
    REG32_WRITE(UART_FIFO_REG, c);
}

UINT8 UartReadChar(VOID)
{
    UINT8 rxFifoCnt = (UINT8)REG32_READ(UART_STATUS_REG);
    UINT8 c = 0;
    if (rxFifoCnt > 0) {
        c = (UINT8)REG32_READ(UART_FIFO_REG);
    }
    return c;
}

VOID UartPutc(CHAR c)
{
    UartWriteChar(c);
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
    REG32_MASK_SET(UART_INTCLR_REG, (RXFIFO_TOUT_INT_MASK | RXFIFO_FULL_INT_MASK));
    (VOID)LOS_HwiClear(NUM_HAL_INTERRUPT_UART);
}

STATIC INT32 UartHwi(VOID)
{
    UINT32 ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, UartHandler, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s, %d, uart interrupt created failed, ret = %x.\n", __FILE__, __LINE__, ret);
    } else {
        REG32_MASK_SET(UART_INTCLR_REG, (RXFIFO_TOUT_INT_MASK | RXFIFO_FULL_INT_MASK));
        REG32_MASK_SET(UART_INTENA_REG, (RXFIFO_TOUT_INT_MASK | RXFIFO_FULL_INT_MASK));
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
