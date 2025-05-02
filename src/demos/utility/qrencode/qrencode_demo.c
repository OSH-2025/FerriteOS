/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Qrencode Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-05-10
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

#include "qrencode_demo.h"
#include "stdio.h"
#include "los_task.h"
#include "qrencode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define QRENCODE_TASK_STACK_SIZE   0x1000
#define QRENCODE_TASK_PRIORITY     6
#define FILE_NAME                  "/fatfs/liteos.bmp"
#define INFORMATION                "welcome to liteos"
#define QR_VERSION                 5
#define QR_FLAG                    1
#define UINT_SIZE                  4
#define PIXELBYTES                 3
#define BITCOUNTPERPIX             24
#define QR_COLOR_R                 0xff
#define QR_COLOR_G                 0
#define QR_COLOR_B                 0

#pragma pack(push, 2)
typedef struct {
    UINT16 type;
    UINT32 size;
    UINT16 reserved1;
    UINT16 reserved2;
    UINT32 offBits;
} FileHeader;

typedef struct {
    UINT32 size;
    UINT32 width;
    UINT32 height;
    UINT16 planes;
    UINT16 bitCount;
    UINT32 compression;
    UINT32 sizeImage;
    UINT32 xPerMeter;
    UINT32 yPerMeter;
    UINT32 clrUsed;
    UINT32 clrImportant;
} InfoHeader;

typedef  struct {
    FileHeader fileHeader;
    InfoHeader fileInfo;
} BitmapFile;
#pragma pack(pop)

STATIC UINT32 g_demoTaskId;

STATIC VOID BitmapFileInit(FILE *file, INT32 size)
{
    BitmapFile bmpFile;
    bmpFile.fileHeader.type = 0x4D42;
    bmpFile.fileHeader.size = sizeof(FileHeader) + sizeof(InfoHeader);
    bmpFile.fileHeader.reserved1 = 0;
    bmpFile.fileHeader.reserved2 = 0;
    bmpFile.fileHeader.offBits = sizeof(FileHeader) + sizeof(InfoHeader);

    bmpFile.fileInfo.size = sizeof(InfoHeader);
    bmpFile.fileInfo.width = size;
    bmpFile.fileInfo.height = size;
    bmpFile.fileInfo.planes = 1;  // 1:number of planes
    bmpFile.fileInfo.bitCount = BITCOUNTPERPIX;
    bmpFile.fileInfo.compression = 0;
    bmpFile.fileInfo.sizeImage = 0;
    bmpFile.fileInfo.xPerMeter = 0;
    bmpFile.fileInfo.yPerMeter = 0;
    bmpFile.fileInfo.clrUsed = 0;
    bmpFile.fileInfo.clrImportant = 0;

    fwrite(&(bmpFile.fileHeader), sizeof(FileHeader), 1, file);
    fwrite(&(bmpFile.fileInfo), sizeof(InfoHeader), 1, file);
}

STATIC INT32 GetQrcode(QRcode *qrCode,  FILE *fp)
{
    INT32 x, y;
    UINT32 dataLen;
    UINT32 unWidthAdjusted;
    UINT8 *pRgbDate = NULL;
    UINT8 *pDestData = NULL;
    UINT8 *pSourceData = NULL;

    unWidthAdjusted = qrCode->width * PIXELBYTES;
    if (unWidthAdjusted % UINT_SIZE) {
        unWidthAdjusted = (unWidthAdjusted / UINT_SIZE + 1) * UINT_SIZE;
    }

    dataLen = unWidthAdjusted * qrCode->width;
    pRgbDate = (UINT8*)malloc(dataLen);
    if (pRgbDate == NULL) {
        return -1;
    }

    if (memset_s(pRgbDate, dataLen, 0xff, dataLen) != EOK) {
        free(pRgbDate);
        return -1;
    }

    pSourceData = qrCode->data;
    for (x = 0; x < qrCode->width; x++) {
        pDestData = pRgbDate + unWidthAdjusted * x;
        for (y = 0; y < qrCode->width; y++) {
            if (*pSourceData & 1) {
                *(pDestData) = QR_COLOR_B;      // Three bytes per pixel
                *(pDestData + 1) = QR_COLOR_G;  // 1:move one step
                *(pDestData + 2) = QR_COLOR_R;  // 2:move two steps
            }
            pDestData += PIXELBYTES;
            pSourceData++;
        }
    }
    fwrite(pRgbDate, sizeof(UINT8), dataLen, fp);

    return dataLen;
}

STATIC INT32 DemoTaskEntry(VOID)
{
    UINT32 dataLen;
    FILE *fp = NULL;
    QRcode *qrCode = NULL;

    printf("Qrencode demo task start to run.\n");
    fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        printf("Open file failed.\n");
        return  -1;
    }

    qrCode = QRcode_encodeString(INFORMATION, QR_VERSION, QR_ECLEVEL_H, QR_MODE_8, QR_FLAG);
    if (qrCode == NULL) {
        printf("Qrencode create failed.\n");
        fclose(fp);
        return -1;
    }

    BitmapFileInit(fp, qrCode->width);
    dataLen = GetQrcode(qrCode, fp);
    if (dataLen < 0) {
        fclose(fp);
        printf("Failed to generate the QR code.\n");
        return -1;
    }

    fclose (fp);
    printf("Qrencode demo task finished.\n");

    return 0;
}

VOID QrencodeDemoTask(VOID)
{
    INT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = QRENCODE_TASK_STACK_SIZE;
    taskInitParam.pcName = "QrencodeDemoTask";
    taskInitParam.usTaskPrio = QRENCODE_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create qrencode demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
