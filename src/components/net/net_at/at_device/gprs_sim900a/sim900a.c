/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Sim900a At Device
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

#include "sim900a.h"
#include "at_api.h"
#include "atiny_socket.h"

AtAdaptorApi g_sim900aInterface;
char prefix_name[15];

static int32_t Sim900aEchoOff(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_CMD_ECHO_OFF, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL, NULL);
}

static int32_t Sim900aEchoOn(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_CMD_ECHO_ON, strlen(AT_CMD_ECHO_OFF), "OK\r\n", NULL, NULL);
}

static int32_t Sim900aReset(void)
{
    int32_t ret = 0;
    AtTaskHandle *at = AtGetTaskHandle();
    ret = at->writeCmd((int8_t *)AT_CMD_SHUT, strlen(AT_CMD_SHUT), "SHUT OK", NULL, NULL);
    return ret;
}

static int32_t Sim900aSetMuxMode(int32_t m)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d", AT_CMD_MUX, (int)m);
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK", NULL, NULL);
}

static int32_t Sim900aConnect(const int8_t *host, const int8_t *port, int32_t proto)
{
    int32_t ret = AT_FAILED;
    AtTaskHandle *at = AtGetTaskHandle();
    int32_t id = at->getLinkedId();
    Sim900aReset();
    char cmd1[64] = {0};
    snprintf(cmd1, 64, "%s=\"B\"", AT_CMD_CLASS);
    at->writeCmd((int8_t *)cmd1, strlen(cmd1), "OK", NULL, NULL);
    char cmd2[64] = {0};
    snprintf(cmd2, 64, "%s=1,\"IP\",\"CMNET\"", AT_CMD_PDP_CONT);
    at->writeCmd((int8_t *)cmd2, strlen(cmd2), "OK", NULL, NULL);
    char cmd3[64] = {0};
    snprintf(cmd3, 64, "%s=1", AT_CMD_PDP_ATT);
    at->writeCmd((int8_t *)cmd3, strlen(cmd3), "OK", NULL, NULL);
    char cmd4[64] = {0};
    snprintf(cmd4, 64, "%s=1", AT_CMD_CIPHEAD);
    at->writeCmd((int8_t *)cmd4, strlen(cmd4), "OK", NULL, NULL);
    char cmd5[64] = {0};

    AT_LOG_DEBUG("host:%s, port:%s", host, port);

    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd5, 64, "%s=\"%s\",\"%s\",\"%s\"", AT_CMD_CONN, proto == ATINY_PROTO_UDP ? "UDP" : "TCP", host,
            port);
    } else {
        at->writeCmd((int8_t *)(AT_CMD_PDP_ACT "=1,1"), strlen(AT_CMD_PDP_ACT "=1,1"), "OK", NULL, NULL);
        at->writeCmd((int8_t *)AT_CMD_CSTT, strlen(AT_CMD_CSTT), "OK", NULL, NULL);
        at->writeCmd((int8_t *)AT_CMD_CIICR, strlen(AT_CMD_CIICR), "OK", NULL, NULL);
        at->writeCmd((int8_t *)AT_CMD_CIFSR, strlen(AT_CMD_CIFSR), "OK", NULL, NULL);
        snprintf(cmd5, 64, "%s=%ld,\"%s\",\"%s\",\"%s\"", AT_CMD_CONN, id, proto == ATINY_PROTO_UDP ? "UDP" : "TCP",
            host, port);
    }
    if ((id < 0) || (id >= AT_MAX_LINK_NUM)) {
        AT_LOG("no vailed linkedId for use(id = %ld)", id);
        return -1;
    }
    // create recv payload sem;
    ret = at->sem.create(0, (UINT32 *)&at->linkedId[id].payload.recvSem);
    if (ret != LOS_OK) {
        AT_LOG("esp8266 init at_recv_sem failed!");
        return AT_FAILED;
    }
    at->writeCmd((int8_t *)cmd5, strlen(cmd5), "CONNECT OK", NULL, NULL);
    return id;
}

int32_t Sim900aRecvTimeout(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout)
{
    (void)ipaddr; // gprs not need remote ip
    (void)port;   // gprs not need remote port

    uint32_t ret;
    uint32_t totalSize;
    uint32_t readBytes;
    uint32_t remainBytes;

    if (id > AT_MAX_LINK_NUM) {
        return 0;
    }
    AtTaskHandle *at = AtGetTaskHandle();
    ret = at->sem.pend(at->linkedId[id].payload.recvSem, timeout);
    if (ret != LOS_OK) {
        AT_LOG("%s, sempend timeout, time=%d\n", __func__, timeout);
        return 0;
    }
    if (at->linkedId[id].payload.offset < 0) {
        at->linkedId[id].payload.offset = 0;
    }
    totalSize = at->linkedId[id].payload.totalSize;
    
    if (totalSize <= at->linkedId[id].payload.offset) { /* nothing to read */
        return 0;
    }

    remainBytes = totalSize - at->linkedId[id].payload.offset;
    readBytes = (remainBytes < len ? remainBytes : len);

    memcpy(buf, at->linkedId[id].payload.buff + at->linkedId[id].payload.offset, readBytes);
    at->linkedId[id].payload.offset += readBytes;
    at->sem.post(at->linkedId[id].payload.recvSem);

    return readBytes;
}

int32_t Sim900aRecv(int32_t id, uint8_t *buf, uint32_t len)
{
    return Sim900aRecvTimeout(id, buf, len, NULL, NULL, LOS_WAIT_FOREVER);
}

int32_t Sim900aSend(int32_t id, const uint8_t *buf, uint32_t len)
{
    int32_t ret = -1;
    char cmd[64] = {0};
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd, 64, "%s=%ld", AT_CMD_SEND, len);
    } else {
        snprintf(cmd, 64, "%s=%ld,%ld", AT_CMD_SEND, id, len);
    }
    ret = at->write((int8_t *)cmd, (int8_t *)"SEND OK", (int8_t *)buf, len);
    return ret;
}

void Sim900aCheck(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    while (at->writeCmd((int8_t *)AT_CMD_AT, strlen(AT_CMD_AT), "OK", NULL, NULL) == AT_FAILED) {
        printf("\r\ncheck module response unnormal\r\n");
        printf("\r\nplease check the module pin connection and the power switch\r\n");
        SIM900A_DELAY(500);
    }
    if (at->writeCmd((int8_t *)AT_CMD_CPIN, strlen(AT_CMD_CPIN), "OK", NULL, NULL) != AT_FAILED) {
        printf("detected sim card\n");
    }
    if (at->writeCmd((int8_t *)AT_CMD_COPS, strlen(AT_CMD_COPS), "CHINA MOBILE", NULL, NULL) != AT_FAILED) {
        printf("registerd to the network\n");
    }
}

static int32_t Sim900aRecvCallback(int32_t id)
{
    return AT_FAILED;
}

static int32_t Sim900aClose(int32_t id)
{
    AtTaskHandle *at = AtGetTaskHandle();
    char cmd[64] = {0};
    if (at->multiMode == AT_MUXMODE_SINGLE) {
        snprintf(cmd, 64, "%s", AT_CMD_CLOSE);
    } else {
        at->linkedId[id].usable = 0;
        snprintf(cmd, 64, "%s=%ld", AT_CMD_CLOSE, id);
    }
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK", NULL, NULL);
}

int32_t Sim900aDataHandler(void *arg, int8_t *buf, int32_t len)
{
    if (buf == NULL || len <= 0) {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("entry!");

    int32_t ret = 0;
    int32_t linkId = 0;
    int32_t dataLen = 0;
    char *p1 = NULL;
    char *p2 = NULL;
    AtTaskHandle *at = AtGetTaskHandle();
    if ((p1 = strstr((char *)buf, prefix_name)) != NULL) {
        p2 = strstr(p1, ",");
        if (p2 == NULL) {
            AT_LOG("got data prefix invailed!");
            return ret;
        }

        if (at->multiMode == AT_MUXMODE_MULTI) {
            linkId = 0;
            for (p2++; *p2 <= '9' && *p2 >= '0'; p2++) {
                linkId = linkId * 10 + (*p2 - '0');
            }
        }

        for (p2++; *p2 <= '9' && *p2 >= '0'; p2++) {
            dataLen = (dataLen * 10 + (*p2 - '0'));
        }
        p2++; // over ':'

        if (dataLen > len) {
            AT_LOG("error !! receive data not complete dataLen:%ld len:%ld", dataLen, len);
            return ret;
        }

        if (at->multiMode == AT_MUXMODE_MULTI) {
            p2++;
            p2++; // multi-connect prefix is +RECEIVE,0,13:\r\n+packet content
        }
        // memcpy buffer
        if (at->linkedId[linkId].payload.buff != NULL) {
            AtFree(at->linkedId[linkId].payload.buff);
        }
        at->linkedId[linkId].payload.buff = AtMalloc(dataLen);
        if (at->linkedId[linkId].payload.buff == NULL) {
            AT_LOG("malloc for recv payload failed!");
            return ret;
        }
        memcpy(at->linkedId[linkId].payload.buff, p2, dataLen);
        at->linkedId[linkId].payload.totalSize = dataLen;
        at->linkedId[linkId].payload.offset = 0;
        at->linkedId[linkId].payload.lastEnd = 0;
        (void)at->sem.post(at->linkedId[linkId].payload.recvSem);
    }
    return ret;
}

static char *Sim900aCmdMatch(const char *buf, char *featureStr, int len)
{
    (void)len;
    return strstr(buf, featureStr);
}

static int32_t Sim900aConfigInit(void)
{
    AtConfig at_user_conf = {
        .name = AT_MODU_NAME,
        .usartPort = AT_USART_PORT,
        .buardrate = AT_BUARDRATE,
        .maxLinkIdNum = AT_MAX_LINK_NUM,
        .maxBufferLen = MAX_AT_USERDATA_LEN,
        .cmdBeginStr = AT_CMD_BEGIN,
        .lineEndStr = AT_LINE_END,
        .multiMode = AT_MUXMODE_MULTI, // support multi connection mode
        .timeout = AT_CMD_TIMEOUT, // ms
    };
    AtTaskHandle *at = AtGetTaskHandle();
    at->init(&at_user_conf);
    // single and multi connect prefix is different
    if (at->multiMode == AT_MUXMODE_MULTI) {
        memcpy(prefix_name, AT_DATAF_PREFIX_MULTI, sizeof(AT_DATAF_PREFIX_MULTI));
    } else {
        memcpy(prefix_name, AT_DATAF_PREFIX, sizeof(AT_DATAF_PREFIX));
    }
    at->oobRegister((char *)prefix_name, strlen((char *)prefix_name), Sim900aDataHandler, Sim900aCmdMatch);
    Sim900aEchoOff();
    Sim900aCheck();
    Sim900aReset();
    Sim900aSetMuxMode(at->multiMode);
    at->writeCmd((int8_t *)AT_CMD_CHECK_MUX, strlen(AT_CMD_CHECK_MUX), "OK", NULL, NULL);
    return AT_OK;
}

static int32_t Sim900aDeinit(void)
{
    int id = 0;
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->linkedId != NULL) {
        for (id = 0; id < AT_MAX_LINK_NUM; id++) {
            if (at->linkedId[id].usable == AT_LINK_INUSE) {
                if (Sim900aClose(id) != AT_OK) {
                    AT_LOG("Sim900aClose(%d) failed", id);
                }
            }
        }
    }

    at->deInit();
    return AT_OK;
}

static int32_t Sim900aGetLocalMaxFd(void) {
    return AT_MAX_LINK_NUM;
}

AtAdaptorApi AtGetSim900aInterface(void)
{
    AtAdaptorApi sim900aInterface = {
        .init = Sim900aConfigInit,
        .getLocalMaxFd = Sim900aGetLocalMaxFd,
        .connect = Sim900aConnect, /* TCP or UDP connect */
        .send = Sim900aSend,       /* send data, if no response, retrun error */
        .recvTimeout = Sim900aRecvTimeout,
        .recv = Sim900aRecv,
        .close = Sim900aClose,     /* close connect */
        .recvCallback = Sim900aRecvCallback, /* receive event handle, no available by now */
        .deInit = Sim900aDeinit
    };
    return sim900aInterface;
}


