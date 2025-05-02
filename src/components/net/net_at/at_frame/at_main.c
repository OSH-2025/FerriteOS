/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: At Main Implementation
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

#include "at_main.h"
#include "los_memory.h"
#include "los_sys.h"
#include "los_tick.h"
#ifdef WITH_SOTA
#include "sota/sota.h"
#endif
#include "at_hal.h"

AtOobType g_atOob;

AtTaskHandle g_at;

uint32_t AtGetTime(void)
{
    return ((uint32_t)(LOS_TickCountGet()* (OS_SYS_MS_PER_SECOND / LOSCFG_BASE_CORE_TICK_PER_SECOND)))/ 1000;
}

/* Data type conversion */
int CharToInt(const char* port)
{
    int tmp = 0;
    while ((*port >= '0') && (*port <= '9')) {
        tmp = tmp * 10 + *port - '0';
        port++;
    }
    return tmp;
}

void *AtMalloc(size_t size)
{
    void *pMem = LOS_MemAlloc(m_aucSysMem0, size);
    if (pMem != NULL) {
        memset(pMem, 0, size);
    }
    return pMem;
}

void AtFree(void *ptr)
{
    (void)LOS_MemFree(m_aucSysMem0, ptr);
}

void AtListenerListAdd(AtListener *p)
{
    AtListener *head = g_at.listener.head;
    AtListener *cur = NULL;

    p->next = NULL;
    if (head == NULL) {
        g_at.listener.head = p;
        return;
    }

    cur = head;
    while (cur->next) {
        cur = cur->next;
    }

    cur->next = p;
 }

void AtListenerListDel(AtListener *p)
{
    AtListener *head = g_at.listener.head;
    AtListener *cur = NULL;

    if (p == head) {
        g_at.listener.head = head->next;
        return;
    }

    cur = head;
    while (cur->next) {
        if (cur->next == p) {
            cur->next = p->next;
            break;
        }
    }
}

void AtListnerListDestroy(AtListener *pHead)
{
    AtListener *head = NULL;
    while (pHead != NULL) {
        head = pHead;
        pHead = head->next;
        if (head->HandleData != NULL) {
            AtFree(head);
        }
    }
}

static void AtListenerNodeDel(AtListener *listener, AtListener *pre)
{
    if (g_at.listener.head == listener) {
        g_at.listener.head = listener->next;
    } else {
        if (pre) {
            pre->next = listener->next;
        }
    }
    AtFree(listener);

    g_at.mutex.post(g_at.mutex.sendMux);
}

static void AtListenerTimeoutNodesDel(AtListener *pHead)
{
    AtListener *pre = NULL;
    AtListener *next = NULL;
    AtListener *listener = NULL;
    for (listener = pHead; listener != NULL; listener = next) {
        next = listener->next;
        if (listener->HandleData == NULL) {
            continue;
        }
        if (listener->expireTime <= AtGetTime()) {
            AT_LOG("get recv data timeout");
            AtListenerNodeDel(listener, pre);
        }
        else {
            pre = listener;
        }
    }
}

int32_t AtGetUnusedLinkId(void)
{
    int i = 0;

    if (g_at.multiMode == AT_MUXMODE_MULTI) {
        for (i = 0; i < g_at.config.maxLinkIdNum; i++) {
            if (g_at.linkedId[i].usable == AT_LINK_UNUSE) {
                break;
            }
        }
    }

    if (i < g_at.config.maxLinkIdNum) {
        g_at.linkedId[i].usable = AT_LINK_INUSE;
    }

    return i;
}

static void StoreRespBuffer(int8_t *respBuffer, const int8_t *src, uint32_t src_len, uint32_t* maxlen)
{
    int copy_len;

    copy_len = MIN(*maxlen, src_len);
    memcpy((char *)respBuffer, (char *)src, copy_len);
    *maxlen = copy_len;
}

int32_t AtCmdInCallback(const int8_t *cmd, int32_t len,
                    int32_t (*HandleData)(const int8_t *data, uint32_t len),  uint32_t timeout)
{
    int32_t ret = AT_FAILED;

    if (HandleData != NULL) {
        if (g_at.mutex.pend(g_at.mutex.sendMux, 0) != AT_OK) {
            return AT_FAILED;
        }
    }

    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    g_at.config.transmit((uint8_t *)cmd, len, 1);
    if (HandleData != NULL) {
        AtListener *listener = NULL;

        listener = AtMalloc(sizeof(AtListener));
        if (listener == NULL) {
            AT_LOG("AtMalloc fail");
            goto EXIT;
        }
        memset(listener, 0, sizeof(AtListener));

        listener->HandleData = HandleData;
        listener->expireTime = AtGetTime() + timeout;
        AtListenerListAdd(listener);
    }
    ret = AT_OK;

EXIT:
    g_at.mutex.post(g_at.mutex.cmdMux);

    AT_LOG("len %ld,cmd %s", len, cmd);

    return ret;
}

static void AtWriteTaskMsg(AtMsgType type)
{
    AtRecvQueue recvBuffer;
    int ret;

    (void)memset_s(&recvBuffer, sizeof(AtRecvQueue), 0, sizeof(AtRecvQueue));
    recvBuffer.msgType = type;

    ret = g_at.queue.write(g_at.queue.queueRecvId, &recvBuffer, sizeof(AtRecvQueue), 0);
    if (ret != LOS_OK) {
        g_at.queue.queueWrErrorCounts++;
    }
}

int32_t AtCmdMultiSuffix(const int8_t *cmd, int  len, AtCmdInfo *cmdInfo)
{
    AtListener listener;
    int ret;
    int print_len;

    if ((cmdInfo == NULL) || (cmd == NULL)) {
        return AT_FAILED;
    }

    memset(&listener, 0, sizeof(listener));
    listener.cmdInfo = *cmdInfo;
    print_len = ((cmdInfo->respBuffer && cmdInfo->respLen) ? (int)*(cmdInfo->respLen) : -1);
    AT_LOG("cmd len %d, %p, %d", print_len, cmdInfo->respBuffer, (int)cmdInfo->respLen);

    g_at.mutex.pend(g_at.mutex.sendMux, LOS_WAIT_FOREVER);

    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    AtListenerListAdd(&listener);
    if (g_at.config.transmit == NULL) {
        AT_LOG("transmit is null.");
    }
    g_at.config.transmit((uint8_t *)cmd, len, 1);
    g_at.mutex.post(g_at.mutex.cmdMux);

    ret = g_at.sem.pend(g_at.sem.respSem, g_at.timeout); // wait for receive response
    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER); 
    AtListenerListDel(&listener);
    g_at.mutex.post(g_at.mutex.cmdMux);

    g_at.mutex.post(g_at.mutex.sendMux);
    AtWriteTaskMsg(AT_SENT_DONE);

    if (ret != LOS_OK) {
        AT_LOG("g_at.sem.pend for listener.respSem failed(ret = %x, cmd = %s)!", ret, cmd);
        return AT_FAILED;
    }

    *cmdInfo = listener.cmdInfo;
    return AT_OK;
}

int32_t AtWriteCmd(int8_t *cmd, int32_t len, const char *suffix, char *respBuffer, int *respLen)
{
    const char *suffix_array[1] = {suffix};
    AtCmdInfo cmdInfo;

    memset(&cmdInfo, 0, sizeof(cmdInfo));
    cmdInfo.suffix = suffix_array;
    cmdInfo.suffixNumber = array_size(suffix_array);
    cmdInfo.respBuffer = respBuffer;
    cmdInfo.respLen = (uint32_t *)respLen;

    return AtCmdMultiSuffix(cmd, len, &cmdInfo);
}

static int32_t AtWriteData(int8_t *cmd, int8_t *suffix, int8_t *buf, int32_t len)
{
    const char *suffix_array[1];
    AtListener listener;
    int ret = AT_FAILED;

    memset(&listener, 0, sizeof(listener));
    suffix_array[0] = ">"; //ready to write data.
    listener.cmdInfo.suffixNumber = 1;
    listener.cmdInfo.suffix = suffix_array;
    g_at.mutex.pend(g_at.mutex.sendMux, LOS_WAIT_FOREVER);
    AtListenerListAdd(&listener);
    
    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    g_at.config.transmit((uint8_t *)cmd, strlen((char *)cmd), 1);
    g_at.mutex.post(g_at.mutex.cmdMux);

    (void)g_at.sem.pend(g_at.sem.respSem, 200);

    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    suffix_array[0] = (char *)suffix;
    AtListenerListDel(&listener);
    AtListenerListAdd(&listener);
    g_at.config.transmit((uint8_t *)buf, len, 0);
    g_at.mutex.post(g_at.mutex.cmdMux);

    ret = g_at.sem.pend(g_at.sem.respSem, g_at.timeout);

    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    AtListenerListDel(&listener);
    g_at.mutex.post(g_at.mutex.cmdMux);

    g_at.mutex.post(g_at.mutex.sendMux);
    AtWriteTaskMsg(AT_SENT_DONE);

    if (ret != LOS_OK) {
        AT_LOG("g_at.sem.pend for listener.respSem failed(ret = %x)!", ret);
        return AT_FAILED;
    }
    return len;
}

static int CloudCmdMatching(int8_t *buf, int32_t len)
{
    char *cmp = NULL;
    for (int i = 0; i < g_atOob.oobNum; i++) {
        cmp = g_atOob.oob[i].cmdMatch((const char *)buf, g_atOob.oob[i].featureStr, g_atOob.oob[i].len);
        if ((cmp != NULL) && g_atOob.oob[i].callback != NULL) {
            (void)g_atOob.oob[i].callback(g_atOob.oob[i].arg, (int8_t *)buf, (int32_t)len);
            return len;
        }
    }
    return 0;
}

static int AtHandleCallbackCmdResponse(AtListener *listener, int8_t *respBuffer, uint32_t respLen)
{
    if (listener->HandleData == NULL) {
        return AT_FAILED;
    }

    if (listener->HandleData(respBuffer, respLen) == AT_OK) {
        AtListenerNodeDel(listener, NULL);
        return AT_OK;
    }

    return AT_FAILED;
}

static void AtHandleResponse(int8_t *respBuffer, uint32_t respLen)
{
    AtListener *listener = NULL;

    listener = g_at.listener.head;
    if (listener == NULL) {
        return;
    }
    
    if (AtHandleCallbackCmdResponse(listener, respBuffer, respLen) == AT_OK) {
        return;
    }

    for (uint32_t i = 0; i < listener->cmdInfo.suffixNumber; i++) {
        if (listener->cmdInfo.suffix[i] == NULL) {
            continue;
        }
        char *suffix = strstr((char *)respBuffer, (const char *)listener->cmdInfo.suffix[i]);
        if (suffix != NULL) {
            AT_LOG("match suffix=%s\n", suffix);
            if ((listener->cmdInfo.respBuffer != NULL) && (listener->cmdInfo.respLen != NULL) && (respLen > 0)) {
                StoreRespBuffer((int8_t *)listener->cmdInfo.respBuffer, respBuffer, respLen, listener->cmdInfo.respLen);
            }
            listener->cmdInfo.matchIndex = i;
            (void)g_at.sem.post(g_at.sem.respSem);
            break;
        } else {
             AT_LOG("match suffix is null\n");
        }
    }
}

static uint32_t AtGetQueueWaitTime(void)
{
    uint32_t ret;

    if (g_at.stepCallback == NULL) {
        return LOS_WAIT_FOREVER;
    }

    g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
    if ((g_at.listener.head == NULL) || (g_at.listener.head->HandleData == NULL)) {
        ret = LOS_WAIT_FOREVER;
    } else {
        const uint32_t min_wait_time = 100;
        uint32_t current = AtGetTime();
        ret = ((current >= g_at.listener.head->expireTime) ? min_wait_time : ((g_at.listener.head->expireTime - current) * 1000));
    }

    g_at.mutex.post(g_at.mutex.cmdMux);
    return ret;
}

void AtRecvTask(void)
{
    uint32_t recvLen = 0;
    uint8_t *buffer = g_at.userData;
    int ret = 0;
    AtRecvQueue queueBuffer;
    UINT32 queueLen = sizeof(AtRecvQueue);
    while (1) {
        uint32_t waitTime = AtGetQueueWaitTime();
        ret = g_at.queue.read(g_at.queue.queueRecvId, &queueBuffer, &queueLen, waitTime); //recv usart buffer.
        if (ret != LOS_OK) {
            continue;
        }

        if (queueBuffer.msgType == AT_TASK_QUIT) {
            AT_LOG("at recv task quit");
            break;
        }
        g_at.mutex.pend(g_at.mutex.recvMux, LOS_WAIT_FOREVER);
        g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
        AtListenerTimeoutNodesDel(g_at.listener.head);
        g_at.mutex.post(g_at.mutex.cmdMux);

        if (g_at.stepCallback) {
            g_at.stepCallback();
        }

        if (queueBuffer.msgType != AT_USART_RX) {
            AT_LOG("at recv msg sent done");
            g_at.mutex.post(g_at.mutex.recvMux);
            continue;
        }
        
        if (g_at.config.maxBufferLen <= 0) {
            AT_LOG("no buffer, task quit");
            g_at.mutex.post(g_at.mutex.recvMux);
            break;
        }
        memset(buffer, 0, g_at.config.maxBufferLen);
        AtReadQueueBuffer(buffer, &recvLen, (AtRecvQueue *)&queueBuffer);

        if (recvLen <= 0) {
            AT_LOG("err, recvLen = %ld", recvLen);
            g_at.mutex.post(g_at.mutex.recvMux);
            continue;
        }
        AT_LOG_DEBUG("recvLen len = %lu buf = %s ", recvLen, buffer);
        
        g_at.mutex.pend(g_at.mutex.cmdMux, LOS_WAIT_FOREVER);
        AtListenerTimeoutNodesDel(g_at.listener.head);
        AtHandleResponse((int8_t *)buffer, recvLen);
        g_at.mutex.post(g_at.mutex.cmdMux);

        CloudCmdMatching((int8_t *)buffer, recvLen);
        g_at.mutex.post(g_at.mutex.recvMux);
    }
    g_at.queue.delete(g_at.queue.queueRecvId); // After the task exits, delete queue
    LOS_TaskDelete(g_at.atTaskId);
}

uint32_t AtRecvTaskCreate(AtTaskHandle *at)
{
    uint32_t ret = LOS_OK;
    TSK_INIT_PARAM_S taskInitParam;

    taskInitParam.usTaskPrio = 0;
    taskInitParam.pcName = "AtRecvTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)AtRecvTask;
    taskInitParam.uwStackSize = 0x1000;

    ret = LOS_TaskCreate((UINT32 *)&at->atTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}

void AtOobInit(void)
{
    g_atOob.oobNum = 0;
    memset(g_atOob.oob, 0, OOB_MAX_NUM * sizeof(struct oob_s));
}

static int32_t AtTaskInit(AtTaskHandle *at)
{
    int ret = -1;
    if (at == NULL) {
        AT_LOG("invaild param!");
        return ret;
    }
    /* init uart received queue */
    ret = at->queue.create("recvQueue", 32, (UINT32 *)&at->queue.queueRecvId, 0, sizeof(AtRecvQueue));
    if (ret != LOS_OK) {
        AT_LOG("init recvQueue failed!");
        return AT_FAILED;
    }
    at->queue.queueRecvFlag = true;

    ret = at->sem.create(0, (UINT32 *)&at->sem.recvSem);
    if (ret != LOS_OK) {
        AT_LOG("init at_recv_sem failed!");
        goto at_recv_sem_failed;
    }

    ret = at->mutex.create((UINT32 *)&at->mutex.cmdMux);
    if (ret != LOS_OK) {
        AT_LOG("init cmdMux failed!");
        goto at_cmd_mux_failed;
    }

    ret = at->mutex.create((UINT32 *)&at->mutex.sendMux);
    if (ret != LOS_OK) {
        AT_LOG("init sendMux failed!");
        goto atSendMuxFailed;
    }
    at->sendMuxFlag = true;

    ret = at->mutex.create((UINT32 *)&at->mutex.recvMux);
    if (ret != LOS_OK) {
        AT_LOG("init sendMux failed!");
        goto at_recv_mux_failed;
    }


    ret = at->sem.create(0, (UINT32 *)&at->sem.respSem);
    if (ret != LOS_OK) {
        AT_LOG("init respSem failed!");
        goto at_resp_sem_failed;
    }
#ifndef USE_USARTRX_DMA
    at->recv.buff = AtMalloc(at->config.maxBufferLen);
    if (at->recv.buff == NULL) {
        AT_LOG("malloc recvBuffer failed!");
        goto mallocRecvBuffError;
    }
    at->recv.maxLen = at->config.maxBufferLen;
#else
    at->recv.buff = AtMalloc(at->config.maxBufferLen);
    if (at->recv.buff == NULL) {
        AT_LOG("malloc recvBuffer failed!");
        goto mallocRecvBuffError;
    }
    at->recv.maxLen = at->config.maxBufferLen;
#endif

    // init AT cmd response
    at->cmdResp = AtMalloc(at->config.maxBufferLen);
    if (at->cmdResp == NULL) {
        AT_LOG("malloc cmdResp failed!");
        goto malloc_respBuffer;
    }

    at->userData = AtMalloc(at->config.maxBufferLen);
    if (at->userData == NULL) {
        AT_LOG("malloc userData failed!");
        goto malloc_userdata_buf;
    }
    at->savedData = AtMalloc(at->config.maxBufferLen);
    if (at->savedData == NULL) {
        AT_LOG("malloc savedData failed!");
        goto malloc_saveddata_buf;
    }
    uint32_t size = at->config.maxLinkIdNum * sizeof(AtNetLink);
    at->linkedId = (AtNetLink *)AtMalloc(size);
    if (at->linkedId == NULL) {
        AT_LOG("malloc for at linkedId array failed!");
        goto malloc_linkid_failed;
    }
    memset(at->linkedId, 0, size);

    at->listener.head = NULL;
    at->multiMode =at->config.multiMode;
    at->timeout =at->config.timeout;
    return AT_OK;

malloc_linkid_failed:
  AtFree(at->savedData);
malloc_saveddata_buf:
  AtFree(at->userData);
malloc_userdata_buf:
  AtFree(at->cmdResp);
malloc_respBuffer:
  AtFree(at->recv.buff);
mallocRecvBuffError:
    (void)at->sem.delete(at->sem.respSem);
at_resp_sem_failed:
    (void)at->mutex.delete(at->mutex.recvMux);
at_recv_mux_failed:
atSendMuxFailed:
    (void)at->mutex.delete(at->mutex.cmdMux);
at_cmd_mux_failed:
    (void)at->sem.delete(at->sem.recvSem);
at_recv_sem_failed:

    if (at->sendMuxFlag) {
        (void)at->mutex.delete(at->mutex.sendMux);
        at->sendMuxFlag = false;
    }

    if (at->queue.queueRecvFlag) {
        (VOID)at->queue.delete(at->queue.queueRecvId);
        at->queue.queueRecvFlag = false;
    }
    return AT_FAILED;
}

static int32_t AtTaskDeinit(AtTaskHandle *at)
{
    int32_t ret = AT_OK;

    if (at == NULL) {
        AT_LOG("invaild param!");
        return AT_FAILED;
    }

   at->mutex.pend(at->mutex.cmdMux, LOS_WAIT_FOREVER);
   AtListnerListDestroy(at->listener.head);
   at->mutex.post(at->mutex.cmdMux);

    if (at->sem.delete(at->sem.recvSem) != LOS_OK) {
        AT_LOG("delete g_at.recvSem failed!");
        ret = AT_FAILED;
    }

    if (at->mutex.delete(at->mutex.cmdMux) != LOS_OK) {
        AT_LOG("delete g_at.mutex.cmdMux failed!");
        ret = AT_FAILED;
    }

    if (at->sem.delete(at->sem.respSem) != LOS_OK) {
        AT_LOG("delete g_at.sem.respSem failed!");
        ret = AT_FAILED;
    }

    if (at->sendMuxFlag) {
        (void)at->mutex.delete(at->mutex.sendMux);
      at->sendMuxFlag = false;
    }

    if (at->queue.queueRecvFlag) {
        (VOID)at->queue.delete(at->queue.queueRecvId);
      at->queue.queueRecvFlag = false;
    }

    if (at->recv.buff) {
      AtFree(at->recv.buff);
      at->recv.buff = NULL;
    }

    if (at->cmdResp != NULL) {
      AtFree(at->cmdResp);
      at->cmdResp = NULL;
    }

    if (at->userData != NULL) {
      AtFree(at->userData);
      at->userData = NULL;
    }

    if (at->savedData != NULL) {
      AtFree(at->savedData);
      at->savedData = NULL;
    }

    if (at->linkedId != NULL) {
      AtFree(at->linkedId);
      at->linkedId = NULL;
    }

  at->atTaskId = 0xFFFF;
  at->listener.head = NULL;
  at->multiMode = AT_MUXMODE_SINGLE;
  at->timeout = 0;

    return ret;
}

static void AtSetConfig(AtConfig *config)
{
    if (config != NULL) {
        memcpy(&g_at.config, config, sizeof(AtConfig));
    }  
    g_at.config.transmit = AtUsartTransmit;
    g_at.config.transmitDeinit = AtUsartDeinit;
}

static AtConfig *AtGetConfig(void)
{
    return &g_at.config;
}

static void AtListenerInit(AtListenerHandle *listenerHandle)
{
    listenerHandle->head = NULL;
    listenerHandle->ListAdd = AtListenerListAdd;
    listenerHandle->ListDel = AtListenerListDel;
    listenerHandle->ListDestroy = AtListnerListDestroy;
    listenerHandle->NodeDel = AtListenerNodeDel;
    listenerHandle->TimeoutNodesDel = AtListenerTimeoutNodesDel;
}

static int32_t AtInit(AtConfig *config)
{
    AtUsartRecv uartRecv;
    if (config == NULL) {
        AT_LOG("Config is NULL, failed!!\n");
        return AT_FAILED;
    }

    /* AT user config init */
    if (g_at.config.set != NULL) {
        g_at.config.set(config);
    }

    AT_LOG_DEBUG("Config %s(buffer total is %lu)......\n", g_at.config.name, g_at.config.maxBufferLen);

    if (AtTaskInit(&g_at) != AT_OK) {
        AT_LOG("prepare AT struct failed!");
        return AT_FAILED;
    }

    AtOobInit();
    uartRecv.uartPort = g_at.config.usartPort;
    uartRecv.buardrate = g_at.config.buardrate;
    uartRecv.queueId = g_at.queue.queueRecvId;
    uartRecv.buffer = g_at.recv.buff;
    uartRecv.maxLen = g_at.recv.maxLen;
    if (AtUsartInit(&uartRecv) != AT_OK) {
        AT_LOG("UsartInit failed!");
        (void)AtTaskDeinit(&g_at);
        return AT_FAILED;
    }

    AtListenerInit(&g_at.listener);

    if (AtRecvTaskCreate(&g_at) != AT_OK) {
        AT_LOG("AtRecvTaskCreate failed!");
        (void)AtTaskDeinit(&g_at);
        return AT_FAILED;
    }

    AT_LOG("Config complete!!\n");
    return AT_OK;
}

void AtDeInit(void)
{
    int cnt = 0;
    const int maxTryNum = 10;
    TSK_INFO_S stTaskInfo;

    while ((LOS_TaskInfoGet(g_at.atTaskId, &stTaskInfo) != LOS_OK) && (cnt < maxTryNum)) {
        AtWriteTaskMsg(AT_TASK_QUIT);
        LOS_TaskDelay(1000);
        cnt++;
    }

    if (LOS_TaskInfoGet(g_at.atTaskId, &stTaskInfo) != LOS_OK) {
        if (LOS_TaskDelete(g_at.atTaskId) != LOS_OK) {
            AT_LOG("AtRecvTask delete failed!");
        }
    }

    g_at.config.transmitDeinit();
    if (AtTaskDeinit(&g_at) != LOS_OK) {
        AT_LOG("AtTaskDeinit failed!");
    }
    AtOobInit();
}


static int32_t AtOobRegister(char *featureStr, int cmdlen, OobCallback callback, OobCmdMatch cmdMatch)
{
    OobType *oob = NULL;
    if ((featureStr == NULL) || (cmdMatch == NULL) || (g_atOob.oobNum == OOB_MAX_NUM) || (cmdlen >= OOB_CMD_LEN - 1)) {
        return -1;
    }
    oob = &(g_atOob.oob[g_atOob.oobNum++]);
    memcpy(oob->featureStr, featureStr, cmdlen);
    oob->len = strlen(featureStr);
    oob->callback = callback;
    oob->cmdMatch = cmdMatch;
    return 0;
}

void AtRegStepCallback(AtTaskHandle *atTask, void (*stepCallback)(void))
{
    atTask->stepCallback = stepCallback;
}

void AtTaskHandleInit(void)
{
    g_at.atTaskId = 0xFFFF;
    g_at.recv.buff = NULL;
    g_at.cmdResp = NULL;
    g_at.userData = NULL;
    g_at.linkedId = NULL;
    g_at.init = AtInit;
    g_at.deInit = AtDeInit;
    g_at.writeCmd = AtWriteCmd;
    g_at.write = AtWriteData;
    g_at.oobRegister = AtOobRegister;
    g_at.getLinkedId = AtGetUnusedLinkId;
    g_at.cmdMultiSuffix = AtCmdMultiSuffix;
    g_at.config.set = AtSetConfig;
    g_at.config.get = AtGetConfig;
    g_at.sem.create = LOS_SemCreate;
    g_at.sem.pend = LOS_SemPend;
    g_at.sem.post = LOS_SemPost;
    g_at.sem.delete = LOS_SemDelete;
    g_at.mutex.create = LOS_MuxCreate;
    g_at.mutex.pend = LOS_MuxPend;
    g_at.mutex.post = LOS_MuxPost;
    g_at.mutex.delete = LOS_MuxDelete;
    g_at.queue.create = LOS_QueueCreate;
    g_at.queue.read = LOS_QueueReadCopy;
    g_at.queue.write = LOS_QueueWriteCopy;
    g_at.queue.delete = LOS_QueueDelete;
}

AtTaskHandle *AtGetTaskHandle()
{
    return &g_at;
}