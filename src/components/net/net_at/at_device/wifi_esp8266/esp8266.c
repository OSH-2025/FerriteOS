/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Esp8266 At Device
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

#include "esp8266.h"
#include "at_api.h"
#include "atiny_socket.h"

static int32_t Esp8266EchoOff(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_CMD_ECHO_OFF, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL, NULL);
}

static int32_t Esp8266Reset(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_CMD_RST, strlen(AT_CMD_RST), "ready\r\n", NULL, NULL);
}

static int32_t Esp8266SetNetMode(NetModeType mode)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_CWMODE, (int)mode);
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
}

static int32_t Esp8266SetMultiMode(int32_t mode)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_MUX, (int)mode);
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
}

static int32_t Esp8266JoinAP(char *pssid, char *ppasswd)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=\"%s\",\"%s\"", AT_CMD_JOINAP, pssid, ppasswd);
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
}

static int32_t Esp8266Connect(const int8_t *host, const int8_t *port, int32_t proto)
{
    int32_t ret = AT_FAILED;
    int32_t id = 0;
    char cmd[64] = {0};

    AT_LOG("host:%s, port:%s", host, port);
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd, 64, "%s=\"%s\",\"%s\",%s", AT_CMD_CONN, proto == ATINY_PROTO_UDP ? "UDP" : "TCP", host, port);
    } else {
        id = at->getLinkedId(); // get new socket fd.
        if ((id < 0) || (id >= AT_MAX_LINK_NUM)) {
            AT_LOG("no vailed linkedId for use(id = %ld)", id);
            return AT_FAILED;
        }
        snprintf(cmd, 64, "%s=%ld,\"%s\",\"%s\",%s", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP ? "UDP" : "TCP", host,port);
    }
    // create recv payload sem;
    ret = at->sem.create(0, (UINT32 *)&at->linkedId[id].payload.recvSem);
    if (ret != LOS_OK) {
        AT_LOG("esp8266 init at_recv_sem failed!");
        return AT_FAILED;
    }
    // init AtNetLinkHandle
    memcpy(at->linkedId[id].remoteIP, host, sizeof(at->linkedId[id].remoteIP));
     
    (void)sscanf((char *)port, "%d", &at->linkedId[id].remotePort);

    ret =  at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
    if (ret == AT_FAILED) {
        at->sem.delete(at->linkedId[id].payload.recvSem);
        AT_LOG("LOS_SemPend for listener return failed!");
        at->linkedId[id].usable = AT_LINK_UNUSE;
        return AT_FAILED;
    }
    return id;
}

static int32_t Esp8266Send(int32_t id, const uint8_t *buf, uint32_t len)
{
    int32_t ret = AT_FAILED;
    char cmd[64] = {0};
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd, 64, "%s=%lu", AT_CMD_SEND, len);
    } else {
        snprintf(cmd, 64, "%s=%ld,%lu", AT_CMD_SEND, id, len);
    }
    ret = at->write((int8_t *)cmd, (int8_t *)"SEND OK\r\n", (int8_t *)buf, len);
    return ret;
}

static int32_t Esp8266RecvTimeout(int32_t id, uint8_t *buf, uint32_t len, char *host, int *port, int32_t timeout)
{
    uint32_t ret;
    uint32_t totalSize;
    uint32_t read_bytes;
    uint32_t remain_bytes;

    if (id > AT_MAX_LINK_NUM) {
        return 0;
    }
    AtTaskHandle *at = AtGetTaskHandle();
    ret = at->sem.pend(at->linkedId[id].payload.recvSem, timeout);
    if (ret != LOS_OK) {
        AT_LOG("Recv timeout, there is no buffer to read.\n");
        return 0;
    }
    if (at->linkedId[id].payload.offset < 0) {
        at->linkedId[id].payload.offset = 0;
    }
    totalSize = at->linkedId[id].payload.totalSize;
    if (totalSize <= at->linkedId[id].payload.offset) { /* nothing to read */
        return 0;
    }

    remain_bytes = totalSize - at->linkedId[id].payload.offset;

    read_bytes = (remain_bytes < len ? remain_bytes : len);

    memcpy(buf, at->linkedId[id].payload.buff + at->linkedId[id].payload.offset, read_bytes);
    at->linkedId[id].payload.offset += read_bytes;
    at->sem.post(at->linkedId[id].payload.recvSem); // in order to copy the remaining data next time.
    return read_bytes;
}

int32_t Esp8266Recv(int32_t id, uint8_t *buf, uint32_t len)
{
    return Esp8266RecvTimeout(id, buf, len, NULL, NULL, LOS_WAIT_FOREVER);
}

int32_t Esp8266Close(int32_t id)
{
    char cmd[64] = {0};
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->linkedId == NULL) {
        return -1;
    }

    if (at->linkedId[id].usable == AT_LINK_UNUSE) {
        return 0;
    }

    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd, 64, "%s", AT_CMD_CLOSE);
    } else {
        snprintf(cmd, 64, "%s=%ld", AT_CMD_CLOSE, id);
    }
    return  at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
}

static int32_t Esp8266DataHandler(void *arg, int8_t *buf, int32_t len)
{
    if ((buf == NULL) || (len <= 0)) {
        AT_LOG("param invailed!");
        return AT_FAILED;
    }
    AT_LOG("entry!");

    // process data frame, for example:+IPD,linkedId,len:data
    int32_t linkedId = 0;
    int32_t data_len = 0;
    int32_t remotePort = 0;
    char *remoteIP = NULL;
    char *p1 = NULL;
    char *p2 = NULL;
    AtTaskHandle *at = AtGetTaskHandle();
    if ((p1 = strstr((char *)buf, AT_DATAF_PREFIX)) != NULL) {
        p2 = strstr(p1, ",");
        if (p2 == NULL) {
            AT_LOG("got data prefix invailed!");
            return 0;
        }

        if (at->multiMode == AT_MUXMODE_MULTI) {
            linkedId = 0;
            for (p2++; *p2 <= '9' && *p2 >= '0'; p2++) {
                linkedId = linkedId * 10 + (*p2 - '0');
            }
        }

        for (data_len = 0, p2++; *p2 <= '9' && *p2 >= '0'; p2++) {
            data_len = (data_len * 10 + (*p2 - '0'));
        }
        
        if (at->config.dinfo == 1) {
            // remote ip
            remoteIP = (char *)at->linkedId[linkedId].remoteIP;
            for (p2++; *p2 != ','; p2++) {
                *(remoteIP++) = *p2;
            }
            // remote port
            remotePort = 0;
            for (p2++; *p2 <= '9' && *p2 >= '0'; p2++) {
                remotePort = (remotePort * 10 + (*p2 - '0'));
            }
            at->linkedId[linkedId].remotePort = remotePort;
        }
        p2++; // over ':'

        if (at->linkedId[linkedId].payload.buff != NULL) {
            AtFree(at->linkedId[linkedId].payload.buff);
        }
        at->linkedId[linkedId].payload.buff = AtMalloc(data_len);
        if (at->linkedId[linkedId].payload.buff == NULL) {
            AT_LOG("malloc for recv payload failed!");
                return 0;
        }
        memcpy(at->linkedId[linkedId].payload.buff, p2, data_len);
        at->linkedId[linkedId].payload.totalSize = data_len;
        at->linkedId[linkedId].payload.offset = 0;
        at->linkedId[linkedId].payload.lastEnd = 0;
        (void)at->sem.post(at->linkedId[linkedId].payload.recvSem);
    }
    return 0;
}

/* get local IP */
int8_t Esp8266GetLocalIp(int8_t *ip, int8_t *gw, int8_t *mask)
{
    char resp[512] = {0};
    int len = 512;
    AtTaskHandle *at = AtGetTaskHandle();
    at->writeCmd((int8_t *)AT_CMD_CHECK_IP, strlen((char *)AT_CMD_CHECK_IP), "OK", resp, &len);

    AT_LOG("resp:%s", resp);
    char *p1, *p2;
    p1 = strstr(resp, "ip");
    if (ip && p1) {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(ip, p1 + 1, p2 - p1 - 1);
    }

    p1 = strstr(resp, "gateway");
    if (gw && p1) {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(gw, p1 + 1, p2 - p1 - 1);
    }

    p1 = strstr(resp, "netmask");
    if (mask && p1) {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(mask, p1 + 1, p2 - p1 - 1);
    }

    return AT_OK;
}

/* get local mac */
int8_t Esp8266GetLocalMac(int8_t *mac)
{
    char resp[512] = {0};
    char *p1, *p2;
    int len = 512;
    AtTaskHandle *at = AtGetTaskHandle();
    at->writeCmd((int8_t *)AT_CMD_CHECK_MAC, strlen((char *)AT_CMD_CHECK_MAC), "OK", resp, &len);
    AT_LOG("resp:%s", resp);

    p1 = strstr(resp, ":");
    if (mac && p1) {
        p1 = strstr(p1, "\"");
        p2 = strstr(p1 + 1, "\"");
        memcpy(mac, p1 + 1, p2 - p1 - 1);
    }

    return AT_OK;
}

int32_t Esp8266Bind(const int8_t *host, const int8_t *port, int32_t proto)
{
    int ret = AT_FAILED;
    int port_i = 0;
    char cmd[64] = {0};

    (void)sscanf((char *)port, "%d", &port_i);
    AT_LOG("get port = %d\r\n", port_i);
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->multiMode != AT_MUXMODE_MULTI) {
        AT_LOG("Only support in multi mode!\r\n");
        return ret;
    }
    int id = at->getLinkedId();
    snprintf(cmd, 64, "%s=%d,\"%s\",\"0.0.0.0\",0,%d,0", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP ? "UDP" : "TCP", port_i);
    at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
    return id;
}

int32_t Esp8266RecvCallback(int32_t id)
{
    return AT_FAILED;
}

int32_t Esp8266Deinit(void)
{
    int id = 0;
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->linkedId != NULL) {
        for (id = 0; id < AT_MAX_LINK_NUM; id++) {
            if (at->linkedId[id].usable == AT_LINK_INUSE) {
                if (Esp8266Close(id) != AT_OK) {
                    AT_LOG("Esp8266Close(%d) failed", id);
                }
            }
        }
    }

    at->deInit();
    return AT_OK;
}

static int32_t Esp8266ShowDinfo(int32_t s)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%ld", AT_CMD_SHOW_DINFO, s);
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK\r\n", NULL, NULL);
}

static char *Esp8266SendCmdMatch(const char *buf, char *featureStr, int len)
{
    (void) len;
    if (buf == NULL || featureStr == NULL) {
        return NULL;
    }
    return strstr(buf, featureStr);
}

AtConfig g_deviceConf = {
    .name = AT_MODU_NAME,
    .usartPort = AT_USART_PORT,
    .buardrate = AT_BUARDRATE,
    .maxLinkIdNum = AT_MAX_LINK_NUM,
    .maxBufferLen = MAX_AT_USERDATA_LEN,
    .cmdBeginStr = AT_CMD_BEGIN,
    .lineEndStr = AT_LINE_END,
    .multiMode = AT_MUXMODE_MULTI,            
    .timeout = AT_CMD_TIMEOUT, // ms
    .dinfo = 0, // close cipdinfo
};

int32_t Esp8266Init(void)
{ 
    AtTaskHandle *at = AtGetTaskHandle();
    at->init(&g_deviceConf);
    at->oobRegister(AT_DATAF_PREFIX, strlen(AT_DATAF_PREFIX), Esp8266DataHandler, Esp8266SendCmdMatch);

    Esp8266Reset();     // restart esp8266.
    Esp8266EchoOff(); // close the echo
    Esp8266ShowDinfo(g_deviceConf.dinfo); // +IPD--> ip, port

    Esp8266SetNetMode(STA);
    while (Esp8266JoinAP(WIFI_SSID, WIFI_PASSWD) == AT_FAILED) {
        AT_LOG("connect ap failed, repeat...");
    };
    Esp8266SetMultiMode(at->multiMode);

    static int8_t ip[32];
    static int8_t gw[32];
    static int8_t mac[32];
    Esp8266GetLocalIp(ip, gw, NULL);
    Esp8266GetLocalMac(mac);
    AT_LOG_DEBUG("get ip:%s, gw:%s mac:%s", ip, gw, mac);
    return AT_OK;
}

static int32_t Esp8266GetLocalMaxFd(void)
{
    return AT_MAX_LINK_NUM;
}

AtAdaptorApi AtGetEsp8266Interface(void)
{
    AtAdaptorApi esp8266Interface = {
        .init = Esp8266Init,
        .getLocalMac = Esp8266GetLocalMac, /* get local MAC */
        .getLocalIp = Esp8266GetLocalIp,   /* get local IP */
        .getLocalMaxFd = Esp8266GetLocalMaxFd,
        /* build TCP or UDP connection */
        .connect = Esp8266Connect,
        .bind = Esp8266Bind,
        .send = Esp8266Send,
        .recvTimeout = Esp8266RecvTimeout,
        .recv = Esp8266Recv,
        .close = Esp8266Close,     /* close connection */
        .recvCallback = Esp8266RecvCallback, /* operation for events, not implements yet */
        .deInit = Esp8266Deinit,
    };
    return esp8266Interface;
}