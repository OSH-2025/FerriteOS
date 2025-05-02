/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Ipv6 Client Demo
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

#if !defined(LOSCFG_COMPONENTS_NET_LWIP) || !defined(LOSCFG_LWIP_IPV6)
#error This demo needs to enable Components "Components-->Network-->Enable Network-->Enable Lwip-->Enable Ipv6"
#endif

#include "lwip/sockets.h"
#include "los_task.h"
#include "los_base.h"
#include "los_sys.h"
#include "client_demo.h"

#define SERVER_ADDR             "2000::4639:c4ff:fe94:5d44"
#define SERVER_PORT             1883
#define BUFFER_SIZE             128

#define IPV6_TASK_PRIORITY      4
#define IPV6_TASK_STACK_SIZE    0x1000

STATIC UINT32 g_demoTaskId;

#ifdef LOSCFG_DEMOS_IPV6_TCP_CLIENT
STATIC VOID TcpDemoTaskEntry(VOID)
{
    struct sockaddr_in6 server_addr;
    char msg[128];
    int rbytes = -1;
    int client_fd = 0;
    int ret = 0;
    int i = 0;

    printf("Ipv6 tcp demo task start to run.\n");
DO_TASK:
    do {
        client_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        if (client_fd < 0) {
            printf("Create socket failed.\n");
            return;
        }

        server_addr.sin6_family = AF_INET6;
        server_addr.sin6_port = htons(SERVER_PORT);
        if (inet_pton(AF_INET6, SERVER_ADDR, &server_addr.sin6_addr) <= 0) {
            printf("Execute inet_pton failed.\n");
            return;
        }

        ret = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (ret < 0) {
            close(client_fd);
            LOS_TaskDelay(200);
            if (i > 10) {
                printf("Socket connect failed, try again.\n");
                i++;
                goto DO_TASK;
            }
        }
    } while (ret < 0);

    while ((rbytes = read(client_fd, msg, sizeof(msg))) > 0) {
        printf("Read from server: %d bytes read\n> %s\n", rbytes, msg);
        char wbuf[32] = "Hi ipv6 server";
        rbytes = write(client_fd, wbuf, strlen(wbuf));
        if (rbytes > 0) {
            printf("write to server: %d bytes written\n> %s\n", rbytes, wbuf);
        }
    }
    printf("Ipv6 tcp demo task finished.\n");
    close(client_fd);
}
#endif

#ifdef LOSCFG_DEMOS_IPV6_UDP_CLIENT
STATIC VOID UdpDemoTaskEntry(VOID)
{
    struct sockaddr_in6 server_addr;
    char msg[128] = "Hi ipv6 server";
    int rbytes = -1;

    printf("Ipv6 udp demo task start to run.\n");
    int client_fd = socket(AF_INET6, SOCK_DGRAM, 17);
    if (client_fd < 0) {
        printf("Create socket failed.\n");
        return;
    }

    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, SERVER_ADDR, &server_addr.sin6_addr) <= 0) {
        printf("Execute inet_pton failed.\n");
        return;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Socket connect failed.\n");
        return;
    }

    LOS_TaskDelay(1500);
    rbytes = write(client_fd, msg, strlen(msg));
    if (rbytes > 0) {
        printf("Write to server: %d bytes written\n> %s\n", rbytes, msg);
    } else {
        printf("Write faile\n");
    }
    printf("Ipv6 udp demo task finished.\n");
    close(client_fd);
}
#endif

VOID Ipv6DemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = IPV6_TASK_PRIORITY;
    taskInitParam.pcName = "Ipv6DemoTask";
#ifdef LOSCFG_DEMOS_IPV6_TCP_CLIENT
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TcpDemoTaskEntry;
#endif
#ifdef LOSCFG_DEMOS_IPV6_UDP_CLIENT
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)UdpDemoTaskEntry;
#endif
    taskInitParam.uwStackSize = IPV6_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create ipv6 demo task failed.\n");
    }
}
