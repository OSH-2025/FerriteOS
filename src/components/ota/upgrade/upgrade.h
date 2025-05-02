/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Upgrade HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-10-10
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

#ifndef _UPGRADE_H
#define _UPGRADE_H

#include "los_typedef.h"
#include "los_base.h"
#include "los_task.h"
#include "los_typedef.h"
#include "los_sys.h"
#include "ota_port.h"
#include "mbedtls/sha256.h"
#include "flag_manager.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    PROCESSING_SUCCEEDED = 0, 
    EQUIPMENT_IN_USE,
    POOR_SIGNAL_QUALITY,
    LATEST_VERSION,
    LOW_BATTERY,
    SPACE_INSUFFICIENT,
    DOWNLOAD_TIMEOUT,
    VERFIY_FAILED,
    TYPE_UNSUPPORTED,
    OUT_OF_MEMORY,
    INSTALL_FAILED,
    INTERNAL_ABNORMAL = 0xFF
} ResultCodeType;

typedef struct {
    int len;
    int postion;
    uint8_t buffer[];
} FwBuffer;

typedef struct {
    char host[32];
    char port[8];
    char url[256];
    char token[128];
    char sign[128];
    char version[32];
    int fileSize;
    uint8_t signCheck[32]; // sha256 length: 32 bytes
    int writeSize;
    ota_opt_s opt;
    flag_op_s flagOp;
    FwBuffer *fwBuffer;
    int (*callback)(char *version, int resultCode, int progress);
} UpgradeHandle;

typedef struct {
    char *buffer;
    int size;
    char version[16];
    int progress; // device upgrade progress. The value ranges from 0 to 100
    ResultCodeType resultCode; // Upgrade status of the device
} UpgradeProgress;

int UpgradeQueueWrite(UpgradeHandle *handle, int len);
int UpgradeInit(void);
uint32_t UpgradeMainTask(UpgradeHandle *handle, uint16_t len);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _UPGRADE_H */
