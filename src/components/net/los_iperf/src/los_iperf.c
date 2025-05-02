/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: implementation for iperf2.
 * Author: Huawei LiteOS Team
 * Create: 2020-02-26
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
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <securec.h>
#include "endian.h"
#include "los_task.h"
#include "los_mux.h"
#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef CUSTOM_AT_COMMAND
#define IPERF_PRINT(fmt, args...) hi_at_printf(fmt, ##args)
#else
#ifdef LOSCFG_KERNEL_SMP
#define IPERF_PRINT(fmt, args...) dprintf(fmt, ##args)
#else
#define IPERF_PRINT(fmt, args...) printf(fmt, ##args)
#endif
#endif

#define IPERF_OK   0
#define IPERF_FAIL (-1)
#define IPERF_INVALID_SOCKET (-1)

#define IPERF_DEFAULT_PORT 5001
#define IPERF_DEFAULT_INTERVAL 1
#define IPERF_DEFAULT_TIME 30
#define IPERF_DEFAULT_MAX_TIME (86400 * 100) /* 100 Days */
#define IPERF_DEFAULT_UDP_BW (1024 * 1024) /* 1Mbps */
#define IPERF_DEFAULT_UDP_BUFLEN   1470
#define IPERF_DEFAULT_TCP_BUFLEN   0x2000 /* 8K */
#define IPERF_DEFAULT_RX_TIMEOUT 10 /* 10sec */
#define IPERF_DEFAULT_TX_TIMEOUT 10 /* 10sec */

#define IPERF_MASK_CLIENT (1U << 0)
#define IPERF_MASK_SERVER (1U << 1)
#define IPERF_MASK_UDP    (1U << 2)
#define IPERF_MASK_TCP    (1U << 3)
#define IPERF_MASK_IP_V6   (1U << 4)

#define IPERF_TRAFFIC_NAME "iperf_traffic"
#define IPERF_TRAFFIC_CLIENT_PRIORITY 20
#define IPERF_TRAFFIC_SERVER_PRIORITY 3
#define IPERF_TRAFFIC_STACK_SIZE 0x1000 /* Must be greater than or equals 4K */
#define IPERF_REPORTER_NAME "iperf_reporter"
#define IPERF_REPORTER_PRIORITY 2
#ifdef LOSCFG_KERNEL_SMP
#define IPERF_REPORTER_STACK_SIZE 0x1000 /* Must be greater than or equals 4K */
#else
#define IPERF_REPORTER_STACK_SIZE 0x800 /* Must be greater than or equals 2K */
#endif

#define IPERF_HEADER_VERSION_V1 0x80000000
#define IPERF_THOUSAND  1000       /* 1000 */
#define IPERF_MILLION   1000000    /* 1000 * 1000 */
#define IPERF_BILLION   1000000000 /* 1000 * 1000 * 1000 */
#define IPERF_KILO      0x400      /* 1024 */
#define IPERF_MEGA      0x100000   /* 1024 * 1024 */
#define IPERF_GIGA      0x40000000 /* 1024 * 1024 * 1024 */
#define IPERF_BITS_PER_BYTE 8
#define IPERF_MAX_DELTA 2000
#define IPERF_AMOUNT_1 0x80000000
#define IPERF_AMOUNT_2 0x7FFFFFFF

#define IPERF_UDP_FIN_MAX_RETRIES 10 /* UDP FIN or FINACK max retries */
#define IPERF_UDP_DELAY (IPERF_MILLION / LOSCFG_BASE_CORE_TICK_PER_SECOND)
#define IPERF_UDP_FIN_TMO 250000 /* 250ms: select timeout for UDP FIN */
#define IPERF_UDP_FINACK_TMO 1 /* 1s: select timeout for UDP FINACK */

#define IPERF_IS_SERVER(mask) (((mask) & IPERF_MASK_SERVER) == IPERF_MASK_SERVER)
#define IPERF_IS_CLIENT(mask) (((mask) & IPERF_MASK_CLIENT) == IPERF_MASK_CLIENT)
#define IPERF_IS_UDP(mask)    (((mask) & IPERF_MASK_UDP)    == IPERF_MASK_UDP)
#define IPERF_IS_TCP(mask)    (((mask) & IPERF_MASK_TCP)    == IPERF_MASK_TCP)
#define IPERF_IS_IP_V6(mask)  (((mask) & IPERF_MASK_IP_V6)  == IPERF_MASK_IP_V6)

#define IPERF_MAX_STR_LEN 32

typedef struct {
    union {
        struct in_addr srcIP4;
        struct in6_addr srcIP6;
    } u1;
    union {
        struct in_addr dstIP4;
        struct in6_addr dstIP6;
    } u2;
    UINT16 port;
    UINT8  mask;
    UINT8  tos;
    UINT32 interval;
    UINT32 time;
    UINT64 bandwidth;
    UINT64 amount;
    UINT64 total;
    UINT8 *buffer;
    UINT32 bufLen;
} IperfParams;

typedef struct {
    INT32 id;
    UINT32 sec;
    UINT32 usec;
} IperfUdpHdr;

/* from iperf opensource, preclude some additional elements */
typedef struct {
    INT32 flags;
    INT32 totalLenH;
    INT32 totalLenL;
    INT32 endSec;
    INT32 endUsec;
    INT32 errCnt;
    INT32 oooCnt;
    INT32 datagrams;
    INT32 jitterSec;
    INT32 jitterUsec;
} IperfUdpServerHdr;

typedef struct {
    INT32 flags;
    INT32 numThreads;
    INT32 port;
    INT32 bufLen;
    INT32 winBand;
    INT32 amount;
} IperfUdpClientHdr;

#ifdef LOSCFG_NET_IPERF_JITTER
typedef struct {
    BOOL     finish;
    UINT64 totalLen;
    INT32  lastID;
    INT32  errCnt;
    INT32  oooCnt;
    INT32  lastErrCnt;
    INT32  lastOooCnt;
    INT32  lastDatagrams;
    DOUBLE   lastTransit;
    DOUBLE   jitter;
    struct timeval firstPktTime; /* recv time of first datagram */
    struct timeval lastPktTime;  /* recv time of last datagram */
} IperfUdpStat;
#endif

typedef struct {
    BOOL isRunning;
    BOOL isFinish; /* iperf stopped normally */
    BOOL isKilled; /* iperf killed by user */
    INT32 listenSock;
    INT32 trafficSock;
    INT32 udpPktID;
    UINT32 mutex;
    struct timeval start;
    struct timeval end;
    IperfParams param;
#ifdef LOSCFG_NET_IPERF_JITTER
    IperfUdpStat *udpStat;
#endif
} IperfContext;

STATIC IperfContext *g_iperfContext = NULL;
STATIC UINT32 g_iperfMutex;

STATIC VOID SetGlobalIperfContext(IperfContext *iperfContext)
{
    g_iperfContext = iperfContext;
}

STATIC IperfContext* GetGlobalIperfContext(VOID)
{
    return g_iperfContext;
}

STATIC VOID IperfIpv6Init(struct sockaddr_in6 *sock, sa_family_t family, in_port_t port, struct in6_addr addr)
{
    sock->sin6_family = family;
    sock->sin6_port = htons(port);
    sock->sin6_addr = addr;
}

STATIC VOID IperfIpv4Init(struct sockaddr_in *sock, sa_family_t family,  in_port_t port, struct in_addr addr)
{
    sock->sin_family = family;
    sock->sin_port = htons(port);
    sock->sin_addr = addr;
}

#ifdef IPERF_FEED_WDG
STATIC VOID IperfFeedWdg(void)
{
    hi_cpup_load_check_proc(hi_task_get_current_id(), LOAD_SLEEP_TIME_DEFAULT);
}
#endif /* IPERF_FEED_WDG */

STATIC BOOL IsIperfRunning(VOID)
{
    IperfContext *iperfContext = GetGlobalIperfContext();
    if ((iperfContext != NULL) && (iperfContext->isRunning == TRUE)) {
        return TRUE;
    }
    return FALSE;
}

STATIC INT64 TimeUsec(struct timeval end, struct timeval start)
{
    const INT32 maxGap = IPERF_MAX_DELTA; /* 2000: max delta */

    INT32 delta;
    if ((end.tv_sec - start.tv_sec) > maxGap) {
        delta = maxGap;
    } else if ((end.tv_sec - start.tv_sec) < (-maxGap)) {
        delta = -maxGap;
    } else {
        delta = end.tv_sec - start.tv_sec;
    }

    return (delta * IPERF_MILLION) + (end.tv_usec - start.tv_usec);
}

STATIC FLOAT TimeSec(struct timeval end, struct timeval start)
{
    if ((end.tv_sec - start.tv_sec) < (-IPERF_DEFAULT_MAX_TIME)) {
        return -IPERF_DEFAULT_MAX_TIME;
    }
    if ((end.tv_sec - start.tv_sec) > IPERF_DEFAULT_MAX_TIME) {
        return IPERF_DEFAULT_MAX_TIME;
    }
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (FLOAT)IPERF_MILLION;
}

STATIC VOID IperfDelay(UINT32 usecs)
{
    UINT32 ticks = usecs / IPERF_UDP_DELAY;
    if (ticks) {
        LOS_TaskDelay(ticks);
    } else {
        LOS_TaskDelay(1); /* 1: atleast one tick */
    }
}

#ifdef LOSCFG_NET_IPERF_JITTER
STATIC VOID IperfUdpPktCntCal(IperfContext *context, INT32 *err,
                              INT32 *ooo, INT32 *dgrams)
{
    if ((context->udpStat->errCnt < 0) || (context->udpStat->lastErrCnt < 0)) {
        *err = 0;
    } else {
        *err = context->udpStat->errCnt - context->udpStat->lastErrCnt;
    }

    if ((context->udpStat->oooCnt < 0) || (context->udpStat->lastOooCnt < 0)) {
        *ooo = 0;
    } else {
        *ooo = context->udpStat->oooCnt - context->udpStat->lastOooCnt;
    }

    if ((context->udpStat->lastID < 0) || (context->udpStat->lastDatagrams < 0)) {
        *dgrams = 0;
    } else {
        *dgrams = context->udpStat->lastID - context->udpStat->lastDatagrams;
    }
}
#endif

STATIC VOID PrintBandwidth(const IperfContext *context, UINT64 amount, FLOAT start, FLOAT end, BOOL last)
{
    INT32 unit;
    CHAR amountSuffix;
    CHAR rateSuffix;
    FLOAT transfer;
    FLOAT interval = end - start;

    if (amount > IPERF_MEGA) {
        amountSuffix = 'M';
        transfer = amount / (FLOAT)IPERF_MEGA;
    } else {
        amountSuffix = 'K';
        transfer = amount / (FLOAT)IPERF_KILO;
    }

    if (((amount * IPERF_BITS_PER_BYTE) / interval) >= (FLOAT)IPERF_MILLION) {
        unit = IPERF_MILLION; /* Mbps */
        rateSuffix = 'M';
    } else {
        unit = IPERF_THOUSAND; /* Kbps */
        rateSuffix = 'K';
    }

    /* do NOT use double precision, single precision is enough */
    IPERF_PRINT("%4.1f-%4.1f sec    %6.2f %cBytes    %.2f %cbits/sec\n", start, end,
                transfer, amountSuffix, ((FLOAT)amount * IPERF_BITS_PER_BYTE) / interval / (FLOAT)unit, rateSuffix);

#ifdef LOSCFG_NET_IPERF_JITTER
    if (IPERF_IS_UDP(context->param.mask) && IPERF_IS_SERVER(context->param.mask) && (context->udpStat != NULL)) {
        INT32 errCnt, oooCnt, dgrams;
        FLOAT jitter = context->udpStat->jitter;
        if (last) {
            errCnt = context->udpStat->errCnt;
            oooCnt = context->udpStat->oooCnt;
            dgrams = context->udpStat->lastID;
        } else {
            IperfUdpPktCntCal((IperfContext *)context, &errCnt, &oooCnt, &dgrams);
        }
        printf("%%%%%%%%%%%\n");
        IPERF_PRINT("%6.3f ms %4d/%5d (%.2f%%)\r\n",
                    (jitter * 1000.0), errCnt, dgrams, /* 1000.0: micro seconds */
                    (100.0 * errCnt) / (FLOAT)dgrams); /* 100.0: percentage */
        if (oooCnt > 0) {
            IPERF_PRINT("received %d out-of-order datagrams\r\n", oooCnt);
        }
    } else {
        IPERF_PRINT("\r\n");
    }
#else
    IPERF_PRINT("\r\n");
#endif
}

STATIC VOID IperfReporterProcess(IperfContext *context, UINT32 *passed, UINT64 *lastTotal)
{
    PrintBandwidth(context, context->param.total - *lastTotal, *passed,
                   (*passed + context->param.interval), FALSE);

#ifdef LOSCFG_NET_IPERF_JITTER
    if ((IPERF_IS_UDP(context->param.mask) && IPERF_IS_SERVER(context->param.mask)) && (context->udpStat != NULL)) {
        context->udpStat->lastDatagrams = context->udpStat->lastID;
        context->udpStat->lastOooCnt = context->udpStat->oooCnt;
        context->udpStat->lastErrCnt = context->udpStat->errCnt;
    }
#endif
    *passed += context->param.interval;
    *lastTotal = context->param.total;
    if ((context->param.time != 0) && (*passed >= context->param.time)) {
        context->isFinish = TRUE;
    }
}

STATIC VOID IperfReporterEntry(VOID)
{
    IperfContext *context = NULL;
    UINT64 lastTotal = 0;
    UINT32 passed = 0;
    UINT32 count = 0;

    if (LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER) != LOS_OK) {
        return;
    }
    context = GetGlobalIperfContext();
    if (context == NULL) {
        LOS_MuxPost(g_iperfMutex);
        return;
    }

#ifdef LOSCFG_NET_IPERF_JITTER
    IPERF_PRINT("+IPERF:\r\n%-16s    %-16s    %-16s", "Interval", "Transfer", "Bandwidth");
    if (IPERF_IS_UDP(context->param.mask) && IPERF_IS_SERVER(context->param.mask)) {
        IPERF_PRINT("%-8s    %s\r\n", "Jitter", "Lost/Total Datagrams");
    } else {
        IPERF_PRINT("\r\n");
    }
#else
    IPERF_PRINT("+IPERF:\r\n%-16s    %-16s    %-16s\r\n", "Interval", "Transfer", "Bandwidth");
#endif

    while ((context->isFinish == FALSE) && (context->isKilled == FALSE)) {
        LOS_MuxPost(g_iperfMutex);
        LOS_TaskDelay(LOSCFG_BASE_CORE_TICK_PER_SECOND / 5); /* 5: schedule every 200ms */
        if (LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER) != LOS_OK) {
            break;
        }
        context = GetGlobalIperfContext();
        if (context == NULL) {
            LOS_MuxPost(g_iperfMutex);
            break;
        }
        if ((context->isFinish == TRUE) || (context->isKilled == TRUE)) {
            LOS_MuxPost(g_iperfMutex);
            break;
        }
        if ((++count) >= (context->param.interval * 5)) { /* 5: schedule every 200ms */
            count = 0;
            IperfReporterProcess(context, &passed, &lastTotal);
            if (context->isFinish == TRUE) {
                LOS_MuxPost(g_iperfMutex);
                break;
            }
        }
    }
    IPERF_PRINT("Ok.\r\n");
}

STATIC INT32 IperfReporterGo(VOID)
{
    UINT32 ret;
    UINT32 taskID;
    TSK_INIT_PARAM_S appTask;
    ret = memset_s(&appTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }

    appTask.pfnTaskEntry = (TSK_ENTRY_FUNC)IperfReporterEntry;
    appTask.uwStackSize = IPERF_REPORTER_STACK_SIZE;
    appTask.pcName = IPERF_REPORTER_NAME;
    appTask.usTaskPrio = IPERF_REPORTER_PRIORITY;
    appTask.uwResved = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&taskID, &appTask);
    if (ret != 0) {
        IPERF_PRINT("Create reporter task failed.\n");
        return IPERF_FAIL;
    }
    return IPERF_OK;
}

STATIC VOID IperfProcessUdpServerHdr(IperfContext *context, UINT32 len)
{
    if (len > (sizeof(IperfUdpHdr) + sizeof(IperfUdpServerHdr))) {
        IperfUdpServerHdr *hdr = (IperfUdpServerHdr *)(context->param.buffer + sizeof(IperfUdpHdr));
        FLOAT end;
#ifdef LOSCFG_NET_IPERF_JITTER
        FLOAT jitter;
        INT32 errCnt, oooCnt, dgrams;
#endif
        UINT64 totalLen;

        end = ntohl(hdr->endSec);
        end += ntohl(hdr->endUsec) / (FLOAT)IPERF_MILLION;
#ifdef LOSCFG_NET_IPERF_JITTER
        errCnt = ntohl(hdr->errCnt);
        oooCnt = ntohl(hdr->oooCnt);
        dgrams = ntohl(hdr->datagrams);
        jitter = ntohl(hdr->jitterSec);
        jitter += ntohl(hdr->jitterUsec) / (FLOAT)IPERF_MILLION;
#endif
        totalLen = (((UINT64)ntohl(hdr->totalLenH)) << 32) + ntohl(hdr->totalLenL);  /* 32: bits of INT32 */

        PrintBandwidth(context, totalLen, 0, end, FALSE);
#ifdef LOSCFG_NET_IPERF_JITTER
        IPERF_PRINT("jitter: ""%6.3f ms %4d/%5d (%.2f%%)\r\n",
                    (jitter * 1000.0), errCnt, dgrams, /* 1000.0: micro senconds. */
                    (100.0 * errCnt) / dgrams); /* 100.0: percentage */
        if (oooCnt > 0) {
            IPERF_PRINT("received %d out-of-order datagrams\r\n", oooCnt);
        }
#endif
    }
}

STATIC VOID IperfSendUdpFin(IperfContext *context)
{
    INT32 ret;
    INT32 tries = 0;
    fd_set rdSet;
    struct timeval tmo;
    BOOL sndErr = FALSE;
    BOOL rcvErr = FALSE;

    while (tries++ < IPERF_UDP_FIN_MAX_RETRIES) {
        ret = send(context->trafficSock, context->param.buffer, context->param.bufLen, 0);
        if (ret < 0) {
            sndErr = TRUE;
            break;
        }

        FD_ZERO(&rdSet);
        FD_SET(context->trafficSock, &rdSet);

        tmo.tv_sec = 0;
        tmo.tv_usec = IPERF_UDP_FIN_TMO;

        ret = select(context->trafficSock + 1, &rdSet, NULL, NULL, &tmo);
        if (ret < 0) {
            break;
        }
        if (ret == 0) {
            continue;
        }
        ret = recv(context->trafficSock, context->param.buffer, context->param.bufLen, 0);
        if (ret < 0) {
            rcvErr = TRUE;
            break;
        }
        IperfProcessUdpServerHdr(context, (UINT32)ret);
        return;
    }

    if (((sndErr == TRUE) || (rcvErr == TRUE)) && (context->isKilled == FALSE)) {
        IPERF_PRINT("Send UDP FIN: %s, error=%d.\n", ((sndErr == TRUE) ? "send" : "recv"), errno);
    }

    IPERF_PRINT("WARNING: Did not receive ack of last datagram after %d tries.\n", tries - 1);
}

STATIC VOID IperfUdpFinAckHdr(IperfContext *context, FLOAT duration)
{
    IperfUdpServerHdr *hdr = NULL;
    INT32 ret;
    hdr = (IperfUdpServerHdr *)(context->param.buffer + sizeof(IperfUdpHdr));
    ret = memset_s(hdr, sizeof(IperfUdpServerHdr), 0, sizeof(IperfUdpServerHdr));
    if (ret != EOK) {
        return;
    }
    hdr->flags = htonl(IPERF_HEADER_VERSION_V1);
#ifdef LOSCFG_NET_IPERF_JITTER
    hdr->totalLenH = htonl((INT64)(context->udpStat->totalLen >> 32)); /* 32: bits of INT32 */
    hdr->totalLenL = htonl((INT64)(context->udpStat->totalLen & 0xFFFFFFFF));
    hdr->oooCnt = htonl(context->udpStat->oooCnt);
    hdr->errCnt = htonl(context->udpStat->errCnt - context->udpStat->oooCnt);
    hdr->datagrams = htonl(context->udpStat->lastID);
    hdr->jitterSec = htonl((INT64)context->udpStat->jitter);
    hdr->jitterUsec = htonl((INT64)((context->udpStat->jitter -
                                   (INT64)context->udpStat->jitter) * IPERF_MILLION));
#else
    hdr->totalLenH = htonl((INT64)(context->param.total >> 32)); /* 32: bits of INT32 */
    hdr->totalLenL = htonl((INT64)(context->param.total & 0xFFFFFFFF));
#endif
    hdr->endSec = htonl((INT64)duration);
    hdr->endUsec = htonl((INT64)((duration - (INT64)duration) * IPERF_MILLION));
}


STATIC VOID IperfSendUdpFinAck(IperfContext *context)
{
    INT32 ret;
    INT32 tries = 0;
    fd_set rdSet;
    struct timeval tmo;

    BOOL sndErr = FALSE;
    BOOL rcvErr = FALSE;
    UINT32 len = context->param.bufLen;
#ifdef LOSCFG_NET_IPERF_JITTER
    FLOAT duration = TimeSec(context->udpStat->lastPktTime, context->udpStat->firstPktTime);
#else
    FLOAT duration = TimeSec(context->end, context->start);
#endif
    while ((tries++) < IPERF_UDP_FIN_MAX_RETRIES) {
        if (len >= (sizeof(IperfUdpHdr) + sizeof(IperfUdpServerHdr))) {
            IperfUdpFinAckHdr(context, duration);
        }
        /* already connected, no need to use sendto */
        ret = send(context->trafficSock, context->param.buffer, context->param.bufLen, 0);
        if (ret < 0) {
            sndErr = TRUE;
            break;
        }
        FD_ZERO(&rdSet);
        FD_SET(context->trafficSock, &rdSet);

        tmo.tv_sec = IPERF_UDP_FINACK_TMO;
        tmo.tv_usec = 0;

        ret = select(context->trafficSock + 1, &rdSet, NULL, NULL, &tmo);
        if (ret == 0) {
            return;
        }
        if (ret < 0) {
            break;
        }
        ret = recv(context->trafficSock, context->param.buffer, context->param.bufLen, 0);
        if (ret <= 0) {
            return;
        }
        len = (UINT32)ret;
    }
    if (((sndErr == TRUE) || (rcvErr == TRUE)) && (context->isKilled == FALSE)) {
        IPERF_PRINT("Send UDP FINACK: %s, error=%d.\n", ((sndErr == TRUE) ? "send" : "recv"), errno);
    }

    IPERF_PRINT("WARNING: Ack of last datagram failed after %d tries.\n", tries - 1);
}

STATIC VOID IperfGenerateUdpClientHdr(IperfContext *context)
{
    if (context->param.bufLen >= sizeof(IperfUdpHdr) + sizeof(IperfUdpClientHdr)) {
        IperfUdpClientHdr *hdr = (IperfUdpClientHdr *)(context->param.buffer + sizeof(IperfUdpHdr));

        /* only support normal mode */
        hdr->flags = 0;
        /* only support one client (-P not supported) */
        hdr->numThreads = 1;
        hdr->port = htons(context->param.port);
        hdr->bufLen = htonl(context->param.bufLen);
        hdr->winBand = htonl((INT64)context->param.bandwidth);
        if ((context->param.amount & IPERF_AMOUNT_1) != 0) {
            hdr->amount = htonl(-(INT64)context->param.amount);
        } else {
            hdr->amount = htonl((INT64)context->param.amount);
            hdr->amount &= htonl(IPERF_AMOUNT_2);
        }
    }
}

/* set server socket option */
STATIC INT32 IperfServerSetOption(IperfContext *context)
{
    INT32 ret;
    struct timeval rcvTmo;
    rcvTmo.tv_sec = IPERF_DEFAULT_RX_TIMEOUT;
    rcvTmo.tv_usec = 0;
    ret = setsockopt(context->trafficSock, SOL_SOCKET, SO_RCVTIMEO, (const CHAR*)&rcvTmo, sizeof(rcvTmo));
    if (ret < 0) {
        IPERF_PRINT("Set rcvtimeo failed, error=%d.\n", errno);
        return IPERF_FAIL;
    }

    INT32 tos = context->param.tos;
    ret = setsockopt(context->trafficSock, SOL_IP, IP_TOS, &tos, sizeof(tos));
    if (ret < 0) {
        IPERF_PRINT("Set reuse failed, error=%d.\n", errno);
        return IPERF_FAIL;
    }

    return IPERF_OK;
}

STATIC INT32 IperfServerFirstDataProcess(IperfContext *context, const struct sockaddr *from, socklen_t len)
{
    INT32 ret;
    /* start reporter when first packet received */
    if (IperfReporterGo() < 0) {
        return IPERF_FAIL;
    }
    gettimeofday(&context->start, NULL);
    if (IPERF_IS_UDP(context->param.mask)) {
#ifdef LOSCFG_NET_IPERF_JITTER
        context->udpStat->firstPktTime.tv_sec = context->start.tv_sec;
        context->udpStat->firstPktTime.tv_usec = context->start.tv_usec;
#endif
        ret = connect(context->trafficSock, from, len);
        if (ret < 0) {
            IPERF_PRINT("Execute connect failed, error=%d.\n", errno);
            return IPERF_FAIL;
        }
    }

    return IPERF_OK;
}

STATIC INT32 IperfClearContext(INT32 sock, IperfContext *context)
{
    close(sock);
    context->trafficSock = IPERF_INVALID_SOCKET;
    context->listenSock = IPERF_INVALID_SOCKET;
    return IPERF_FAIL;
}

/* iperf server testing phase1:
 * 1, create socket based on domain and type;
 * 2, bind and accept;
 * 3, set necessary socket option, include reuse, RCVTIMEO;
 * return -1 if failed or socket fd if succeed.
 */
STATIC INT32 IperfServerPhase1(IperfContext *context, INT32 domain, socklen_t addrLen,
                               struct sockaddr *local, struct sockaddr *remote)
{
    INT32 ret;
    INT32 sock;
    INT32 reuse = 1;
    BOOL isUdp = IPERF_IS_UDP(context->param.mask);
    sock = socket(domain, (isUdp ? SOCK_DGRAM : SOCK_STREAM), 0);
    if (sock < 0) {
        IPERF_PRINT("Execute socket failed, error=%d.\n", errno);
        return IPERF_FAIL;
    }
    context->trafficSock = sock;
    context->listenSock = sock;
    ret = setsockopt(context->trafficSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (ret < 0) {
        IPERF_PRINT("Set reuse failed, error=%d.\n", errno);
        return IperfClearContext(sock, context);
    }
    ret = bind(sock, local, addrLen);
    if (ret < 0) {
        IPERF_PRINT("Execute bind failed, error=%d.\n", errno);
        return IperfClearContext(sock, context);
    }
    if (isUdp == FALSE) {
        /* backlog 1 means only accept one tcp connection */
        if (listen(sock, 1) < 0) {
            IPERF_PRINT("Execute listen failed, error=%d.\n", errno);
            return IperfClearContext(sock, context);
        }
        context->listenSock = sock;
        /* block to wait for new connection */
        ret = accept(sock, remote, &addrLen);
        if (ret < 0) {
            if (context->isKilled == FALSE) {
                IPERF_PRINT("Execute accept failed, error=%d.\n", errno);
            }
            return IperfClearContext(sock, context);
        }
        close(sock);
        context->listenSock = IPERF_INVALID_SOCKET;
        sock = ret;
    }
    context->trafficSock = sock;
    if (IperfServerSetOption(context) < 0) {
        return IperfClearContext(sock, context);
    }
    return sock;
}

#ifdef LOSCFG_NET_IPERF_JITTER
STATIC VOID IperfProcessUdpHdr(IperfContext *context, UINT32 len)
{
    INT32 packetID;
    DOUBLE transit;
    DOUBLE transitDelta;
    struct timeval sndTime;
    const FLOAT epsinon = 0.0000001; /* set const initial value */

    IperfUdpHdr *udpHdr = (IperfUdpHdr*)context->param.buffer;

    if (len >= sizeof(IperfUdpHdr)) {
        context->udpStat->totalLen += len;

        sndTime.tv_sec = ntohl(udpHdr->sec);
        sndTime.tv_usec = ntohl(udpHdr->usec);
        gettimeofday(&context->udpStat->lastPktTime, NULL);

        transit = TimeSec(context->udpStat->lastPktTime, sndTime);
        if ((context->udpStat->lastTransit > epsinon) || (context->udpStat->lastTransit < -epsinon)) {
            transitDelta = transit - context->udpStat->lastTransit;
            if (transitDelta < 0.0) {
                transitDelta = -transitDelta;
            }
            /* J = J + ( | D(i-1,i) | - J ) / 16 */
            context->udpStat->jitter += (transitDelta - context->udpStat->jitter) / (16.0); /* 16.0: from RFC 1889 */
        }
        context->udpStat->lastTransit = transit;

        packetID = ntohl(udpHdr->id);
        if (packetID < 0) { /* UDP FIN datagram */
            context->udpStat->finish = TRUE;
            packetID = -packetID;
        }

        BOOL oooChanged = FALSE;
        BOOL errChanged = FALSE;
        if (packetID != context->udpStat->lastID + 1) {
            if (packetID < context->udpStat->lastID + 1) {
                context->udpStat->oooCnt++;
                oooChanged = TRUE;
            } else {
                context->udpStat->errCnt += packetID - context->udpStat->lastID - 1;
                errChanged = TRUE;
            }
        }

        if (packetID > context->udpStat->lastID) {
            context->udpStat->lastID = packetID;
        }

        if (oooChanged && context->udpStat->oooCnt < 0) { /* overflow happeded */
            context->udpStat->oooCnt = 0;
        }
        if (errChanged && context->udpStat->errCnt < 0) { /* overflow happeded */
            context->udpStat->errCnt = 0;
        }
    }
}
#endif

STATIC BOOL IperfServerDataProcess(IperfContext *context,
                                   UINT32 len)
{
    if (IPERF_IS_UDP(context->param.mask)) {
#ifdef LOSCFG_NET_IPERF_JITTER
        IperfProcessUdpHdr(context, len);
        if (context->udpStat->finish == TRUE) {
            return TRUE;
        }
#else
        return TRUE;
#endif
    }
    return FALSE;
}

/* iperf client testing phase2:
 * 1, recv data from client;
 * 2, start reporter when first data received;
 * 3, send FINACK when testing finished if UDP;
 * 4, feed watchdog if needed;
 */
STATIC VOID IperfServerPhase2(IperfContext *context)
{
    INT32 recvLen;
    struct sockaddr peer;
    struct sockaddr *from = &peer;
    socklen_t slen = sizeof(struct sockaddr);
    socklen_t *fromLen = &slen;
    BOOL firstData = FALSE;
    IperfUdpHdr *udpHdr = (IperfUdpHdr *)context->param.buffer;
#ifdef LOSCFG_NET_IPERF_JITTER
    if (IPERF_IS_UDP(context->param.mask)) {
        context->udpStat->lastID = -1;
    }
#endif
    while ((context->isFinish == FALSE) && (context->isKilled == FALSE)) {
        recvLen = recvfrom(context->trafficSock, context->param.buffer,
                           context->param.bufLen, 0, from, fromLen);
        if (recvLen < 0) {
            if ((firstData == FALSE) && (errno == EAGAIN)) {
                continue;
            }
            IPERF_PRINT("Execute recvfrom failed, error=%d.\n", errno);
            break;
        }
        if (recvLen == 0) { /* tcp connection closed by peer side */
            context->isFinish = TRUE;
            break;
        }
        if (firstData == FALSE) {
            firstData = TRUE;
            IperfServerFirstDataProcess(context, from, *fromLen);
            from = NULL;
            fromLen = NULL;
        }

        context->param.total += (UINT32)recvLen;
        if (IperfServerDataProcess(context, (UINT32)recvLen) && ((INT32)ntohl(udpHdr->id) < 0)) {
            context->isFinish = TRUE;
            break;
        }

#ifdef IPERF_FEED_WDG
    IperfFeedWdg();
#endif /* IPERF_FEED_WDG */
    }

    gettimeofday(&context->end, NULL);
}

/* iperf server testing phase3:
 * 1, print the whole testing result;
 * 2, close traffic socket;
 */
STATIC VOID IperfServerPhase3(IperfContext *context)
{
    if (context->start.tv_sec != 0) { /* already connected */
        FLOAT duration = TimeSec(context->end, context->start);
#ifdef LOSCFG_NET_IPERF_JITTER
        if (IPERF_IS_UDP(context->param.mask)) {
            duration = TimeSec(context->udpStat->lastPktTime, context->udpStat->firstPktTime);
        }
#endif
        IperfDelay(100000); /* 100000: delay 100ms to make reporter print first */
        PrintBandwidth(context, context->param.total, 0, duration, TRUE);
        if (IPERF_IS_UDP(context->param.mask) && (context->isFinish == TRUE)) {
            IperfSendUdpFinAck(context);
        }
    }

    close(context->trafficSock);
    context->trafficSock = IPERF_INVALID_SOCKET;
    context->isFinish = TRUE;
}

STATIC VOID IperfServerGo(IperfContext *context)
{
    INT32 ret;
    INT32 domain;
    INT32 sock;
    struct sockaddr_in local;
    struct sockaddr_in remote;
    struct sockaddr_in6 local6;
    struct sockaddr_in6 remote6;
    struct sockaddr *localAddr = NULL;
    struct sockaddr *remoteAddr = NULL;
    socklen_t addrLen;

    if (IPERF_IS_IP_V6(context->param.mask)) {
        domain = AF_INET6;
        ret = memset_s(&local6, sizeof(local6), 0, sizeof(local6));
        if (ret != EOK) {
            return;
        }
        IperfIpv6Init(&local6, domain, context->param.port, context->param.u1.srcIP6);
        localAddr = (struct sockaddr *)&local6;
        remoteAddr = (struct sockaddr *)&remote6;
        addrLen = sizeof(struct sockaddr_in6);
    } else {
        domain = AF_INET;
        ret = memset_s(&local, sizeof(local), 0, sizeof(local));
        if (ret != EOK) {
            return;
        }
        IperfIpv4Init(&local, domain, context->param.port, context->param.u1.srcIP4);
        localAddr = (struct sockaddr *)&local;
        remoteAddr = (struct sockaddr *)&remote;
        addrLen = sizeof(struct sockaddr_in);
    }

    sock = IperfServerPhase1(context, domain, addrLen, localAddr, remoteAddr);
    if (sock < 0) {
        return;
    }

    IperfServerPhase2(context);

    IperfServerPhase3(context);
}

STATIC INT32 IperfClientPhaseClean(IperfContext *context, INT32 sock)
{
    close(sock);
    if (context->trafficSock > 0) {
        context->trafficSock = IPERF_INVALID_SOCKET;
    }
    return IPERF_FAIL;
}

/* iperf client testing phase1:
 * 1, create socket based on domain and type;
 * 2, set reuseaddr option to makesure bind ok;
 * 3, bind to local address (-B option);
 * 4, set socket option, include ToS (-S option);
 * 5, connect to peer address (-c option);
 * 6, start reporter task;
 *
 * return -1 if failed or socket fd if succeed.
 */
STATIC INT32 IperfClientPhase1(IperfContext *context, INT32 domain, socklen_t addrLen,
                               struct sockaddr *local, struct sockaddr *remote)
{
    INT32 sock;
    BOOL isUdp = IPERF_IS_UDP(context->param.mask);
    sock = socket(domain, (isUdp ? SOCK_DGRAM : SOCK_STREAM), 0);
    if (sock < 0) {
        IPERF_PRINT("Execute socket failed, error=%d.\n", errno);
        return IperfClientPhaseClean(context, sock);
    }

    INT32 reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        IPERF_PRINT("Set reuseaddr failed, error=%d.\n", errno);
        return IperfClientPhaseClean(context, sock);
    }

    if (bind(sock, local, addrLen) < 0) {
        IPERF_PRINT("Execute bind failed, error=%d.\n", errno);
        return IperfClientPhaseClean(context, sock);
    }
    INT32 tos = context->param.tos;
    if (setsockopt(sock, IPPROTO_IP, IP_TOS, &tos, sizeof(tos)) < 0) {
        IPERF_PRINT("Set tos failed, error=%d.\n", errno);
        return IperfClientPhaseClean(context, sock);
    }
    context->trafficSock = sock; /* Must set traffic socket before connect */
    if (connect(sock, remote, addrLen) < 0) {
        if (context->isKilled == FALSE) {
            IPERF_PRINT("Execute connect failed, error=%d.\n", errno);
        }
        return IperfClientPhaseClean(context, sock);
    }
    if (IperfReporterGo() < 0) {
        IPERF_PRINT("Start reporter failed.\n");
        return IperfClientPhaseClean(context, sock);
    }

    return sock;
}

STATIC VOID IperfClientUdpPre(IperfContext *context, INT32 *delayTarget)
{
    IperfGenerateUdpClientHdr(context); /* construct udp client header */
    *delayTarget = (int)((UINT64)context->param.bufLen *
                         ((IPERF_BITS_PER_BYTE * (FLOAT)IPERF_MILLION) / context->param.bandwidth));
    if ((*delayTarget < 0) || (*delayTarget > IPERF_MILLION)) {
        *delayTarget = IPERF_MILLION;
    }
}

/* iperf client testing phase2:
 * 1, generate client header if UDP;
 * 2, send data to server;
 * 3, quit testing of amout reached or send failed;
 * 4, feed watchdog if needed;
 */
STATIC VOID IperfClientPhase2(IperfContext *context)
{
    struct timeval lastPktTime, packetTime;
    IperfUdpHdr *udpHdr = (IperfUdpHdr*)context->param.buffer;
    INT32 delay = 0;
    INT32 adjust;
    INT32 delayTarget = 0;
    INT32 sendLen;
    BOOL isUdp = IPERF_IS_UDP(context->param.mask);

    if (isUdp) {
        gettimeofday(&lastPktTime, NULL);
        IperfClientUdpPre(context, &delayTarget);
    }
    gettimeofday(&context->start, NULL);
    while ((context->isKilled == FALSE) && (context->isFinish == FALSE)) {
        if (isUdp) {
            gettimeofday(&packetTime, NULL);
            udpHdr->id   = htonl(context->udpPktID++);
            udpHdr->sec  = htonl(packetTime.tv_sec);
            udpHdr->usec = htonl(packetTime.tv_usec);

            adjust = delayTarget + TimeUsec(lastPktTime, packetTime);
            lastPktTime.tv_sec  = packetTime.tv_sec;
            lastPktTime.tv_usec = packetTime.tv_usec;

            if ((adjust > 0) || (delay > 0)) {
                delay += adjust;
            }

            if (delay > (INT32)IPERF_UDP_DELAY) {
                IperfDelay(delay);
            }
        }
        sendLen = send(context->trafficSock, context->param.buffer, context->param.bufLen, 0); /* 0: no flag */
        if (sendLen < 0) {
            if (errno == ENOMEM) {
                IperfDelay(0); /* 0: delay one tick */
                continue;
            }
            IPERF_PRINT("Execute send failed, error=%d.\n", errno);
            break;
        }

        context->param.total += (UINT32)sendLen;
        if ((context->param.amount != 0) && (context->param.total >= context->param.amount)) {
            break;
        }
#ifdef IPERF_FEED_WDG
        IperfFeedWdg();
#endif /* IPERF_FEED_WDG */
    }
    gettimeofday(&context->end, NULL);
}

/* iperf client testing phase3:
 * 1, send FIN packet in UDP testing;
 * 2, output testing result, include time, amount and average speed;
 * 3, close traffic socket;
 */
STATIC VOID IperfClientPhase3(IperfContext *context)
{
    BOOL isUdp = IPERF_IS_UDP(context->param.mask);
    struct timeval packetTime;
    IperfUdpHdr *udpHdr = (IperfUdpHdr*)context->param.buffer;

    context->isFinish = TRUE;

    if (isUdp) {
        gettimeofday(&packetTime, NULL);
        udpHdr->id = htonl(-(context->udpPktID)); /* negative value */
        udpHdr->sec = htonl(packetTime.tv_sec);
        udpHdr->usec = htonl(packetTime.tv_usec);
        IperfSendUdpFin(context);
    } else {
        FLOAT duration = TimeSec(context->end, context->start);
        PrintBandwidth(context, context->param.total, 0, duration, FALSE);
    }

    close(context->trafficSock);
    context->trafficSock = IPERF_INVALID_SOCKET;
}

STATIC VOID IperfClientGo(IperfContext *context)
{
    INT32 domain, sock;
    struct sockaddr_in local, remote;
    struct sockaddr_in6 local6, remote6;
    struct sockaddr *localAddr = NULL;
    struct sockaddr *remoteAddr = NULL;
    socklen_t addrLen;
    struct timeval sndTmo;

    if (IPERF_IS_IP_V6(context->param.mask)) {
        domain = AF_INET6;
        (VOID)memset_s(&local6, sizeof(local6), 0, sizeof(local6));
        (VOID)memset_s(&remote6, sizeof(remote6), 0, sizeof(remote6));
        IperfIpv6Init(&local6, domain, 0, context->param.u1.srcIP6);
        IperfIpv6Init(&remote6, domain, context->param.port, context->param.u2.dstIP6);
        localAddr = (struct sockaddr *)&local6;
        remoteAddr = (struct sockaddr *)&remote6;
        addrLen = sizeof(struct sockaddr_in6);
    } else {
        domain = AF_INET;
        (VOID)memset_s(&local, sizeof(local), 0, sizeof(local));
        (VOID)memset_s(&remote, sizeof(remote), 0, sizeof(remote));
        IperfIpv4Init(&local, domain, 0, context->param.u1.srcIP4);
        IperfIpv4Init(&remote, domain, context->param.port, context->param.u2.dstIP4);
        localAddr = (struct sockaddr *)&local;
        remoteAddr = (struct sockaddr *)&remote;
        addrLen = sizeof(struct sockaddr_in);
    }

    sock = IperfClientPhase1(context, domain, addrLen, localAddr, remoteAddr);
    if (sock < 0) {
        IPERF_PRINT("Create sock failed.\n");
        return;
    }

    sndTmo.tv_sec = IPERF_DEFAULT_TX_TIMEOUT;
    sndTmo.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &sndTmo, sizeof(sndTmo));

    IperfClientPhase2(context);
    IperfClientPhase3(context);
}

STATIC BOOL IperfBufferInit(IperfContext *context)
{
#ifdef LOSCFG_NET_IPERF_JITTER
    INT32 ret;
    if (IPERF_IS_UDP(context->param.mask) && IPERF_IS_SERVER(context->param.mask)) {
        context->udpStat = malloc(sizeof(IperfUdpStat));
        if (context->udpStat == NULL) {
            IPERF_PRINT("Error: no free memory\r\n");
            return FALSE;
        }
        ret = memset_s(context->udpStat, sizeof(IperfUdpStat), 0, sizeof(IperfUdpStat));
        if (ret != EOK) {
            free(context->udpStat);
            return FALSE;
        }
    }
#endif
    context->param.buffer = (UINT8 *)malloc(context->param.bufLen);
    if (context->param.buffer == NULL) {
        IPERF_PRINT("Error: no free memory\r\n");
        return FALSE;
    }
    return TRUE;
}

STATIC VOID IperfTrafficClean(IperfContext *context)
{
    UINT32 mtx;
    LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER);
#ifdef LOSCFG_NET_IPERF_JITTER
    if (IPERF_IS_UDP(context->param.mask) && IPERF_IS_SERVER(context->param.mask)) {
        if (context->udpStat != NULL) {
            free(context->udpStat);
            context->udpStat = NULL;
        }
    }
#endif

    if (context->param.buffer != NULL) {
        free(context->param.buffer);
        context->param.buffer = NULL;
    }
    free(context);
    SetGlobalIperfContext(NULL);
    LOS_MuxPost(g_iperfMutex);
    mtx = g_iperfMutex;
    g_iperfMutex = 0xFFFFFFFF; /* 0xFFFFFFFF: invalid handle */
    LOS_MuxDelete(mtx);
}

STATIC VOID IperfTrafficEntry(UINT32 p0)
{
    UINT32 i;
    IperfContext *context = (IperfContext *)p0;

    if (LOS_MuxCreate(&g_iperfMutex) != LOS_OK) {
        IPERF_PRINT("Create mutex failed.\n");
        return;
    }
    if (IperfBufferInit(context) == FALSE) {
        IperfTrafficClean(context);
    }
    for (i = 0; i < context->param.bufLen; i++) {
        context->param.buffer[i] = (i % 10) + '0'; /* 10: iperf data is '0' to '9' */
    }

    context->isRunning = TRUE;

    (VOID)LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER);
    SetGlobalIperfContext(context);
    (VOID)LOS_MuxPost(g_iperfMutex);

    if (IPERF_IS_SERVER(context->param.mask)) {
        IperfServerGo(context);
    } else {
        IperfClientGo(context);
    }
    IperfTrafficClean(context);
}

STATIC INT32 IperfStart(IperfContext *context)
{
    UINT32 ret;
    UINT32 taskID;
    TSK_INIT_PARAM_S appTask;
    ret = memset_s(&appTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return ret;
    }

    appTask.pfnTaskEntry = (TSK_ENTRY_FUNC)IperfTrafficEntry;
    appTask.uwStackSize  = IPERF_TRAFFIC_STACK_SIZE;
    appTask.pcName = IPERF_TRAFFIC_NAME;
    if (IPERF_IS_SERVER(context->param.mask)) {
        appTask.usTaskPrio = IPERF_TRAFFIC_SERVER_PRIORITY;
    } else {
        appTask.usTaskPrio = IPERF_TRAFFIC_CLIENT_PRIORITY;
    }
    appTask.uwResved = LOS_TASK_STATUS_DETACHED;
    LOS_TASK_PARAM_INIT_ARG(appTask, context);

    ret = LOS_TaskCreate(&taskID, &appTask);
    if (ret != 0) {
        IPERF_PRINT("Create iperf task failed.\n");
        free(context);
        return IPERF_FAIL;
    }
    return IPERF_OK;
}

STATIC INT32 IperfStop(VOID)
{
    IperfContext *iperfContext = NULL;
    if (LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER) != LOS_OK) {
        return IPERF_FAIL;
    }
    iperfContext = GetGlobalIperfContext();
    if (IsIperfRunning()) {
        iperfContext->isKilled = TRUE;
    } else {
        (VOID)LOS_MuxPost(g_iperfMutex);
        return IPERF_FAIL;
    }

    (VOID)LOS_MuxPost(g_iperfMutex);

    LOS_TaskDelay(LOSCFG_BASE_CORE_TICK_PER_SECOND); /* 1000ms: makesure atleast three UDP FIN send out */

    if (LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER) != LOS_OK) {
        return IPERF_OK; /* iperf already stopped */
    }
    iperfContext = GetGlobalIperfContext();
    if ((iperfContext != NULL) && (iperfContext->listenSock != IPERF_INVALID_SOCKET)) {
        close(iperfContext->listenSock);
        iperfContext->listenSock = IPERF_INVALID_SOCKET;
    }
    if ((iperfContext != NULL) && (iperfContext->trafficSock != IPERF_INVALID_SOCKET)) {
        close(iperfContext->trafficSock);
        iperfContext->trafficSock = IPERF_INVALID_SOCKET;
    }

    (VOID)LOS_MuxPost(g_iperfMutex);
    return IPERF_OK;
}

STATIC BOOL IperfIntStrCheck(const CHAR *str)
{
    INT32 i;
    UINT32 len = strlen(str);
    for (i = 0; i < len; i++) {
        if (str[i] == '\n') {
            continue;
        }
        if ((str[i] > '9') || (str[i] < '0')) {
            return FALSE;
        }
    }
    return TRUE;
}

STATIC BOOL IperfStrCheck(const CHAR *str, CHAR *arr, UINT32 len)
{
    if (str == NULL || (strlen(str) <= 0) || (strlen(str) >= len - 1)) {
        return FALSE;
    }

    if (strcpy_s(arr, len, str) != EOK) {
        return FALSE;
    }
    return TRUE;
}

STATIC BOOL IperfByteAtoi(const CHAR *str, UINT64 *out)
{
    INT64 num;
    UINT8 suffix;
    CHAR arr[IPERF_MAX_STR_LEN]; /* 32: max string len */
    if (IperfStrCheck(str, arr, sizeof(arr)) != TRUE) {
        return FALSE;
    }

    suffix = arr[strlen(arr) - 1];
    if ((suffix == 'G') || (suffix == 'M') || (suffix == 'K') ||
        (suffix == 'g') || (suffix == 'm') || (suffix == 'k')) {
        arr[strlen(arr) - 1] = 0;
    } else {
        suffix = '\n';
    }

    num = atoi(arr);
    if (!IperfIntStrCheck(arr) || (num <= 0)) {
        return FALSE;
    }

    switch (suffix) {
        case 'G':
            num *= IPERF_GIGA;
            break;
        case 'M':
            num *= IPERF_MEGA;
            break;
        case 'K':
            num *= IPERF_KILO;
            break;
        case 'g':
            num *= IPERF_BILLION;
            break;
        case 'm':
            num *= IPERF_MILLION;
            break;
        case 'k':
            num *= IPERF_THOUSAND;
            break;
        case '\n':
            num = num;
            break;
        default:
            return FALSE;
    }
    *out = (UINT64)num;
    return TRUE;
}

#ifdef IPERF_DEBUG
STATIC VOID IperfInfoShow(IperfContext *context)
{
#define IPERF_MAX_ADDRLEN 64
    CHAR sIP[IPERF_MAX_ADDRLEN];
    CHAR dIP[IPERF_MAX_ADDRLEN];
    INT32 domain = ((IPERF_IS_IP_V6(context->param.mask)) ? AF_INET6 : AF_INET);

    IPERF_PRINT("Iperf info: %s-%s-%s\n",
                ((IPERF_IS_UDP(context->param.mask)) ? "UDP" : "TCP"),
                ((IPERF_IS_CLIENT(context->param.mask)) ? "Client" : "Server"),
                ((domain == AF_INET) ? "IPV4" : "IPV6"));
    inet_ntop(domain, &context->param.u1.srcIP6, sIP, IPERF_MAX_ADDRLEN);
    inet_ntop(domain, &context->param.u2.dstIP6, dIP, IPERF_MAX_ADDRLEN);
    IPERF_PRINT("SrcIP: %s, DstIP: %s, Port: %d.\n", sIP, dIP, context->param.port);

    IPERF_PRINT("Interval: %usec, Time: %usec, Amount: %llu, Bandwidth: %llu, BufLen: %u.\n",
                context->param.interval, context->param.time, context->param.amount,
                context->param.bandwidth, context->param.bufLen);
#undef IPERF_MAX_ADDRLEN
}
#endif

STATIC BOOL IperfOptionConv(INT32 index, INT32 argc, const CHAR **argv, UINT32 *para, const CHAR *paramStr)
{
    if ((index + 1) == argc) {
        IPERF_PRINT("Parameter %s is error.\n", argv[index]);
        return FALSE;
    }
    if (IperfIntStrCheck(argv[index + 1])) {
        INT32 num = atoi(argv[index + 1]);
        if (num >= 0) {
            *para = (UINT32)num;
            return TRUE;
        }
    }
    IPERF_PRINT("Invalid value of '%s' for %s\r\n", argv[index + 1], paramStr);
    return FALSE;
}

/* without value */
STATIC VOID ParseParam1(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 i;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-V")) { /* Must be the first option */
            context->param.mask |= IPERF_MASK_IP_V6;
        }

        if (!strcmp(argv[i], "-u")) {
            context->param.mask |= IPERF_MASK_UDP;
        }

        if (!strcmp(argv[i], "-s")) {
            context->param.mask |= IPERF_MASK_SERVER;
        }
    }
}

/* with value, but not IP address */
STATIC INT32 ParseParam2(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 i;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-i")) {
            if (IperfOptionConv(i++, argc, argv, &context->param.interval, "-i interval") == FALSE) {
                return IPERF_FAIL;
            }
        }

        if (!strcmp(argv[i], "-p")) {
            UINT32 port;
            if (IperfOptionConv(i++, argc, argv, &port, "-p port") == FALSE) {
                return IPERF_FAIL;
            }
            context->param.port = port;
        }

        if (!strcmp(argv[i], "-t")) {
            UINT32 time;
            if (IperfOptionConv(i++, argc, argv, &time, "-t time") == FALSE) {
                return IPERF_FAIL;
            }
            if (time > IPERF_DEFAULT_MAX_TIME) {
                time = IPERF_DEFAULT_MAX_TIME;
            }
            context->param.time = time;
        }

        if (!strcmp(argv[i], "-S")) {
            UINT32 tos;
            if (IperfOptionConv(i++, argc, argv, &tos, "-S tos") == FALSE) {
                return IPERF_FAIL;
            }
            context->param.tos = tos;
        }
    }
    return IPERF_OK;
}

/* with IP address */
STATIC INT32 ParseParam3(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 domain = IPERF_IS_IP_V6(context->param.mask) ? AF_INET6 : AF_INET;
    INT32 i;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-B")) {
            if ((i + 1) == argc) {
                IPERF_PRINT("-B srcIP is needed\r\n");
                return IPERF_FAIL;
            }
            if (inet_pton(domain, argv[++i], (VOID *)&context->param.u1.srcIP6) <= 0) {
                IPERF_PRINT("Invalid value of '%s' for -B srcIP\r\n", argv[i]);
                return IPERF_FAIL;
            }
        }

        if (!strcmp(argv[i], "-c")) {
            context->param.mask |= IPERF_MASK_CLIENT;
            if ((i + 1) == argc) {
                IPERF_PRINT("-c dstIP is needed\r\n");
                return IPERF_FAIL;
            }
            if (inet_pton(domain, argv[++i], (VOID *)&context->param.u2.dstIP6) <= 0) {
                IPERF_PRINT("Invalid value of '%s' for -c dstIP\r\n", argv[i]);
                return IPERF_FAIL;
            }
        }
    }

    return IPERF_OK;
}

/* -b (bandwidth) */
STATIC INT32 ParseParamLowerB(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 i;
    BOOL found = FALSE;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-b")) {
            found = TRUE;
            break;
        }
    }

    if (found == FALSE) {
        return IPERF_OK;
    }

    BOOL isUdpClient = IPERF_IS_CLIENT(context->param.mask) && IPERF_IS_UDP(context->param.mask);
    if (isUdpClient == FALSE) {
        IPERF_PRINT("-b only allowed for UDP Client\r\n");
        return IPERF_FAIL;
    }
    if ((i + 1) == argc) {
        IPERF_PRINT("-b bandwidth is needed\r\n");
        return IPERF_FAIL;
    }
    if (IperfByteAtoi(argv[++i], &context->param.bandwidth) == FALSE) {
        IPERF_PRINT("Invalid value of '%s' for -b bandwidth\r\n", argv[i]);
        return IPERF_FAIL;
    }

    return IPERF_OK;
}

/* -n (amount) */
STATIC INT32 ParseParamLowerN(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 i;
    BOOL found = FALSE;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-n")) {
            found = TRUE;
            break;
        }
    }

    if (found == FALSE) {
        return IPERF_OK;
    }

    BOOL isClient = IPERF_IS_CLIENT(context->param.mask);
    if (isClient == FALSE) {
        IPERF_PRINT("-n only allowed for Client\r\n");
        return IPERF_FAIL;
    }
    if ((i + 1) == argc) {
        IPERF_PRINT("-n amount is needed\r\n");
        return IPERF_FAIL;
    }
    if (IperfByteAtoi(argv[++i], &context->param.amount) == FALSE) {
        IPERF_PRINT("Invalid value of '%s' for -n amount\r\n", argv[i]);
        return IPERF_FAIL;
    }

    return IPERF_OK;
}

/* -l (send or receive buffer length) */
STATIC INT32 ParseParamLowerL(IperfContext *context, INT32 argc, const CHAR **argv)
{
    INT32 i;

    BOOL found = FALSE;
    for (i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-l")) {
            found = TRUE;
            break;
        }
    }

    if (found == FALSE) {
        return IPERF_OK;
    }

    if ((i + 1) == argc) {
        IPERF_PRINT("Buffer length is needed.\n");
        return IPERF_FAIL;
    }
    UINT64 bufLen;
    if (IperfByteAtoi(argv[++i], &bufLen) == FALSE) {
        IPERF_PRINT("Invalid value of '%s' for -l buffer-length\r\n", argv[i]);
        return IPERF_FAIL;
    }
    /* 2: buffer length must be smaller than 16KB */
    if (bufLen > IPERF_DEFAULT_TCP_BUFLEN * 2 ||
        bufLen < (sizeof(IperfUdpHdr) + sizeof(IperfUdpClientHdr))) {
        IPERF_PRINT("Buffer length should be [36, 16K]\r\n");
        return IPERF_FAIL;
    }

    context->param.bufLen = bufLen;
    return IPERF_OK;
}

STATIC INT32 IperfParseParameter(IperfContext *context, INT32 argc, const CHAR **argv)
{
    if (context == NULL) {
        return (INT32)IPERF_FAIL;
    }
    ParseParam1(context, argc, argv); /* -V, -u, -s */
    if (ParseParam2(context, argc, argv) < 0) { /* -i, -p, -t, -S */
        return (INT32)IPERF_FAIL;
    }
    if (ParseParam3(context, argc, argv) < 0) { /* -c, -B */
        return (INT32)IPERF_FAIL;
    }
    if (ParseParamLowerB(context, argc, argv) < 0) { /* -u */
        return (INT32)IPERF_FAIL;
    }
    if (ParseParamLowerN(context, argc, argv) < 0) { /* -n */
        return (INT32)IPERF_FAIL;
    }
    if (ParseParamLowerL(context, argc, argv) < 0) { /* -l */
        return (INT32)IPERF_FAIL;
    }

    if ((context->param.time != 0) && (context->param.amount != 0)) {
        IPERF_PRINT("Error: only need one end condition -t(time) or -n(amount).\r\n");
        return (INT32)IPERF_FAIL;
    }

    return (INT32)IPERF_OK;
}

VOID IperfSetDefaultParameter(IperfContext *context)
{
    if (context->param.port == 0) {
        context->param.port = IPERF_DEFAULT_PORT;
    }

    if (context->param.interval == 0) {
        context->param.interval = IPERF_DEFAULT_INTERVAL;
    }

    if ((context->param.time == 0) && (context->param.amount == 0) &&
        IPERF_IS_CLIENT(context->param.mask)) {
        context->param.time = IPERF_DEFAULT_TIME;
    }

    if ((context->param.bandwidth == 0) && IPERF_IS_UDP(context->param.mask) &&
        IPERF_IS_CLIENT(context->param.mask)) {
        context->param.bandwidth = IPERF_DEFAULT_UDP_BW;
    }

    if (context->param.bufLen == 0) {
        context->param.bufLen = ((IPERF_IS_UDP(context->param.mask)) ?
                                  IPERF_DEFAULT_UDP_BUFLEN : IPERF_DEFAULT_TCP_BUFLEN);
    }
}

VOID IperfHelp(VOID)
{
    IPERF_PRINT("Usage: iperf [-s|-c host] [options].\n");
    IPERF_PRINT("Try 'iperf --help' for more information.\n");
}

STATIC VOID IperfUsage(VOID)
{
    IPERF_PRINT("Usage: Iperf [-s|-c host] [options].\n");
    IPERF_PRINT("       Iperf [-h|-help|--help].\n");
    IPERF_PRINT("Client/Server:\n");
    IPERF_PRINT("  -i, #[interval]  Pause n seconds between periodic bandwidth reports(default 1 secs).\n");
    IPERF_PRINT("  -l, #[length]    Length of buffer to read or write, defalut 1470(UDP)/8KB(TCP).\n");
    IPERF_PRINT("  -u,              Use UDP rather than TCP.\n");
    IPERF_PRINT("  -p, #[port]      Set server port to listen on/connect to to n (default 5001).\n");
    IPERF_PRINT("  -V,              Set the domain to IPv6(Must be 1st parameter).\n");
    IPERF_PRINT("  -S, #[tos]       Set the IP ToS, 0-255, default 0.\n");
    IPERF_PRINT("  -B, #<host>      Bind to <host>, an unicast address, not support multicast.\n");
    IPERF_PRINT("  -k               Kill the iperf.\n");
    IPERF_PRINT("Client specific:\n");
    IPERF_PRINT("  -c, #[host]      Run in client mode, connecting to <host>.\n");
    IPERF_PRINT("  -t, #[time]      Time in seconds to transmit for (default 30 secs).\n");
    IPERF_PRINT("  -n, #[KM]        Number of bytes to transmit (instead of -t).\n");
    IPERF_PRINT("  -b, #[KM]        For UDP, bandwidth to send in bits/sec(default 1 Mbits/sec).\n");
    IPERF_PRINT("Server specific:\n");
    IPERF_PRINT("  -s,              Run in server mode.\n");
}

STATIC BOOL IperfCheckSingleParam(CHAR cmd, const CHAR* allowed)
{
    INT32 j;
    UINT32 cnt = strlen(allowed);
    for (j = 0; j < cnt; j++) {
        if (cmd == allowed[j]) {
            return TRUE;
        }
    }

    return FALSE;
}

STATIC INT32 IperfCheckParameter(INT32 argc, const CHAR **argv)
{
    INT32 i;
    const CHAR *ptr = NULL;
    const CHAR allowed[] = "ilupctnbskVSB";
    BOOL valid = TRUE;

    for (i = 0; i < argc; i++) {
        ptr = argv[i];
        if (*ptr == '-') {
            ++ptr;
            if ((IperfCheckSingleParam(*ptr, allowed) == FALSE) ||
                (strlen(ptr) > 1)) { /* 1: only one alpha */
                valid = FALSE;
                break;
            }
        }
    }
    if (valid == FALSE) {
        IPERF_PRINT("Option %s is invalid.\n", argv[i]);
        IperfHelp();
        return IPERF_FAIL;
    }
    return IPERF_OK;
}

UINT32 CmdIperf(INT32 argc, const CHAR **argv)
{
    INT32 ret;
    if ((argc < 1) || (argv == NULL) || (*argv == NULL)) {
        IperfHelp();
        return (UINT32)IPERF_FAIL;
    }

    if ((!strcmp(argv[0], "--help")) || (!strcmp(argv[0], "-help")) || (!strcmp(argv[0], "-h"))) {
        IperfUsage();
        return (UINT32)IPERF_OK;
    }

    /* check if any option unsupported */
    if (IperfCheckParameter(argc, argv) < 0) {
        return (UINT32)IPERF_FAIL;
    }

    if (!strcmp(argv[0], "-k")) {
        if (IperfStop() < 0) {
            IPERF_PRINT("No iperf is running.\n");
            return (UINT32)IPERF_FAIL;
        }
        return IPERF_OK;
    }
    if (LOS_MuxPend(g_iperfMutex, LOS_WAIT_FOREVER) == LOS_OK && IsIperfRunning()) {
        /* mux available means iperf running. */
        IPERF_PRINT("Iperf is running.\n");
        (VOID)LOS_MuxPost(g_iperfMutex);
        return (UINT32)IPERF_FAIL;
    }

    IperfContext *context = malloc(sizeof(IperfContext));
    if (context == NULL) {
        IPERF_PRINT("Execute function malloc failed.\n");
        return (UINT32)IPERF_FAIL;
    }
    ret = memset_s(context, sizeof(IperfContext), 0, sizeof(IperfContext));
    if (ret != IPERF_OK) {
        free(context);
        return (UINT32)IPERF_FAIL;
    }
    context->trafficSock = IPERF_INVALID_SOCKET;
    context->listenSock = IPERF_INVALID_SOCKET;
    if (IperfParseParameter(context, argc, argv) < 0) {
        free(context);
        return (UINT32)IPERF_FAIL;
    }

    IperfSetDefaultParameter(context);

#ifdef IPERF_DEBUG
    IperfInfoShow(context);
#endif

    return (UINT32)IperfStart(context);
}
#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(iperf_shellcmd, CMD_TYPE_EX, "iperf", 0, (CmdCallBackFunc)CmdIperf);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
