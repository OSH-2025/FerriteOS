/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main Process
 * Author: Huawei LiteOS Team
 * Create: 2021-10-28
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

#include "main.h"
#include "los_task_pri.h"
#include "arch/canary.h"

STATIC VOID BssClean(VOID)
{
    (VOID)memset_s(&__bss_start, ((UINTPTR)&__bss_end - (UINTPTR)&__bss_start),
                    0, ((UINTPTR)&__bss_end - (UINTPTR)&__bss_start));
}

typedef struct {
    UINT8 magic;
    UINT8 segmentCount;
    UINT8 spiMode;
    UINT8 spiSpeed : 4;
    UINT8 spiSize : 4;
    UINT32 entryAddr;
} __attribute__((packed)) EspImageHead;

typedef struct {
    UINT32 loadAddr;
    UINT32 dataLen;
    UINT32 data[0];
} EspImageSegmentHead;

#define FLASH_BASE                      (0x40200000)
#define FLASH_SIZE                      (0x100000)
STATIC VOID AnalysisImage(UINT32 imageAddr)
{
    EspImageHead *imageHead = (EspImageHead *)(FLASH_BASE + (imageAddr & (FLASH_SIZE - 1)));
    EspImageSegmentHead *segment = (EspImageSegmentHead *)((UINTPTR)imageHead + sizeof(EspImageHead));
    UINT8 segmentCount = ((*(volatile UINT32 *)imageHead) & 0xFF00) >> 8; /* 8, only access for a word */
    for (INT32 i = 1; i < segmentCount; i++) {
        segment = (EspImageSegmentHead *)((UINTPTR)segment + sizeof(EspImageSegmentHead) + segment->dataLen);
        if ((segment->loadAddr >= FLASH_BASE) && (segment->loadAddr < (FLASH_BASE + FLASH_SIZE))) {
            continue;
        }

        UINT32 *dest = (UINT32 *)segment->loadAddr;
        UINT32 *src = segment->data;
        UINT32 size = segment->dataLen / sizeof(UINT32);
        while (size--) {
            *dest++ = *src++;
        }
    }
}

INT32 main(UINT32 imageAddr)
{
    AnalysisImage(imageAddr);
    __asm__ __volatile__("movi       a0, 0x40100000\n"
                         "wsr        a0, vecbase\n": : :"memory");
    __asm__ __volatile__("mov sp, %0;rsync" : : "r"(&__heap_start));
    BssClean();
#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());
    printf("\n********Hello Huawei LiteOS********\n"
            "\nLiteOS Kernel Version : %s\n"
            "build date : %s %s\n\n"
            "**********************************\n",
            HW_LITEOS_KERNEL_VERSION_STRING, __DATE__, __TIME__);

    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    OsStart();

    return LOS_OK;
}

