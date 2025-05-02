/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Agent Tiny Mqtt Demo
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

#include "agent_tiny_mqtt_demo.h"
#include "los_base.h"
#include "los_task_pri.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "mqtt_client.h"
#include "osdepends/atiny_osdep.h"
#include "cJSON.h"
#include "MQTTClient.h"
#include "flash_adaptor.h"
#include "mqtt_config.h"
#include "mqtt_events.h"
#include "atiny_log.h"
#if defined (LOSCFG_COMPONENTS_OTA)
#include "ota_port.h"
#include "ota_api.h"
#include "upgrade.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static demoFlashHandle g_flashHandle;

static void MqttDemoDataReport(void)
{
    int ret;
    char *deviceId = NULL;
    uint16_t deviceIdLen = 0;
    MQTT_GetConnectDeviceId(&deviceId, &deviceIdLen);
    while (1) {
        char *demoEventTime = "20210908T081630Z";
        ret = PropertiesReport(deviceId, demoEventTime);
        if (ret == ATINY_OK) {
            ATINY_LOG(LOG_DEBUG, "report data success.\n");
        }
        (void)LOS_TaskDelay(8 * 1000); //continue mqtt publish after 8s delay.
    }
}

static INT32 MqttDemoCreateReportTask(void)
{
    uint32_t ret;
    TSK_INIT_PARAM_S taskInitParam;
    uint32_t taskHandle;

    taskInitParam.usTaskPrio = 5;
    taskInitParam.pcName = "MqttDemoDataReport";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)MqttDemoDataReport;
    taskInitParam.uwStackSize = 0x1000;

    ret = LOS_TaskCreate(&taskHandle, &taskInitParam);
    return ret;
}

static int MqttDemoGetTime(char *time, int32_t len)
{
    #define MAX_TIME_DST 11
    char *demoTime = NULL;
    uint16_t timeLen;
    const int32_t min_time_len = 11;
    if ((time == NULL) || (len < min_time_len)) {
        ATINY_LOG(LOG_ERR, "invalid param len %d", len);
        return ATINY_ERR;
    }
    MQTT_GetCurrentRealTime(&demoTime, &timeLen);
    (void)strcpy_s(time, MAX_TIME_DST, demoTime); // 11 : time buffer size

    return ATINY_OK;
}

static int HandleRecvMsg(char *topic, int32_t topicLen, char *payload, int32_t payloadLen)
{
    // parse topic.
    char *p = strstr(topic, "/");
    if (p == NULL) {
        return ATINY_ERR;
    }
    
    char *properties = strstr(topic, "properties");
    if (properties) {
        char *propertiesType = strstr(properties, "/");
        if (propertiesType == NULL) {
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "propertiesType=%s\n", propertiesType++);
        return ATINY_OK;
    }

    char *commands = strstr(topic, "commands");
    if (commands) {
        char *commandsType = strstr(commands, "/");
        if (commandsType == NULL) {
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "commandsType=%s\n", commandsType++);
        return ATINY_OK;
    }

    char *messages = strstr(topic, "messages");
    if (messages) {
        char *msgType = strstr(messages, "/");
        if (msgType == NULL) {
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "msgType=%s\n", msgType++);
        return ATINY_OK;
    }

    char *events = strstr(topic, "events");
    if (events) {
        char *eventType = strstr(events, "/");
        if (eventType == NULL) {
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "eventType=%s\n", eventType++);
        EventsDownPayloadParse(payload, payloadLen);
        return ATINY_OK;
    }

    return ATINY_OK;
}

static int MqttDemoRecvMsg(void *msg, int32_t len)
{
    int ret = ATINY_ERR;
    MessageData *data = (MessageData *)msg;
    if (data == NULL) {
        ATINY_LOG(LOG_ERR, "msg is null.");
        return ATINY_ERR;
    }
    // get topic.
    char *topic = data->topicName->lenstring.data;
    if(topic == NULL) {
        return ATINY_ERR;
    }
    int32_t topicLen = data->topicName->lenstring.len;
    ATINY_LOG(LOG_INFO, "recv topic %s", topic);

    // get payload.
    char *payload = data->message->payload;
    if(payload == NULL) {
        return ATINY_ERR;
    }
    int32_t payloadLen = data->message->payloadlen;
    ATINY_LOG(LOG_INFO, "recv payload %s", payload);

    ret = HandleRecvMsg(topic, topicLen, payload, payloadLen);

    return ret;
}

static int MqttDemoCmdIoctl(mqtt_cmd_e cmd, void *arg, int32_t len)
{
    int result = ATINY_ERR;
    switch (cmd) {
        case MQTT_GET_TIME:
            result = MqttDemoGetTime(arg, len);
            break;
        case MQTT_RCV_MSG:
            result = MqttDemoRecvMsg(arg, len);
            break;
        case MQTT_SAVE_SECRET_INFO:
            if (g_flashHandle.write_flash_info == NULL) {
                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "write_flash_info null");
            } else {
                result = g_flashHandle.write_flash_info(arg, len);
            }
            break;
        case MQTT_READ_SECRET_INFO:
            if (g_flashHandle.read_flash_info == NULL) {
                result = ATINY_ERR;
                ATINY_LOG(LOG_ERR, "read_flash_info null");
            } else {
                result = g_flashHandle.read_flash_info(arg, len);
            }
            break;
        default:
            break;
        }
    return result;
}

void AgentTinyMqttDemoEntry(void)
{
    UINT32 ret;
    mqtt_param_s atinyParams;
    mqtt_device_info_s deviceInfo;
    ret = memset_s(&atinyParams, sizeof(mqtt_param_s), 0, sizeof(mqtt_param_s));
    if (ret != EOK) {
        return;
    }
    ret = memset_s(&deviceInfo, sizeof(mqtt_device_info_s), 0, sizeof(mqtt_device_info_s));
    if (ret != EOK) {
        return;
    }
    cJSON_InitHooks(NULL); // cjson init
    g_flashHandle.init = FlashAdaptorInit; // flash init
    g_flashHandle.write_flash_info = FlashAdaptorWriteMqttInfo;
    g_flashHandle.read_flash_info = FlashAdaptorReadMqttInfo;
    if (g_flashHandle.init != NULL) {
        g_flashHandle.init();
    }
#if defined(LOSCFG_COMPONENTS_OTA)
    UpgradeInit();
#endif
    MQTT_GetServerIp(&atinyParams.server_ip, &atinyParams.ip_len);
    MQTT_GetServerPort(&atinyParams.server_port, &atinyParams.port_len);
#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS
    #ifdef LOSCFG_DTLS_PSK_ENCRYPT
        atinyParams.info.security_type = MQTT_SECURITY_TYPE_PSK;
        MQTT_GeConnectSecurePsk(&atinyParams.info.u.psk.psk_id, &atinyParams.info.u.psk.psk_id_len,
                                &atinyParams.info.u.psk.psk, &atinyParams.info.u.psk.psk_len);
    #elif defined(LOSCFG_DTLS_CERT_ENCRYPT) /* LOSCFG_DTLS_PSK_ENCRYPT */
        atinyParams.info.security_type = MQTT_SECURITY_TYPE_CA;
        MQTT_GetConnectSecureCaCrt(&atinyParams.info.u.ca.ca_crt, &atinyParams.info.u.ca.ca_len);
    #else /* LOSCFG_DTLS_CERT_ENCRYPT */
        atinyParams.info.security_type = MQTT_SECURITY_TYPE_NONE;
    #endif
#else
    atinyParams.info.security_type = MQTT_SECURITY_TYPE_NONE;
#endif /* LOSCFG_COMPONENTS_SECURITY_MBEDTLS */
    atinyParams.cmd_ioctl = MqttDemoCmdIoctl;
    if (atiny_mqtt_init(&atinyParams) != ATINY_OK) {
        return;
    }
    ret = MqttDemoCreateReportTask();
    if (ret != LOS_OK) {
        return;
    }
    deviceInfo.codec_mode = MQTT_CODEC_MODE_JSON;
    deviceInfo.sign_type = MQTT_SIGN_TYPE_HMACSHA256_NO_CHECK_TIME;
    MQTT_GetConnectPwd(&deviceInfo.secret, &deviceInfo.secret_len);
    if (MQTT_GetConnectType() == MQTT_STATIC_CONNECT) {
        deviceInfo.connection_type = MQTT_STATIC_CONNECT;
        MQTT_GetConnectDeviceId(&deviceInfo.u.s_info.deviceid, &deviceInfo.u.s_info.deviceid_len);
    }
    (void)atiny_mqtt_bind(&deviceInfo);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
