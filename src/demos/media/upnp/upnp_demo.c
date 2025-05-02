/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Upnp Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-21
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

#include "los_task.h"
#include "upnp.h"
#include "upnp_demo.h"

#define UPNP_TASK_DELAY         3000
#define UPNP_TASK_STACK_SIZE    0x4000
#define UPNP_TASK_PRIORITY      6
#define MAX_SEARCH_TIME         5

STATIC UINT32 g_demoTaskId;
UpnpClient_Handle g_demoCpHandle = -1;
const CHAR G_DEMO_DEVICE_TYPE[] = "urn:schemas-upnp-org:device:mydevice:1";

INT32 CtrlPointCallbackEventHandler(Upnp_EventType eventType, void *event, void *cookie)
{
    (void)event;
    (void)cookie;
    switch (eventType) {
        /* SSDP Stuff */
        case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
        case UPNP_DISCOVERY_SEARCH_RESULT:
            printf("Search device.\n");
            break;
        case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
            printf("Discovery advertisement byebye.\n");
            break;
        case UPNP_DISCOVERY_SEARCH_TIMEOUT:
            break;
        default:
            break;
    }
    return 0;
}

VOID UpnpDemoEntry(VOID)
{
    INT32 ret;
    UINT16 port = 0;
    CHAR *ipAddr = NULL;

    (void)LOS_TaskDelay(UPNP_TASK_DELAY);
    printf("\nUpnp demo task start to run.\n");
    ret = UpnpInit(ipAddr, port);
    if (ret != UPNP_E_SUCCESS) {
        printf("Liteos start upnp failed.\n");
        (void)UpnpFinish();
        return;
    }

    ret = UpnpRegisterClient(CtrlPointCallbackEventHandler, &g_demoCpHandle, &g_demoCpHandle);
    if (ret != UPNP_E_SUCCESS) {
        printf("Registering control point failed.\n");
        (void)UpnpFinish();
        return;
    }

    ret = UpnpSearchAsync(g_demoCpHandle, MAX_SEARCH_TIME, G_DEMO_DEVICE_TYPE, NULL);
    if (ret != UPNP_E_SUCCESS) {
        printf("Sending search request failed.\n");
    }
    printf("Upnp demo task finished.\n");
    (void)UpnpFinish();
}

VOID UpnpDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    taskInitParam.usTaskPrio = UPNP_TASK_PRIORITY;
    taskInitParam.pcName = "UpnpDemoEntry";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)UpnpDemoEntry;
    taskInitParam.uwStackSize = UPNP_TASK_STACK_SIZE;

    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create upnp demo task failed.\n");
        return;
    }
}
