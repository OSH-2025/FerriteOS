/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Uriparser Demo Implementation
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

#include <uriparser/Uri.h>
#include <arpa/inet.h>
#include "los_task.h"

#define TASK_STACK_SIZE       0x800
#define TASK_PRIORITY         5

STATIC UINT32 g_demoTaskId;

#define RANGE(x) (INT32)((x).afterLast - (x).first), ((x).first)

INT32 UriParse(const CHAR *uriStr)
{
    INT32 ret = EXIT_SUCCESS;

    UriParserStateA state = {0};
    UriUriA uri;
    CHAR ipstr[INET6_ADDRSTRLEN];

    state.uri = &uri;
    printf("uri:          %s\n", uriStr);
    if (uriParseUriA(&state, uriStr) != URI_SUCCESS) {
        /* Failure */
        printf("Failure:      %s @ '%.18s' (#%lu)\n",
               (state.errorCode == URI_ERROR_SYNTAX)
               ? "syntax"
               : (state.errorCode == URI_ERROR_MALLOC)
               ? "not enough memory"
               : "liburiparser bug (please report)",
               state.errorPos,
               (UINT32)(state.errorPos - uriStr));
        ret = EXIT_FAILURE;
        uriFreeUriMembersA(&uri);
        return ret;
    }
    if (uri.scheme.first) {
        printf("scheme:       %.*s\n", RANGE(uri.scheme));
    }
    if (uri.userInfo.first) {
        printf("userInfo:     %.*s\n", RANGE(uri.userInfo));
    }
    if (uri.hostText.first) {
        printf("hostText:     %.*s\n", RANGE(uri.hostText));
    }
    if (uri.hostData.ip4) {
        inet_ntop(AF_INET, uri.hostData.ip4->data, ipstr, sizeof(ipstr));
        printf("hostData.ip4: %s\n", ipstr);
    }
    if (uri.hostData.ip6) {
        inet_ntop(AF_INET6, uri.hostData.ip6->data, ipstr, sizeof(ipstr));
        printf("hostData.ip6: %s\n", ipstr);
    }
    if (uri.portText.first) {
        printf("portText:     %.*s\n", RANGE(uri.portText));
    }
    if (uri.pathHead) {
        const UriPathSegmentA *p = uri.pathHead;
        for (; p; p = p->next) {
            printf(" .. pathSeg:  %.*s\n", RANGE(p->text));
        }
    }
    if (uri.query.first) {
        printf("query:        %.*s\n", RANGE(uri.query));
    }
    if (uri.fragment.first) {
        printf("fragment:     %.*s\n", RANGE(uri.fragment));
    }
    const CHAR *absolutePathLabel = "absolutePath: ";
    printf("%s%s\n", absolutePathLabel,
           (uri.absolutePath == URI_TRUE) ? "true" : "false");
    if (uri.hostText.first) {
        printf("%*s%s\n", (INT32)strlen(absolutePathLabel), "",
               "(always false for URIs with host)");
    }
    printf("\n");

    uriFreeUriMembersA(&uri);
    return ret;
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 ret;
    printf("Uriparser demo start to run.\n");
    const CHAR *demoUri = "https://www.huawei.com/minisite/liteos/cn/index.html";
    ret = UriParse(demoUri);
    if (ret != EXIT_SUCCESS) {
        printf("Uriparser failed :%d.\n", ret);
    }
    printf("Uriparser demo finished.\n");
}

VOID UriparserDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TASK_STACK_SIZE;
    taskInitParam.pcName = "UriparserDemoTask";
    taskInitParam.usTaskPrio = TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create uriparser demo task failed.\n");
    }
}
