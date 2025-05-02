/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sqlite Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-24
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

#include "sqlite_demo.h"
#include "stdio.h"
#include "los_task.h"
#include "los_vfs.h"
#include "unistd.h"
#include "fcntl.h"
#include "sqlite3.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SQLITE_TASK_PRIORITY    8
#define SQLITE_TASK_STACK_SIZE  0x2000

#define DATEBASE_NAME  "/fatfs/database.db"
#define SQL_BUF_LEN    128

STATIC UINT32 g_demoTaskId;

STATIC INT32 ShowCallback(void *para, int num, char **values, char **name)
{
    INT32 i;
    (VOID)para;
    for (i = 0; i < num; i++) {
        if ((i % 2) == 0) { /* 2: even number */
            printf("%s: %s; ", name[i], values[i]);
        } else {
            printf("%s: %s\n", name[i], values[i]);
        }
    }
    return 0;
}

STATIC INT32 InsertValue(sqlite3 *db, const char *name, const char *number)
{
    CHAR *errMsg = NULL;
    INT32 rc;
    CHAR buf[SQL_BUF_LEN] = {0};

    if (sprintf_s(buf, sizeof(buf), "insert into userinfo values('%s','%s');", name, number) <= 0) {
        printf("Execute sprintf_s failed.\n");
    }

    rc = sqlite3_exec(db, buf, NULL, NULL, &errMsg);
    if (rc != SQLITE_OK) {
        printf("%s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

STATIC INT32 DemoTaskEntry(VOID)
{
    sqlite3 *db = NULL;
    CHAR *errMsg = NULL;

    printf("Sqlite demo task start to run.\n");

    LOS_TaskLock();
    if (sqlite3_open(DATEBASE_NAME, &db) != SQLITE_OK) {
        printf("Open database %s failed, error: %s.\n", DATEBASE_NAME, sqlite3_errmsg(db));
        LOS_TaskUnlock();
        return -1;
    }

    if (sqlite3_exec(db, "drop table userinfo;", NULL, NULL, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
    }

    if (sqlite3_exec(db, "create table userinfo(name,number);", NULL, NULL, &errMsg) != SQLITE_OK) {
        printf("%s\n", errMsg);
        (void)sqlite3_close(db);
        sqlite3_free(errMsg);
        LOS_TaskUnlock();
        return -1;
    }

    if (InsertValue(db, "huawei1", "20210324") != SQLITE_OK) {
        (void)sqlite3_close(db);
        LOS_TaskUnlock();
        return -1;
    }

    if (InsertValue(db, "huawei2", "20210401") != SQLITE_OK) {
        (void)sqlite3_close(db);
        LOS_TaskUnlock();
        return -1;
    }

    if (sqlite3_exec(db, "select *from userinfo;", ShowCallback, NULL, &errMsg) != SQLITE_OK) {
        printf("%s\n", errMsg);
        (void)sqlite3_close(db);
        sqlite3_free(errMsg);
        LOS_TaskUnlock();
        return -1;
    }

    (void)sqlite3_close(db);
    if (errMsg != NULL) {
        sqlite3_free(errMsg);
    }

    LOS_TaskUnlock();
    printf("Sqlite demo task finished.\n");

    return 0;
}

VOID SqliteDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    INT32 mRet = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (mRet != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = SQLITE_TASK_PRIORITY;
    taskInitParam.pcName = "SqliteDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = SQLITE_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create sqlite Demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
