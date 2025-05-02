/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: At API HeadFile
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

#ifndef _AT_API_H
#define _AT_API_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    int32_t (*init)(void);
    int8_t (*getLocalMac)(int8_t *mac);
    int8_t (*getLocalIp)(int8_t *ip, int8_t *gw, int8_t *mask);
    int32_t (*getLocalMaxFd)(void); // get max socket fd.
    int32_t (*bind)(const int8_t *host, const int8_t *port, int32_t proto);
    int32_t (*connect)(const int8_t *host, const int8_t *port, int32_t proto);
    int32_t (*send)(int32_t id, const uint8_t *buf, uint32_t len);
    int32_t (*sendto)(int32_t id, const uint8_t *buf, uint32_t len, char *ipaddr, int port);
    int32_t (*recvTimeout)(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout);
    int32_t (*recv)(int32_t id, uint8_t *buf, uint32_t len);
    int32_t (*recvFrom)(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port);
    int32_t (*close)(int32_t id);
    int32_t (*recvCallback)(int32_t id);
    int32_t (*deInit)(void);
} AtAdaptorApi;

int32_t AtApiRegister(AtAdaptorApi *api);
int32_t AtApiBind(const char *host, const char *port, int proto);
int32_t AtApiConnect(const char *host, const char *port, int proto);
int32_t AtApiSend(int32_t id, const unsigned char *buf, uint32_t len);
int32_t AtApiSendTo(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int port);
int32_t AtApiRecv(int32_t id, unsigned char *buf, size_t len);
int32_t AtApiRecvTimeout(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout);
int32_t AtApiRecvClose(int32_t fd);

#endif /* _AT_API_H */
