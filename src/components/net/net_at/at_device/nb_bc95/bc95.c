/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Bc95 At Device
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

#include <string.h>
#include <ctype.h>
#include "bc95.h"
#include "at_hal.h"

AtAdaptorApi bc95_interface;
char rbuf[AT_DATA_LEN];
char wbuf[AT_DATA_LEN];

typedef struct {
    uint32_t dataLen;
    int linkIndex;
    bool validFlag;
} NbDataInfoHandle;

char tmpbuffer[AT_DATA_LEN] = {0}; // transform to hex

SocketInfo sockInfo[MAX_SOCK_NUM];
static NbDataInfoHandle g_nbDataInfo;

static int NbAllocSock(int socket)
{
    int idx;

    for (uint32_t i = 0; i < MAX_SOCK_NUM; ++i) {
        if (sockInfo[i].usedFlag && (sockInfo[i].socket == socket)) {
            return i;
        }
    }

    idx = (socket % MAX_SOCK_NUM);
    if (!sockInfo[idx].usedFlag) {
        return idx;
    }

    for (uint32_t i = 0; i < MAX_SOCK_NUM; ++i) {
        if (!sockInfo[i].usedFlag) {
            return i;
        }
    }
    AT_LOG("save socket fail %d", socket);
    return MAX_SOCK_NUM;
}

static int NbSockToIndex(int socket)
{
    int idx;

    idx = (socket % MAX_SOCK_NUM);

    if (sockInfo[idx].usedFlag && (socket == sockInfo[idx].socket)) {
        return idx;
    }

    for (uint32_t i = 0; i < MAX_SOCK_NUM; ++i) {
        if (sockInfo[i].usedFlag && (socket == sockInfo[i].socket)) {
            return i;
        }
    }

    return MAX_SOCK_NUM;
}

int32_t StrToHex(const char *bufin, int len, char *bufout)
{
    int i = 0;
    if (NULL == bufin || len <= 0 || NULL == bufout) {
        return -1;
    }

    for (i = 0; i < len; i++) {
        sprintf(bufout + i * 2, "%02X", bufin[i]);
    }

    return 0;
}

static void HexStrToStr(const uint8_t *source, uint8_t *dest, int sourceLen)
{
    short i;
    uint8_t highByte, lowByte;
    for (i = 0; i < sourceLen; i += 2) {
        highByte = toupper(source[i]);
        lowByte = toupper(source[i + 1]);

        if (highByte > 0x39) {
            highByte -= 0x37;
        } else {
            highByte -= 0x30;
        }

        if (lowByte > 0x39) {
            lowByte -= 0x37;
        } else {
            lowByte -= 0x30;
        }

        dest[i / 2] = (highByte << 4) | lowByte;
    }

    return;
}

int32_t NbReboot(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_NB_reboot, strlen(AT_NB_reboot), "OK", NULL, NULL);
}

// "AT+CFUN?\r"
int32_t NbHwDetect(void) {
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)AT_NB_hw_detect, strlen(AT_NB_hw_detect), "+CFUN:1", NULL, NULL);
}

int32_t NbCheckCsq(void)
{
    char *cmd = "AT+CSQ\r";
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "+CSQ:", NULL, NULL);
}

int32_t NbSetCdpServer(char *host, char *port)
{
    char *cmd = "AT+NCDP=";
    char *cmd2 = "AT+NCDP?";
    char *cmdNNMI = "AT+NNMI=1\r";
    char *cmdCMEE = "AT+CMEE=1\r";
    char cdpbuf[128] = {0};
    int ret = -1;
    char ipaddr[100] = {0};
    AtTaskHandle *at = AtGetTaskHandle();
    if ((strlen(host) > 70) || (strlen(port) > 20) || (host == NULL) || (port == NULL)) {
        ret = at->writeCmd((int8_t *)cmdNNMI, strlen(cmdNNMI), "OK", NULL, NULL);
        ret = at->writeCmd((int8_t *)cmdCMEE, strlen(cmdCMEE), "OK", NULL, NULL);
        return ret;
    }

    snprintf(ipaddr, sizeof(ipaddr) - 1, "%s,%s\r", host, port);
    snprintf(cdpbuf, sizeof(cdpbuf) - 1, "%s%s%c", cmd, ipaddr, '\r');

    ret = at->writeCmd((int8_t *)cdpbuf, strlen(cdpbuf), "OK", NULL, NULL);
    if (ret < 0) {
        return ret;
    }
    ret = at->writeCmd((int8_t *)cmd2, strlen(cmd2), ipaddr, NULL, NULL);
    ret = at->writeCmd((int8_t *)cmdNNMI, strlen(cmdNNMI), "OK", NULL, NULL);
    return ret;
}

int32_t NbSendPsk(char *pskId, char *psk)
{
    char *cmds = "AT+QSECSWT";                 // AT+QSECSWT=1,100    OK
    char *cmdp = "AT+QSETPSK";                 // AT+QSETPSK=86775942,E6F4C799   OK
    sprintf(wbuf, "%s=%d,%d\r", cmds, 1, 100); // min
    AtTaskHandle *at = AtGetTaskHandle();
    at->writeCmd((int8_t *)wbuf, strlen(wbuf), "OK", NULL, NULL);
    snprintf(wbuf, AT_DATA_LEN, "%s=%s,%s\r", cmdp, pskId, psk);
    return at->writeCmd((int8_t *)wbuf, strlen(wbuf), "OK", NULL, NULL);
}

int32_t NbSetNoEncrypt(void)
{
    char *cmd = "AT+QSECSWT=0\r";
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "OK", NULL, NULL);
}

#ifdef WITH_SOTA
int sota_cmd(int8_t *cmd, int32_t len, const char *suffix, char *respBuffer, int *respLen)
{
    AT_LOG("sota_cmd:%s", cmd);
    AtTaskHandle *at = AtGetTaskHandle();
    LOS_MuxPend(at->cmdMux, LOS_WAIT_FOREVER);
    at->config.UsartTtransmit((uint8_t *)cmd, len, 1);
    LOS_MuxPost(at->cmdMux);

    return AT_OK;
}

int nb_send_str(const char *buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    memset(wbuf, 0, AT_DATA_LEN);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN, "%s%d,%s%c", cmd1, (int)len / 2, buf, '\r');
    return sota_cmd((int8_t *)wbuf, strlen(wbuf), "OK", NULL, NULL);
}
#endif
int32_t NbSendPayload(const char *buf, int len)
{
    char *cmd1 = "AT+NMGS=";
    char *cmd2 = "AT+NQMGS\r";
    int ret;
    char *str = NULL;
    int curcnt = 0;
    int recvBufferlen;
    static int sndcnt = 0;
    if ((buf == NULL) || (len > AT_MAX_PAYLOADLEN)) {
        AT_LOG("payload too long");
        return -1;
    }
    memset(tmpbuffer, 0, AT_DATA_LEN);
    memset(wbuf, 0, AT_DATA_LEN);
    StrToHex(buf, len, tmpbuffer);
    memset(rbuf, 0, AT_DATA_LEN);
    snprintf(wbuf, AT_DATA_LEN, "%s%d,%s%c", cmd1, (int)len, tmpbuffer, '\r');
    AtTaskHandle *at = AtGetTaskHandle();
    ret = at->writeCmd((int8_t *)wbuf, strlen(wbuf), "OK", NULL, NULL);
    if (ret < 0) {
        return -1;
    }
    ret = at->writeCmd((int8_t *)cmd2, strlen(cmd2), "SENT=", rbuf, &recvBufferlen);
    if (ret < 0) {
        return -1;
    }
    str = strstr(rbuf, "SENT=");
    if (str == NULL) {
        return -1;
    }
    sscanf(str, "SENT=%d,%s", &curcnt, wbuf);
    if (curcnt == sndcnt) {
        return -1;
    }
    sndcnt = curcnt;
    return ret;
}

int32_t NbQueryIp(void)
{
    char *cmd = "AT+CGPADDR\r";
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "+CGPADDR:0,", NULL, NULL);
}

int32_t NbGetNetStat(void)
{
    char *cmd = "AT+CGATT?\r";
    AtTaskHandle *at = AtGetTaskHandle();
    return at->writeCmd((int8_t *)cmd, strlen(cmd), "CGATT:1", NULL, NULL);
}

static int32_t NbCmdWithSuffixes(const int8_t *cmd, int len, const char *suffix_ok, const char *suffix_err,
    char *respBuffer, uint32_t *respLen)
{
    const char *suffix[2] = {0};
    AtCmdInfo cmdInfo = {0};

    suffix[0] = suffix_ok;
    suffix[1] = suffix_err;

    cmdInfo.suffix = suffix;
    cmdInfo.suffixNumber = array_size(suffix);

    cmdInfo.respBuffer = respBuffer;
    cmdInfo.respLen = respLen;
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->cmdMultiSuffix(cmd, len, &cmdInfo) != AT_OK) {
        return AT_FAILED;
    }

    if (cmdInfo.matchIndex != 0) {
        AT_LOG("cmdInfo.matchIndex %d", cmdInfo.matchIndex);
        return AT_FAILED;
    }

    return AT_OK;
}

int32_t NbCreateSock(int port, int proto)
{
    int socket;
    int recvBufferlen = AT_DATA_LEN;
    const char *cmdudp = "AT+NSOCR=DGRAM,17,"; // udp
    const char *cmdtcp = "AT+NSOCR=STREAM,6,"; // tcp
    int ret;
    char buf[64];
    int cmdLen;

    if (proto != 17 && proto != 6) {
        AT_LOG("proto invalid!");
        return -1;
    }
    memset(rbuf, 0, AT_DATA_LEN);

    if (proto == 17) {
        cmdLen = snprintf(buf, sizeof(buf), "%s%d,1\r", cmdudp, port); // udp
    } else {
        cmdLen = snprintf(buf, sizeof(buf), "%s%d,1\r", cmdtcp, port);
    }

    NbCmdWithSuffixes((int8_t *)buf, cmdLen, "OK", "ERROR", rbuf, (uint32_t *)&recvBufferlen);
    ret = sscanf(rbuf, "%d\r%s", &socket, tmpbuffer);
    if ((2 == ret) && (socket >= 0) && (strnstr(tmpbuffer, "OK", sizeof(tmpbuffer)))) {
        return socket;
    }

    AT_LOG("sscanf fail,ret=%d,socket=%d", ret, socket);
    return -1;
}

static bool NbAddrIsValid(const char *addr)
{
    const int size = 4;
    int tmp[4];
    int ret;

    ret = sscanf(addr, "%d.%d.%d.%d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
    return (size == ret);
}

int NbDecomposeStr(const char *str, int *readLeft, int *out_sockid)
{
    const char *tmp, *trans;
    int sockid;
    QueueBuffer qbuf;
    // int ret = AT_FAILED;
    int rlen;
    int linkId;

    tmp = strstr(str, ",");
    if (tmp == NULL) {
        return AT_FAILED;
    }

    sockid = CharToInt(str);
    trans = strstr(tmp + 1, ",");
    if (trans == NULL) {
        return AT_FAILED;
    }
    strncpy(qbuf.ipaddr, tmp + 1, MIN((trans - tmp), AT_DATA_LEN / 2));
    qbuf.ipaddr[trans - tmp - 1] = '\0';
    if (!NbAddrIsValid(qbuf.ipaddr)) {
        return AT_FAILED;
    }

    qbuf.port = CharToInt((char *)(trans + 1));
    tmp = strstr(trans + 1, ",");
    if (tmp == NULL) {
        return AT_FAILED;
    }
    rlen = CharToInt((char *)(tmp + 1));
    if (rlen >= AT_DATA_LEN / 2 || rlen < 0) {
        AT_LOG("rlen %d", rlen);
        return AT_FAILED;
    }

    trans = strstr(tmp + 1, ",");
    if (trans == NULL) {
        return AT_FAILED;
    }

    tmp = strstr(trans + 1, ",");
    if (tmp == NULL) {
        return AT_FAILED;
    }

    *readLeft = CharToInt((char *)(tmp + 1));

    *out_sockid = sockid;

    linkId = NbSockToIndex(sockid);
    if (linkId >= MAX_SOCK_NUM) {
        AT_LOG("sockid invalid %d", sockid);
        return AT_OK;
    }

    // memcpy buffer
    AtTaskHandle *at = AtGetTaskHandle();
    if (at->linkedId[linkId].payload.buff != NULL) {
        AtFree(at->linkedId[linkId].payload.buff);
    }
    at->linkedId[linkId].payload.buff = AtMalloc(rlen);
    if (at->linkedId[linkId].payload.buff == NULL) {
        AT_LOG("malloc for recv payload failed!");
        goto END;
    }

    HexStrToStr((const uint8_t *)(trans + 1), at->linkedId[linkId].payload.buff, (rlen * 2));

    at->linkedId[linkId].payload.totalSize = rlen;
    at->linkedId[linkId].payload.offset = 0;
    at->linkedId[linkId].payload.lastEnd = 0;
    (void)at->sem.post(at->linkedId[linkId].payload.recvSem);
END:
    return AT_OK;
}

static void nb_close_sock(int sock)
{
    char buf[64];
    int cmdLen = snprintf(buf, sizeof(buf), "%s%d\r", "AT+NSOCL=", sock);
    NbCmdWithSuffixes((int8_t *)buf, cmdLen, "OK", "ERROR", NULL, NULL);
}


static int NbCreateSockLink(int portNum, int *linkId)
{
    int ret = 0;
    int sock;
    int id;
    sock = NbCreateSock(portNum, UDP_PROTO);
    if (sock < 0) {
        AT_LOG("sock num exceeded,ret is %d", sock);
        return AT_FAILED;
    }

    id = NbAllocSock(sock);
    if (id >= MAX_SOCK_NUM) {
        AT_LOG("sock num exceeded,socket is %d", sock);
        goto CLOSE_SOCk;
    }

    // create recv payload sem;
    AtTaskHandle *at = AtGetTaskHandle();
    ret = at->sem.create(0, (UINT32 *)&at->linkedId[id].payload.recvSem);
    if (ret != LOS_OK) {
        AT_LOG("esp8266 init at_recv_sem failed!");
        return AT_FAILED;
    }

    *linkId = id;
    sockInfo[id].socket = sock;
    sockInfo[id].usedFlag = true;
    return AT_OK;

CLOSE_SOCk:
    nb_close_sock(sock);

    return AT_FAILED;
}

int32_t NbBind(const int8_t *host, const int8_t *port, int32_t proto)
{
    int id = 0;
    int portNum;

    (void)host;
    (void)proto;
    portNum = CharToInt((char *)port);

    if (NbCreateSockLink(portNum, &id) != AT_OK) {
        return AT_FAILED;
    }

    sockInfo[id].localPort = *(unsigned short *)portNum;

    return AT_OK;
}

int32_t NbConnect(const int8_t *host, const int8_t *port, int32_t proto)
{
    int id = 0;
    static uint16_t localPort = NB_STAT_LOCALPORT;
    const int COAP_SEVER_PORT = 5683;

    if (NbCreateSockLink(localPort, &id) != AT_OK) {
        return AT_FAILED;
    }

    localPort++;
    if (localPort == COAP_SEVER_PORT || localPort == (COAP_SEVER_PORT + 1)) {
        localPort = 5685;
    }

    strncpy(sockInfo[id].remoteIp, (const char *)host, sizeof(sockInfo[id].remoteIp));
    sockInfo[id].remoteIp[sizeof(sockInfo[id].remoteIp) - 1] = '\0';
    sockInfo[id].remotePort = CharToInt((char *)port);

    AT_LOG("id:%d remoteIp:%s port:%d", id, sockInfo[id].remoteIp, sockInfo[id].remotePort);

    return id;
}

int32_t NbSendto(int32_t id, const uint8_t *buf, uint32_t len, char *ipaddr, int port)
{
    char *cmd = "AT+NSOST=";
    int dataLen = len / 2;
    int cmdLen;

    if ((buf == NULL) || (dataLen > AT_MAX_PAYLOADLEN) || (id >= MAX_SOCK_NUM)) {
        AT_LOG("invalid args");
        return -1;
    }

    AT_LOG("id:%d remoteIp:%s port:%d", (int)sockInfo[id].socket, ipaddr, port);
    memset(wbuf, 0, AT_DATA_LEN);
    memset(tmpbuffer, 0, AT_DATA_LEN);
    StrToHex((const char *)buf, len, tmpbuffer);

    cmdLen = snprintf(wbuf, AT_DATA_LEN, "%s%d,%s,%d,%d,%s\r", cmd, (int)sockInfo[id].socket, ipaddr, port, (int)len,
        tmpbuffer);


    if (NbCmdWithSuffixes((int8_t *)wbuf, cmdLen, "OK", "ERROR", NULL, NULL) != AT_OK) {
        return AT_FAILED;
    }

    return len;
}


int32_t NbSend(int32_t id, const uint8_t *buf, uint32_t len)
{
    if (id >= MAX_SOCK_NUM) {
        AT_LOG("invalid args");
        return AT_FAILED;
    }
    return NbSendto(id, buf, len, sockInfo[id].remoteIp, (int)sockInfo[id].remotePort);
}

int32_t NbRecv(int32_t id, uint8_t *buf, uint32_t len)
{
    return NbRecvTimeout(id, buf, len, NULL, NULL, LOS_WAIT_FOREVER);
}

int32_t NbRecvFrom(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port)
{
    return NbRecvTimeout(id, buf, len, ipaddr, port, LOS_WAIT_FOREVER);
}

int32_t NbRecvTimeout(int32_t id, uint8_t *buf, uint32_t len, char *ipaddr, int *port, int32_t timeout)
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
    at->sem.post(at->linkedId[id].payload.recvSem); // 

    return readBytes;
}


int32_t NbClose(int32_t id)
{
    if ((id >= MAX_SOCK_NUM) || (!sockInfo[id].usedFlag)) {
        AT_LOG("link id %ld invalid", id);
        return AT_FAILED;
    }

    nb_close_sock(sockInfo[id].socket);
  
    (void)memset(&sockInfo[id], 0, sizeof(sockInfo[id]));

    return AT_OK;
}

int32_t NbRecvCb(int32_t id)
{
    return AT_FAILED;
}

static int32_t NbInit(void)
{
    AtConfig atDeviceConf = {
        .name = AT_MODU_NAME,
        .usartPort = AT_USART_PORT,
        .buardrate = AT_BUARDRATE,
        .maxLinkIdNum = AT_MAX_LINK_NUM,
        .maxBufferLen = MAX_AT_USERDATA_LEN,
        .cmdBeginStr = AT_CMD_BEGIN,
        .lineEndStr = AT_LINE_END,
        .multiMode = 1,             // support multi connection mode
        .timeout = AT_CMD_TIMEOUT, // ms
    };
    AtTaskHandle *at = AtGetTaskHandle();
    at->config.set(&atDeviceConf);
    memset(&sockInfo, 0, sizeof(sockInfo));
    memset(&g_nbDataInfo, 0, sizeof(g_nbDataInfo));
    AtRegStepCallback(at, NbStep);

    return AT_OK;
}

int32_t NbDeinit(void)
{
    for (int i = 0; i < MAX_SOCK_NUM; ++i) {
        if (sockInfo[i].usedFlag) {
            NbClose(i);
        }
    }
    return NbReboot();
}

static int32_t BC95GetLocalMaxFd(void) {
    return AT_MAX_LINK_NUM;
}

void NbReattach(void)
{
    (void)NbCmdWithSuffixes((int8_t *)CGATT, strlen(CGATT), "OK", "ERROR", NULL, NULL);
    (void)NbCmdWithSuffixes((int8_t *)CGATT_DEATTACH, strlen(CGATT_DEATTACH), "OK", "ERROR", NULL, NULL);
    LOS_TaskDelay(1000);
    (void)NbCmdWithSuffixes((int8_t *)CGATT_ATTACH, strlen(CGATT_ATTACH), "OK", "ERROR", NULL, NULL);
}

static int NbCmdRecvData(int sockid, int readLeft);

static int32_t NbHandleSockData(const int8_t *data, uint32_t len)
{
    (void)len;
    char *curr = (char *)data;

    if (strstr((char *)data, "ERROR") != NULL) {
        return AT_OK;
    }

    do {
        int readLeft;
        int sockid;

        char *next = strstr(curr, "\r\n");

        if (next == curr) {
            curr += 2;
        }

        if (next != NULL) {
            next += 2;
        }

        if (NbDecomposeStr(curr, &readLeft, &sockid) == AT_OK) {
            return AT_OK;
        }
        curr = next;
    } while (curr);

    return AT_FAILED;
}

static int NbCmdRecvData(int sockid, int readLeft)
{
    int cmdlen;
    char cmdbuf[40];
    const char *cmd = "AT+NSORF=";
    const uint32_t timeout = 10;

    cmdlen = snprintf(cmdbuf, sizeof(cmdbuf), "%s%d,%d\r", cmd, sockid, readLeft);
    return AtCmdInCallback((int8_t *)cmdbuf, cmdlen, NbHandleSockData, timeout);
}

static int32_t NbHandleData(const char *buf)
{
    int32_t sockid;
    int32_t dataLen;
    const char *p1, *p2;
    int linkIndex;

    p2 = strstr(buf, AT_DATAF_PREFIX);
    if (p2 == NULL) {
        return AT_FAILED;
    }
    p2 += strlen(AT_DATAF_PREFIX);

    p1 = strstr(p2, ",");
    if (p1 == NULL) {
        return AT_FAILED;
    }
    sockid = CharToInt(p2);
    dataLen = CharToInt(p1 + 1);
    linkIndex = NbSockToIndex(sockid);
    if (linkIndex >= MAX_SOCK_NUM) {
        AT_LOG("invalid sock id %ld", sockid);
        return AT_OK;
    }

    if (NbCmdRecvData(sockid, dataLen) != AT_OK) {
        g_nbDataInfo.dataLen = (uint32_t)dataLen;
        g_nbDataInfo.linkIndex = linkIndex;
        g_nbDataInfo.validFlag = true;
    } else {
        g_nbDataInfo.validFlag = false;
    }

    return AT_OK;
}

int32_t NbCmdMatch(const char *buf, char *featureStr, int len)
{
    if (buf == NULL) {
        return AT_FAILED;
    }

    NbHandleData(buf);

    return AT_FAILED;
}

void NbStep(void)
{
    if ((!g_nbDataInfo.validFlag) || (!sockInfo[g_nbDataInfo.linkIndex].usedFlag)) {
        return;
    }

    if (NbCmdRecvData(sockInfo[g_nbDataInfo.linkIndex].socket, g_nbDataInfo.dataLen) == AT_OK) {
        g_nbDataInfo.validFlag = false;
    }
}

AtAdaptorApi AtGetBC95Interface(void)
{
    AtAdaptorApi bc95Interface = {
        .init = NbInit,
        .bind = NbBind,
        .connect = NbConnect,
        .getLocalMaxFd = BC95GetLocalMaxFd,
        .send = NbSend,
        .sendto = NbSendto,
        .recvTimeout = NbRecvTimeout,
        .recv = NbRecv,
        .recvFrom = NbRecvFrom,
        .close = NbClose,
        .recvCallback = NbRecvCb,
        .deInit = NbDeinit,
    };
    return bc95Interface;
}