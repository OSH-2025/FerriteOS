/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Librws Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-14
 * Redistribution and use inputBuf source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions inputBuf binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer inputBuf the documentation and/or other materials
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

#include "librws_demo.h"
#include <string.h>
#include <librws.h>
#include "los_task.h"

#define WEBSOCKET_DEMO_SERVER           "192.168.10.182"
#define WEBSOCKET_DEMO_PORT             8000
#define LIBRWS_TASK_PRIORITY            6
#define LIBRWS_TASK_STACK_SIZE          0x1400
#define LIBRWS_DEMO_WAIT_TIME           20000

STATIC UINT32 g_demoTaskId;
STATIC rws_socket g_socket = NULL;

STATIC VOID OnSocketReceivedText(rws_socket socket, const CHAR *text, const UINT32 length)
{
    (VOID)socket;
    INT32 ret;
    CHAR *buff = NULL;
    if ((text == NULL) || (length == 0)) {
        printf("Websocket text receive failed.\n");
        return;
    }
    buff = (CHAR *)malloc(length);
    if (buff == NULL) {
        printf("Websocket text receive failed.\n");
        return;
    }
    ret = memcpy_s(buff, length, text, length);
    if (ret != EOK) {
        free(buff);
        printf("Websocket text receive failed.\n");
        return;
    }

    printf("Websocket receive len:%d text:\n%s\n", length, text);
    free(buff);
    buff = NULL;
}

STATIC VOID OnSocketReceivedBin(rws_socket socket, const VOID *data, const UINT32 length)
{
    (VOID)socket;
    INT32 ret;
    CHAR *buff = NULL;
    if ((data == NULL) || (length == 0)) {
        printf("Websocket bin receive failed.\n");
        return;
    }
    buff = (CHAR *)malloc(length);
    if (buff == NULL) {
        printf("Websocket bin receive failed.\n");
        return;
    }
    ret = memcpy_s(buff, length, data, length);
    if (ret != EOK) {
        printf("Websocket bin receive failed.\n");
        free(buff);
        return;
    }

    printf("Websocket receive bin:\n<%s>\n", buff);
    free(buff);
    buff = NULL;
}

STATIC VOID OnSocketConnected(rws_socket socket)
{
    const CHAR *demoText = "LiteOS Websocket demo data";
    (VOID)rws_socket_send_text(socket, demoText);
    printf("\nWebsocket connected\n");
}

STATIC VOID OnSocketDisconnected(rws_socket socket)
{
    rws_error error = rws_socket_get_error(socket);
    if (error) {
        printf("Websocket disconnect with code, error: %i, %s\n",
               rws_error_get_code(error),
               rws_error_get_description(error));
    }
}

STATIC VOID WebsocketConnect(VOID)
{
    g_socket = rws_socket_create();
    rws_socket_set_scheme(g_socket, "ws");
    rws_socket_set_host(g_socket, WEBSOCKET_DEMO_SERVER);
    rws_socket_set_path(g_socket, "/");
    rws_socket_set_port(g_socket, WEBSOCKET_DEMO_PORT);

    rws_socket_set_on_disconnected(g_socket, OnSocketDisconnected);
    rws_socket_set_on_connected(g_socket, OnSocketConnected);
    rws_socket_set_on_received_text(g_socket, OnSocketReceivedText);
    rws_socket_set_on_received_bin(g_socket, OnSocketReceivedBin);

    (VOID)rws_socket_connect(g_socket);
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Librws demo start to run.\n");
    (VOID)LOS_TaskDelay(LIBRWS_DEMO_WAIT_TIME);    // wait lwip dhcp get ip.
    WebsocketConnect();
}

VOID LibrwsDemo(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = LIBRWS_TASK_STACK_SIZE;
    taskInitParam.pcName = "LibrwsTask";
    taskInitParam.usTaskPrio = LIBRWS_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create librws demo task failed.\n");
    }
}