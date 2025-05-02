/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: LiteOS adaptor file.
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

void OsBackTrace(void)
{
}

#ifdef LOSCFG_KERNEL_NX
#include "arch/mpu.h"
void MPU_Config(void)
{
    MPU_REGION_INFO mpuInfo;

    mpuInfo.number = 0; // region 0
    mpuInfo.baseAddress = 0x08000000; // start addr of flash
    mpuInfo.accessPermission = MPU_DEFS_RASR_AP_RO;
    mpuInfo.sharable = MPU_ACCESS_SHAREABLE;
    mpuInfo.cachable = MPU_ACCESS_CACHEABLE;
    mpuInfo.buffable = MPU_ACCESS_BUFFERABLE;
    mpuInfo.regionSize = MPU_REGION_SIZE_2MB;
    mpuInfo.hfnmiena = MPU_HFNMIENA_ENABLE;
    mpuInfo.xn = MPU_INSTRUCTION_ACCESS_ENABLE;
    (void)ArchProtectionRegionSet(&mpuInfo);

    mpuInfo.number = 1; // region 1
    mpuInfo.baseAddress = 0x20000000; // start addr of ram
    mpuInfo.accessPermission = MPU_DEFS_RASR_AP_FULL_ACCESS;
    mpuInfo.sharable = MPU_ACCESS_SHAREABLE;
    mpuInfo.cachable = MPU_ACCESS_CACHEABLE;
    mpuInfo.buffable = MPU_ACCESS_BUFFERABLE;
    mpuInfo.regionSize = MPU_REGION_SIZE_512KB;
    mpuInfo.hfnmiena = MPU_HFNMIENA_ENABLE;
    mpuInfo.xn = MPU_INSTRUCTION_ACCESS_DISABLE;
    (void)ArchProtectionRegionSet(&mpuInfo);
}
#endif