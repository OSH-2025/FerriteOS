/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: At Hal Implementation
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

#include "at_hal.h"
#include "usart.h"
#include "main.h"
#include <string.h>
#include "los_task.h"
#include "los_queue.h"

UART_HandleTypeDef g_usartHandle;

static USART_TypeDef *g_atUsartId = USART2; //default set
static uint32_t g_atIRQn = USART2_IRQn; // default set
static AtUsartRecv g_atUsartRecv;
static AtUsartRingBuffer ringBuffer;

#define USART_LINE_END "\r\n"

#define USART_TRANSMIT_TIMEOUT 0xffff

static void UsartAdaptor(uint32_t port)
{
#ifdef LOSCFG_PLATFORM_STM32L431_BearPi
    g_atUsartId = LPUART1;
    g_atIRQn = LPUART1_IRQn;
    return;
#endif

#if  defined(STM32F072xB)
    switch (port) {
        case 3: // 3: usart3
            g_atUsartId = USART3;
        case 4:
            g_atIRQn = USART3_4_IRQn;
            return;
            break;
    }
#endif

    switch (port) {
        case 6: // 6: usart6
#if  defined(USART6)  
            g_atUsartId = USART6;
            g_atIRQn = USART6_IRQn;
#else
            return;
#endif
            break;
        case 1: // 1: usart1
#if  defined(USART1)        
            g_atUsartId = USART1;
            g_atIRQn = USART1_IRQn;
#else
            return;
#endif
            break;
        case 2: // 2: usart2
#if  defined(USART2)        
            g_atUsartId = USART2;
            g_atIRQn = USART2_IRQn;
#else
            return;
#endif
            break;
        case 3: // 3: usart3
#if  defined(USART3)        
            g_atUsartId = USART3;
            g_atIRQn = USART3_IRQn;
#else
            return;
#endif
            break;
        case 5:
#if  defined(UART5)        
            g_atUsartId = UART5;
            g_atIRQn = UART5_IRQn;
#else
            return;
#endif
            break;
        default:
            return;
            break;
    }
}

void AtUsartIrqHandler(void)
{
    uint8_t ch;
    AtRecvQueue recv;

    if (__HAL_UART_GET_FLAG(&g_usartHandle, UART_FLAG_RXNE) != RESET) {
        HAL_UART_Receive(&g_usartHandle, &ch, 1, 0);
        g_atUsartRecv.buffer[ringBuffer.wi++] = ch;
        if (ringBuffer.wi == ringBuffer.ri) {
            ringBuffer.buff_full = true;
        }
        if (ringBuffer.wi >= g_atUsartRecv.maxLen) {
            ringBuffer.wi = 0;
        }
    } else if (__HAL_UART_GET_FLAG(&g_usartHandle, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&g_usartHandle);
        /*
        Ring Buffer ri------------------------>wi

         __________________________________________________
         |      msg0           |  msg1        |   msg2    |
         ri(pre_ri0)        pre_ri1         pre_ri2     wi(pre_ri3)
         __________________________________________________

         read_resp ---->ri= pre_ri1----------->---------->ri=wi=pre_ri3(end)
        */
        
        recv.ori = ringBuffer.ri;
        recv.end = ringBuffer.wi;
        ringBuffer.ri = ringBuffer.wi;

        recv.msgType = AT_USART_RX;
        LOS_QueueWriteCopy(g_atUsartRecv.queueId, &recv, sizeof(AtRecvQueue), 0);
    }
}

void AtReadQueueBuffer(uint8_t *buffer, uint32_t *bufferLen, AtRecvQueue *recvQueue)
{
    uint32_t len = 0;
    
    if (buffer == NULL) {
        *bufferLen = 0;
        return;
    }

    if (ringBuffer.buff_full == true) {
        printf("buf maybe full,buff_full is %d", ringBuffer.buff_full);
    }

    if (recvQueue->end == recvQueue->ori) {
        *bufferLen = 0;
        return;
    }
    /* Copy uart queue cache to buffer  */
    if (recvQueue->end > recvQueue->ori) {
        /* current buffer: | [0]-----[ri]-------[wi]--------[end]| */
        len = recvQueue->end - recvQueue->ori;
        /* copy buffer: | [ri]-------[wi] | */
        (void)memcpy_s(buffer, len, &g_atUsartRecv.buffer[recvQueue->ori], len);
    } else {
        /* current buffer: | [0]-----[wi]-------[ri]--------[end] | */
        uint32_t tmp_len = g_atUsartRecv.maxLen - recvQueue->ori;
        /* first copy buffer: | [ri]-------[end] | */
        (void)memcpy_s(buffer, tmp_len, &g_atUsartRecv.buffer[recvQueue->ori], tmp_len);
        /* step 2: | [0]-------[wi] | */
        (void)memcpy_s(buffer + tmp_len, recvQueue->end, &g_atUsartRecv.buffer[0], recvQueue->end);
        len = recvQueue->end + tmp_len;
    }
    
    *bufferLen = len;
}

int32_t AtUsartInit(AtUsartRecv *handle)
{
    UART_HandleTypeDef *usart = &g_usartHandle;
    
    g_atUsartRecv.buffer = handle->buffer;
    g_atUsartRecv.maxLen = handle->maxLen;
    g_atUsartRecv.queueId = handle->queueId;
    UsartAdaptor(handle->uartPort);
    usart->Instance = g_atUsartId;
    usart->Init.BaudRate = handle->buardrate;

    usart->Init.WordLength = UART_WORDLENGTH_8B;
    usart->Init.StopBits = UART_STOPBITS_1;
    usart->Init.Parity = UART_PARITY_NONE;
    usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    usart->Init.Mode = UART_MODE_RX | UART_MODE_TX;
    if (HAL_UART_Init(usart) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_NVIC_EnableIRQ(g_atIRQn);
    __HAL_UART_CLEAR_FLAG(usart, UART_FLAG_TC);
    LOS_HwiCreate(g_atIRQn + 16, 0, 0, AtUsartIrqHandler, 0);
    __HAL_UART_ENABLE_IT(usart, UART_IT_IDLE);
    __HAL_UART_ENABLE_IT(usart, UART_IT_RXNE);

    return LOS_OK;
}

void AtUsartDeinit(void)
{
    UART_HandleTypeDef *husart = &g_usartHandle;
    __HAL_UART_DISABLE(husart);
    __HAL_UART_DISABLE_IT(husart, UART_IT_IDLE);
    __HAL_UART_DISABLE_IT(husart, UART_IT_RXNE);
}

void AtUsartTransmit(uint8_t *cmd, int32_t len, int lineEndFlag)
{
    (void)HAL_UART_Transmit(&g_usartHandle, (uint8_t *)cmd, len, USART_TRANSMIT_TIMEOUT);
    if (lineEndFlag) {
        (void)HAL_UART_Transmit(&g_usartHandle, (uint8_t *)USART_LINE_END, sizeof(USART_LINE_END), USART_TRANSMIT_TIMEOUT);
    }
}
