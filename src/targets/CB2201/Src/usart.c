/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Uart Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-10-21
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

#define UART_BAUDRATE          115200

STATIC usart_handle_t g_consoleHandle;

VOID UsartInit(VOID)
{
    (VOID)drv_pinmux_config(CONSOLE_TXD, CONSOLE_TXD_FUNC);
    (VOID)drv_pinmux_config(CONSOLE_RXD, CONSOLE_RXD_FUNC);

    /* init the console */
    g_consoleHandle = csi_usart_initialize(CONSOLE_IDX, NULL);

    /* config the UART */
    (VOID)csi_usart_config(g_consoleHandle, UART_BAUDRATE, USART_MODE_ASYNCHRONOUS,
                     USART_PARITY_NONE, USART_STOP_BITS_1, USART_DATA_BITS_8);
}

void UsartWrite(const CHAR ch)
{
    if (g_consoleHandle == NULL) {
        return;
    }

    (VOID)csi_usart_putchar(g_consoleHandle, ch);
}

UINT8 UsartRead(VOID)
{
    UINT8 ch = 0;

    if (g_consoleHandle == NULL) {
        return 0;
    }

    (VOID)csi_usart_getchar(g_consoleHandle, &ch);

    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
    (VOID)LOS_HwiClear(NUM_HAL_INTERRUPT_UART);
}

INT32 UsartHwi(VOID)
{
    (VOID)LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, UartHandler, NULL);
    (VOID)LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);

    return LOS_OK;
}

UartControllerOps g_genericUart = {
    .uartInit = UsartInit,
    .uartWriteChar = UsartWrite,
    .uartReadChar = UsartRead,
    .uartHwiCreate = UsartHwi
};


