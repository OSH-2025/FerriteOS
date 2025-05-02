/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tftp Server Shell Implementation
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

#include "shell_tftp_server.h"
#include "tftp_server.h"
#include "lwip/netdb.h"
#include "lwip/udp.h"
#include "fcntl.h"
#include "los_typedef.h"
#include "los_task.h"

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif

#define FS_PATH_MAX_LENGTH 32
#define FS_FILE_NAME_MAX_LENGTH 128

#define TFTP_SERVER_TASK_STACK_SIZE 2048
#define TFTP_SERVER_TASK_PRIORITY 6

#define TYPE_VAL_ARGS 2

STATIC INT32 g_fileFd = -1;
STATIC CHAR g_fsPath[FS_PATH_MAX_LENGTH];
STATIC UINT32 g_tftpServerTskHandle;
STATIC UINT8 g_isRuning = FALSE;

struct tftp_context g_ctx;

STATIC INT32 CheckPath(const CHAR *fname)
{
    if (strncmp(fname, "/fatfs/", strlen("/fatfs/")) == 0) {
      return 0;
    }
    if (strncmp(fname, "/ramfs/", strlen("/ramfs/")) == 0) {
      return 0;
    }
    if (strncmp(fname, "/littlefs/", strlen("/littlefs/")) == 0) {
      return 0;
    }
    if (strncmp(fname, "/spiffs/", strlen("/spiffs/")) == 0) {
      return 0;
    }

    return -1;
}

STATIC VOID* TftpOpen(const CHAR *fname, const CHAR *mode, UINT8 write)
{
    (VOID)mode;
    INT32 ret;
    CHAR fileName[FS_FILE_NAME_MAX_LENGTH];

    ret = sprintf_s(fileName, FS_FILE_NAME_MAX_LENGTH, "%s%s", g_fsPath, fname);
    if (ret <= 0) {
        printf("Execute sprintf_s file name failed.\n");
        return NULL;
    }
    if (CheckPath(fileName) < 0){
        printf("check is err.\n");
        return NULL;
    }
    if (write) {
        g_fileFd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 664);
    } else {
        g_fileFd = open(fileName, O_RDONLY);
    }
    return (VOID *)(&g_fileFd);
}

STATIC VOID TftpClose(VOID *handle)
{
    if (handle == NULL) {
        printf("File close handle error.\n");
        return;
    }
    (VOID)close((INT32)(*(INT32 *)handle));
    return;
}

STATIC INT32 TftpRead(VOID *handle, VOID *buf, INT32 bytes)
{
    if ((handle == NULL) || (buf == NULL)) {
        printf("File read handle error.\n");
        return -1;
    }
    return read(*(INT32 *)handle, buf, (size_t)bytes);
}

STATIC INT32 TftpWrite(VOID *handle, struct pbuf *p)
{
    INT32 ret;
    if (handle == NULL) {
        printf("File write handle is error.\n");
        return -1;
    }
    struct pbuf *pp = p;
    INT32 writeLen = 0;
    do {
        ret = write(*(INT32 *)handle, pp->payload, pp->len);
        if (ret <= 0) {
            break;
        }
        writeLen += ret;
        pp = pp->next;
    } while (pp != NULL);

    if (writeLen == 0) {
        return -1;
    }

    return writeLen;
}

UINT32 TftpServerContextInit(VOID)
{
    INT32 ret;
    /* registering callback functions with vfs interface. */
    ret = memset_s(&g_ctx, sizeof(struct tftp_context), 0, sizeof(struct tftp_context));
    if (ret != EOK) {
        g_isRuning = FALSE;
        return LOS_NOK;
    }
    g_ctx.open = TftpOpen;
    g_ctx.close = TftpClose;
    g_ctx.read = TftpRead;
    g_ctx.write = TftpWrite;
    if (tftp_init(&g_ctx) != LOS_OK) {
        g_isRuning = FALSE;
        return LOS_NOK;
    }
    printf("Tftp server start run.\n");
    g_isRuning = TRUE;
    return LOS_OK;
}


VOID TftpServerClose(VOID)
{
    tftp_cleanup();
}

STATIC VOID PrintfHelp(VOID)
{
    printf("usage:\nTransfer a file from/to tftp client.\n");
    printf("tftpServer -s -m \"file path\"\n");
    printf("    -s: start run tftp server.\n");
    printf("    -m: mount file system to file path.\n");
    printf("    stop: stop tftp server.\n");
    printf("example:\n");
    printf("    tftpServer -s -m /fatfs/ \n");
}

STATIC INT32 CheckParam(INT32 argc, const CHAR **argv)
{
    INT32 i = 0;
    while (i < argc) {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "help") == 0)) {
            PrintfHelp();
            return -1;
        }
        if (strcmp(argv[i], "-s") == 0) {
            if (g_isRuning) {
                printf("Tftp server is running.\n");
                return -1;
            }
            i += 1;
            continue;
        }
        if (strcmp(argv[i], "-m") == 0) {
            if (strlen(argv[i + 1]) > FS_PATH_MAX_LENGTH) {
                printf("Mount pathname too long.\n");
                return -1;
            }

            const CHAR *p = argv[i + 1];
            if ((p[0] != '/') || (strchr(p + 1, '/') == NULL)) {
                printf("Mount path format error.\n");
                return -1;
            }
            if ((strlen(g_fsPath) != 0) || (strcmp(g_fsPath, argv[i + 1]) == 0)) {
                printf("Tftp server is running.\n");
                return -1;
            }
            (VOID)memcpy_s(g_fsPath, FS_PATH_MAX_LENGTH, argv[i + 1], strlen(argv[i + 1]));
            i += TYPE_VAL_ARGS; /* Index of the next parameter */
            continue;
        }
        if (strcmp(argv[i], "stop") == 0) {
            if (g_isRuning) {
                TftpServerClose();
            }
            (VOID)memset_s(g_fsPath, FS_PATH_MAX_LENGTH, 0, FS_PATH_MAX_LENGTH);
            g_fileFd = -1;
            g_isRuning = FALSE;
            printf("Tftp server stoped.\n");
            return -1;
        }
        PrintfHelp();
        return -1;
    }
    return 0;
}

STATIC INT32 ShellTftpServer(INT32 argc, const CHAR **argv)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;
    if (CheckParam(argc, argv) < 0) {
        return -1;
    }

    ret = (UINT32)memset_s(&taskInitParam, sizeof(taskInitParam), 0, sizeof(taskInitParam));
    if (ret != EOK) {
        return -1;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)TftpServerContextInit;
    taskInitParam.uwStackSize = TFTP_SERVER_TASK_STACK_SIZE;
    taskInitParam.pcName = "TftpServer";
    taskInitParam.usTaskPrio = TFTP_SERVER_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_tftpServerTskHandle, &taskInitParam);
    return (INT32)ret;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(tftp_server_shellcmd, CMD_TYPE_EX, "tftpServer", 0, (CmdCallBackFunc)ShellTftpServer);
#endif /* LOSCFG_SHELL */
