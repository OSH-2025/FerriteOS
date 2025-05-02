/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Targets Stm32f429 Src Board Ota
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
#include "stm32f4xx.h"
#include "board_ota.h"
#include "hal_flash.h"
#include "hal_spi_flash.h"
#include "upgrade_flag.h"

#define OTA_FLASH_BASE 0x08000000
#define OTA_MEMORY_BASE 0x20000000
#define OTA_PC_MASK 0xFF000000
#define OTA_STACK_MASK 0x2FFC0000
#define OTA_COPY_BUF_SIZE 0x1000

typedef void (*jumpFunc)(void);

static int prv_spi2inner_copy(uint32_t srcAddr, int32_t imgLen)
{
    int ret;
    int32_t copyLen;
    int remainLen;
    uint8_t buf[OTA_COPY_BUF_SIZE];
    uint32_t destAddr = OTA_DEFAULT_IMAGE_ADDR;

    ret = hal_flash_erase(destAddr, imgLen);
    if (ret != 0) {
        OTA_LOG("write inner flash failed");
        return OTA_ERRNO_INNER_FLASH_WRITE;
    }
    remainLen = imgLen;
    while (remainLen > 0) {
        copyLen = remainLen > OTA_COPY_BUF_SIZE ? OTA_COPY_BUF_SIZE : remainLen;
        ret = hal_spi_flash_read(buf, copyLen, srcAddr);
        if (ret != 0) {
            (void)hal_flash_lock();
            OTA_LOG("read spi flash failed");
            return OTA_ERRNO_SPI_FLASH_READ;
        }
        srcAddr += copyLen;
        ret = hal_flash_write(buf, copyLen, &destAddr);
        if (ret != 0) {
            (void)hal_flash_lock();
            OTA_LOG("write inner flash failed");
            return OTA_ERRNO_INNER_FLASH_WRITE;
        }
        remainLen -= copyLen;
    }
    (void)hal_flash_lock();
    return OTA_ERRNO_OK;
}

static int prv_inner2spi_copy(int32_t imgLen)
{
    int ret;
    int remainLen;
    int32_t copyLen;
    uint8_t buf[OTA_COPY_BUF_SIZE];
    uint32_t srcAddr = OTA_DEFAULT_IMAGE_ADDR;
    uint32_t bckAddr = OTA_IMAGE_BCK_ADDR; // backup addr
    ret = hal_spi_flash_erase(bckAddr, imgLen);
    if (ret != 0) {
        OTA_LOG("write spi flash failed.");
        return OTA_ERRNO_SPI_FLASH_WRITE;
    }
    remainLen = imgLen;
    while (remainLen > 0) {
        copyLen = imgLen > OTA_COPY_BUF_SIZE ? OTA_COPY_BUF_SIZE : remainLen;
        ret = hal_flash_read(buf, copyLen, srcAddr);
        if (ret != 0) {
            OTA_LOG("read inner flash failed.");
            return OTA_ERRNO_INNER_FLASH_READ;
        }
        srcAddr += copyLen;

        ret = hal_spi_flash_write(buf, copyLen, &bckAddr);
        if (ret != 0) {
            OTA_LOG("write spi flash failed.");
            return OTA_ERRNO_SPI_FLASH_WRITE;
        }
        remainLen -= copyLen;
    }
    return OTA_ERRNO_OK;
}

int jumpToApp(uint32_t binSize)
{
    jumpFunc jump;
    uint32_t jumpAddr = *(__IO uint32_t *)(OTA_DEFAULT_IMAGE_ADDR + 4);
    uint32_t appAddr = *(__IO uint32_t *)(OTA_DEFAULT_IMAGE_ADDR);

    if ((jumpAddr & OTA_PC_MASK) == OTA_FLASH_BASE) {
        if ((appAddr & OTA_STACK_MASK) == OTA_MEMORY_BASE) {
            flag_set_info(UPGRADE_NONE, binSize);
            jump = (jumpFunc)jumpAddr;
            __set_MSP(appAddr);
            jump();
        } else {
            OTA_LOG("app address[%lx] of the image is ilegal.", appAddr);
            return OTA_ERRNO_ILEGAL_STACK;
        }
    } else {
        OTA_LOG("jump address[%lx] of the image is ilegal.", jumpAddr);
        return OTA_ERRNO_ILEGAL_PC;
    }
    return OTA_ERRNO_OK;
}

int ImgUpdateCopy(int32_t oldImgLen, int32_t newImgLen, uint32_t newImgAddr)
{
    int ret;
    if ((oldImgLen <= 0) || (newImgLen <= 0)) {
        OTA_LOG("ilegal oldImgLen(%ld) or newImgLen(%ld)", oldImgLen, newImgLen);
        return OTA_ERRNO_ILEGAL_PARAM;
    }
    ret = prv_inner2spi_copy(oldImgLen); // copy from internal flash to external spi flash
    if (ret != 0) {
        OTA_LOG("back up old image failed.");
        return ret;
    }
    ret = prv_spi2inner_copy(newImgAddr, newImgLen); // copy from external spi flash to internal flash
    if (ret != 0) {
        OTA_LOG("update image failed.");
        return ret;
    }
    return OTA_ERRNO_OK;
}

int ImgRollbackCopy(int32_t imgLen)
{
    int ret;
    if (imgLen < 0) {
        OTA_LOG("ilegal imgLen:%ld", imgLen);
        return OTA_ERRNO_ILEGAL_PARAM;
    }
    ret = prv_spi2inner_copy(OTA_IMAGE_BCK_ADDR, imgLen);
    if (ret != 0) {
        OTA_LOG("rollback image failed.");
        return ret;
    }
    return 0;
}
