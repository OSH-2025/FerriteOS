/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Mqtt Config Implementation
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

#include "mqtt_config.h"
#include <string.h>
#include "agent_tiny_mqtt_demo.h"
#include "mqtt_client.h"

#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS
#ifdef LOSCFG_DTLS_PSK_ENCRYPT
static char *g_serverPort = "8883"; // mqtts port
#define AGENT_TINY_DEMO_PSK_LEN (3) // maximum length of psk
static char *g_pskId = "device psk id"; // set device psk id
uint8_t g_psk[AGENT_TINY_DEMO_PSK_LEN] = {0xab, 0xcd, 0xef}; // set device psk, Hexadecimal data

#elif defined(LOSCFG_DTLS_CERT_ENCRYPT)

static char *g_serverPort = "8883"; // mqtts port
static char g_deviceCert[] = "cert"; // set mqtts cert, For connection authentication
#else /* MQTT_DEMO_USE_PSK */
static char *g_serverPort = "1883"; // mqtt port
#endif /* NO_USE_CERT_OR_PSK */
#else 
static char *g_serverPort = "1883"; // mqtt port
#endif /* LOSCFG_COMPONENTS_SECURITY_MBEDTLS */

static char *g_serverIp = "121.36.42.100";

static char *g_secret = "device secret"; // set device secret
static char *g_productId = "device product id"; //set device product id
static char *g_nodeId = "device node id"; // set device node id
static char *g_deviceId = "device id"; // set device id, It consists of productId and nodeId
static char *g_time = "2021090703"; // simulated time stamp, Time verification for connecting to the server

uint8_t MQTT_GetConnectType(void)
{
    return MQTT_STATIC_CONNECT;
}

void MQTT_GetServerIp(char **serverIp, uint8_t *serverIpLen)
{
   *serverIp = g_serverIp;
   *serverIpLen = strlen(g_serverIp);
}

void MQTT_GetServerPort(char **serverPort, uint8_t *serverPortLen)
{
   *serverPort = g_serverPort;
   *serverPortLen = strlen(g_serverPort);
}

void MQTT_GetConnectDeviceId(char **deviceId, uint16_t *deviceIdLen)
{
    *deviceId = g_deviceId;
    *deviceIdLen = strlen(g_deviceId);
}

void MQTT_GetConnectProductId(char **productId, uint16_t *productIdLen)
{
    *productId = g_productId;
    *productIdLen = strlen(g_productId);
}

void MQTT_GetConnectNodeId(char **nodeId, uint16_t *nodeIdLen)
{
    *nodeId = g_nodeId;
    *nodeIdLen = strlen(g_nodeId);
}

void MQTT_GetConnectPwd(char **pwd, uint16_t *pwdLen)
{
    *pwd = g_secret;
    *pwdLen = strlen(g_secret);
}

void MQTT_GetCurrentRealTime(char **time, uint16_t *timeLen)
{
    *time = g_time;
    *timeLen = strlen(g_time);
}

#if defined (LOSCFG_DTLS_CERT_ENCRYPT)
void MQTT_GetConnectSecureCaCrt(char **ca, uint32_t *caLen)
{
    *ca = g_deviceCert;
    *caLen = sizeof(g_deviceCert); // ca length: sizeof(g_mqttCaCrt)
}
#endif /* LOSCFG_DTLS_CERT_ENCRYPT */

#if defined (LOSCFG_DTLS_PSK_ENCRYPT)
void MQTT_GeConnectSecurePsk(uint8_t **pskId, uint32_t *pskIdLen, uint8_t **psk, uint32_t *pskLen)
{
    *pskId = g_pskId;
    *pskIdLen = strlen(pskId);
    *psk = g_psk;
    pskLen = strlen(psk); 
}
#endif /* LOSCFG_DTLS_PSK_ENCRYPT */
