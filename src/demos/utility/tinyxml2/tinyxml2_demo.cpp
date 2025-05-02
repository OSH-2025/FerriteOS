/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tinyxml2 Demo Implementation
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

#include "tinyxml2_demo.h"
#include <iostream>
#include "tinyxml2.h"
#include "los_vfs.h"
#ifdef LOSCFG_LIB_CPP_EXTEND
#include <thread>
#endif /* LOSCFG_LIB_CPP_EXTEND */


#ifndef LOSCFG_LIB_CPP_EXTEND
#include "los_task.h"

#define TINYXML2_TASK_STACK_SIZE 0x1000
#define TINYXML2_TASK_PRIORITY 5

static uint32_t g_demoTaskId;
#endif /* LOSCFG_LIB_CPP_EXTEND */

using namespace tinyxml2;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static uint32_t ReadXml(void)
{
    XMLDocument xmlDoc;
    if (xmlDoc.LoadFile("/ramfs/test.xml") != 0) {
        std::cout << "Load xml file failed." << std::endl;
        return (uint32_t)XML_ERROR_FILE_COULD_NOT_BE_OPENED;
    }
    XMLElement *xmlRoot = xmlDoc.RootElement();
    XMLElement *userNode = xmlRoot->FirstChildElement("user");
    const char *getSchool = userNode->Attribute("school");
    const char *getGrade = userNode->Attribute("grade");
    std::cout << "school : " << getSchool << std::endl;
    std::cout << "grade : " << getGrade << std::endl;

    const char *getName = userNode->FirstChildElement("name")->GetText();
    const char *getSex = userNode->FirstChildElement("sex")->GetText();
    const char *getAge = userNode->FirstChildElement("age")->GetText();
    std::cout << "name : " << getName << std::endl;
    std::cout << "sex : " << getSex << std::endl;
    std::cout << "age : " << getAge << std::endl;
    return (uint32_t)XML_SUCCESS;
}

static void DemoTaskEntry(void)
{
    std::cout << "Tinyxml2 demo start to run." << std::endl;

    XMLDocument doc;
    XMLDeclaration *declaration = doc.NewDeclaration();
    (void)doc.InsertFirstChild(declaration);

    XMLElement *root = doc.NewElement("root");
    (void)doc.InsertEndChild(root);

    XMLElement *userNode = doc.NewElement("user");
    userNode->SetAttribute("school", "guangming");
    userNode->SetAttribute("grade", "8");
    (void)root->InsertEndChild(userNode);

    XMLElement *name = doc.NewElement("name");
    XMLText *nameText = doc.NewText("xiaoqiang");
    (void)name->InsertFirstChild(nameText);
    (void)userNode->InsertEndChild(name);

    XMLElement *sex = doc.NewElement("sex");
    XMLText *sexText = doc.NewText("male");
    (void)sex->InsertFirstChild(sexText);
    (void)userNode->InsertEndChild(sex);

    XMLElement *age = doc.NewElement("age");
    XMLText *ageText = doc.NewText("14");
    (void)age->InsertFirstChild(ageText);
    (void)userNode->InsertEndChild(age);

    uint32_t ret = doc.SaveFile("/ramfs/test.xml");
    if (ret != XML_SUCCESS) {
        std::cout << "Save xml file failed." << std::endl;
        return;
    }
    ret = ReadXml();
    if (ret != XML_SUCCESS) {
        std::cout << "Read xml file failed." << std::endl;
        return;
    }
    std::cout << "Tinyxml2 demo finished." << std::endl;
}

void Tinyxml2DemoTask(void)
{
#ifdef LOSCFG_LIB_CPP_EXTEND
    std::thread tinyxml2Demo(DemoTaskEntry);
    tinyxml2Demo.join();
#else
    uint32_t ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TINYXML2_TASK_STACK_SIZE;
    taskInitParam.pcName = "Tinyxml2DemoTask";
    taskInitParam.usTaskPrio = TINYXML2_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        std::cout << "Create tinyxml2 demo task failed." << std::endl;
    }
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
