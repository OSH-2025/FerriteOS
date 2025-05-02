/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Http Client Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-20
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

#include "lwip/apps/http_client.h"
#include "http_parser.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef HTTPC_DEBUG_REQUEST
#define HTTPC_DEBUG_REQUEST     0
#endif

#define TASK_PRIORITY           7
#define TASK_STACK_SIZE         0x2000
#define DEFAULT_HOST            "your.ip.address"
#define DEFAULT_PORT            80
#define DEFAULT_URL             "/index.html"
#define HTTP_CLIENT_WAIT_TIME   20000

STATIC UINT32 g_demoTaskId;

typedef enum ehttpc_parse_state {
  HTTPC_PARSE_WAIT_FIRST_LINE = 0,
  HTTPC_PARSE_WAIT_HEADERS,
  HTTPC_PARSE_RX_DATA
} httpc_parse_state_t;

typedef struct _httpc_state {
  struct altcp_pcb *pcb;
  ip_addr_t remote_addr;
  UINT16 remote_port;
  INT32 timeout_ticks;
  struct pbuf *request;
  struct pbuf *rx_hdrs;
  UINT16 rx_http_version;
  UINT16 rx_status;
  altcp_recv_fn recv_fn;
  const httpc_connection_t *conn_settings;
  VOID *callback_arg;
  UINT32 rx_content_len;
  UINT32 hdr_content_len;
  httpc_parse_state_t parse_state;
#if HTTPC_DEBUG_REQUEST
  CHAR *server_name;
  CHAR *uri;
#endif
} httpc_state_t;

STATIC INT32 OnmessageBegin(http_parser *parser)
{
    (VOID)parser;
    printf("\n***MESSAGE BEGIN***\n\n");
    return LOS_OK;
}

STATIC INT32 OnHeadersComplete(http_parser *parser)
{
    (VOID)parser;
    printf("\n***HEADERS COMPLETE***\n\n");
    return LOS_OK;
}

STATIC INT32 OnmessageComplete(http_parser *parser)
{
    (VOID)parser;
    printf("\n***MESSAGE COMPLETE***\n\n");
    return LOS_OK;
}

STATIC INT32 OnUrl(http_parser *parser, const CHAR *at, UINT32 length)
{
    (VOID)parser;
    printf("Url: %.*s\n", (INT32)length, at);
    return LOS_OK;
}

STATIC INT32 onHeaderField(http_parser *parser, const CHAR *at, UINT32 length)
{
    (VOID)parser;
    printf("Header field: %.*s\n", (INT32)length, at);
    return LOS_OK;
}

STATIC INT32 OnHeaderValue(http_parser *parser, const CHAR *at, UINT32 length)
{
    (VOID)parser;
    printf("Header value: %.*s\n", (INT32)length, at);
    return LOS_OK;
}

STATIC INT32 OnBody(http_parser *parser, const CHAR *at, UINT32 length)
{
    (VOID)parser;
    printf("Body: %.*s\n", (INT32)length, at);
    return LOS_OK;
}

STATIC VOID HttpParse(const CHAR *data, UINT32 dataLen)
{
    INT32 ret;
    UINT32 executeLen;
    http_parser parser = {0};
    http_parser_settings settings;

    ret = memset_s(&settings, sizeof(http_parser_settings), 0, sizeof(http_parser_settings));
    if (ret != EOK) {
        printf("Http parser init failed.");
        return;
    }
    settings.on_message_begin = OnmessageBegin;
    settings.on_url = OnUrl;
    settings.on_header_field = onHeaderField;
    settings.on_header_value = OnHeaderValue;
    settings.on_headers_complete = OnHeadersComplete;
    settings.on_body = OnBody;
    settings.on_message_complete = OnmessageComplete;

    http_parser_init(&parser, HTTP_BOTH);
    executeLen = http_parser_execute(&parser, &settings, data, dataLen);
    if (executeLen != dataLen) {
        printf("%s (%s)\n", http_errno_description(HTTP_PARSER_ERRNO(&parser)),
               http_errno_name(HTTP_PARSER_ERRNO(&parser)));
    }
}

STATIC err_t HttpcRecv(VOID *arg, struct altcp_pcb *pcb, struct pbuf *p, err_t err)
{
    (VOID)arg;
    (VOID)err;
    struct pbuf *q;
    printf("Http receive:\n");
    if (p == NULL) {
        return ERR_BUF;
    }
    for (q = p; q != NULL; q = q->next) {
        if (q->payload != NULL) {
            printf("%s", (CHAR *)q->payload);
        }
    }
    printf("Http receive finished.\n");
    altcp_recved(pcb, p->tot_len);
    (VOID)pbuf_free(p);
    return ERR_OK;
}

STATIC err_t HttpcHeaderDone(httpc_state_t *connection, VOID *arg, struct pbuf *hdr, UINT16 hdrLen, UINT32 contentLen)
{
    (VOID)connection;
    (VOID)arg;
    (VOID)hdrLen;
    (VOID)contentLen;
    struct pbuf *q;
    for (q = hdr; (q != NULL) && (q->payload != NULL); q = q->next) {
        HttpParse((CHAR *)q->payload, q->len);
    }
    return ERR_OK;
}

STATIC VOID HttpcResult(VOID *arg, httpc_result_t httpcResult, UINT32 rxContentLen, UINT32 srvRes, err_t err)
{
    (VOID)arg;
    (VOID)rxContentLen;
    (VOID)srvRes;
    (VOID)err;
    if (httpcResult != HTTPC_RESULT_OK) {
        printf("Http close failed result :%d.\n", httpcResult);
    }
    printf("Http client demo finished.\n");
}

STATIC httpc_connection_t g_setting = {
    .proxy_addr = {0},
    .proxy_port = 0,
    .use_proxy = 0,
    .headers_done_fn = HttpcHeaderDone,
    .result_fn = HttpcResult,
};

err_t HttpcGet(const CHAR *server, UINT16 port, const CHAR *url, altcp_recv_fn recv_fn)
{
    struct pbuf *q;
    httpc_state_t *states = NULL;
    (VOID)httpc_get_file_dns(server, port, url, &g_setting, recv_fn, NULL, &states);
    for(q = states->request; (q != NULL) && (q->payload != NULL); q = q->next) {
        HttpParse((CHAR *)q->payload, q->len);
    }
    return ERR_OK;
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Http client demo start to run.\n");
    (VOID)LOS_TaskDelay(HTTP_CLIENT_WAIT_TIME); // wait lwip dhcp get ip.
    (VOID)HttpcGet(DEFAULT_HOST, DEFAULT_PORT, DEFAULT_URL, (altcp_recv_fn)HttpcRecv);
}

VOID HttpClientDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TASK_STACK_SIZE;
    taskInitParam.pcName = "HttpClientDemoTask";
    taskInitParam.usTaskPrio = TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create http client demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
