/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Fat Fs Sd Hal Implementation
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

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#if defined (__GNUC__) || defined (__CC_ARM)
#include "fcntl.h"
#include <los_printf.h>
#endif

#include "los_vfs.h"
#include "los_fatfs.h"

// #include <hal_spi_flash.h>
#include "sd_diskio.h"
#include "los_hwi.h"
/* Defines ------------------------------------------------------------------*/
#define SPI_FLASH_ID            0xEF4018

#define SPI_FLASH_SECTOR_SIZE   (4 * 1024)
#define SPI_FLASH_PAGE_SIZE     256
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
static DSTATUS Stm32f7FatfsStatus(BYTE lun)
{
    return SD_CheckStatus(lun);
}

static DSTATUS Stm32f7FatfsInit(BYTE lun)
{
    return SD_initialize(lun);
}

static DRESULT Stm32f7FatfsRead(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    LOS_IntLock();
    DRESULT ret = SD_read(lun, buff, sector, count);
    LOS_IntUnLock();
    return ret;
}

static DRESULT Stm32f7FatfsWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    LOS_IntLock();
    DRESULT ret =  SD_write(lun, buff, sector, count);
    LOS_IntUnLock();
    return ret;
}

static DRESULT Stm32f7FatfsIoctl(BYTE lun, BYTE cmd, void *buff)
{
    return SD_ioctl(lun, cmd, buff);
}

static uint8_t BspSdErase(uint32_t StartAddr, uint32_t EndAddr) {
    return BSP_SD_Erase(StartAddr, EndAddr);
}

static struct diskio_drv g_fatfsConfig =
{
    Stm32f7FatfsInit,
    Stm32f7FatfsStatus,
    Stm32f7FatfsRead,
    Stm32f7FatfsWrite,
    Stm32f7FatfsIoctl
};

DWORD get_fattime (void)
{
    return 0;
}

struct diskio_drv* FatfsConfigGet(void)
{
    return &g_fatfsConfig;
}

void FatfsDriverInit(int needErase)
{
    if (needErase) {
        BspSdErase(FF_PHYS_ADDR, FF_PHYS_SIZE);
    }
}
