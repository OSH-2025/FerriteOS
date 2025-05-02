/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: EasyFlash Port Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-10
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

#include <easyflash.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "los_mux.h"

#ifdef LOSCFG_COMPONENTS_SFUD
#include "sfud_cfg.h"
#include <sfud.h>
#endif

#ifdef LOSCFG_COMPONENTS_EASYFLASH

#ifdef LOSCFG_EASYFLASH_INFO_MODE
static char g_logBuffer[128];
#endif /* LOSCFG_EASYFLASH_INFO_MODE */

static uint32_t g_efMuxId;

/**
 * Flash port for hardware initialize.
 *
 * @param default_env default ENV set for user
 * @param default_env_size default ENV size
 *
 * @return result
 */

/* default environment variables set for user */
static const ef_env g_defaultEnvSet[1] = {{"TickCount", "0"}};

EfErrCode ef_port_init(ef_env const **default_env, size_t *default_env_size)
{
    EfErrCode result = EF_NO_ERR;
    *default_env = g_defaultEnvSet;
    *default_env_size = sizeof(g_defaultEnvSet) / sizeof(g_defaultEnvSet[0]);
    LOS_MuxCreate(&g_efMuxId);
    result = sfud_init();
    if (result != EF_NO_ERR) {
        printf("Sfud init failed\n");
    }
    return result;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
EfErrCode ef_port_read(uint32_t addr, uint32_t *buf, size_t size)
{
    EfErrCode result = EF_NO_ERR;
    sfud_err sfudResult = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + SFUD_W25Q256JV_DEVICE_INDEX;
    sfudResult = sfud_read(flash, addr, size, (uint8_t *)buf);
    if (sfudResult != SFUD_SUCCESS) {
        result = EF_READ_ERR;
    }
    return result;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
EfErrCode ef_port_erase(uint32_t addr, size_t size)
{
    EfErrCode result = EF_NO_ERR;

    /* make sure the start address is a multiple of EF_ERASE_MIN_SIZE */
    EF_ASSERT(addr % EF_ERASE_MIN_SIZE == 0);

    sfud_err sfudResult = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + SFUD_W25Q256JV_DEVICE_INDEX;
    sfudResult = sfud_erase(flash, addr, size);

    if (sfudResult != SFUD_SUCCESS) {
        result = EF_ERASE_ERR;
    }
    return result;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
EfErrCode ef_port_write(uint32_t addr, const uint32_t *buf, size_t size)
{
    EfErrCode result = EF_NO_ERR;
    
    sfud_err sfudResult = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + SFUD_W25Q256JV_DEVICE_INDEX;
    sfudResult = sfud_erase_write(flash, addr, size, (const uint8_t *)buf);

    if (sfudResult != SFUD_SUCCESS) {
        result = EF_WRITE_ERR;
    }
    return result;
}

/**
 * lock the ENV ram cache
 */
void ef_port_env_lock(void) 
{
    LOS_MuxPend(g_efMuxId, LOS_WAIT_FOREVER);
}

/**
 * unlock the ENV ram cache
 */
void ef_port_env_unlock(void) 
{
    LOS_MuxPost(g_efMuxId);
}

/**
 * This function is print flash debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 *
 */
void ef_log_debug(const char *file, const long line, const char *format, ...)
{
#ifdef LOSCFG_EASYFLASH_DEBUG_MODE
    va_list args;
    /* args point to the first variable parameter */
    va_start(args, format);

    printf("[Flash](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsprintf(g_logBuffer, format, args);
    printf("%s\n", g_logBuffer);
    va_end(args);
#endif /* LOSCFG_EASYFLASH_DEBUG_MODE */
}

/**
 * This function is print flash routine info.
 *
 * @param format output format
 * @param ... args
 */
void ef_log_info(const char *format, ...)
{
#ifdef LOSCFG_EASYFLASH_INFO_MODE
    va_list args;
    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[Flash]");
    /* must use vprintf to print */
    vsprintf(g_logBuffer, format, args);
    printf("%s\n", g_logBuffer);
    va_end(args);
#endif /* LOSCFG_EASYFLASH_INFO_MODE */
}
#endif /* LOSCFG_COMPONENTS_EASYFLASH */
