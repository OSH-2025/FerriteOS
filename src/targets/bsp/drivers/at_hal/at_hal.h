/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: At Hal HeadFile
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

#ifndef _AT_HAL_H
#define _AT_HAL_H

#include <stdio.h>
#include <los_typedef.h>

enum {
    AT_USART_RX,
    AT_TASK_QUIT,
    AT_SENT_DONE
};

/* AT ring buff, save the received serial data */
typedef struct {
    uint32_t wi; // write position
    uint32_t ri; // read postion
    uint8_t buff_full; // write full flag
} AtUsartRingBuffer;

typedef struct {
    uint32_t uartPort;
    uint32_t buardrate;
    uint32_t queueId;
    uint8_t *buffer; // recv buff;
    uint32_t maxLen; // at frame buffer max length.
} AtUsartRecv;

typedef uint32_t AtMsgType;
typedef struct {
    uint32_t ori;
    uint32_t end;
    AtMsgType msgType;
} AtRecvQueue;

int32_t AtUsartInit(AtUsartRecv *handle);
void AtUsartDeinit(void);
void AtUsartTransmit(uint8_t *cmd, int32_t len, int lineEndFlag);
void AtReadQueueBuffer(uint8_t *buffer, uint32_t *bufferLen, AtRecvQueue *recvQueue);
#endif /* _AT_HAL_H */
