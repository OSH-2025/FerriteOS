/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Curl Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-05
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

#include "curl_demo.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "los_task.h"
#include "los_vfs.h"
#include "fcntl.h"
#include "curl/curl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CURL_TASK_STACK_SIZE  0x2000
#define CURL_TASK_PRIORITY    8

#define URL_ADDRESS           "192.168.3.37"
#define FILE_PATH             "/fatfs/curl.txt"
#define BUFF_LEN              1024

STATIC UINT32 g_demoTaskId;

STATIC INT32 DownloadCallback(const VOID *buffer, size_t size, size_t nMemByte, VOID *userFd)
{
    INT32 ret;
    INT32 fd = *(INT32 *)userFd;
    ret = write(fd, buffer, (size * nMemByte));
    return ret;
}

STATIC INT32 DemoTaskEntry(VOID)
{
    INT32 fd = -1;
    CHAR buff[BUFF_LEN];
    CURL *curl = NULL;
    CURLcode result = CURLE_OK;
    long httpCode = 0;

    printf("Curl demo task start to run.\n");

    fd = open(FILE_PATH, O_CREAT | O_RDWR | O_TRUNC, 0664); /* 0664: file permission */
    if (fd < 0) {
        printf("Open file %s failed.\n", FILE_PATH);
        return -1;
    }

    (VOID)curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl != NULL) {
        (VOID)curl_easy_setopt(curl, CURLOPT_URL, URL_ADDRESS);
        (VOID)curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadCallback);
        (VOID)curl_easy_setopt(curl, CURLOPT_WRITEDATA, (VOID *)&fd);
        (VOID)curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        (VOID)curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        (VOID)curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        (VOID)curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);

        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            printf("Execute curl_easy_perform failed.\n");
        }
        (VOID)curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        (VOID)memset_s(buff, BUFF_LEN, 0, BUFF_LEN);
        buff[BUFF_LEN - 1] = '\0';
        (VOID)lseek(fd, 0, SEEK_SET);
        if ((result == CURLE_OK) && (read(fd, buff, BUFF_LEN - 1) <= 0)) {
            printf("Read file %s failed, htttCode: %ld.\n", FILE_PATH, httpCode);
        } else {
            printf("*** readed %s %u data ***\r\n%s\n"
                   "**************************************\r\n", FILE_PATH, strlen(buff), buff);
        }
    }

    (VOID)close(fd);
    curl_global_cleanup();
    printf("Curl demo task finished.\n");
    return (INT32)result;
}

VOID CurlDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = CURL_TASK_PRIORITY;
    taskInitParam.pcName = "CurlDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = CURL_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create curl demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
