/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-27
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

#include <los_hwi.h>
#include "platform.h"
#include "usart.h"

#define REG32_WRITE(addr, val)       (*(volatile UINT32 *)(addr) = (val))
#define REG32_READ(addr)             (*(volatile UINT32 *)(addr))

#define UART_RXREADY_FLAG            (0x1)
#define UART_TXEMPTY_FLAG            (0x20)

#define UART_BAUD                    (115200)
#define AUX_UART_CLOCK               (500000000)

#define BYTE_OF_REG                  (4)
#define PIN_NUMS                     (10) /* Each register controls 10 pins */

VOID GpioPinMode(UINT32 pinNo, UINT32 value, UINTPTR base)
{
    UINTPTR reg = base + ((pinNo / PIN_NUMS) * BYTE_OF_REG);
    UINT32 shift = (pinNo % PIN_NUMS) * 3; /* Each pin mode is controlled by 3bit */

    UINT32 curval = REG32_READ(reg);
    curval &= ~(0x7U << shift); /* 0x7:0b111 */
    curval |= value << shift;
    REG32_WRITE(reg, curval);
}

VOID UsartInit(VOID)
{
    REG32_WRITE(AUX_ENABLES, 1);       /* Enable aux uart */
    REG32_WRITE(AUX_MU_IER_REG, 0);
    REG32_WRITE(AUX_MU_CNTL_REG, 0);
    REG32_WRITE(AUX_MU_LCR_REG, 0x3);  /* Set to 8 bit mode */
    REG32_WRITE(AUX_MU_MCR_REG, 0);
    REG32_WRITE(AUX_MU_IIR_REG, 0xC6); /* Enable fifo and clear tx/rx fifo */
    REG32_WRITE(AUX_MU_BAUD_REG, ((AUX_UART_CLOCK / (UART_BAUD * 8)) - 1)); /* BAUD_REG = (clock / baudrate * 8 - 1) */

    GpioPinMode(GPIO_TX0, FUNC5_ALT, GPFSEL0);
    GpioPinMode(GPIO_RX0, FUNC5_ALT, GPFSEL0);
    REG32_WRITE(AUX_MU_CNTL_REG, 3);   /* Enable tx/rx */
}

VOID UsartWrite(const CHAR c)
{
    while (!(REG32_READ(AUX_MU_LSR_REG) & UART_TXEMPTY_FLAG)) {};
    REG32_WRITE(AUX_MU_IO_REG, (UINT32)c);
}

UINT8 UsartRead(VOID)
{
    UINT8 ch = 0xff;
    if (REG32_READ(AUX_MU_LSR_REG) & UART_RXREADY_FLAG) {
        ch = (UINT8)(REG32_READ(AUX_MU_IO_REG) & 0xff);
    }
    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
}

INT32 UsartHwi(VOID)
{
    UINT32 ret;
    /* uart interrupt priority should be the highest in interrupt preemption mode */
    ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, (HWI_PROC_FUNC)UartHandler, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s,%d, uart interrupt created error:%x\n", __FUNCTION__, __LINE__, ret);
    } else {
        REG32_WRITE(AUX_MU_IER_REG, 0x1);
        (VOID)ArchIrqUnmask(NUM_HAL_INTERRUPT_UART);
    }
    return (INT32)ret;
}

UartControllerOps g_genericUart = {
    .uartInit = UsartInit,
    .uartWriteChar = UsartWrite,
    .uartReadChar = UsartRead,
    .uartHwiCreate = UsartHwi
};
