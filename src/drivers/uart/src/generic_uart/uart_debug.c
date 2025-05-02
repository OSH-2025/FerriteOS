/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Uart Module Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_task.h"
#include "los_queue.h"
#include "usart.h"

#define UART_QUEUE_SIZE        64
#define UART_QUEUE_BUF_MAX_LEN 1
#define UART_QUEUE_REC_DELAY   5

STATIC UINT32 g_uartQueue;

INT32 uart_putc(CHAR c)
{
    return uart_write(&c, 1, 0);
}

UINT8 uart_getc(VOID)
{
    UINT8 ch = 0;
    if (g_genericUart.uartReadChar != NULL) {
        ch = g_genericUart.uartReadChar();
    }
    (VOID)LOS_QueueWriteCopy(g_uartQueue, &ch, sizeof(UINT8), 0);
    return ch;
}

VOID uart_early_init(VOID)
{
    if (g_genericUart.uartInit != NULL) {
        g_genericUart.uartInit();
    }
}

INT32 ShellQueueCreat(VOID)
{
    return (INT32)LOS_QueueCreate("uartQueue", UART_QUEUE_SIZE, &g_uartQueue, 0, UART_QUEUE_BUF_MAX_LEN);
}

INT32 uart_hwiCreate(VOID)
{
    if (g_genericUart.uartHwiCreate != NULL) {
        g_genericUart.uartHwiCreate();
    }
    return LOS_OK;
}

UINT32 uart_read(UINT8 *rec, UINT32 timeout)
{
    if (rec == NULL) {
        return LOS_NOK;
    }
    UINT32 len;
    len = UART_QUEUE_BUF_MAX_LEN;
    return LOS_QueueReadCopy(g_uartQueue, rec, &len, timeout);
}

INT32 uart_write(const CHAR *buf, INT32 len, INT32 timeout)
{
    (VOID)timeout;
    UINT32 i;
    for (i = 0; i < len; i++) {  
        if (g_genericUart.uartWriteChar != NULL) {
            g_genericUart.uartWriteChar(buf[i]);
        }
    }
    return len;
}

VOID UartPuts(const CHAR *s, UINT32 len, BOOL isLock)
{
    UINT32 i;
    (VOID)isLock;
    if (s == NULL) {
        return;
    }
    for (i = 0; i < len; i++) {
        /*
         * Only system uart output needs to add extra '\r' to improve
         * the compatibility.
         */
        if (*(s + i) == '\n') {
            (VOID)uart_putc('\r');
        }
        (VOID)uart_putc(*(s + i));
    }
}

VOID uart_init(VOID) 
{
}

