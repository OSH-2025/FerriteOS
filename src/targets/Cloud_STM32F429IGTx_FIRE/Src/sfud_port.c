/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sfud Port Implementation
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

#ifdef LOSCFG_COMPONENTS_SFUD

#include <sfud.h>
#include <stdarg.h>
#include "los_mux.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "hal_spi_flash.h"

typedef struct {
    uint32_t muxId;
} SpiUserData;

static SpiUserData g_spiData;

static char g_logBuf[256];

static sfud_err SF_WriteRead(const sfud_spi *spi, const uint8_t *wBuf, size_t wSize, uint8_t *rBuf,
        size_t rSize)
{   
    sfud_err result = SFUD_SUCCESS;
    if (wSize) {
        SFUD_ASSERT(wBuf);
    }
    if (rSize) {
        SFUD_ASSERT(rBuf);
    }

    if (wSize && rSize) {
        prv_spi_flash_transmit_and_receive((uint8_t*)wBuf, wSize, (uint8_t*)rBuf, rSize);
    } else if (wSize) {
		prv_spi_flash_transmit((uint8_t*)wBuf, wSize);
    }
    return result;
}

/*
 * set about 100 microsecond delay
 */
static void SetDelay(void)
{
    LOS_TaskDelay(100);
}

static void SpiSetLock(const sfud_spi *spi) {
    SFUD_ASSERT(spi);
    SpiUserData *data = (SpiUserData *)spi->user_data;
    SFUD_ASSERT(data);
    int ret = LOS_MuxPend(data->muxId, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        printf("Sfud mux pend failed!\n");
    }
}

static void SpiSetUnlock(const sfud_spi *spi) {
    SFUD_ASSERT(spi);
    SpiUserData *data = (SpiUserData *)spi->user_data;
    SFUD_ASSERT(data);
   int ret = LOS_MuxPost(data->muxId);
   if (ret != LOS_OK) {
        printf("Sfud mux post failed!\n");
    }
}

sfud_err sfud_spi_port_init(sfud_flash *sf) {
    sfud_err result = SFUD_SUCCESS;
    int ret = LOS_MuxCreate(&g_spiData.muxId);
    if (ret != LOS_OK) {
        printf("Init sfud mux failed!\n");
    }

    hal_spi_flash_config();
    sf->spi.wr = SF_WriteRead;
    sf->spi.lock = SpiSetLock;
    sf->spi.unlock = SpiSetUnlock;
    sf->spi.user_data = &g_spiData;
    sf->retry.delay = SetDelay;
    /* adout 60 seconds timeout */
    sf->retry.times = 60 * 10000;

    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;
    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(g_logBuf, sizeof(g_logBuf), format, args);
    printf("%s\n", g_logBuf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;
    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(g_logBuf, sizeof(g_logBuf), format, args);
    printf("%s\n", g_logBuf);
    va_end(args);
}
#endif /* LOSCFG_COMPONENTS_SFUD */
