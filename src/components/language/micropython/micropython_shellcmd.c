/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Micropython Shell Command Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-22
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
#include "mpconfig.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_COMPONENTS_FS_RAMFS
#define PYTHON_TEST_FILE "/ramfs/hello.py"
#define BUFFER_SIZE      256
#endif

extern int python_main(int argc, char **argv);

#ifdef LOSCFG_COMPONENTS_FS_RAMFS
static int HelloWorld(void)
{
    int fd;
    char data[] = {
        "print(\"Hello, wlorld! I'm Micropython.\")\n"
        "a = 8\n"
        "b = 512\n"
        "c = a + b\n"
        "print(c)\n"
        "print(b\"bytes 3344\x01\")\n"
        "for i in range(4):\n"
        "    print(i)\n"
        "\n"
    };

    fd = open(PYTHON_TEST_FILE, O_RDONLY);
    if (fd > 0) {
        close(fd);
        return 0;
    }
    fd = open(PYTHON_TEST_FILE, O_CREAT | O_RDWR);
    if (fd < 0) {
        printf("Open file %s failed.\n", PYTHON_TEST_FILE);
        return -1;
    }
    write(fd, data, strlen(data));
    close(fd);
    return 0;
}
#endif

static void Usage(void)
{
    printf("\nUsage: micropython [options]\n");
    printf("Options:\n");
    printf("  -h, --help      print this help\n");
    printf("  -v, --version   print tool and library version\n");
    printf("  FILE            input python file\n");
}

static int ParameterAnalysis(int argc, char **argv)
{
    int i;

    if (argc != 2) {    // 2: Number of parameters
        Usage();
        return -1;
    }

    for (i = 1; i < argc; i++) {
        if (((strlen("-h") == strlen(argv[i])) && (strncmp("-h", argv[i], strlen("-h")) == 0)) ||
            ((strlen("--help") == strlen(argv[i])) && (strncmp("--help", argv[i], strlen("--help")) == 0))) {
            Usage();
            return 0;
        } else if (((strlen("-v") == strlen(argv[i])) && (strncmp("-v", argv[i], strlen("-v")) == 0)) ||
                   ((strlen("--version") == strlen(argv[i])) &&
                   (strncmp("--version", argv[i], strlen("--version")) == 0))) {
            printf("Version: %d.%d.%d\n", MICROPY_VERSION_MAJOR, MICROPY_VERSION_MINOR, MICROPY_VERSION_MICRO);
            return 0;
        }
    }
    return -1;
}

static int MicropythonShellCmd(int argc, char **argv)
{
    int i;
    char *tmpArgv[argc + 1];

    tmpArgv[0] = "micropython";
    for (i = 0; i < argc; i++) {
        tmpArgv[i + 1] = argv[i];
    }

#ifdef LOSCFG_COMPONENTS_FS_RAMFS
    HelloWorld();
    if ((strlen(PYTHON_TEST_FILE) == strlen(argv[0])) &&
        (strncmp(argv[0], PYTHON_TEST_FILE, strlen(PYTHON_TEST_FILE)) == 0)) {
        int fd = open(PYTHON_TEST_FILE, O_RDWR);
        if (fd > 0) {
            char buf[BUFFER_SIZE] = {0};
            if (read(fd , buf, BUFFER_SIZE) > 0) {
                printf("This test run the following script code: \n%s\n", buf);
            }
            close(fd);
        }
    }
#endif
    if (ParameterAnalysis(argc + 1, tmpArgv) == 0) {
        return 0;
    }

    int flag = open(argv[0], O_RDONLY);
    if (flag < 0) {
        printf("Can't open file %s\n", argv[0]);
        Usage();
    } else {
        close(flag);
        python_main(argc + 1, tmpArgv);
    }
    return 0;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(micropython_shellcmd, CMD_TYPE_EX, "micropython", XARGS, (CMD_CBK_FUNC)MicropythonShellCmd);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
