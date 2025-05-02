/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Fs Init Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-01
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

#include "fs/fs_init.h"
#include "los_task.h"

#define FS_INIT_TASK_SIZE 2048
#define FS_INIT_TASK_PRIORITY 2

static uint32_t g_taskId;

static void FileSystemEntry(void)
{
#if defined LOSCFG_COMPONENTS_FS_SPIFFS && defined LOSCFG_COMPONENTS_FS_LITTLEFS
#error "Spiffs and Littlefs can't open at the same time, please check file system option."
#endif
#if defined LOSCFG_COMPONENTS_FS_SPIFFS || defined LOSCFG_COMPONENTS_FS_FATFS || \
    defined LOSCFG_COMPONENTS_FS_LITTLEFS || defined LOSCFG_COMPONENTS_FS_RAMFS
    int ret;
#endif
#ifdef LOSCFG_COMPONENTS_FS_SPIFFS
    struct spiffs_drv_t *spiffsConfig = SpiffsConfigGet();
    ret = SpiffsInit(0, spiffsConfig);
    if (ret != LOS_OK) {
        PRINT_ERR("Spiffs init failed.\n");
    }
    PRINT_INFO("Spiffs inited.\n");
#endif
#ifdef LOSCFG_COMPONENTS_FS_FATFS
    uint8_t driver;
    struct diskio_drv *fatfsConfig = FatfsConfigGet();
    ret = FatfsInit(0, fatfsConfig, &driver);
    if (ret != LOS_OK) {
        PRINT_ERR("Fatfs init failed.\n");
    }
    PRINT_INFO("Fatfs inited at driver:%d\n", driver);
#endif
#ifdef LOSCFG_COMPONENTS_FS_LITTLEFS
    struct lfs_config *littlefsConfig = LittlefsConfigGet();
    ret = LittlefsInit(0, littlefsConfig);
    if (ret != LOS_OK) {
        PRINT_ERR("Littlefs init failed.\n");
    }
    PRINT_INFO("Littlefs inited.\n");
#endif
#ifdef LOSCFG_COMPONENTS_FS_RAMFS
    RamfsInit();
    ret = RamfsMount(RAMFS_PATH, RAMFS_SIZE);
    if (ret != LOS_OK) {
        PRINT_ERR("Ramfs init failed.\n");
    }
    PRINT_INFO("Ramfs inited.\n");
#endif
}

void FileSystemInit(void)
{
    uint32_t ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }

    taskInitParam.usTaskPrio = FS_INIT_TASK_PRIORITY;
    taskInitParam.pcName = "FileSystemTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)FileSystemEntry;
    taskInitParam.uwStackSize = FS_INIT_TASK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_taskId, &taskInitParam);
    if (ret != LOS_OK) {
        PRINT_ERR("Create filesystem init task failed.\n");
    }
}
