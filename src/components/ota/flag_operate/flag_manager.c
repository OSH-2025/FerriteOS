/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Ota Flag Operate Manager
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

#include <stddef.h>
#include <string.h>
#include "flag_manager.h"

static flag_op_s g_flag_op;

#define FLASH_UNIT_SIZE (256)
#define FLASH_FLAG_SIZE (512)

int flag_init(flag_op_s *flag)
{
    if ((flag == NULL) || (flag->func_flag_read == NULL) || (flag->func_flag_write == NULL)) {
        return -1;
    }
    g_flag_op.func_flag_read = flag->func_flag_read;
    g_flag_op.func_flag_write = flag->func_flag_write;
    return 0;
}

int flag_read(flag_type_e flagType, void *buf, int32_t len)
{
    int ret;
    uint8_t flagBuffer[FLASH_FLAG_SIZE];

    if ((buf == NULL) || (len <= 0) || (len > FLASH_FLAG_SIZE)) {
        return -1;
    }

    ret = g_flag_op.func_flag_read(flagBuffer, FLASH_FLAG_SIZE);
    if (ret != 0) {
        return -1;
    }

    switch (flagType) {
        case FLAG_BOOTLOADER:
            memcpy(buf, flagBuffer, len);
            break;
        case FLAG_APP:
            memcpy(buf, flagBuffer + FLASH_UNIT_SIZE, len);
            break;
        default:
            break;
    }
    return 0;
}

int flag_write(flag_type_e flagType, const void *buf, int32_t len)
{
    int ret;
    uint8_t flagBuffer[FLASH_FLAG_SIZE];

    if ((buf == NULL) || (len <= 0) || (len > FLASH_FLAG_SIZE)) {
        return -1;
    }

    // read flag from flash
    ret = g_flag_op.func_flag_read(flagBuffer, FLASH_FLAG_SIZE);
    if (ret != 0) {
        return -1;
    }

    switch (flagType) {
        case FLAG_BOOTLOADER:
            memcpy(flagBuffer, buf, len);
            break;
        case FLAG_APP:
            memcpy(flagBuffer + FLASH_UNIT_SIZE, buf, len);
            break;
        default:
            break;
    }
    // save new flag
    if (g_flag_op.func_flag_write == NULL) {
        return -1;
    }
    ret = g_flag_op.func_flag_write(flagBuffer, FLASH_FLAG_SIZE);
    return ret; 
}

