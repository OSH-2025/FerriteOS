/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: LiteOS Shell Show Implementation File
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "show.h"
#include "shmsg.h"
#include "shcmd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC BOOL g_shellSourceFlag = FALSE;
#ifndef LOSCFG_SHELL_CONSOLE
/* If not using console, then allocated shell struct will store in this global variable */
STATIC ShellCB *g_shellCB = NULL;
#endif

STATIC UINT32 OsShellSourceInit(VOID)
{
    UINT32 ret;

    if (g_shellSourceFlag) {
        return LOS_OK;
    }

    ret = OsCmdInit();
    if (ret != LOS_OK) {
        return ret;
    }

    ret = OsShellSysCmdRegister();
    if (ret != LOS_OK) {
        OsCmdDeInit();
        return ret;
    }
    g_shellSourceFlag = TRUE;
    return LOS_OK;
}

STATIC VOID OsShellSourceDeInit(VOID)
{
    OsCmdDeInit();
    OsShellSysCmdUnregister();
    g_shellSourceFlag = FALSE;
}

STATIC UINT32 OsShellCreateTask(ShellCB *shellCB)
{
    UINT32 ret = ShellTaskInit(shellCB);
    if (ret != LOS_OK) {
        return ret;
    }

    ret = ShellEntryInit(shellCB);
    if (ret != LOS_OK) {
        (VOID)LOS_TaskDelete(shellCB->shellTaskHandle);
    }
    return ret;
}

STATIC INLINE UINT32 OsShellConsoleInit(INT32 consoleId, ShellCB *shellCB)
{
    g_shellCB = shellCB;
    return LOS_OK;
}

STATIC INLINE VOID OsShellConsoleDeinit(INT32 consoleId)
{
    g_shellCB = NULL;
}

UINT32 OsShellInit(INT32 consoleId)
{
    UINT32 ret = LOS_NOK;
    ShellCB *shellCB = NULL;

    if (OsShellSourceInit() != LOS_OK) {
        return ret;
    }

    shellCB = LOS_MemAlloc(m_aucSysMem0, sizeof(ShellCB));
    if (shellCB == NULL) {
        goto ERR_OUT_SOURCE_DEINIT;
    }
    (VOID)memset_s(shellCB, sizeof(ShellCB), 0, sizeof(ShellCB));

    if (OsShellConsoleInit(consoleId, shellCB) != LOS_OK) {
        goto ERR_OUT_FREE_MEM;
    }

    if (LOS_MuxCreate(&shellCB->keyMutex) != LOS_OK) {
        goto ERR_OUT_DEINIT_CONSOLE;
    }

    if (LOS_MuxCreate(&shellCB->historyMutex) != LOS_OK) {
        goto ERR_OUT_DELETE_KEY_MUX;
    }

    if (strncpy_s(shellCB->shellWorkingDirectory, PATH_MAX, "/", 1) != EOK) {
        goto ERR_OUT_DELETE_HISTORY_MUX;
    }

    ret = OsShellKeyInit(shellCB);
    if (ret != LOS_OK) {
        goto ERR_OUT_DELETE_HISTORY_MUX;
    }

    ret = OsShellCreateTask(shellCB);
    if (ret != LOS_OK) {
        goto ERR_OUT_DEINIT_KEY;
    }

    return LOS_OK;

ERR_OUT_DEINIT_KEY:
    OsShellKeyDeInit(shellCB);
ERR_OUT_DELETE_HISTORY_MUX:
    (VOID)LOS_MuxDelete(shellCB->historyMutex);
ERR_OUT_DELETE_KEY_MUX:
    (VOID)LOS_MuxDelete(shellCB->keyMutex);
ERR_OUT_DEINIT_CONSOLE:
    OsShellConsoleDeinit(consoleId);
ERR_OUT_FREE_MEM:
    (VOID)LOS_MemFree((VOID *)m_aucSysMem0, shellCB);
ERR_OUT_SOURCE_DEINIT:
    OsShellSourceDeInit();

    return ret;
}

INT32 OsShellDeinit(INT32 consoleId)
{
    ShellCB *shellCB = NULL;

#ifdef LOSCFG_SHELL_CONSOLE
    CONSOLE_CB *consoleCB = OsGetConsoleByID(consoleId);
    if (consoleCB == NULL) {
        PRINT_ERR("shell deinit error.\n");
        return -1;
    }

    shellCB = (ShellCB *)consoleCB->shellHandle;
    consoleCB->shellHandle = NULL;
#else
    shellCB = g_shellCB;
#endif
    ShellTaskDeinit(shellCB);

    return 0;
}

CHAR *OsShellGetWorkingDirectory(VOID)
{
    ShellCB *shellCB = NULL;

#ifdef LOSCFG_SHELL_CONSOLE
    CONSOLE_CB *consoleCB = OsGetConsoleByTaskID(LOS_CurTaskIDGet());
    if (consoleCB == NULL) {
        return NULL;
    }

    shellCB = (ShellCB *)consoleCB->shellHandle;
    if (shellCB == NULL) {
        return NULL;
    }
    return shellCB->shellWorkingDirectory;
#else
    shellCB = g_shellCB;
    return shellCB->shellWorkingDirectory;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
