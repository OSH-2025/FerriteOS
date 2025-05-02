/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: pcap dump implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-20
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <netif/etharp.h>
#include <pcap.h>
#include <errno.h>

#ifdef LOSCFG_SHELL
#include "shcmd.h"

#define TYPE_VAL_ARGS 2

#define PCAP_TIMEOUT_MS 1000
#define PCAP_TASK_STACK_SIZE 0x2000
#define PCAP_TASK_PRIORITY 6

STATIC UINT32 g_dumpTskHandle;
STATIC pcap_t *g_pcapHandle = NULL;        /* Session handle */

typedef struct {
    CHAR *filterExp; /* The filter expression */
    CHAR *fileName;  /* Name of the packet capture file */
    CHAR *netDev;    /* Device to sniff on */
    INT32 pCount;           /* Total number of packet capture times */
    INT32 promiscuous;
} InputCmdParam;


STATIC VOID PrintHelp(VOID)
{
    printf("USAGE:\n \
        tcpdump -i ifname -w \"path\" [-c \"package-count\"] [\"filter expression\"]\n \
        example1: tcpdump -i eth0 -w /ramfs/cap -c 15\n \
        example2: tcpdump -i eth0 -w /ramfs/cap -c 15 \"arp or ip\"\n \
        example3: tcpdump stop\n");
}

STATIC INT32 CheckParam(INT32 argc, const CHAR **argv, InputCmdParam *param)
{
    INT32 i = 0;
    INT32 ret;
    while (i < argc) {
        ret = strcmp(argv[i], "-h");
        if (ret == 0) {
            PrintHelp();
            return -1;
        }
        // Don't put the interface into promiscuous mode.
        ret = strcmp(argv[i], "-p");
        if (ret == 0) {
            printf("Promiscuous mode off.\n");
            i += 1; /* Index of the next parameter */
            param->promiscuous = 0;
            continue;
        } else {
            param->promiscuous = 1;
        }
        if ((strcmp(argv[i], "-c") == 0) && ((i + 1) < argc)) {
            param->pCount = atoi(argv[i + 1]);
            printf("Count: %d.\n", param->pCount);
            i += TYPE_VAL_ARGS; /* Index of the next parameter */
            continue;
        }
        if ((strcmp(argv[i], "-i") == 0) && ((i + 1) < argc)) {
            param->netDev = (INT8 *)argv[i + 1];
            printf("Interface: %s.\n", param->netDev);
            i += TYPE_VAL_ARGS; /* Index of the next parameter */
            continue;
        }
        if ((strcmp(argv[i], "-w") == 0) && ((i + 1) < argc)) {
            param->fileName = (INT8 *)argv[i + 1];
            printf("File Name: %s .\n", param->fileName);
            i += TYPE_VAL_ARGS; /* Index of the next parameter */
            continue;
        }
        if ((i + 1) == argc) {
            param->filterExp = (CHAR *)argv[i];
            printf("Filter: %s.\n", param->filterExp);
            break;
        }
        PrintHelp();
        return -1;
    }
    return 0;
}

STATIC VOID PcapClose(pcap_t **pcapHandle)
{
    pcap_close(*pcapHandle);
    *pcapHandle = NULL;
}

STATIC VOID PcapClean(pcap_dumper_t **pd, pcap_t **pcapHandle)
{
    pcap_dump_close(*pd);
    PcapClose(pcapHandle);
}

STATIC INT32 DumpMain(INT32 argc, const CHAR **argv)
{
    const bpf_u_int32 netmask = 0; /* The netmask of our sniffing device */
    pcap_dumper_t *pd = NULL;
    struct bpf_program fp; /* The compiled filter expression */
    CHAR errBuf[PCAP_ERRBUF_SIZE]; /* Error string */
    INT32 ret;
    InputCmdParam param;
    if (memset_s(&param, sizeof(InputCmdParam), 0, sizeof(InputCmdParam)) != EOK) {
        return -1;
    }
    if (CheckParam(argc, argv, &param) < 0) {
        return -1;
    }
    if ((param.fileName == NULL) || (param.netDev == NULL)) {
        printf("Incorrect input. For details, see the shell command \"tcpdump -h\".\n");
        return -1;
    }
    g_pcapHandle = pcap_open_live(param.netDev, BUFSIZ, param.promiscuous, PCAP_TIMEOUT_MS, errBuf);
    if (g_pcapHandle == NULL) {
        printf("Couldn't open device %s: %s.\n",  param.netDev, errBuf);
        return -1;
    }
    if (pcap_compile(g_pcapHandle, &fp, param.filterExp, 0, netmask) == -1) {
        printf("Couldn't parse filter %s: %s.\n", param.filterExp, pcap_geterr(g_pcapHandle));
        PcapClose(&g_pcapHandle);
        return -1;
    }
    if (pcap_setfilter(g_pcapHandle, &fp) == -1) {
        printf("Couldn't install filter %s: %s.\n",  param.filterExp, pcap_geterr(g_pcapHandle));
        PcapClose(&g_pcapHandle);
        return -1;
    }
    if ((pd = pcap_dump_open(g_pcapHandle, param.fileName)) == NULL) {
        /*
         * Print out error message if pcap_dump_open failed. This will
         * be the below message followed by the pcap library error text,
         * obtained by pcap_geterr().
         */
        printf("Error opening savefile \"%s\" for writing: %s.\n", param.fileName, pcap_geterr(g_pcapHandle));
        PcapClose(&g_pcapHandle);
        return -1;
    }
    if ((ret = pcap_loop(g_pcapHandle, param.pCount, (pcap_handler)pcap_dump, (CHAR *)pd)) < 0) {
        PcapClean(&pd, &g_pcapHandle);
        if (ret == PCAP_ERROR_BREAK) {
            printf("Tcpdump stoped.\n");
        }
        return -1;
    }
    PcapClean(&pd, &g_pcapHandle);
    printf("Tcpdump file saved.\n");
    return 0;
}

STATIC VOID DumpTask(const VOID *p)
{
    INT32 i;
    INT32 argc = *(UINT32 *)p;
    CHAR **argv = (CHAR **)(UINTPTR)p + 1;

    (VOID)DumpMain(argc, (const CHAR **)argv);
    for (i = 0; i < argc; i++) {
        free(argv[i]);
        argv[i] = NULL;
    }
    free((UINT32*)(UINTPTR)p);
    p = NULL;
}

INT32 ShellCmdTcpDump(UINT32 argc, const CHAR **argv)
{
    UINT32 ret;
    UINT32 *buf = NULL;
    CHAR **argsBuf = NULL;
    TSK_INIT_PARAM_S taskInitParam;

    if (argv == NULL) {
        return -1;
    }
    ret = (UINT32)memset_s(&taskInitParam, sizeof(taskInitParam), 0, sizeof(taskInitParam));
    if (ret != EOK) {
        return -1;
    }
    if (argc == 0) {
        PrintHelp();
        return 0;
    }
    if ((argc == 1) && (strcmp(argv[0], "stop") == 0)) {
        if (g_pcapHandle != NULL) {
            pcap_breakloop(g_pcapHandle);
        }
        return 0;
    }
    if (g_pcapHandle != NULL) {
        printf("Tcpdump is running.\n");
        return 0;
    }

    buf = (UINT32*)zalloc(sizeof(UINT32) + (argc) * sizeof(CHAR**));
    if (buf == NULL) {
        return -1;
    }
    buf[0] = argc;
    argsBuf = (CHAR**)(buf + 1);
    UINT32 i;
    for (i = 0; i < argc; i++) {
        argsBuf[i] = strdup(argv[i]);
    }

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DumpTask;
    taskInitParam.uwStackSize = PCAP_TASK_STACK_SIZE;
    taskInitParam.pcName = "TcpDump";
    taskInitParam.usTaskPrio = PCAP_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(taskInitParam, buf);
    ret = LOS_TaskCreate(&g_dumpTskHandle, &taskInitParam);
    return (INT32)ret;
}

SHELLCMD_ENTRY(lwip_dump_shellcmd, CMD_TYPE_EX, "tcpdump", 0, (CMD_CBK_FUNC)ShellCmdTcpDump);

#endif /* LWIP_SHELL */

