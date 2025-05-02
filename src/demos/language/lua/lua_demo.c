/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Lua Demo Implementation
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

#include "lua_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "lua.h"
#include "los_vfs.h"
#include "los_ramfs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LUA_ARGV              "lua"
#define LUA_ARGC              2
#define LUA_FILE_NAME         "/ramfs/test.lua"
#define LUA_TASK_PRIORITY     7
#define LUA_TASK_STACK_SIZE   0x3000

STATIC UINT32 g_demoTaskId;

CHAR g_luaData[] = "function max(a, b)\n\
    if (a > b) then\n\
        c = a;\n\
    else\n\
        c = b;\n\
    end\n\
    return c;\n\
end\n\
\n\
print(\"the max num is \", max(1, 2))\n\
print(\"the max num is \", max(4, 3))\n\
";

VOID LuaDemoEntry(VOID)
{
    INT32 ret;
    CHAR *tmpArgv[LUA_ARGC] = {0};
    INT32 fd = 0;

    printf("Lua demo task start to run.\n");

    fd = open(LUA_FILE_NAME, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd < 0) {
        printf("Create lua file failed.\n");
        return;
    }
    ret = write(fd, g_luaData, strlen(g_luaData));
    if (ret < 0) {
        printf("Write lua file failed.\n");
        (VOID)close(fd);
        return;
    }
    (VOID)close(fd);

    tmpArgv[0] = LUA_ARGV;
    tmpArgv[1] = LUA_FILE_NAME;
    printf("Run lua file.\n");
    ret = LuaMain(LUA_ARGC, tmpArgv);
    if (ret == LOS_OK) {
        printf("Run lua file finished.\n");
    } else {
        printf("Run Lua file failed.\n");
    }
    printf("Lua demo task finished.\n");
}

VOID LuaDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LuaDemoEntry;
    taskInitParam.uwStackSize = LUA_TASK_STACK_SIZE;
    taskInitParam.pcName = "LuaDemoTask";
    taskInitParam.usTaskPrio = LUA_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create lua demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
