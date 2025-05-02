/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Jerry Shell Command Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-16
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

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif

#include "unistd.h"
#include "fcntl.h"
#include "jerry_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_COMPONENTS_FS_RAMFS
static int HelloWorld(void)
{
    int fd;
    char data[] = "print(\"Hello, wlorld! I'm JerryScript.\");";
    char fileName[] = "/ramfs/hello.js";

    fd = open(fileName, O_RDONLY);
    if (fd > 0) {
        close(fd);
        return 0;
    }
    fd = open(fileName, O_CREAT | O_RDWR);
    if (fd < 0) {
        printf("Open file %s failed.\n", fileName);
        return -1;
    }
    write(fd, data, strlen(data));
    close(fd);
    return 0;
}
#endif

static int JerryShellCmd(int argc, char **argv)
{
    int i;
    char *tmpArgv[argc + 1];

    tmpArgv[0] = "jerry";
    for (i = 0; i < argc; i++) {
        tmpArgv[i + 1] = argv[i];
    }

#ifdef LOSCFG_COMPONENTS_FS_RAMFS
    HelloWorld();
#endif

    JerryMain(argc + 1, tmpArgv);
    return 0;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(jerry_shellcmd, CMD_TYPE_EX, "jerry", XARGS, (CMD_CBK_FUNC)JerryShellCmd);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

