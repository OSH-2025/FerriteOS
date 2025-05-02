/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Bidireference Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-20
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

#include "bidireference_demo.h"
#include <string.h>
#include "bidirefp.h"
#include "los_ramfs.h"
#include "los_vfs.h"
#include "los_task.h"

#define INPUT_ARG                2
#define UNICODE_FILENAME         "/ramfs/unicode.txt"
#define BRACKETS_FILENAME        "/ramfs/BidiBrackets.txt"
#define BIDIREFC_TASK_PRIORITY   7
#define BIDIREFC_TASK_STACK_SIZE 2048

STATIC UINT32 g_demoTaskId;

CHAR g_demoUnicodeData[] = "2000;EN QUAD;Zs;0;WS;2002;;;;N;;;;;\n"
                           "2460;CIRCLED DIGIT ONE;No;0;ON;"
                           "<circle> 0031;;1;1;N;;;;;\n";
CHAR g_demoBidiRefBrackets[] = "0028; 0029; o # LEFT PARENTHESIS\n0029; 0028; c # RIGHT PARENTHESIS\n";

STATIC VOID DemoTaskEntry(VOID)
{
    FILE *fdi = NULL;
    CHAR *algorithmType[] = {"-u63", "-z", "-d3"};

    printf("Bidireference demo task start to run.\n");
    fdi = fopen(UNICODE_FILENAME, "w");
    if (fdi == NULL) {
        printf("Fopen %s failed.\n", UNICODE_FILENAME);
    }
    fwrite(g_demoUnicodeData, 1, strlen(g_demoUnicodeData) + 1, fdi);
    fclose(fdi);
    fdi = NULL;
    fdi = fopen(BRACKETS_FILENAME, "a+");
    if (fdi == NULL) {
        printf("Fopen %s failed.\n", BRACKETS_FILENAME);
    }
    fwrite(g_demoBidiRefBrackets, 1, strlen(g_demoBidiRefBrackets) + 1, fdi);
    fclose(fdi);
    EnterBidiAlgo(INPUT_ARG, algorithmType);
    printf("Bidireference demo task finished.\n");
}

VOID BidiRefDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = BIDIREFC_TASK_STACK_SIZE;
    taskInitParam.pcName = "BidiRefDemoTask";
    taskInitParam.usTaskPrio = BIDIREFC_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create bidireference demo task failed.\n");
    }
}
