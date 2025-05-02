/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Test Dtls Server
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

#ifndef LOSCFG_LWIP_IPV4
#error This demo needs to enable Components "Components-->Network-->Enable Network-->Enable Lwip-->Enable Ipv4"
#endif

#include "test_dtls_server.h"
#include "dtls_interface.h"
#include "osdepends/atiny_osdep.h"
#include "atiny_socket.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SERVER_PSK      "11223344556677881122334455667788"
#define SERVER_IDENTITY "testserver1"
#define SERVER_PORT     "5658"
#define DTLS_SERVER_DEBUG

#define DTLS_TASK_PRIORITY          3
#define DTLS_SERVER_DEMO_TASK_SIZE  0x1000
STATIC UINT32 g_demoTaskId;

#ifdef DTLS_SERVER_DEBUG
#define LOG(fmt, arg...) do {                                   \
        printf("%s:%d " fmt "\r\n", __func__, __LINE__, ##arg); \
    } while (0)
#else
#define LOG(fmt, arg...)
#endif

STATIC VOID DemoTaskEntry(VOID)
{
    int ret;
    mbedtls_ssl_context *ssl = NULL;
    dtls_establish_info_s establish_info;

    printf("Dtls server demo task start to run.\n");
    mbedtls_net_context *bind_ctx = atiny_malloc(sizeof(mbedtls_net_context));
    mbedtls_net_context *cli_ctx = atiny_malloc(sizeof(mbedtls_net_context));
    if ((bind_ctx == NULL) || (cli_ctx == NULL)) {
        LOG("malloc failed!");
        return;
    }

    establish_info.psk_or_cert = VERIFY_WITH_PSK;
    establish_info.udp_or_tcp = MBEDTLS_NET_PROTO_UDP;
    establish_info.v.p.psk = (unsigned char *)SERVER_PSK;
    establish_info.v.p.psk_len = strlen(SERVER_PSK);
    establish_info.v.p.psk_identity = (unsigned char *)SERVER_IDENTITY;

    bind_ctx = (mbedtls_net_context *)atiny_net_bind(NULL, SERVER_PORT, 1);
    if (bind_ctx == NULL) {
        LOG("bind failed!");
        return;
    }

    ssl = dtls_ssl_new(&establish_info, MBEDTLS_SSL_IS_SERVER);
    if (bind_ctx == NULL) {
        LOG("dtls_ssl_new failed!");
        return;
    }

    do {
        unsigned char buf[64] = {0};
        ret = dtls_accept(bind_ctx, cli_ctx, NULL, 0, 0);
        LOG("dtls_accept return ret = %d, bindfd = %d, cli_fd = %d", ret, bind_ctx->MBEDTLS_PRIVATE(fd), cli_ctx->MBEDTLS_PRIVATE(fd));
        mbedtls_ssl_set_bio(ssl, cli_ctx, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

        // new psk and handshake should been done for each client, now only for frist connection.
        do {
            ret = mbedtls_ssl_handshake(ssl);
        }
        while ((ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE));
        if (ret != 0) {
            LOG("mbedtls_ssl_handshake failed return 0x%x\n", -ret);
            break;
        }

        do {
            ret = dtls_read(ssl, buf, sizeof(buf), 5000);
        } while (ret < 0);
        LOG("Read from client: %d bytes read\n%s\n", ret, buf);

        unsigned char hello[] = "Hi Client\r\n";
        ret = dtls_write(ssl, hello, sizeof(hello));
        if (ret < 0) {
            LOG("dtls_write failed returned %d\n", ret);
            break;
        }
        LOG("Write to client: %d bytes written\n%s\n", ret, hello);
        LOS_TaskDelay(2000);
    } while (0);
    printf("Dtls server demo task finished.\n");

    if (ssl != NULL) {
        dtls_ssl_destroy(ssl);
    }
    mbedtls_net_free(bind_ctx);
}

VOID DtlsServerDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = DTLS_TASK_PRIORITY;
    taskInitParam.pcName = "DtlsServerDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = DTLS_SERVER_DEMO_TASK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create dtls server demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
