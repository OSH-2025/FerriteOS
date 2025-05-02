/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Mqtt Config Headile
 * Author: Huawei LiteOS Team
 * Create: 2021-09-10
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
#ifndef _MQTT_CONFIG_H
#define _MQTT_CONFIG_H
#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

uint8_t MQTT_GetConnectType(void);
void MQTT_GetCurrentRealTime(char **time, uint16_t *timeLen);
uint8_t MQTT_GetConnectType(void);
void MQTT_GetServerIp(char **serverIp, uint8_t *serverIpLen);
void MQTT_GetServerPort(char **serverPort, uint8_t *serverPortLen);
void MQTT_GetConnectDeviceId(char **deviceId, uint16_t *deviceIdLen);
void MQTT_GetConnectProductId(char **productId, uint16_t *productIdLen);
void MQTT_GetConnectNodeId(char **nodeId, uint16_t *nodeIdLen);
void MQTT_GetConnectPwd(char **pwd, uint16_t *pwdLen);

#ifdef LOSCFG_DTLS_PSK_ENCRYPT
void MQTT_GeConnectSecurePsk(uint8_t **pskId, uint32_t *pskIdLen, uint8_t **psk, uint32_t *pskLen);
#endif /* LOSCFG_DTLS_PSK_ENCRYPT */

#ifdef LOSCFG_DTLS_CERT_ENCRYPT
void MQTT_GetConnectSecureCaCrt(char **ca, uint32_t *caLen);
#endif /* LOSCFG_DTLS_CERT_ENCRYPT */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MQTT_CONFIG_H */
