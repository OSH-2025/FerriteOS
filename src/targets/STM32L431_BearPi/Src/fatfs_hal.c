/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2022. All rights reserved.
 * Description: Fat Fs Hal
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
#include "los_vfs.h"
#include "los_fatfs.h"
#include "hal_spi_flash.h"
#include "hal_qspi_flash.h"

/* Physical offset(in bytes) in spi flash used for FatFs
/  Must be on 4k boundary */
#define SPIFLASH_PHYS_ADDR 0

/* Physical size(in bytes) of the spi flash used for FatFs
/  Must be on 4k boundary */
#define SPIFLASH_PHYS_SIZE 0x400000 // 4MB

#define SPI_FLASH_ID 0xEF4017
#define SPI_FLASH_SECTOR_SIZE (4 * 1024)
#define SPI_FLASH_PAGE_SIZE 256

static DSTATUS SpiFlashStatus(BYTE lun)
{
    DSTATUS status = STA_NOINIT;

    if (hal_spi_flash_get_id() == SPI_FLASH_ID) {
        status &= ~STA_NOINIT;
    }
    return status;
}

static DSTATUS SpiFlashInit(BYTE lun)
{
    DSTATUS status = STA_NOINIT;

    hal_spi_flash_config();
    status = SpiFlashStatus(lun);
    return status;
}

static DRESULT SpiFlashRead(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    int ret;
    ret = hal_spi_flash_read(buff, count * SPI_FLASH_SECTOR_SIZE, SPIFLASH_PHYS_ADDR + sector * SPI_FLASH_SECTOR_SIZE);
    if (ret != 0) {
        return RES_ERROR;
    }
    return RES_OK;
}

static DRESULT SpiFlashWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    int ret;
    ret = hal_spi_flash_erase_write(buff, count * SPI_FLASH_SECTOR_SIZE, SPIFLASH_PHYS_ADDR + sector * SPI_FLASH_SECTOR_SIZE);
    if (ret != 0) {
        return RES_ERROR;
    }
    return RES_OK;
}

static DRESULT SpiFlashIoctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;
    switch (cmd) {
        case GET_SECTOR_COUNT:
            *(DWORD *)buff = SPIFLASH_PHYS_SIZE / SPI_FLASH_SECTOR_SIZE;
            break;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = SPI_FLASH_SECTOR_SIZE;
            break;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            break;
        default:
            break;
    }
    res = RES_OK;
    return res;
}

static struct diskio_drv spiFlashDrv = {
    SpiFlashInit,
    SpiFlashStatus,
    SpiFlashRead,
    SpiFlashWrite,
    SpiFlashIoctl
};

void FatfsDriverInit(int needErase)
{
    if (needErase) {
        (void)hal_spi_flash_config();
        (void)hal_spi_flash_erase(SPIFLASH_PHYS_ADDR, SPIFLASH_PHYS_SIZE);
    }
}

struct diskio_drv* FatfsConfigGet(void)
{
    return &spiFlashDrv;
}

DWORD get_fattime(void)
{
    return 0;
}

/* Private functions -------------------------------------------------------- */