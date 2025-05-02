/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: At Main HeadFile
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

#ifndef _AT_MAIN_H
#define _AT_MAIN_H
#include <stdbool.h>
#include "los_queue.h"
#include "los_mux.h"
#include "los_task.h"
#include "los_sem.h"

#include "at_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* MACRO DEFINE */
#ifdef LOSCFG_COMPONENTS_NET_AT_INFO
#define AT_LOG(fmt, arg...)  printf("[%lu][%s:%d][I]"fmt"\n", AtGetTime(), __func__, __LINE__, ##arg)
#else
static inline void __do_nothing(const char *fmt, ...) { (void)fmt; }
#define AT_LOG(fmt, arg...)  __do_nothing(fmt, ##arg)
#endif

#ifdef LOSCFG_COMPONENTS_NET_AT_DEBUG
#define AT_LOG_DEBUG(fmt, arg...)  printf("[%lu][%s:%d][D]"fmt"\n", AtGetTime(), __func__, __LINE__, ##arg)
#else
#define AT_LOG_DEBUG(fmt, arg...)
#endif

#define AT_OK             0
#define AT_FAILED         -1
#define AT_TIMEOUT        -2

#define AT_LINK_UNUSE     0
#define AT_LINK_INUSE     1

#define AT_MUXMODE_SINGLE 0
#define AT_MUXMODE_MULTI  1

#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif /* MIN */

#ifndef array_size
#define array_size(a) (sizeof(a) / sizeof(*(a)))
#endif

/* VARIABLE DECLEAR */

/* TYPE REDEFINE */
typedef int32_t (*OobCallback)(void *arg, int8_t *buf, int32_t buflen);

typedef char *(*OobCmdMatch)(const char *buf, char *featureStr, int len);

#define MAXIPLEN  40
typedef struct {
    uint32_t len;
    uint8_t *addr;
    char ipaddr[MAXIPLEN];
    int port;
} QueueBuffer;

typedef struct {
    uint32_t recvSem; // waiting to read net payload
    uint32_t totalSize; // net recv size
    uint32_t offset; // current read position
    uint32_t lastEnd; 
    uint8_t *buff; // net pyaload, (malloc(size))
} AtNetRcvBuff;

typedef struct {
    uint32_t fd; // convert between socket_fd and linkedId
    uint32_t maxFd; // max link number of socket fd
    uint32_t qid; // queue id
    uint32_t usable; // used or not
    uint8_t remoteIP[16]; // server ip
    uint32_t remotePort;  // server port
    AtNetRcvBuff payload; // current recv palyload
} AtNetLink;

typedef struct {
    const char **suffix;
    int suffixNumber;
    int matchIndex;
    char *respBuffer;
    uint32_t *respLen;
} AtCmdInfo;

typedef struct _listener {
    struct _listener *next;
    AtCmdInfo cmdInfo;
    uint32_t expireTime;
    int32_t (*HandleData)(const int8_t *data, uint32_t len);
} AtListener;

typedef struct {
    AtListener *head;
    void (*ListAdd)(AtListener *p);
    void (*ListDel)(AtListener *p);
    void (*ListDestroy)(AtListener *pHead);
    void (*NodeDel)(AtListener *listener, AtListener *pre);
    void (*TimeoutNodesDel)(AtListener *pHead);
} AtListenerHandle;

#define OOB_MAX_NUM  5
#define OOB_CMD_LEN  40
#define AT_DATA_LEN  1024
typedef struct oob_s {
    char featureStr[OOB_CMD_LEN];
    int len;
    int runFlag;
    OobCmdMatch cmdMatch;
    OobCallback callback;
    void *arg;
} OobType;

typedef struct at_oob_s {
    OobType oob[OOB_MAX_NUM];
    int32_t oobNum;
} AtOobType;

/* AT device configure operation */
typedef struct _AtConfig {
    char *name;
    uint32_t usartPort;
    uint32_t buardrate;
    uint32_t maxLinkIdNum;
    uint32_t maxBufferLen; // malloc 3 block memory for intener use, len * 3
    char *cmdBeginStr;
    char *lineEndStr;
    uint32_t multiMode; 
#ifdef LOSCFG_COMPONENTS_NET_AT_ESP8266
    uint32_t dinfo;
#endif
    uint32_t timeout; // command respond timeout
    uint8_t  *userData; // extension field
    void (*set)(struct _AtConfig *config);
    struct _AtConfig *(*get)(void);
    void (*transmit)(uint8_t *cmd, int32_t len, int lineEndFlag); // at device data transport method.
    void (*transmitDeinit)(void);
} AtConfig;

/* AT semaphore interface */
typedef struct {
    uint32_t recvSem;
    uint32_t respSem;   // the response semaphore of recv
    uint32_t (*create)(uint16_t count, uint32_t *semHandle);
    uint32_t (*pend)(uint32_t semHandle, uint32_t timeout);
    uint32_t (*post)(uint32_t semHandle);
    uint32_t (*delete)(uint32_t semHandle);
} AtSemHandle;

/* AT mutex interface */
typedef struct {
    uint32_t cmdMux;   // send cmd mutex
    uint32_t sendMux;  // transport mutex
    uint32_t recvMux;  // transport mutex
    uint32_t (*create)(uint32_t *muxHandle);
    uint32_t (*pend)(uint32_t muxHandle, uint32_t timeout);
    uint32_t (*post)(uint32_t muxHandle);
    uint32_t (*delete)(uint32_t muxHandle);
} AtMutexHandle;

/* AT uart recvived queue */
typedef struct {
    uint32_t queueRecvId;
    bool     queueRecvFlag;
    uint32_t queueWrErrorCounts;
    uint32_t (*create)(const char *queueName, uint16_t len, uint32_t *queueId, uint32_t flags, uint16_t maxMsgSize);
    uint32_t (*delete)(uint32_t queueId);
    uint32_t (*read)(uint32_t queueId, void *bufferAddr, uint32_t *bufferSize, uint32_t timeout);
    uint32_t (*write)(uint32_t queueId, void *bufferAddr, uint32_t bufferSize, uint32_t timeout);
} AtQueueHandle;

typedef struct {
    uint8_t *buff;
    uint32_t maxLen;
} AtRecvBuff;

typedef struct at_task {
    uint32_t atTaskId;
    AtSemHandle sem;
    AtMutexHandle mutex;
    AtQueueHandle queue;
    AtListenerHandle listener;
    bool sendMuxFlag;
    AtRecvBuff recv;
    uint8_t *cmdResp; // AT cmd response, default 512 bytes
    uint8_t *userData; // data form servers, default 512 bytes
    uint8_t *savedData;
    uint32_t multiMode; // support multi connection mode
    AtNetLink *linkedId;
    uint32_t timeout; // command respond timeout
    AtConfig config;
    
    /* AT Callback function */
    void (*stepCallback)(void);
    int32_t (*init)(AtConfig *config);
    int32_t (*writeCmd)(int8_t *cmd, int32_t len, const char *suffix, char *resp_buf, int *resp_len);
    int32_t (*write)(int8_t *cmd, int8_t *suffix, int8_t *buf, int32_t len);
    /* get unused linkedId, use in multi connection mode */
    int32_t (*getLinkedId)(void);
    /* register uset msg process to the listener list */
    int32_t (*oobRegister)(char *featureStr, int cmdlen, OobCallback callback, OobCmdMatch cmd_match);
    void (*deInit)(void);
    int32_t (*cmdMultiSuffix)(const int8_t *cmd, int len, AtCmdInfo *cmdInfo);
} AtTaskHandle;

int CharToInt(const char *port);
int AtUpdateResultSend(void);
int32_t AtCmdInCallback(const int8_t *cmd, int32_t len, int32_t (*HandleData)(const int8_t *data, uint32_t len), uint32_t timeout);
uint32_t AtGetTime(void);
void AtRegStepCallback(AtTaskHandle *atTask, void (*stepCallback)(void));

void *AtMalloc(size_t size);
void AtFree(void *ptr);

extern uint16_t AtFotaTimer;

void AtTaskHandleInit(void);
AtTaskHandle *AtGetTaskHandle(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _AT_MAIN_H */
