/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Libxml2 Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-23
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

#include "libxml2_demo.h"
#include "stdio.h"
#include "errno.h"
#include "los_base.h"
#include "los_task.h"
#include "los_config.h"
#include "libxml/xmlmemory.h"
#include "libxml/parser.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define XML2_TASK_PRIORITY      6
#define XML2_TASK_STACK_SIZE    0x2000
#define FATFS_MAIN_DIR          "/fatfs"
#define XML2_FILENAME           "/fatlos_test.xml"

STATIC UINT32 g_demoTaskId;

STATIC INT32 DemoTaskEntry(VOID)
{
    xmlDocPtr doc;
    xmlNodePtr curNode;
    xmlChar *tmpStr = NULL;

    printf("Libxml2 demo task start to run.\n");

    doc = xmlParseFile(XML2_FILENAME);
    if (doc == NULL) {
        printf("Parsefile failed.\n");
        return LOS_NOK;
    }

    curNode = xmlDocGetRootElement(doc);
    if ((curNode == NULL) || (curNode->name == NULL)) {
        printf("Getrootelement failed.\n");
        xmlFreeDoc(doc);
        return LOS_NOK;
    }

    if (xmlStrcmp(curNode->name, (const xmlChar *) "info")) {
        printf("Root element [%s] is not info.\n", curNode->name);
        return LOS_NOK;
    }

    curNode = curNode->xmlChildrenNode;

    while (curNode != NULL) {
        if (!xmlStrcmp(curNode->name, (const xmlChar *) "name")) {
            tmpStr = xmlNodeGetContent(curNode);
            PRINTK("name: %s \n", tmpStr);
            xmlFree(tmpStr);
        }

        if (!xmlStrcmp(curNode->name, (const xmlChar *) "addr")) {
            tmpStr = xmlNodeGetContent(curNode);
            PRINTK("addr: %s \n", tmpStr);
            xmlFree(tmpStr);
        }

        curNode = curNode->next;
    }

    xmlFreeDoc(doc);
    printf("Libxml2 demo task finished.\n");

    return LOS_OK;
}

VOID Libxml2DemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = XML2_TASK_STACK_SIZE;
    taskInitParam.pcName = "Libxml2DemoTask";
    taskInitParam.usTaskPrio = XML2_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create libxml2 demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
