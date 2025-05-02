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
#include "print.h"
#include "lwip/netif.h"

#define DELAY_TIME 1000
#define PORT 8080
#define UDPCLIENT_TASK_PRIORITY 11
#define UDPCLIENT_TASK_STACK_SIZE 0x1000

static struct sockaddr_nin *g_addr = NULL;
static int g_num = 1;
static UINT32 g_udpClientTskHandle;

VOID Delay(__IO uint32_t nCount)
{
    UINT32 tick;
    tick = LOS_MS2Tick(nCount);
    LOS_TaskDelay(tick);

    int sockfd = socket(AF_NIP, SOCK_DGRAM, 0);
    g_addr->snin_family = AF_NIP;
    g_addr->snin_len = 104;
    g_addr->snin_port = htons(PORT);

    socklen_t slen = sizeof(*g_addr);
    char buf[100] = {0}; // 100, buffer size.
    char buf2[400] = ""; // 400, buffer size.
    sprintf(buf, "%d:from client", g_num++);

    int senderr = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)g_addr, slen);
    if (senderr >= 0) {
        if (g_printOpen) {
            printf("newip_src_address:");
            nip_addr_debug_print_val(0x80U, netif_default->nip_addr);
            printf(" newip_dst_address:");
            nip_addr_debug_print_val(0x80U, (*g_addr).snin_addr);
            printf("\r\npayload_info:%s\n", buf);
        }
    } else {
        printf("Send failed!\n");
    }


    recvfrom(sockfd, buf2, sizeof(buf2), 0, (struct sockaddr*)g_addr, &slen);
    if (g_printOpen) {
        printf("newip_src_address:");
        nip_addr_debug_print_val(0x80U, (*g_addr).snin_addr);
        printf(" newip_dst_address:");
        nip_addr_debug_print_val(0x80U, netif_default->nip_addr);
        printf("\r\npayload_info:%s\n", buf2);
    }
    close(sockfd);
}

VOID LED_GPIO_Config(VOID)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    GPIO_InitStructure.Pin = LED1_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    LED_RGBOFF;
}

static UINT32 UdpClientDemoEntry(VOID)
{
    LED_GPIO_Config();
    printf("udpclient\n");
    while (1) {
        LED1(ON);
        Delay(DELAY_TIME);

        LED1(OFF);
        Delay(DELAY_TIME);

        LED2(ON);
        Delay(DELAY_TIME);
        LED2(OFF);
        Delay(DELAY_TIME);

        LED3(ON);
        Delay(DELAY_TIME);
        LED3(OFF);
        Delay(DELAY_TIME);

        LED_RED;
        Delay(DELAY_TIME);

        LED_GREEN;
        Delay(DELAY_TIME);

        LED_BLUE;
        Delay(DELAY_TIME);

        LED_YELLOW;
        Delay(DELAY_TIME);

        LED_PURPLE;
        Delay(DELAY_TIME);

        LED_CYAN;
        Delay(DELAY_TIME);

        LED_WHITE;
        Delay(DELAY_TIME);

        LED_RGBOFF;
        Delay(DELAY_TIME);
    }
    return 0;
}

static UINT32 CreateUdpClientTask(VOID)
{
    UINT32 ret = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    (VOID)memset_s(&task_init_param, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    task_init_param.usTaskPrio = UDPCLIENT_TASK_PRIORITY;
    task_init_param.pcName = "udpclient_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)UdpClientDemoEntry;
    task_init_param.uwStackSize = UDPCLIENT_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_udpClientTskHandle, &task_init_param);
    if (ret != LOS_OK) {
        return ret;
    }
    return ret;
}

int CmdUdpClient(int argc, const char **argv)
{
    UINT32 ret = LOS_OK;
    struct nip_addr *naddr = NULL;
    if (argc != 2) {
        printf("Wrong number of parameters.");
        return -1;
    }
    if (strcmp(argv[0], "close") == 0) {
        if ((strcmp(argv[1], "0") == 0) || (atoi(argv[1]) > 0)) {
            close(atoi(argv[1]));
            return 0;
        } else {
            printf("No socket.");
            return -1;
        }

    }
    g_addr = malloc(sizeof(*g_addr));
    naddr = malloc(sizeof(*naddr));
    nip_addr_set_zero(naddr);
    int aton_err = nipaddr_aton2(argv[0], argv[1], naddr);
    if (aton_err < 0) {
        printf("aton_err:%d\n", aton_err);
        return -1;
    }
    nip_addr_copy(g_addr->snin_addr, *naddr);

    ret = CreateUdpClientTask();
    if (ret != LOS_OK) {
        PRINT_ERR("Udp client task create fail.\n");
    }
    return (int)ret;
}
#endif /* LOSCFG_LWIP_NIP */
