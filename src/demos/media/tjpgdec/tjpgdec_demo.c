/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tjpgdec Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-08-18
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

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "tjpgd.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TJPGDEC_TASK_STACK_SIZE 4096
#define TJPGDEC_TASK_PRIORITY   5
#define WORKSPACE_SIZE          3500
#define DEMO_FILENAME           "/fatfs/test.jpg"

#define MAX_LENGTH (1024 * 1024) // max length: 1024 * 1024

STATIC UINT32 g_demoTaskId;

/* Session identifier for input/output functions */
typedef struct {
    FILE *fp;               /* Input stream */
    UINT8 *fbuf;            /* Output frame buffer */
    UINT32 wfbuf;           /* Width of the frame buffer [pix] */
} Iodev;

/**
 * @description: User defined input funciton
 * @param {JDEC*} jd Decompression object
 * @param {UINT8*} buff Pointer to the read buffer (null to remove data)
 * @param {size_t} nbyte Number of bytes to read/remove
 * @return {*} Returns number of bytes read (zero on error)
 */
STATIC size_t InputFunc(JDEC *jd, UINT8 *buff, size_t nbyte)
{
    if ((jd == NULL) || (jd->device == NULL)) {
        printf("jd data error.\n");
        return 0;
    }
    /* Session identifier (5th argument of jd_prepare function) */
    Iodev *dev = (Iodev *)jd->device;

    if ((buff != NULL) && (dev->fp != NULL))  {
        /* Read data from imput stream */
        return fread(buff, 1, nbyte, dev->fp);
    } else {
        /* Remove data from input stream */
        if (dev->fp == NULL) {
            return 0;
        }
        return (size_t)fseek(dev->fp, (long)nbyte, SEEK_CUR) ? 0 : nbyte;
    }
}

/**
 * @description: User defined output funciton
 * @param {JDEC*} jd Decompression object
 * @param {VOID*} bitmap Bitmap data to be output
 * @param {JRECT*} rect Rectangular region of output image
 * @return {*} Returns 1 to continue, 0 to abort
 */
STATIC INT32 OutputFunc(JDEC *jd, VOID *bitmap, JRECT *rect)
{
    if ((jd == NULL) || (jd->device == NULL)) {
        printf("jd data error.\n");
        return 0;
    }
    /* Session identifier (5th argument of jd_prepare function) */
    Iodev *dev = (Iodev *)jd->device;
    UINT8 *src, *dst;
    UINT32 y;
    UINT32 bws;
    INT32 ret;
    UINT32 bwd;
    /* Put progress indicator */
    if (rect->left == 0) {
        /* 100UL : percentage */
        printf("\r%lu%%", (rect->top << jd->scale) * 100UL / jd->height);
    }
    /* Copy the output image rectanglar to the frame buffer (assuming RGB888 output) */
    src = (UINT8 *)bitmap;
    /* 3: Left-top of destination rectangular */
    dst = dev->fbuf + 3 * (rect->top * dev->wfbuf + rect->left);
    if (rect->right < rect->left || dev->wfbuf > MAX_LENGTH) {
        return 0;
    }
    /* 3: Width of output rectangular [byte] */
    bws = (UINT32)((UINT16)3 * (UINT16)(rect->right - rect->left + 1));
    /* 3: Width of frame buffer [byte] */
    if (bws > WORKSPACE_SIZE) {
        return 0;
    }
    if (dev->wfbuf > (UINT32_MAX / 3)) {
        return 0;
    }
    bwd = 3 * dev->wfbuf;
    for (y = rect->top; y <= rect->bottom; y++) {
        /* Copy a line */
        ret = memcpy_s(dst, bwd, src, bws);
        if (ret != EOK) {
            return 0;
        }
        /* Next line */
        src += bws;
        dst += bwd;
    }
    /* Continue to decompress */
    return 1;
}

STATIC VOID DemoTaskEntry(VOID)
{
    JRESULT res;      /* Result code of TJpgDec API */
    JDEC jdec;        /* Decompression object */
    VOID *work;       /* Pointer to the work area */
    Iodev devid;      /* Session identifier */
    UINT32 devidBufLen;
    printf("Tjpgdec demo start to run.\n");
    /* Initialize input stream */
    devid.fp = fopen(DEMO_FILENAME, "rb");
    if (devid.fp == NULL) {
        return;
    }
    /* Prepare to decompress */
    work = (VOID *)malloc(WORKSPACE_SIZE);
    if (work == NULL) {
        (VOID)fclose(devid.fp);
        return;
    }
    res = jd_prepare(&jdec, InputFunc, work, WORKSPACE_SIZE, &devid);
    if (res == JDR_OK) {
        /* It is ready to dcompress and image info is available here */
        printf("Image size is %u x %u.\n%u bytes of work ares is used.\n",
               (UINT32)jdec.width, (UINT32)jdec.height, (UINT32)(WORKSPACE_SIZE - jdec.sz_pool));

        /* Initialize output device */
        /* Create frame buffer for output image (assuming RGB888 cfg) */
        devidBufLen = jdec.width * jdec.height;
        // 3,
        if (devidBufLen > (UINT32_MAX / 3) || devidBufLen == 0) {
            free(work);
            (VOID)fclose(devid.fp);
            return;
        }

        devidBufLen *= 3; // 3, add devidBufLen length.
        UINT32 memUsed0 = LOS_MemTotalUsedGet(m_aucSysMem0);
        UINT32 totalMem0 = LOS_MemPoolSizeGet(m_aucSysMem0);
        UINT32 freeMem0 = totalMem0 - memUsed0;
        if (devidBufLen > freeMem0) {
            printf("Insufficient memory remaining.\n");
            free(work);
            (VOID)fclose(devid.fp);
            return;
        }
        devid.fbuf = (UINT8 *)malloc(devidBufLen);
        if (devid.fbuf == NULL) {
            free(work);
            (VOID)fclose(devid.fp);
            return;
        }
        if (jdec.width >= devidBufLen || jdec.height >= devidBufLen) {
            free(work);
            (VOID)fclose(devid.fp);
            free(devid.fbuf);
            return;
        }
        devid.wfbuf = (UINT32)jdec.width;
        /* 3: Start to decompress with 1/8 scaling */
        res = jd_decomp(&jdec, OutputFunc, 3);
        if (res == JDR_OK) {
            /* Decompression succeeded. You have the decompressed image in the frame buffer here. */
            printf("\rDecompression succeeded.\n");
        }
        /* Discard frame buffer */
        free(devid.fbuf);
    } else {
        printf("jd_prepare() failed (rc=%d)\n", (INT32)res);
    }
    /* Discard work area */
    free(work);
    /* Close the JPEG file */
    (VOID)fclose(devid.fp);
    printf("Tjpgdec demo finished.\n");
}

VOID TjpgDecDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TJPGDEC_TASK_STACK_SIZE;
    taskInitParam.pcName = "TjpgdecDemoTask";
    taskInitParam.usTaskPrio = TJPGDEC_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create tjpgdec demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
