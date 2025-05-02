/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Jsmn Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-05-14
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

#include "jsmn_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "los_task.h"
#include "jsmn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TOK_LEN                128 /* We expect no more than 128 tokens */
#define ARR_STR_START          2
#define JSMN_TASK_PRIORITY     3
#define JSMN_TASK_STACK_SIZE   0x1000

STATIC UINT32 g_demoTaskId;

STATIC const CHAR *g_demoJsonString =
    "{\"name\": \"lanche\", \"sex\": male, \"age\": 26,\n  "
    "\"hobbies\": [\"draw\", \"play games\", \"travles\", \"runing\"]}";

STATIC INT32 JsonEq(const CHAR *json, jsmntok_t *tok, const CHAR *s)
{
    if ((tok->type == JSMN_STRING) && ((INT32)strlen(s) == tok->end - tok->start)
        && (strncmp(json + tok->start, s, tok->end - tok->start)) == 0) {
        return 0;
    }
    return -1;
}

STATIC VOID JsonArray(INT32 *i, jsmntok_t *tok)
{
    INT32 j;
    printf("- hobbies:\n");
    if (tok[*i + 1].type != JSMN_ARRAY) {
        return;
    }
    /* Parse the json array */
    for (j = 0; j < tok[*i + 1].size; j++) {
        jsmntok_t *g = &tok[*i + j + ARR_STR_START];
        printf("  * %.*s\n", g->end - g->start, g_demoJsonString + g->start);
    }
    *i += tok[*i + 1].size + 1;
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 i;
    INT32 ret;
    jsmn_parser p;
    jsmntok_t t[TOK_LEN];

    printf("Jsmn demo task start to run.\n");
    jsmn_init(&p);
    ret = jsmn_parse(&p, g_demoJsonString, strlen(g_demoJsonString), t, TOK_LEN);

    if ((ret < 1) || (t[0].type != JSMN_OBJECT)) {
        printf("Failed to parse JSON: %d\n", ret);
        return;
    }

    for (i = 1; i < ret; i++) {
        if (JsonEq(g_demoJsonString, &t[i], "name") == 0) {
            printf("- name: %.*s\n", t[i + 1].end - t[i + 1].start,
                   g_demoJsonString + t[i + 1].start);
            i++;
        } else if (JsonEq(g_demoJsonString, &t[i], "sex") == 0) {
            printf("- sex: %.*s\n", t[i + 1].end - t[i + 1].start,
                   g_demoJsonString + t[i + 1].start);
            i++;
        } else if (JsonEq(g_demoJsonString, &t[i], "age") == 0) {
            printf("- age: %.*s\n", t[i + 1].end - t[i + 1].start,
                   g_demoJsonString + t[i + 1].start);
            i++;
        } else if (JsonEq(g_demoJsonString, &t[i], "hobbies") == 0) {
            JsonArray(&i, t);
        } else {
            printf("Unexpected str: %.*s\n", t[i].end - t[i].start,
                   g_demoJsonString + t[i].start);
        }
    }
    printf("Jsmn demo task finished.\n");
}

VOID JsmnDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam = {0};

    taskInitParam.usTaskPrio = JSMN_TASK_PRIORITY;
    taskInitParam.pcName = "JsmnDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = JSMN_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create jsmn demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
