/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Upgrade Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-10-10
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

#include "upgrade.h"
#include "dtls_interface.h"
#include "los_sys.h"
#include "ota_port.h"
#include "los_task.h"
#include "los_queue.h"
#include "los_sem.h"
#include "atiny_log.h"
#include "los_swtmr.h"
#include "mbedtls/sha256.h"
#include "sota_hal.h"
#include "upgrade_flag.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PORT_BUF_LEN 16 // maximum byte length of port
#define UPGRADE_SERVER_IP "" // set firmware upgrade server address
#define UPGRADE_SERVER_PORT "8943" // set firmware upgrade server port

#define RECEIVE_TASK_SIZE 0x800
#define RECEIVE_TASK_PRIORITY 3

#define EVENT_TASK_SIZE 0x1000
#define EVENT_TASK_PRIORITY 5

#define WRITE_TASK_SIZE 0x800
#define WRITE_TASK_PRIORITY 3

#define RESPONSE_HEAD_END "\r\n\r\n"

#define QUEUE_SIZE  4

#define RETRY_TIMES 10

#define RECV_SIZE 1024

#define REQUEST_SIZE 512

#define URL_HEAD 26

static UpgradeHandle *g_upgradeHandle = NULL;
static mbedtls_ssl_context *g_upgradeContext = NULL;

static uint32_t g_sendQueueHandle;
static uint32_t g_recvQueueHandle;
static uint32_t g_writeQueueHandle;
static EVENT_CB_S g_eventGroup;
static uint32_t g_eventTaskHandle;
static uint32_t g_progressTaskHandle;
static uint32_t g_upgradeTaskHandle;
static uint32_t g_fwTaskHandle;
static uint16_t g_timerHandle;
static uint8_t g_retryCounts = 0;
static uint32_t g_writeSemHandle;
static uint32_t g_writeMutexHandle;

typedef enum {
  CONNECT_EVENT = 1,
  CLOSE_EVENT = 2,
  DOWNLOAD_ERR_EVENT = 4,
  DOWNLOAD_DONE_EVENT = 8,
} UPGRADE_EVENT;
const uint8_t g_caCert[] = ""; // it is used to verify the identity of the platform. 
static int UpgradeProgressWrite(char *version, ResultCodeType resultCode, int progress); // function declaration
static uint32_t UpgradeFwWriteTaskInit(void); // function declaration

static int UpgradeDeviceConnect(const char *host, const char *port) 
{
    int ret;
    dtls_establish_info_s establish;
    dtls_shakehand_info_s shakehand;
    establish.psk_or_cert = VERIFY_WITH_CERT;
    establish.udp_or_tcp = MBEDTLS_NET_PROTO_TCP;
    establish.v.c.ca_cert = g_caCert;
    establish.v.c.cert_len = sizeof(g_caCert);
    g_upgradeContext = dtls_ssl_new(&establish, MBEDTLS_SSL_IS_CLIENT);
    if (g_upgradeContext == NULL) {
        dtls_ssl_destroy(g_upgradeContext);
        return 1;
    }
    memset_s(&shakehand, sizeof(dtls_shakehand_info_s), 0, sizeof(dtls_shakehand_info_s));
    shakehand.client_or_server = MBEDTLS_SSL_IS_CLIENT;
    shakehand.udp_or_tcp = MBEDTLS_NET_PROTO_TCP;
    shakehand.u.c.host = host;
    shakehand.u.c.port = port;
    ret = dtls_shakehand(g_upgradeContext, &shakehand);
    if (ret != 0) {
        dtls_ssl_destroy(g_upgradeContext);
    }
    return ret;
}

static int UpgradeRequestSend(UpgradeHandle *handle)
{
    int ret;
    uint16_t requestLen = 0;
    uint8_t *request = (uint8_t *)malloc(REQUEST_SIZE); // requset data length: 512 bytes
    if (request == NULL) {
        ATINY_LOG(LOG_ERR, "request malloc failed.\n");
        return -1;
    }
    (void)memset_s(request, REQUEST_SIZE, 0, REQUEST_SIZE);
    // https get method.
    (void)snprintf_s(request, REQUEST_SIZE, REQUEST_SIZE - 1, \
        "GET %s HTTP/1.1\r\nHost: %s:%s\r\nAuthorization: Bearer %s\r\n\r\n", \
        handle->url, handle->host, handle->port, handle->token);

    requestLen = strlen(request);
    if (request <= 0) {
        ATINY_LOG(LOG_ERR, "get requeset length error[%d].", requestLen);
        free(request);
        return -1;
    }
    if (g_upgradeContext == NULL) {
        free(request);
        return -1;
    }
    ret = dtls_write(g_upgradeContext, request, strlen((char *)request));
    ATINY_LOG(LOG_DEBUG, "write size=%d\n", ret);
    free(request);

    return ret;
}

static int UpgradeFileDownload(UpgradeHandle *handle)
{
    int ret;
    uint8_t *buffer = NULL;
    FwBuffer *fwBuffer = NULL;
    int totalLen = 0; // total length of firmware
    int progress = 0;
    int lastProgress = 0;
    int err;
    uint8_t *pos = NULL;
    uint8_t *ptr = NULL;
    mbedtls_sha256_context *ctx = NULL;
    int fwSize = sizeof(FwBuffer) + RECV_SIZE;

    ctx = (mbedtls_sha256_context *)malloc(sizeof(mbedtls_sha256_context));
    if (ctx == NULL) {
        return -1;
    }
    mbedtls_sha256_init(ctx);
    mbedtls_sha256_starts(ctx, false); // sha256 check start

    if ((buffer = (uint8_t *)malloc(RECV_SIZE)) == NULL) { // set buffer size 1K bytes
        mbedtls_sha256_free(ctx);
        mbedtls_free(ctx);
        return -1;
    }
    if ((fwBuffer = (FwBuffer *)malloc(fwSize)) == NULL) { // fireware buffer
        free(buffer);
        mbedtls_sha256_free(ctx);
        mbedtls_free(ctx);
        return -1;
    }
    ret = dtls_read(g_upgradeContext, buffer, RECV_SIZE, 5000); // set timeout 5000 ms
    if ((ret > 0) && (ptr = strstr(buffer, RESPONSE_HEAD_END)) != NULL) {
        int headLen = ptr - buffer + strlen(RESPONSE_HEAD_END);
        int fwLen = ret - headLen; // get current fireware buffer len;
        pos = ptr + strlen(RESPONSE_HEAD_END); // get fireware buffer start positon
        memset_s(fwBuffer, fwSize, 0, fwSize);
        fwBuffer->postion = totalLen;
        fwBuffer->len = fwLen;
        memcpy_s(fwBuffer->buffer, RECV_SIZE, pos, fwLen);
        LOS_QueueWriteCopy(g_writeQueueHandle, fwBuffer, fwSize, 3000); // write current fireware buffer to queue, timeout 3000 ms
        mbedtls_sha256_update(ctx, pos, fwLen);
        totalLen += fwLen; // cumulative firmware length
    } else {
        mbedtls_sha256_free(ctx);
        mbedtls_free(ctx);
        free(buffer);
        free(fwBuffer);
        LOS_SwtmrStart(g_timerHandle); // reconnect the server after 1s
        return -1;
    }
    do {
        memset_s(fwBuffer, fwSize, 0, fwSize); // reset fireware buffer
        LOS_SemPend(g_writeSemHandle, LOS_WAIT_FOREVER); // wait write queue done
        ret = dtls_read(g_upgradeContext, buffer, RECV_SIZE, 5000); // set timeout 5000ms
        if (ret == -3) {
            LOS_SemPost(g_writeMutexHandle);
            continue;
        }
        if (ret > 0) {
            memcpy_s(fwBuffer->buffer, RECV_SIZE, buffer, ret);
            fwBuffer->postion = totalLen;
            fwBuffer->len = ret;
            LOS_QueueWriteCopy(g_writeQueueHandle, fwBuffer, fwSize, 5000); // write current fireware buffer to queue, timeout: 5000ms
            err = LOS_SemPend(g_writeSemHandle, 5000); // wait write queue done, timeout: 5000ms
            if (err != LOS_OK) {
                mbedtls_sha256_free(ctx);
                mbedtls_free(ctx);
                free(buffer);
                free(fwBuffer);
                LOS_SwtmrStart(g_timerHandle); // reconnect the server after 1s
                return -1;
            }
            LOS_SemPost(g_writeSemHandle);
            mbedtls_sha256_update(ctx, fwBuffer->buffer, ret);
            totalLen += ret; // get new cumulative value
            // calculate the upgrade progress, and then push the progress to the server
            progress = ((totalLen * 100) / handle->fileSize);
            if ((progress / 20) > 0 && ((progress % 20) == 0) && (lastProgress < progress)) { // push messages every 20%
                lastProgress = progress;
                UpgradeProgressWrite(handle->version, PROCESSING_SUCCEEDED, progress);
            }
        } else {
            LOS_SemPost(g_writeMutexHandle);
        }
    } while (ret > 0);
    if (totalLen < handle->fileSize) {
        mbedtls_sha256_free(ctx);
        mbedtls_free(ctx);
        free(buffer);
        free(fwBuffer);
        LOS_SwtmrStart(g_timerHandle); // reconnect the server after 2s
        return -1;
    }
    handle->writeSize = totalLen;
    ATINY_LOG(LOG_DEBUG, "read total buffer size=%d\n", totalLen);
    mbedtls_sha256_finish(ctx, handle->signCheck);
    mbedtls_sha256_free(ctx);
    mbedtls_free(ctx);
    free(buffer);
    free(fwBuffer);
    return 0;
}

static int UpgradeDeviceInit(UpgradeHandle *handle)
{
    int retrytimes = 5; // maximum 5 retries
    int ret = 0;
    do {
        ret = UpgradeDeviceConnect(handle->host, handle->port);
        if (ret == 0) {
            break; // get ssl handle.
        }
        retrytimes--;
        LOS_TaskDelay(1000); // when reconnecting, set 1000 ms delay
    } while (retrytimes > 0);
    return ret;
}

uint32_t UpgradeMainTask(UpgradeHandle *handle, uint16_t len)
{
    if (handle->callback == NULL) {
        ATINY_LOG(LOG_ERR, "callback is null.");
        return LOS_NOK;
    }
    if (g_upgradeHandle != NULL) {
        free(g_upgradeHandle);
        g_upgradeHandle = NULL;
    }
    g_upgradeHandle = (UpgradeHandle *)malloc(sizeof(UpgradeHandle));
    if (g_upgradeHandle == NULL) {
        return LOS_NOK;
    }
    strcpy_s(g_upgradeHandle->host, sizeof(g_upgradeHandle->host), UPGRADE_SERVER_IP);
    strcpy_s(g_upgradeHandle->port, sizeof(g_upgradeHandle->port), UPGRADE_SERVER_PORT);
    strcpy_s(g_upgradeHandle->url, sizeof(g_upgradeHandle->url), &handle->url[URL_HEAD]);
    strcpy_s(g_upgradeHandle->token, sizeof(g_upgradeHandle->token), handle->token);
    strcpy_s(g_upgradeHandle->sign, sizeof(g_upgradeHandle->sign), handle->sign);
    strcpy_s(g_upgradeHandle->version, sizeof(g_upgradeHandle->version), handle->version);
    g_upgradeHandle->fileSize = handle->fileSize;
    g_upgradeHandle->callback = handle->callback;
    LOS_EventWrite(&g_eventGroup, CONNECT_EVENT);
    return LOS_OK;
}

static void UpgradeClose(void)
{
    if (g_upgradeHandle->fwBuffer != NULL) {
        free(g_upgradeHandle->fwBuffer);
        g_upgradeHandle->fwBuffer = NULL;
    }
    LOS_QueueDelete(g_writeQueueHandle); // delete fireware write queue
    LOS_TaskDelete(g_fwTaskHandle); // delete fireware write task
    if (g_upgradeContext != NULL) {
        dtls_ssl_destroy(g_upgradeContext);
        g_upgradeContext = NULL;
    }
}

static int UpgradeFlagRead(void *buf, int32_t len)
{
    if (g_upgradeHandle != NULL) {
        return g_upgradeHandle->opt.read_flash(OTA_UPDATE_INFO, buf, len, 0);
    }
    return -1;
}

static int UpgradeFlagWrite(const void *buf, int32_t len)
{
    if (g_upgradeHandle != NULL) {
        return g_upgradeHandle->opt.write_flash(OTA_UPDATE_INFO, buf, len, 0);
    }
    return -1;
}

static int UpgradeSignCheck(const uint8_t *inStrings, int strLen, uint8_t *inBytes, int byteLen)
{
    int ret;
    uint8_t checkSign[32];
    if ((inStrings == NULL) || (inBytes == NULL)) {
        return -1;
    }
    if ((strLen <= 0) || (byteLen <= 0)) {
        return -1;
    }
    ret = HexStrToBytes(inStrings, checkSign, strLen);
    if (ret != 0) {
        return -1;
    }
    for (size_t i = 0; i < byteLen; i++) {
        if (checkSign[i] != inBytes[i]) {
            return -1;
        }   
    }
    return 0;
}

static int UpgradeStart(UpgradeHandle *handle)
{
    int ret = 0;
    int checkFlag = 0;
    ret= UpgradeFwWriteTaskInit(); // fireware write task
    if (ret != LOS_OK) {
        return ret;
    }
    // init spiflash rd/wr callback
    memset_s(&handle->opt, sizeof(ota_opt_s), 0, sizeof(ota_opt_s));
    hal_get_ota_opt(&handle->opt);

    // init upgrade flag rd/wr callback
    handle->flagOp.func_flag_read = UpgradeFlagRead;
    handle->flagOp.func_flag_write = UpgradeFlagWrite;
    (void)flag_init(&handle->flagOp);
    ret = flag_upgrade_init();
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_FATAL, "flag upgrade init failed.");
        return ret;
    }
    ret = UpgradeDeviceInit(handle);
    if (ret == 0) { // upgrade init ok
        UpgradeRequestSend(handle);
        ret = UpgradeFileDownload(handle);
    } else {
        ATINY_LOG(LOG_ERR, "Upgrade device init failed.\n");
    }
    // check fireware
   
    checkFlag = UpgradeSignCheck(handle->sign, strlen(handle->sign), handle->signCheck, sizeof(handle->signCheck));
    if (checkFlag == 0) {
        flag_set_info(UPGRADE_FULL, handle->writeSize); // set new upgrade flag
        atiny_reboot(); // system reboot
    }

    if ((ret == 0) && (handle != NULL)) {
        free(handle);
    }
    UpgradeClose();
    return ret;
}

static void UpgradeEventNotify(void)
{
    uint32_t ret;			
    while (1) {
        ret = LOS_EventRead(&g_eventGroup, CONNECT_EVENT | CLOSE_EVENT | DOWNLOAD_ERR_EVENT, LOS_WAITMODE_OR, LOS_WAIT_FOREVER);
        ATINY_LOG(LOG_DEBUG, "upgrade event notify, ret=%d", ret);
        switch (ret) {
            case CONNECT_EVENT:
                if (g_upgradeHandle != NULL) {
                    UpgradeStart(g_upgradeHandle);
                } else {
                    ATINY_LOG(LOG_ERR, "upgrade handle is null.");
                }
                LOS_EventClear(&g_eventGroup, ~CONNECT_EVENT);
                break;
            case CLOSE_EVENT:
                UpgradeClose();
                LOS_EventClear(&g_eventGroup, ~CLOSE_EVENT);
                break;
            case DOWNLOAD_ERR_EVENT:
                if (g_upgradeHandle != NULL) {
                    UpgradeProgressWrite(g_upgradeHandle->version, DOWNLOAD_TIMEOUT, 0);
                }
                g_retryCounts = 0;
                LOS_EventClear(&g_eventGroup, ~DOWNLOAD_ERR_EVENT);
            default:
                break;
        }
    }
}

static int UpgradeEventTask(void)
{
    int ret;
    ret = LOS_EventInit(&g_eventGroup);
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "event create failed.\n");
        return ret;
    }
    TSK_INIT_PARAM_S taskTnitParam;
    taskTnitParam.usTaskPrio = EVENT_TASK_PRIORITY;
    taskTnitParam.pcName = "UpgradeEventNotify";
    taskTnitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)UpgradeEventNotify;
    taskTnitParam.uwStackSize = EVENT_TASK_SIZE;
    ret = LOS_TaskCreate(&g_eventTaskHandle, &taskTnitParam);
    return ret;
}

static void UpgradeProgressSendTask(void)
{
    UINT32 ret = LOS_OK;
    UpgradeProgress recvQueue;
    uint32_t recvLen = sizeof(UpgradeProgress);
    while (1) {
        ret = LOS_QueueReadCopy(g_sendQueueHandle, &recvQueue, &recvLen, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            ATINY_LOG(LOG_DEBUG, "receive data: Version[%s], progress[%d]\n", recvQueue.version, recvQueue.progress);
            if ((g_upgradeHandle != NULL) && (g_upgradeHandle->callback != NULL)) {
                g_upgradeHandle->callback(recvQueue.version, recvQueue.resultCode, recvQueue.progress);
            }
        } else {
            ATINY_LOG(LOG_ERR, "receive failed.\n");
            LOS_TaskDelay(2000); // delay 2000 ms
        }
    }
}

static int UpgradeProgressWrite(char *version, ResultCodeType resultCode, int progress)
{
    int ret;
    UpgradeProgress sendQueue;
    (void)memset_s(&sendQueue, sizeof(UpgradeProgress), 0, sizeof(UpgradeProgress));
    (void)strcpy_s(sendQueue.version, sizeof(sendQueue.version), version);
    sendQueue.progress = progress;
    sendQueue.resultCode = resultCode;
    ret = LOS_QueueWriteCopy(g_sendQueueHandle, &sendQueue, sizeof(UpgradeProgress), 0); // queue write timeout 0 s
    return ret;
}

static void UpgradeFwWriteTask(void)
{
    UINT32 ret = LOS_OK;
    int len = RECV_SIZE + sizeof(FwBuffer);
    int lastPos = 0;
    while (1) {
        if (g_upgradeHandle == NULL) {
            ATINY_LOG(LOG_ERR, "upgrade handle is null.");
            return;
        }
        ret = LOS_QueueReadCopy(g_writeQueueHandle, g_upgradeHandle->fwBuffer, &len, LOS_WAIT_FOREVER);
        if (ret == LOS_OK) {
            if (g_upgradeHandle->opt.write_flash == NULL) {
                ATINY_LOG(LOG_ERR, "upgrade handle is null.");
                return;
            }
            g_upgradeHandle->opt.write_flash(OTA_FULL_SOFTWARE, g_upgradeHandle->fwBuffer->buffer, \
                g_upgradeHandle->fwBuffer->len, g_upgradeHandle->fwBuffer->postion);
        } else {
            ATINY_LOG(LOG_ERR, "receive failed.\n");
            LOS_TaskDelay(2000); // delay 2000 ms
        }
        LOS_SemPost(g_writeSemHandle);
    }
}

static uint32_t UpgradeFwWriteTaskInit(void)
{
    int ret;
    int recvBufferLen = RECV_SIZE + sizeof(FwBuffer);

	ret = LOS_QueueCreate("FwWriteQueue", 2, &g_writeQueueHandle, 0, recvBufferLen);
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "queue create failed.\n", ret);
        return ret;
    }
    g_upgradeHandle->fwBuffer = (FwBuffer *)malloc(recvBufferLen);
    if (g_upgradeHandle->fwBuffer == NULL) {
        LOS_QueueDelete(g_writeQueueHandle);
        return 1;
    }
    TSK_INIT_PARAM_S taskTnitParam;
    taskTnitParam.usTaskPrio = WRITE_TASK_PRIORITY;
    taskTnitParam.pcName = "UpgradeFwWriteTask";
    taskTnitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)UpgradeFwWriteTask;
    taskTnitParam.uwStackSize = WRITE_TASK_SIZE;

    ret = LOS_TaskCreate(&g_fwTaskHandle, &taskTnitParam);
    return (uint32_t)ret;
}

static uint32_t UpgradeProgressTask(void)
{
    int ret;
    ret = LOS_QueueCreate("ProgressQueue", QUEUE_SIZE, &g_sendQueueHandle, 0, sizeof(UpgradeProgress));
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "queue create failed.\n", ret);
        return ret;
    }
    TSK_INIT_PARAM_S taskTnitParam;
    taskTnitParam.usTaskPrio = RECEIVE_TASK_PRIORITY;
    taskTnitParam.pcName = "QueueReceiveTask";
    taskTnitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)UpgradeProgressSendTask;
    taskTnitParam.uwStackSize = RECEIVE_TASK_SIZE;

    ret = LOS_TaskCreate(&g_progressTaskHandle, &taskTnitParam);
    return (uint32_t)ret;
}

static void TimerCallback(uint32_t arg)
{
    if (g_retryCounts < RETRY_TIMES) {
        LOS_EventWrite(&g_eventGroup, CONNECT_EVENT);
        g_retryCounts += 1;
    } else {
        LOS_EventWrite(&g_eventGroup, DOWNLOAD_ERR_EVENT);
    }
}

int UpgradeInit(void)
{
    int ret = LOS_OK;
    ret = LOS_SemCreate(1, &g_writeSemHandle);
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "write sem create failed.\n", ret);
        return ret;
    }
    ret = LOS_MuxCreate(&g_writeMutexHandle);
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "write mutex create failed.\n", ret);
        return ret;
    }
    ret = LOS_SwtmrCreate(2000, LOS_SWTMR_MODE_NO_SELFDELETE, (SWTMR_PROC_FUNC)TimerCallback, &g_timerHandle, 0); // set software timer 2000 ms
    if (ret != LOS_OK) {
        ATINY_LOG(LOG_ERR, "software timer create failed.\n");
    }
    UpgradeProgressTask();
    UpgradeEventTask();
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

