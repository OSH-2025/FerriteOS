/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: NewIP Addres Command Implementation
 * Author: Huawei LiteOS Team
 * Create: 2022-02-17
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

#if LOSCFG_LWIP_NIP
#include "shell.h"
#include "shcmd.h"
#include "stm32f4xx.h"

#include "los_sys.h"
#include "los_task.h"
#include "errno.h"

#include "lwip/sockets.h"
#include "lwip/inet.h"

#include "lwip/nip_addr.h"
#include "bsp_led.h"

#include "lwip/netif.h"
#include "print.h"

#define PORT 8080
#define UDPSERVER_TASK_PRIORITY 11
#define UDPSERVER_TASK_STACK_SIZE 0x1000

static UINT32 g_udpServerTskHandle;
static struct sockaddr_nin *g_addr = NULL;


UINT32 udpserver_demo_entry(VOID)
{
    int ret;
    int sockfd = -1;
    g_addr = malloc(sizeof(struct sockaddr_nin));
    nip_addr_copy(g_addr->snin_addr, netif_default->nip_addr);
    printf("udpserver\n");
    sockfd = socket(AF_NIP, SOCK_DGRAM, 0);
    printf("socket:%d\n", sockfd);

    g_addr->snin_len = 104; // 104, snin length.
    g_addr->snin_port = htons(PORT);
    g_addr->snin_family = AF_NIP;

    ret = bind(sockfd, (struct sockaddr *)g_addr, sizeof(struct sockaddr_nin));
    if (ret < 0) {
        printf("bind failed.\n");
        printf("%d\n", errno);
    }
    struct sockaddr_nin cli;
    socklen_t len = sizeof(cli);
    (VOID)memset_s(&cli, len, 0, len);
    nip_addr_set_zero(&cli.snin_addr);

    while (1) {
        char buf[100] = ""; // 100, buffer size.
        char buf2[100] = "from server"; // 100, buffer size.
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cli, &len);
        if (g_printOpen) {
            printf("newip_src_address:");
            nip_addr_debug_print_val(0x80U, cli.snin_addr);
            printf(" newip_dst_address:");
            nip_addr_debug_print_val(0x80U, netif_default->nip_addr);
            printf("\r\npayload_info:%s\n", buf);
        }
        int length = sendto(sockfd, buf2, sizeof(buf2), 0, (struct sockaddr *)&cli, len);
        if (length >= 0) {
            if (g_printOpen) {
                printf("newip_src_address:");
                nip_addr_debug_print_val(0x80U, netif_default->nip_addr);
                printf(" newip_dst_address:");
                nip_addr_debug_print_val(0x80U, cli.snin_addr);
                printf("\r\npayload_info:%s\n", buf2);
            }
        } else {
            printf("Send failed!\n");
        }
    }
    close(sockfd);
    return 0;
}

static UINT32 CreateUdpServerTask(VOID)
{
    UINT32 ret = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    (VOID)memset_s(&task_init_param, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    task_init_param.usTaskPrio = UDPSERVER_TASK_PRIORITY;
    task_init_param.pcName = "udpserver_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)udpserver_demo_entry;
    task_init_param.uwStackSize = UDPSERVER_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_udpServerTskHandle, &task_init_param);

    return ret;
}

int CmdUdpserver(int argc, const char **argv)
{
    UINT32 ret = LOS_OK;
    if (argc == 2) {
        if (strcmp(argv[0], "close") == 0) {
            if (atoi(argv[1]) < 0) {
                printf("No socket.\n");
                return -1;
            }
            close(atoi(argv[1]));
            return 0;
        } else {
            printf("No command.\n");
            return -1;
        }
    } else if (argc != 0) {
        printf("Wrong number of parameters.\n");
        return -1;
    }

    ret = CreateUdpServerTask();
    if (ret != LOS_OK) {
        PRINT_ERR("Udp server task create fail.\n");
    }
    return ret;
}
#endif /* LOSCFG_LWIP_NIP */
