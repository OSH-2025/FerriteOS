/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: NB IOT API
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

#include "los_nb_api.h"
#include "at_api.h"
#ifdef LOSCFG_COMPONENTS_NET_AT_BC95
#include "bc95.h"
#endif

int los_nb_init(const int8_t *host, const int8_t *port, SecureParam *psk)
{
    int ret;
    int timeCount = 0;
    AtTaskHandle *at = AtGetTaskHandle();
    /* when used nb with agenttiny */
    /* the following para is replaced by call nb_int() */
    AtConfig atDeviceConfig = {
        .name = AT_MODU_NAME,
        .usartPort = AT_USART_PORT,
        .buardrate = AT_BUARDRATE,
        .maxLinkIdNum = AT_MAX_LINK_NUM,
        .maxBufferLen = MAX_AT_USERDATA_LEN,
        .cmdBeginStr = AT_CMD_BEGIN,
        .lineEndStr = AT_LINE_END,
        .multiMode = 1, // support multi connection mode
        .timeout = AT_CMD_TIMEOUT, // ms
    };

    at->init(&atDeviceConfig);

    NbReboot();
    if (psk != NULL) {
        if (psk->setPskFlag) {
            NbSendPsk(psk->pskId, psk->psk);
        } else {
            NbSetNoEncrypt();
        }
    }

    while (1) {
        ret = NbHwDetect();
        printf("call NbHwDetect, ret is %d\n", ret);
        if (ret == AT_OK) {
            break;
        }
    }
    while (timeCount < 120) {
        ret = NbGetNetStat();
        NbCheckCsq();
        if (ret != AT_FAILED) {
            ret = NbQueryIp();
            break;
        }
        timeCount++;
    }
    if (ret != AT_FAILED) {
        NbQueryIp();
    }
    ret = NbSetCdpServer((char *)host, (char *)port);
    return ret;
}

int los_nb_report(const char *buf, int len)
{
    if ((buf == NULL) || (len <= 0)) {
        return -1;
    }
    return NbSendPayload(buf, len);
}

int los_nb_notify(char *featureStr, int cmdlen, OobCallback callback, OobCmdMatch cmdMatch)
{
    if ((featureStr == NULL) || (cmdlen <= 0) || (cmdlen >= OOB_CMD_LEN - 1)) {
        return -1;
    }
    AtTaskHandle *at = AtGetTaskHandle();
    return at->oobRegister(featureStr, cmdlen, callback, cmdMatch);
}

int los_nb_deinit(void)
{
    AtTaskHandle *at = AtGetTaskHandle();
    NbReboot();
    at->deInit();
    return 0;
}
