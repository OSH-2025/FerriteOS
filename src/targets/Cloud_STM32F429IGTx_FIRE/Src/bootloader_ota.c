/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Targets Stm32f429 Src Bootloader
 * Author: Huawei LiteOS Team
 * Create: 2021-10-25
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
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "hal_flash.h"
#include "hal_spi_flash.h"
#include "usart.h"
#include "sys_init.h"
#include "board_ota.h"
#include "flag_manager.h"
#include "upgrade_flag.h"
#include "ota_api.h"
#include "ota_port.h"
#include "recover_image.h"
#include "los_memory_pri.h"

static ota_opt_s g_opt;

static void ErrorHandler(char *file, int line)
{
    OTA_LOG("[%s][%d]\n", file, line);
    while (1) {}
}

static int jump(uint32_t oldBinSize)
{
    int ret;
    OTA_LOG("jump to application.\n");
    ret = jumpToApp(oldBinSize);
    if (ret != OTA_ERRNO_OK) {
        OTA_LOG("jump to app failed, try to rollback.\n");
        (void)recover_set_update_fail();
        ret = ImgRollbackCopy(oldBinSize);
        if (ret != 0) {
            OTA_LOG("rollback failed, system start up failed.\n");
            ErrorHandler(__FILE__, __LINE__);
        }
    }
    OTA_LOG("begin to try to jump to application again\n");
    ret = jumpToApp(oldBinSize);
    if (ret != 0) {
        OTA_LOG("rollback succeed, system start up failed.\n");
        ErrorHandler(__FILE__, __LINE__);
    }
    return ret;
}

static int UpgradeFlagRead(void *buf, int32_t len)
{
    int ret;
    if (g_opt.read_flash == NULL) {
        return -1;
    }
    ret = g_opt.read_flash(OTA_UPDATE_INFO, buf, len, 0);
    return ret; 
}

static int UpgradeFlagWrite(const void *buf, int32_t len)
{
    int ret;
    if (g_opt.write_flash == NULL) {
        return -1;
    }
    ret = g_opt.write_flash(OTA_UPDATE_INFO, buf, len, 0);
    return ret; 
}

int main(void)
{
    int ret;
    uint32_t newBinSize = 0;
    uint32_t oldBinSize = 0;
    upgrade_type_e upgradeType;
    upgrade_state_e upgradeState;
    flag_op_s op;

    SystemClock_Config();  // system clock init
    MX_USART1_UART_Init(); // usart1 init
    hal_spi_flash_config(); // spi flash init
    ret = OsMemSystemInit((UINTPTR)OS_SYS_MEM_ADDR); // system memory init
    if (ret != LOS_OK) {
        OTA_LOG("mem init err.\n");
        return ret;
    }
    OTA_LOG("run bootloader.\n");
    hal_get_ota_opt(&g_opt);
    op.func_flag_read = UpgradeFlagRead;
    op.func_flag_write = UpgradeFlagWrite;
    (void)flag_init(&op);
    ret = flag_upgrade_init();
    if (ret != 0) {
        OTA_LOG("warning: read upgrade flag failed.\n");
    }
    flag_get_info(&upgradeType, &newBinSize, &oldBinSize, &upgradeState);
    if ((oldBinSize == 0) || (newBinSize == 0)) {
        oldBinSize = OTA_IMAGE_DOWNLOAD_SIZE;
        newBinSize = OTA_IMAGE_DOWNLOAD_SIZE;
    }
    switch (upgradeType) {
        case UPGRADE_NONE:
            OTA_LOG("normal start up.\n");
            break;
        case UPGRADE_FULL:
            OTA_LOG("full upgrade.\n");
            ret = ImgUpdateCopy(oldBinSize, newBinSize, OTA_IMAGE_DOWNLOAD_ADDR);
            if (ret != 0) {
                OTA_LOG("warning: [full] copy newimage to inner flash failed\n");
                (void)recover_set_update_fail();
            } else {
                oldBinSize = newBinSize;
            }
            break;
        default:
            break;
    }
    ret = jump(oldBinSize);
    return ret;
}
