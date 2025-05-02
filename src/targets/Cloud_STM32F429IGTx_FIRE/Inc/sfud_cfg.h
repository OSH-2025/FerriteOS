/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sfud Port HeadFile
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

#ifndef _SFUD_CFG_H
#define _SFUD_CFG_H

#ifdef LOSCFG_COMPONENTS_SFUD

#ifdef LOSCFG_SFUD_DEBUG_MODE
#define SFUD_DEBUG_MODE
#endif /* LOSCFG_DEBUG_MODE */

#ifdef LOSCFG_SFUD_USING_SFDP
#define SFUD_USING_SFDP
#endif /* LOSCFG_USING_SFDP */

#ifdef LOSCFG_SFUD_USING_FLASH_INFO_TABLE
#define SFUD_USING_FLASH_INFO_TABLE
#endif /* LOSCFG_USING_FLASH_INFO_TABLE */

enum {
    SFUD_W25Q256JV_DEVICE_INDEX = 0,
};

#define SFUD_FLASH_DEVICE_TABLE      \
{                                    \
    [SFUD_W25Q256JV_DEVICE_INDEX] = { \
        .name = "W25Q256JV",         \
        .spi.name = "SPI5",          \
        .chip = {                    \
            .name = "W25Q256JV",     \
            .mf_id = SFUD_MF_ID_WINBOND,      \
            .type_id = 0x40,                  \
            .capacity_id = 0x19,              \
            .capacity = 32L * 1024L * 1024L,  \
            .write_mode = SFUD_WM_PAGE_256B,  \
            .erase_gran = 4096,               \
            .erase_gran_cmd = 0x20,           \
            }                       \
    }                               \
}

#ifdef LOSCFG_SFUD_USING_QSPI
#define SFUD_USING_QSPI
#endif /* LOSCFG_USING_QSPI */

#endif /* LOSCFG_COMPONENTS_SFUD */

#endif /* _SFUD_CFG_H */
