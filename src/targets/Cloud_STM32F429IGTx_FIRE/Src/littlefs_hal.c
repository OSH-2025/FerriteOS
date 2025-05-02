/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Little Fs Hal Implementation
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

#include <stdio.h>
#include <string.h>
#include <los_printf.h>
#include "los_vfs.h"
#include "los_littlefs.h"
#include "hal_spi_flash.h"
#include "los_mux_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define LITTLEFS_PATH "/littlefs/"

#define LITTLEFS_PHYS_ADDR 0x00
#define LITTLEFS_PHYS_SIZE (64 * 1024)

#define READ_SIZE      16
#define PROG_SIZE      16
#define BLOCK_SIZE     4096
#define BLOCK_COUNT    128
#define CACHE_SIZE     16
#define LOOKAHEAD_SIZE 16
#define BLOCK_CYCLES   500

#if defined(LFS_THREADSAFE)
static uint32_t g_lfsMutex;
#endif /* LFS_THREADSAFE */

static int LittlefsRead(const struct lfs_config *cfg, lfs_block_t block,
                        lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t addr = block * (cfg->block_size) + off;
    (void)hal_spi_flash_read((void *)buffer, size, addr);

    return LFS_ERR_OK;
}

static int LittlefsProg(const struct lfs_config *cfg, lfs_block_t block, 
                        lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t addr = block * (cfg->block_size) + off;
    (void)hal_spi_flash_write((const void *)buffer, size, (uint32_t *)&addr);

    return LFS_ERR_OK;
}

static int LittlefsErase(const struct lfs_config *cfg, lfs_block_t block)
{
    uint32_t addr = block * (cfg->block_size);
    int32_t size = cfg->block_size;
    (void)hal_spi_flash_erase(addr, size);

    return LFS_ERR_OK;
}

static int LittlefsSync(const struct lfs_config *cfg)
{
    return LFS_ERR_OK;
}

#if defined(LFS_THREADSAFE)
static int LittlefsLock(const struct lfs_config *conf)
{
    (void)conf;
    (void)LOS_MuxPend(g_lfsMutex, LOS_WAIT_FOREVER);
    return LFS_ERR_OK;
}

static int LittlefsUnlock(const struct lfs_config *conf)
{
    (void)conf;
    (void)LOS_MuxPost(g_lfsMutex);
    return LFS_ERR_OK;
}
#endif /* LFS_THREADSAFE */

static struct lfs_config g_lfsConfig = {
    // block device operations
    .context = NULL,
    .read  = LittlefsRead,
    .prog  = LittlefsProg,
    .erase = LittlefsErase,
    .sync  = LittlefsSync,
#if defined(LFS_THREADSAFE)
    .lock = LittlefsLock,
    .unlock = LittlefsUnlock,
#endif /* LFS_THREADSAFE */
    // block device configuration
    .read_size = READ_SIZE,
    .prog_size = PROG_SIZE,
    .block_size = BLOCK_SIZE,
    .block_count = BLOCK_COUNT,
    .cache_size = CACHE_SIZE,
    .lookahead_size = LOOKAHEAD_SIZE,
    .block_cycles = BLOCK_CYCLES,
};

void LittlefsDriverInit(int needErase)
{
    hal_spi_flash_config();
    if (needErase) {
        hal_spi_flash_erase(LITTLEFS_PHYS_ADDR, LITTLEFS_PHYS_SIZE);
    }
}

struct lfs_config* LittlefsConfigGet(void)
{
#if defined(LFS_THREADSAFE)
    (void)LOS_MuxCreate(&g_lfsMutex);
#endif /* LFS_THREADSAFE */
    return &g_lfsConfig;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
