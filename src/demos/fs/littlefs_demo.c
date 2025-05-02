/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Little Fs Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-01-07
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

#include "fs_common.h"
#include "los_littlefs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LITTLEFS_PATH "/littlefs"
#define NAME_LEN      100

static char g_demoFileName[NAME_LEN] = {0};
static char g_demoDirName[NAME_LEN] = {0};

void LittlefsDemo(void)
{
    int ret;

    printf("Littlefs file system demo task start to run.\n");

    ret = sprintf_s(g_demoFileName, sizeof(g_demoFileName), "%s/%s", LITTLEFS_PATH, LOS_FILE);
    if (ret <= 0) {
        FS_LOG_ERR("Execute sprintf_s file name failed.");
        return;
    }
    ret = sprintf_s(g_demoDirName, sizeof(g_demoDirName), "%s/%s", LITTLEFS_PATH, LOS_DIR);
    if (ret <= 0) {
        FS_LOG_ERR("Execute sprintf_s dir name failed.");
        return;
    }

    los_vfs_io(g_demoFileName, g_demoDirName);
    printf("Littlefs file system demo task finished.\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
