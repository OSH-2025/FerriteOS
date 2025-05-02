/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Fat Fs HeadFile
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef _LOS_FATFS_H
#define _LOS_FATFS_H

#if defined(LOSCFG_COMPONENTS_FS_FATFS)
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Includes ----------------------------------------------------------------- */
#include "ff.h"
#include "diskio.h"
#include <stdint.h>
/* Defines ------------------------------------------------------------------ */
// #define DISK_STATE_INITIALIZED 1
// typedef unsigned char DSTATUS;
#define FF_VOLUMES 1
/* Macros ------------------------------------------------------------------- */
/* Typedefs ----------------------------------------------------------------- */
struct diskio_drv {
    DSTATUS (*initialize)(BYTE);                       /* !< Initialize Disk Drive  */
    DSTATUS (*status)(BYTE);                           /* !< Get Disk Status        */
    DRESULT (*read)(BYTE, BYTE *, DWORD, UINT);        /* !< Read Sector(s)         */
    DRESULT (*write)(BYTE, const BYTE *, DWORD, UINT); /* !< Write Sector(s)        */
    DRESULT (*ioctl)(BYTE, BYTE, void *);              /* !< I/O control operation  */
};

struct disk_dev {
    uint8_t state;
    const struct diskio_drv *drv;
    uint8_t lun;
};

struct disk_mnt {
    struct disk_dev dev[FF_VOLUMES];
    volatile uint8_t num;
};

/* Extern variables --------------------------------------------------------- */
/* Functions API ------------------------------------------------------------ */

void FatfsDriverInit(int needErase);
int FatfsInit(int needErase, struct diskio_drv *drv, uint8_t *drive);
int FatfsMount(const char *path, struct diskio_drv *drv, uint8_t *drive);
int FatfsUnmount(const char *path, uint8_t drive);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* LOSCFG_COMPONENTS_FS_FATFS */
#endif /* _LOS_FATFS_H */
