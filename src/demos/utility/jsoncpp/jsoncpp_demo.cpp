/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Jsoncpp Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-15
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

#include "jsoncpp_demo.h"
#include "json/json.h"
#include <iostream>
#ifdef LOSCFG_LIB_CPP_EXTEND
#include <thread>
#endif /* LOSCFG_LIB_CPP_EXTEND */

#ifndef LOSCFG_LIB_CPP_EXTEND
#include "los_task.h"

#define JSONCPP_TASK_STACK_SIZE    0x1000
#define JSONCPP_TASK_PRIORITY      5

static uint32_t g_demoTaskId;
#endif /* LOSCFG_LIB_CPP_EXTEND */

using namespace std;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static void JsoncppReadDemo(void)
{
    const string jsonStr = R"({"date": 2021, "name": "LiteOS"})";
    const auto jsonStrLength = static_cast<int>(jsonStr.length());
    JSONCPP_STRING err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    const unique_ptr<Json::CharReader> reader(builder.newCharReader());

    bool ret = reader->parse(jsonStr.c_str(), jsonStr.c_str() + jsonStrLength, &root, &err);
    if (ret != true) {
        cout << "Jsoncpp parse string fail." << endl;
        return;
    }
    const string name = root["name"].asString();
    const int date = root["date"].asInt();
    cout << "Jsoncpp read form string:" << endl;
    cout << "name : " << name << endl;
    cout << "date : " << date << endl;
}

static void JsoncppWriteDemo(void)
{
    Json::Value root;
    Json::Value data;
    Json::StreamWriterBuilder builder;

    root["name"] = "LiteOS";
    data["date"] = 2021; /* 2021 is a test num. */
    root["data"] = data;

    const string jsonStr = Json::writeString(builder, root);
    cout << "Jsoncpp write to string:" << endl;
    cout << jsonStr << endl;
}

static void DemoTaskEntry(void)
{
    cout << "Jsoncpp read demo start to run." << endl;
    JsoncppReadDemo();
    cout << "Jsoncpp write demo start to run." << endl;
    JsoncppWriteDemo();
    cout << "Jsoncpp demo finished." << endl;
}

void JsoncppDemoTask(void)
{
#ifdef LOSCFG_LIB_CPP_EXTEND
    thread jsonDemo(DemoTaskEntry);
    jsonDemo.join();
#else
    uint32_t ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = JSONCPP_TASK_STACK_SIZE;
    taskInitParam.pcName = "JsoncppDemoTask";
    taskInitParam.usTaskPrio = JSONCPP_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        cout << "Create jsoncpp demo task failed." << endl;
    }
#endif /* LOSCFG_LIB_CPP_EXTEND */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
