/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ping Shell Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-20
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

#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/raw.h"
#include "lwip/netdb.h"

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif /* LOSCFG_SHELL */

#define PING_COUNT_DEFAULT  4
#define PING_ID_DEFAULT     0xAFAF

#define PING_DELAY_DEFAULT  1000
#define NS_PER_MS           1000000
#define MS_PER_SECOND       1000

#define ARG_PARA_TWO         2
#define PING_TASK_STACK_SIZE 0x2000
#define PING_TAST_PRIORITY   6
#define LWIP_SHELL_CMD_PING_TIMEOUT 2000

STATIC UINT32 g_pingTskHandle;
STATIC UINT32 PingCmd(UINT32 argc, const CHAR **argv);
STATIC UINT16 g_pingSeqNum = 0;

STATIC VOID PingTask(const VOID *p)
{
    UINT32 i;
    UINT32 ret;
    UINT32 argc = *(UINT32 *)p;
    CHAR **argv = (CHAR **)(UINTPTR)p + 1;

    ret = PingCmd(argc, (const char **)argv);
    if (ret != LOS_OK) {
        printf("Os shell ping failed.\n");
    }
    for (i = 0; i < argc; i++) {
        free(argv[i]);
        argv[i] = NULL;
    }
    free((UINT32 *)(UINTPTR)p);
    p = NULL;
}

UINT32 ShellCmdPing(UINT32 argc, const CHAR **argv)
{
    UINT32 ret, i;
    UINT32 *buf = NULL;
    CHAR **argsBuf = NULL;
    TSK_INIT_PARAM_S taskInitParam = {0};

    buf = (UINT32 *)zalloc(sizeof(UINT32) + (argc) * sizeof(CHAR **));
    if (buf == NULL) {
        return 1;
    }
    if (argv == NULL) {
        return 1;
    }
    buf[0] = argc;
    argsBuf = (CHAR **)(buf + 1);
    for (i = 0; i < argc; i++) {
        argsBuf[i] = strdup(argv[i]);
    }

    (VOID)memset_s(&taskInitParam, sizeof(taskInitParam), 0, sizeof(taskInitParam));
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)PingTask;
    taskInitParam.uwStackSize = PING_TASK_STACK_SIZE;
    taskInitParam.pcName = "ping";
    taskInitParam.usTaskPrio = PING_TAST_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, buf);
    ret = LOS_TaskCreate(&g_pingTskHandle, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create ping task failed.\n");
    }
    return ret;
}

STATIC UINT32 PingCmd(UINT32 argc, const CHAR **argv)
{
    int fd = -1;
    int ret, i;
    struct sockaddr_in dstAddr;
    struct icmp_echo_hdr iecho;
    struct pbuf *pbufResp = NULL;
    struct icmp_echo_hdr *iechoResp = NULL;
    struct ip_hdr *iphdrResp = NULL;
    s16_t ipHeaderLen;
    fd_set fdReadSet;
    struct timeval timeVal;
    struct timespec start, end;
    long rtt;
    s32_t pingCount;
    char buf[IP4ADDR_STRLEN_MAX];

    if ((argc < 1) || (*argv == NULL)) {
        printf("Ping : invalid arguments, enter the right command on the command line.\n");
        return LOS_NOK;
    }
    if (argc == ARG_PARA_TWO) {
        pingCount = atoi(argv[1]);
        if (pingCount <= PING_COUNT_DEFAULT) {
            pingCount = PING_COUNT_DEFAULT;
        }
    } else {
        pingCount = PING_COUNT_DEFAULT;
    }
    printf("Ping %d packets start.\n", pingCount);

    dstAddr.sin_family = AF_INET;
    dstAddr.sin_addr.s_addr = inet_addr(argv[0]);
    dstAddr.sin_port = 0;

    if (dstAddr.sin_addr.s_addr == IPADDR_NONE || dstAddr.sin_addr.s_addr == IPADDR_ANY) {
        printf("Ping : invalid ip address : %s.\n", argv[0]);
        return LOS_NOK;
    }

    fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (fd == -1) {
        printf("Ping : socket create failed.\n");
        return LOS_NOK;
    }

    pbufResp = pbuf_alloc(PBUF_RAW, IP_HLEN + sizeof(struct icmp_echo_hdr), PBUF_RAM);
    if (pbufResp == NULL) {
        printf("Ping : pbuf memory allocation failed.\n");
        (void)close(fd);
        g_pingSeqNum = 0;
        return LOS_NOK;
    }
    for (i = 0; i < pingCount; i++) {
        (void)LOS_TaskDelay(PING_DELAY_DEFAULT);
        (void)memset_s(&iecho, sizeof(iecho), 0, sizeof(iecho));
        ICMPH_TYPE_SET(&iecho, (u8_t)ICMP_ECHO);
        ICMPH_CODE_SET(&iecho, 0);
        iecho.chksum = 0;
        iecho.id = PING_ID_DEFAULT;
        iecho.seqno = htons(++g_pingSeqNum);
        iecho.chksum = inet_chksum(&iecho, sizeof(struct icmp_echo_hdr));

        ret = sendto(
            fd, &iecho, sizeof(struct icmp_echo_hdr), 0, (struct sockaddr *)&dstAddr, (socklen_t)sizeof(dstAddr));
        if (ret == -1) {
            printf("Ping : sending ICMP echo msg failed.\n");
            (void)close(fd);
            if (pbufResp != NULL) {
                (void)pbuf_free(pbufResp);
            }
            g_pingSeqNum = 0;
            return LOS_NOK;
        }

        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        /* Wait in select for ICMP response msg */
        FD_ZERO(&fdReadSet);
        FD_SET(fd, &fdReadSet);
        timeVal.tv_sec = LWIP_SHELL_CMD_PING_TIMEOUT / MS_PER_SECOND;
        timeVal.tv_usec = 0;

        ret = select(fd + 1, &fdReadSet, 0, 0, &timeVal);
        if (ret < 0) {
            printf("Ping : select failure.\n");
        } else if (ret == 0) {
            printf("Ping : request timed out.\n");
            continue;
        }

        (void)clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        rtt = ((end.tv_sec - start.tv_sec) * MS_PER_SECOND + (end.tv_nsec - start.tv_nsec) / NS_PER_MS);

        if (pbufResp->payload == NULL) {
            return LOS_NOK;
        }
        ret = recv(fd, pbufResp->payload, pbufResp->len, 0);
        if (ret == -1) {
            printf("Ping : receiving ICMP echo response msg failed.\n");
            (void)close(fd);
            if (pbufResp != NULL) {
                (void)pbuf_free(pbufResp);
            }
            g_pingSeqNum = 0;
            return LOS_NOK;
        }

        iphdrResp = (struct ip_hdr *)(pbufResp->payload);
        ipHeaderLen = (s16_t)IPH_HL_BYTES(iphdrResp);
        if (pbuf_header(pbufResp, (s16_t)(-ipHeaderLen))) {
            printf("Ping : memory management failure.\n");
            (void)close(fd);
            if (pbufResp != NULL) {
                (void)pbuf_free(pbufResp);
            }
            g_pingSeqNum = 0;
            return LOS_NOK;
        }
        iechoResp = (struct icmp_echo_hdr *)pbufResp->payload;

        if (pbuf_header(pbufResp, ipHeaderLen)) {
            printf("Ping : memory management failure.\n");
            (void)close(fd);
            if (pbufResp != NULL) {
                (void)pbuf_free(pbufResp);
            }
            g_pingSeqNum = 0;
            return LOS_NOK;
        }

        if (iphdrResp->src.addr == dstAddr.sin_addr.s_addr) {
            if ((ICMPH_TYPE(iechoResp) == ICMP_ER) || (ICMPH_TYPE(iechoResp) == ICMP_ECHO)) {
                printf("[%d]Reply from %s: time=%ldms TTL=%d.\n",
                    i,
                    inet_ntoa_r(dstAddr.sin_addr.s_addr, buf, (int)sizeof(buf)),
                    rtt,
                    iphdrResp->_ttl);
            }
        }
    }
    (void)close(fd);
    (void)pbuf_free(pbufResp);
    g_pingSeqNum = 0;
    return LOS_OK;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(ping_shellcmd, CMD_TYPE_EX, "ping", XARGS, (CmdCallBackFunc)ShellCmdPing);
#endif /* LOSCFG_SHELL */
