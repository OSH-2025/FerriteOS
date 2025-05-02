/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Mqtt Event Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-20
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

#include "mqtt_events.h"
#include "los_base.h"
#include "los_task_pri.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "mqtt_client.h"
#include "osdepends/atiny_osdep.h"
#include "cJSON.h"
#include "atiny_log.h"
#include "mqtt_config.h"

#if defined(LOSCFG_COMPONENTS_OTA)
#include "upgrade.h"
#endif

#define PROPERTIES_REPORT_TOPIC "$oc/devices/%s/sys/properties/report"
#define EVENTS_UP_TOPIC "$oc/devices/%s/sys/events/up"

#define PAYLOAD_MAX_LEN 1024

int EventsDownPayloadParse(char *payload, int payloadLen)
{
    cJSON *obj = cJSON_Parse((const char *)payload);
    if (obj == NULL ) {
        return ATINY_ERR;
    }
    cJSON *deviceId = cJSON_GetObjectItem(obj, "object_device_id");
    if (deviceId == NULL) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    ATINY_LOG(LOG_DEBUG, "deviceId=%s\n", deviceId->valuestring);

    cJSON *array = cJSON_GetObjectItem(obj, "services");
    if (array == NULL ) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    cJSON *obj1 = cJSON_GetArrayItem(array, 0);
    if (obj1 == NULL ) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    cJSON *eventType = cJSON_GetObjectItem(obj1, "event_type");
    if (eventType == NULL) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    ATINY_LOG(LOG_DEBUG, "event_type=%s\n", eventType->valuestring);
    
    cJSON *serviceId = cJSON_GetObjectItem(obj1, "service_id");
    if (serviceId == NULL) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    ATINY_LOG(LOG_DEBUG, "serviceId=%s\n", serviceId->valuestring);

    cJSON *eventTime = cJSON_GetObjectItem(obj1, "event_time");
    if (eventTime == NULL) {
        cJSON_Delete(obj);
        return ATINY_ERR;
    }
    ATINY_LOG(LOG_DEBUG, "eventTime=%s\n", eventTime->valuestring);

#if defined (LOSCFG_COMPONENTS_OTA)
    if (strncmp(eventType->valuestring, "firmware_upgrade", strlen("firmware_upgrade")) == 0) {
        UpgradeHandle upgrade;
        memset_s(&upgrade, sizeof(UpgradeHandle), 0, sizeof(UpgradeHandle));
        cJSON *obj2 = cJSON_GetObjectItem(obj1, "paras");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        cJSON *item = cJSON_GetObjectItem(obj2, "version");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "version=%s\n", item->valuestring);
        strcpy_s(upgrade.version, sizeof(upgrade.version), item->valuestring);
        
        item = cJSON_GetObjectItem(obj2, "url");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "url=%s\n", item->valuestring);
        strcpy_s(upgrade.url, sizeof(upgrade.url), item->valuestring);

        item = cJSON_GetObjectItem(obj2, "file_size");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        int fileSize = cJSON_GetNumberValue(item);
        ATINY_LOG(LOG_DEBUG, "file_size=%d\n", fileSize);
        upgrade.fileSize = fileSize;

        item = cJSON_GetObjectItem(obj2, "access_token");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "access_token=%s\n", item->valuestring);
        strcpy_s(upgrade.token, sizeof(upgrade.token), item->valuestring);

        item = cJSON_GetObjectItem(obj2, "sign");
        if (obj2 == NULL) {
            cJSON_Delete(obj);
            return ATINY_ERR;
        }
        ATINY_LOG(LOG_DEBUG, "sign=%s\n", item->valuestring);
        strcpy_s(upgrade.sign, sizeof(upgrade.sign), item->valuestring);
        upgrade.callback = UploadUpgradeStatus;
        cJSON_Delete(obj);
        (void)UpgradeMainTask(&upgrade, sizeof(UpgradeHandle));
        return ATINY_OK;
    }
#endif /* LOSCFG_COMPONENTS_OTA */
    if (strncmp(eventType->valuestring, "version_query", strlen("version_query")) == 0) {
        // set version response
        UpoladDeviceVerison(deviceId->valuestring, eventTime->valuestring);
        cJSON_Delete(obj);
        return ATINY_OK;
    }
    return ATINY_OK;
}

// Report software and firmware version information of device
int UpoladDeviceVerison(char *deviceId, char *eventTime)
{
    char *swVersion = GetSoftwareVersion();
    char *fwVersion = GetHardwareVersion();

    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(obj, "object_device_id", cJSON_CreateString(deviceId));
    
    cJSON *obj1 = cJSON_CreateObject();
    cJSON_AddItemToObject(obj1, "service_id", cJSON_CreateString("$ota"));
    cJSON_AddItemToObject(obj1, "event_type", cJSON_CreateString("version_report"));
    cJSON_AddItemToObject(obj1, "event_time", cJSON_CreateString(eventTime));

    cJSON *obj2 = cJSON_CreateObject();
    cJSON_AddItemToObject(obj2, "sw_version", cJSON_CreateString(swVersion));
    cJSON_AddItemToObject(obj2, "fw_version", cJSON_CreateString(fwVersion));
    cJSON_AddItemToObject(obj1, "paras", obj2);
    
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, obj1);
    cJSON_AddItemToObject(obj, "services", array);
    
    char *sendData = cJSON_Print(obj);
    cJSON_Delete(obj);
    char *topic = mqtt_get_topic(EVENTS_UP_TOPIC, sizeof(EVENTS_UP_TOPIC) - 2); // topic lenght: sizeof(EVENTS_UP_TOPIC) - 2
    if (topic == NULL) {
        free(sendData);
        return false;
    }

    int ret = atiny_mqtt_data_send(topic, sendData, strnlen(sendData, PAYLOAD_MAX_LEN), MQTT_QOS_LEAST_ONCE);
    ATINY_LOG(LOG_INFO, "send rsp ret %d, rsp: %s", ret, sendData);
    free(topic);
    free(sendData);
    return ret;
}

int UploadUpgradeStatus(char *version, int resultCode, int progress)
{
    int ret;
    char *deviceId = NULL; 
    uint16_t deviceIdLen;
    char *eventTime = GetEventTime();
    MQTT_GetConnectDeviceId(&deviceId, &deviceIdLen);
    cJSON *base = cJSON_CreateObject();
    cJSON_AddItemToObject(base, "object_device_id", cJSON_CreateString(deviceId));

    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddItemToObject(item1, "service_id", cJSON_CreateString("$ota"));
    cJSON_AddItemToObject(item1, "event_type", cJSON_CreateString("upgrade_progress_report"));
    cJSON_AddItemToObject(item1, "event_time", cJSON_CreateString(eventTime));

    cJSON *item2 = cJSON_CreateObject();
    cJSON_AddItemToObject(item2, "result_code", cJSON_CreateNumber(resultCode));
    cJSON_AddItemToObject(item2, "version", cJSON_CreateString(version));
    cJSON_AddItemToObject(item2, "progress", cJSON_CreateNumber(progress));
    cJSON_AddItemToObject(item1, "paras", item2);
    
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToObject(base, "services", array);
    char *sendData = cJSON_Print(base);
    ATINY_LOG(LOG_DEBUG, "sendData=%s\n", sendData);
    cJSON_Delete(base);

    char *topic = mqtt_get_topic(EVENTS_UP_TOPIC, sizeof(EVENTS_UP_TOPIC) - 2); // topic len: sizeof(EVENTS_UP_TOPIC) - 2
    if (topic == NULL) {
        ATINY_LOG(LOG_DEBUG, "topic is null.");
        free(sendData);
        return false;
    }
    ret = atiny_mqtt_data_send(topic, sendData, strnlen(sendData, PAYLOAD_MAX_LEN), MQTT_QOS_LEAST_ONCE);
    free(topic);
    free(sendData);
    return ret;
}

int PropertiesReport(char *deviceId, char *eventTime)
{
    int ret;
    static int level = 0;
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return LOS_NOK;
    }
    cJSON_AddItemToObject(obj, "object_device_id", cJSON_CreateString(deviceId));

    cJSON *obj1 = cJSON_CreateObject();
    if (obj1 == NULL) {
        cJSON_Delete(obj);
        return LOS_NOK;
    }
    cJSON_AddItemToObject(obj1, "service_id", cJSON_CreateString("Battery"));
    cJSON_AddItemToObject(obj1, "event_time", cJSON_CreateString(eventTime));
    
    cJSON *obj2 = cJSON_CreateObject();
    if (obj2 == NULL) {
        cJSON_Delete(obj);
        cJSON_Delete(obj1);
        return LOS_NOK;
    }
    cJSON_AddItemToObject(obj2, "batteryLevel", cJSON_CreateNumber(level++));
    cJSON_AddItemToObject(obj2, "event_time", cJSON_CreateString(eventTime));
    cJSON_AddItemToObject(obj1, "properties", obj2);

    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        cJSON_Delete(obj);
        return LOS_NOK;
    }
    cJSON_AddItemToArray(array, obj1);
    cJSON_AddItemToObject(obj, "services", array);
    
    char *sendData = cJSON_Print(obj);
    
    cJSON_Delete(obj);
    char *topic = mqtt_get_topic(PROPERTIES_REPORT_TOPIC, sizeof(PROPERTIES_REPORT_TOPIC) - 2); // topic len: sizeof(EVENTS_UP_TOPIC) - 2
    if (topic == NULL) {
        free(sendData);
        return false;
    }
    ret = atiny_mqtt_data_send(topic, sendData, strnlen(sendData, PAYLOAD_MAX_LEN), MQTT_QOS_LEAST_ONCE);
    free(topic);
    free(sendData);
    return ret;
}

char *GetEventTime(void)
{
    return "20210908T081630Z";
}

char *GetSoftwareVersion(void)
{
    return "V1.2";
}

char *GetHardwareVersion(void)
{
    return "V1.2";
}
