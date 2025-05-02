/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Json Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-05
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

#include "json_c_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include "los_vfs.h"
#include "json_object.h"
#include "json_tokener.h"
#include "json_util.h"
#include "json.h"
#include "json_inttypes.h"
#include "json_object_private.h"
#include "json_visit.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define JSON_TEST_NUM          10
#define JSON_TASK_PRIORITY     3
#define JSON_TASK_STACK_SIZE   0x1000

STATIC UINT32 g_demoTaskId;

const CHAR g_demoJsonStr[] = "{"
    "'name' : 'test',"
    "'age' : 19,"
    "'gender' : 'male',"
    "'occupation' : 'programmer',"
    "'conutry' : 'China',"
    "'': 0, "
    "}";

STATIC VOID JsonParseDemo(VOID)
{
    struct json_object *js1 = NULL;
    struct json_object *js2 = NULL;

    js1 = json_tokener_parse(g_demoJsonStr);
    if (js1 == NULL) {
        return;
    }
    printf("%s\n", json_object_get_string(js1));
    printf("Json-c parse string successfully.\n");

    /* Test empty string */
    js2 = NULL;
    if (json_pointer_get(js1, "", NULL) != 0) {
        printf("Get 1\n");
        return;
    }
    if (json_pointer_get(js1, "", &js2) != 0) {
        printf("Get 2\n");
        return;
    }
    if (json_object_equal(js2, js1) != 1) {
        printf("Get 3\n");
        return;
    }
    if (json_object_put(js1) != 1) {
        printf("Json-c test empty string failed.\n");
        return;
    }
    printf("Json-c test empty string successfully.\n");
}

STATIC VOID JsonSetdemo(VOID)
{
    struct json_object *js1 = NULL;

    js1 = json_tokener_parse(g_demoJsonStr);
    if (js1 == NULL) {
        return;
    }

    if (json_pointer_set(&js1, "", json_object_new_int(JSON_TEST_NUM)) != 0) {
        return;
    }
    if (json_object_get_int(js1) != JSON_TEST_NUM) {
        printf("%s\n", json_object_get_string(js1));
    }
    if (json_object_put(js1) != 1) {
        printf("Json get test failed.\n");
        return;
    }
    printf("Json-c get test successfully.\n");
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Json-c demo task start to run.\n");
    JsonParseDemo();
    JsonSetdemo();
    printf("Json-c demo task finished.\n");
}

VOID JsonDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam = {0};

    taskInitParam.usTaskPrio = JSON_TASK_PRIORITY;
    taskInitParam.pcName = "JsonDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = JSON_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create json-c demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
