/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Flash Adaptor Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-21
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

#include "flash_adaptor.h"
#include "stm32f769i_discovery_qspi.h"
#include <string.h>
#include <stdlib.h>
#include "osdepends/atiny_osdep.h"
#include "common.h"
#include "securec.h"

#define MQTT_INFO_ADDR                0x00008000
#define MQTT_INFO_SIZE                0x00008000

void FlashAdaptorInit(void)
{
    (void)BSP_QSPI_Init();
}

int FlashAdaptorWriteMqttInfo(const void *buffer, uint32_t len)
{
    int ret;
    if (len > MQTT_INFO_SIZE) {
        HAL_OTA_LOG("err offset len %lu",  len);
        return HAL_ERROR;
    }
    if ((ret = BSP_QSPI_Erase_Block(MQTT_INFO_ADDR)) != QSPI_OK) {
        printf("Bsp qspi erase failed.\n");
        return ret;
    }
    ret = BSP_QSPI_Write((uint8_t*)buffer, MQTT_INFO_ADDR, len);
    return ret;
}

int FlashAdaptorReadMqttInfo(void *buffer, uint32_t len)
{
    int ret;
    if (len > MQTT_INFO_SIZE) {
        HAL_OTA_LOG("err offset len %lu",  len);
        return HAL_ERROR;
    }
    ret = BSP_QSPI_Read((uint8_t*)buffer, MQTT_INFO_ADDR, len);
    return ret;
}
