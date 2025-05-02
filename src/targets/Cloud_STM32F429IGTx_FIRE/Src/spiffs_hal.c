/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Spi Flash Fs Hal
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

#include <stdio.h>
#include <string.h>

#if defined(__GNUC__) || defined(__CC_ARM)
#include "fcntl.h"
#include <los_printf.h>
#endif

#include "los_vfs.h"
#include "los_spiffs.h"

#include <hal_spi_flash.h>

#define PHYS_ERASE_SIZE 64 * 1024
#define LOG_BLOCK_SIZE 64 * 1024
#define LOG_PAGE_SIZE 256

static void Stm32f4SpiffsInit(int needErase)
{
    hal_spi_flash_config();
    if (needErase) {
        hal_spi_flash_erase(SPIFFS_PHYS_ADDR, SPIFFS_PHYS_SIZE);
    }
}

static s32_t Stm32f4SpiffsRead(struct spiffs_t *fs, u32_t addr, u32_t size, u8_t *buff)
{
    (void)hal_spi_flash_read((void *)buff, size, addr);

    return SPIFFS_OK;
}

static s32_t Stm32f4SpiffsWrite(struct spiffs_t *fs, u32_t addr, u32_t size, u8_t *buff)
{
    (void)hal_spi_flash_write((void *)buff, size, (uint32_t *)&addr);

    return SPIFFS_OK;
}

static s32_t Stm32f4SpiffsErase(struct spiffs_t *fs, u32_t addr, u32_t size)
{
    (void)hal_spi_flash_erase(addr, size);

    return SPIFFS_OK;
}

static struct spiffs_drv_t g_spiffsConfig = {
    Stm32f4SpiffsInit,
    Stm32f4SpiffsRead,
    Stm32f4SpiffsWrite,
    Stm32f4SpiffsErase,
    SPIFFS_PHYS_ADDR,
    SPIFFS_PHYS_SIZE,
    PHYS_ERASE_SIZE,
    LOG_BLOCK_SIZE,
    LOG_PAGE_SIZE
};

struct spiffs_drv_t* SpiffsConfigGet(void)
{
    return &g_spiffsConfig;
}
