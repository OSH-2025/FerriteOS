/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Iniparser Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-03
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

#include "iniparser_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fcntl.h"
#include "los_task.h"
#include "los_vfs.h"
#include "iniparser.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define INI_FILE                  "/ramfs/example.ini"
#define BUF_LEN                   512
/* notfound Value to return in case of error */
#define ERROR_INT                 (-1)
#define ERROR_DOUBLE              (-1.0)
#define INIPARSER_TASK_PRIORITY   6
#define INIPARSER_TASK_STACK_SIZE 0x2000

STATIC UINT32 g_demoTaskId;

STATIC VOID CreateIniFile(VOID)
{
    INT32 fd;
    INT32 ret;
    UINT32 len;
    CHAR buff[BUF_LEN] = {0};

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
        "[hamburger]\n"
        "\n"
        "Lettuce   = yes ;\n"
        "Chickens  = TRUE ;\n"
        "Seafood   = 0 ;\n"
        "Cheese    = Non ;\n"
        "\n"
        "\n"
        "[Beer]\n"
        "\n"
        "Composition = Wheat ;\n"
        "Year        = 2021 ;\n"
        "Country     = China ;\n"
        "Alcohol     = 10.5  ;\n"
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

STATIC INT32 ParseIniFile(CHAR *iniName)
{
    dictionary *ini;

    /* Some temporary variables to hold query results */
    INT32 t;
    INT32 i;
    DOUBLE d;
    const CHAR *s = NULL;
    ini = iniparser_load(iniName);
    if (ini == NULL) {
        printf("Cannot parse file: %s.\n", iniName);
        return -1;
    }
    iniparser_dump(ini, stderr);

    /* Get hamburger attributes */
    printf("\nHamburger:\n");
    t = iniparser_getboolean(ini, "hamburger:lettuce", ERROR_INT);
    printf("Lettuce:     [%d]\n", t);
    t = iniparser_getboolean(ini, "hamburger:chickens", ERROR_INT);
    printf("Chickens:    [%d]\n", t);
    t = iniparser_getboolean(ini, "hamburger:Seafood", ERROR_INT);
    printf("Seafood:     [%d]\n", t);
    t = iniparser_getboolean(ini, "hamburger:cheese", ERROR_INT);
    printf("Cheese:      [%d]\n", t);

    /* Get beer attributes */
    printf("\nBeer:\n");
    s = iniparser_getstring(ini, "beer:composition", NULL);
    printf("Composition: [%s]\n", s ? s : "UNDEF");
    i = iniparser_getint(ini, "beer:year", ERROR_INT);
    printf("Year:        [%d]\n", i);
    s = iniparser_getstring(ini, "beer:country", NULL);
    printf("Country:     [%s]\n", s ? s : "UNDEF");
    d = iniparser_getdouble(ini, "beer:alcohol", ERROR_DOUBLE);
    printf("Alcohol:     [%g]\n", d);
    iniparser_freedict(ini);
    return LOS_OK;
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 ret;

    printf("Iniparser demo task start to run.\n");

    CreateIniFile();
    ret = ParseIniFile(INI_FILE);
    if (ret != LOS_OK) {
        printf("Parse ini file failed.");
        return;
    }
    printf("Iniparser demo task finished.\n");
}

VOID IniparserDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = INIPARSER_TASK_PRIORITY;
    taskInitParam.pcName = "IniparserDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = INIPARSER_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create iniparser demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
