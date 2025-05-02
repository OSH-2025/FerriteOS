/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Inih Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-08
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

#include "inih_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fcntl.h"
#include "los_task.h"
#include "los_vfs.h"
#include "ini.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define INI_FILE              "/ramfs/test.ini"
#define INI_STRING_DATE       "[date]\nauthor=2021\nmonth=07\nday=12"
#define INI_STRING_TIME       "[time]\nhour=16\nmin=02\nsec=31"
#define BUF_LEN               256
#define SECTION_LEN           50
#define INIH_TASK_PRIORITY    6
#define INIH_TASK_STACK_SIZE  0x2000

STATIC UINT32 g_demoTaskId;
CHAR g_prevSection[SECTION_LEN] = { 0 };

typedef struct {
    UINT32 date;
    const CHAR *components;
    const CHAR *author;
    const CHAR *description;
} IniFile;

STATIC VOID CreateIniFile(VOID)
{
    INT32 fd;
    INT32 ret;
    UINT32 len;
    CHAR buff[BUF_LEN] = { 0 };

    fd = open(INI_FILE, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd < 0) {
        printf("Open file %s failed.\n", INI_FILE);
        return;
    }

    ret = sprintf_s(buff, sizeof(buff),
        "#\n"
        "# This is an example of ini file\n"
        "#\n"
        "\n"
        "[INIH]\n"
        "\n"
        "components  = inih ;\n"
        "author      = tester001 ;\n"
        "date        = 20210708 ;\n"
        "description = Inih Demo Implementation ;\n"
        "\n");
    if (ret < 0) {
        printf("Execute sprintf_s failed.\n");
        (VOID)close(fd);
        return;
    }
    len = sizeof(buff) - 1;
    ret = write(fd, buff, len);
    if (ret < 0) {
        printf("Write file %s failed.\n", INI_FILE);
        (VOID)close(fd);
        return;
    }
    (VOID)close(fd);
}

INT32 FileHandler(VOID *user, const CHAR *section, const CHAR *name,
                  const CHAR *value)
{
    if (user == NULL) {
        return LOS_OK;
    }
    IniFile *pConfig = (IniFile *)user;

    if ((value == NULL) || (section == NULL) || (name == NULL)) {
        return LOS_OK;
    }
    if (!strcmp(section, "INIH") && !strcmp(name, "components")) {
        pConfig->components = strdup(value);
    } else if (!strcmp(section, "INIH") && !strcmp(name, "author")) {
        pConfig->author = strdup(value);
    } else if (!strcmp(section, "INIH") && !strcmp(name, "date")) {
        pConfig->date = atoi(value);
    } else if (!strcmp(section, "INIH") && !strcmp(name, "description")) {
        pConfig->description = strdup(value);
    } else {
        /* unknown section/name, error */
        return LOS_OK;
    }
    return LOS_NOK;
}

INT32 StringHandler(VOID *user, const CHAR *section, const CHAR *name,
                    const CHAR *value)
{
    (VOID)user;
    INT32 ret;
    if (section == NULL) {
        return LOS_OK;
    }
    if (strcmp(section, g_prevSection)) {
        printf("[%s]\n", section);
        ret = memcpy_s(g_prevSection,sizeof(g_prevSection)/sizeof(CHAR), section, SECTION_LEN);
	    if (ret != EOK) {
            return ret;
	    }
        g_prevSection[sizeof(g_prevSection) - 1] = '\0';
    }
    printf("%s = %s;\n", name, value);
    return LOS_NOK;
}

VOID StringParseDemo(const CHAR *string)
{
    STATIC UINT32 num;
    INT32 ret;

    g_prevSection[0] = '\0';
    ret = ini_parse_string(string, StringHandler, &num);
    if (ret != LOS_OK) {
        printf("Parse string failed, ret = %d\n", ret);
    }
}


STATIC VOID DemoTaskEntry(VOID)
{
    printf("Inih demo task start to run.\n");
    IniFile config = { 0 };
    CreateIniFile();

    if (ini_parse(INI_FILE, FileHandler, &config) < 0) {
        printf("Can't load %s\n", INI_FILE);
        return;
    }
    printf("[INIH]\ncomponents = %s\nauthor = %s\ndate = %d\ndescription = %s\n\n",
           config.components, config.author, config.date, config.description);

    StringParseDemo(INI_STRING_DATE);
    StringParseDemo(INI_STRING_TIME);

    if (config.components != NULL) {
        free((VOID *)config.components);
    }
    if (config.author != NULL) {
        free((VOID *)config.author);
    }
    if (config.description != NULL) {
        free((VOID *)config.description);
    }
    printf("Inih demo task finished.\n");
}

VOID InihDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = INIH_TASK_PRIORITY;
    taskInitParam.pcName = "InihDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = INIH_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create inih demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
