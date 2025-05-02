/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Libevent Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-10-14
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

#include "libevent_demo.h"
#include "los_task.h"
#include <unistd.h>
#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PORT                       23232
#define LISTEN                     5
#define EVENT_SIZE                 1024
#define BUFFER_LEN                 50
#define LIBEVENT_TASK_PRIORITY     3
#define LIBEVENT_TASK_STACK_SIZE   0x1000

STATIC UINT32 g_demoTaskId;

struct event_base *g_base = NULL;

struct SocketEvent {
    struct event *readEvent;
    struct event *writeEvent;
    CHAR *buffer;
};

VOID FreeSocketEvent(struct SocketEvent* ev)
{
    if (ev == NULL) {
        return;
    }
    if (ev->readEvent != NULL) {
        event_del(ev->readEvent);
    }
    if (ev->readEvent != NULL) {
        free(ev->readEvent);
    }
    if (ev->writeEvent != NULL) {
        free(ev->writeEvent);
    }
    if (ev->buffer != NULL) {
        free(ev->buffer);
    }
    free(ev);
}

void WriteEvent(int sock, short event, void *arg)
{
    (VOID)arg;
    (VOID)event;
    INT32 ret;
    CHAR buffer[BUFFER_LEN] = "Welcome to the LiteOS community.";
    ret = send(sock, buffer, strlen(buffer), 0);
    if (ret < 0) {
        printf("Send data failed.\n");
	    return;
    }
    printf("Send data : %s success.\n", buffer);
}

void ReadEvent(int sock, short event, void *arg)
{
    (VOID)event;
    INT32 size;
    INT32 ret;
    if (arg == NULL) {
        return;
    }
    struct SocketEvent *ev = (struct SocketEvent*)arg;
    ev->buffer = (CHAR *)malloc(EVENT_SIZE);
    if (ev->buffer == NULL) {
        return;
    }
    ev->writeEvent = (struct event *)malloc(sizeof(struct event));
    if (ev->writeEvent == NULL) {
        free(ev->buffer);
        return;
    }
    bzero(ev->buffer, EVENT_SIZE);
    size = recv(sock, ev->buffer, EVENT_SIZE, 0);
    if (size <= 0) {
        free(ev->buffer);
        free(ev->writeEvent);
        (VOID)close(sock);
        return;
    }
    ev->buffer[size] = '\0';
    printf("Receive data : %s, size : %d\n", ev->buffer, size);
    event_set(ev->writeEvent, sock, EV_WRITE, WriteEvent, NULL);
    ret = event_base_set(g_base, ev->writeEvent);
    if (ret != 0) {
        printf("Event base set failed.\n");
        free(ev->buffer);
        free(ev->writeEvent);
        return;
    }
    ret = event_add(ev->writeEvent, NULL);
    if (ret == -1) {
        printf("Event add failed.\n");
    }
    if (ev->buffer != NULL) {
        free(ev->buffer);
    }
    if (ev->writeEvent != NULL) {
        free(ev->writeEvent);
    }
}

void AcceptEvent(int sock, short event, void *arg)
{
    (VOID)event;
    struct sockaddr_in clientAddr;
    INT32 ret;
    INT32 newFd;
    INT32 sinSize;
    struct SocketEvent *ev = (struct SocketEvent*)arg;
    ev->readEvent = (struct event *)malloc(sizeof(struct event));
    if (ev->readEvent == NULL) {
        return;
    }
    sinSize = (INT32)sizeof(struct sockaddr_in);
    newFd = accept(sock, (struct sockaddr *)&clientAddr, &sinSize);
    if (newFd < 0) {
        printf("Establish a connection with the client failed.\n");
        free(ev->readEvent);
        return;
    }
    printf("Establish a connection with the client success.\n");
    event_set(ev->readEvent, newFd, EV_READ | EV_PERSIST, ReadEvent, ev);
    ret = event_base_set(g_base, ev->readEvent);
    if (ret != 0) {
        printf("Event base set failed.\n");
        free(ev->readEvent);
        return;
    }
    ret = event_add(ev->readEvent, NULL);
    if (ret == -1) {
        printf("Event add failed.\n");
    }
    return;
}

VOID DemoTaskEntry(VOID)
{
    printf("Libevents demo task star to run.\n");
    struct sockaddr_in myAddr;
    INT32 ret;
    INT32 sock;
    INT32 flag = 1;
    struct SocketEvent *ev = (struct SocketEvent *)malloc(sizeof(struct SocketEvent));
    if (ev == NULL) {
        return;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return;
    }
    (VOID)setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(INT32));
    ret = memset_s(&myAddr, sizeof(myAddr), 0, sizeof(myAddr));
    if (ret != EOK) {
        (VOID)close(sock);
        return;
    }
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(PORT);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    (VOID)bind(sock, (struct sockaddr*)&myAddr, sizeof(struct sockaddr));
    (VOID)listen(sock, LISTEN);

    struct event listenEvent;
    g_base = event_base_new();
    event_set(&listenEvent, sock, EV_READ | EV_PERSIST, AcceptEvent, ev);
    ret = event_base_set(g_base, &listenEvent);
    if (ret != EOK) {
        printf("Event base set failed.\n");
        FreeSocketEvent(ev);
        return;
    }
    ret = event_add(&listenEvent, NULL);
    if (ret == -1) {
        printf("Event add failed.\n");
        FreeSocketEvent(ev);
        return;
    }
    ret = event_base_dispatch(g_base);
    if (ret == -1) {
        printf("Event base dispatch failed.\n");
    }
    FreeSocketEvent(ev);
    printf("Libevents demo task finished.\n");
}

VOID LibeventDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam = {0};

    taskInitParam.usTaskPrio = LIBEVENT_TASK_PRIORITY;
    taskInitParam.pcName = "LibeventDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = LIBEVENT_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create libevent demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
