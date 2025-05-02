/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: At Api Implementation
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

#include "at_api.h"

static AtAdaptorApi *g_atApi = NULL;

int32_t AtApiRegister(AtAdaptorApi *api)
{
    if (g_atApi == NULL) {
        g_atApi = api;
        if (g_atApi && g_atApi->init) {
            return g_atApi->init();
        }
    }

    return 0;
}

int32_t AtApiBind(const char *host, const char *port, int proto)
{
    int32_t ret = -1;

    if (g_atApi && g_atApi->bind) {
        ret = g_atApi->bind((int8_t *)host, (int8_t *)port, proto);
    }
    return ret;
}

int32_t AtApiConnect(const char *host, const char *port, int proto)
{
    int32_t ret = -1;

    if (g_atApi && g_atApi->connect) {
        ret = g_atApi->connect((int8_t *)host, (int8_t *)port, proto);
    }
    return ret;
}

int32_t AtApiSend(int32_t id, const unsigned char *buf, uint32_t len)
{
    if (g_atApi && g_atApi->send) {
        return g_atApi->send(id, buf, len);
    }
    return -1;
}

int32_t AtApiSendTo(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int port)
{
    if (g_atApi && g_atApi->sendto) {
        return g_atApi->sendto(id, buf, len, ipaddr, port);
    }
    return -1;
}

int32_t AtApiRecv(int32_t id, unsigned char *buf, size_t len)
{
    if (g_atApi && g_atApi->recv) {
        return g_atApi->recv(id, buf, len);
    }
    return -1;
}

int32_t AtApiRecvTimeout(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout)
{ 
    int bytes = 0;
    int rc = 0;
     while (bytes < len) {
        if (g_atApi && g_atApi->recvTimeout) {
            rc = g_atApi->recvTimeout(id, &buf[bytes], (size_t)(len - bytes), ipaddr, port, timeout);
           if (rc <= 0) {
                bytes = rc;
                break;
            } else {
                bytes += rc;
            }
        }
        return bytes;
    }
    return bytes;
}

int32_t AtApiRecvClose(int32_t fd)
{
    if (g_atApi == NULL || g_atApi->getLocalMaxFd == NULL) {
        return -1;
    }
    if (fd < g_atApi->getLocalMaxFd()) {
        if (g_atApi->close) {
            return g_atApi->close(fd);
        }
    }
    return -1;
}
