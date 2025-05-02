/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Iconv Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-01-26
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

#include "iconv_demo.h"
#include <iconv.h>
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OUT_LEN   64
#define BUF_LEN   32
#define ICONV_OK  0
#define ICONV_NOK (-1)

#define ICONV_TASK_PRIORITY   7
#define ICONV_TASK_STACK_SIZE 2048

STATIC UINT32 g_demoTaskId;

typedef struct {
    CHAR *fromCharset;
    CHAR *toCharset;
    CHAR *inBuf;
    UINT32 inLen;
    CHAR *outBuf;
    UINT32 outLen;
} IcomvConvertParam;


STATIC INT32 CodeConvert(IcomvConvertParam *param)
{
    iconv_t cd;
    INT32 ret;

    if (param == NULL) {
        return ICONV_NOK;
    }

    if ((param->fromCharset == NULL) || (param->toCharset == NULL)) {
        return ICONV_NOK;
    }
    cd = iconv_open(param->toCharset, param->fromCharset);
    if (cd == 0) {
        return ICONV_NOK;
    }

    if (param->outBuf == NULL) {
        (VOID)iconv_close(cd);
        return ICONV_NOK;
    }

    ret = memset_s(param->outBuf, param->outLen, 0, param->outLen);
    if (ret != EOK) {
        (VOID)iconv_close(cd);
        return ICONV_NOK;
    }

    if ((INT32)iconv(cd, &param->inBuf, (size_t *)&param->inLen, &param->outBuf,
                     (size_t *)&param->outLen) == ICONV_NOK) {
        (VOID)iconv_close(cd);
        return ICONV_NOK;
    }
    (VOID)iconv_close(cd);
    return ICONV_OK;
}

STATIC INT32 UTF8ToGB18030(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "utf-8";
    param.toCharset = "GB18030";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 GB18030ToUTF8(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "GB18030";
    param.toCharset = "utf-8";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 UTF8ToG2312(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "utf-8";
    param.toCharset = "gb2312";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 G2312ToUTF8(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "gb2312";
    param.toCharset = "utf-8";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 UTF8ToGBK(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "utf-8";
    param.toCharset = "GBK";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 GBKToUTF8(CHAR *inBuf, UINT32 inLen, CHAR *outBuf, UINT32 outLen)
{
    IcomvConvertParam param = {0};
    param.fromCharset = "GBK";
    param.toCharset = "utf-8";
    param.inBuf = inBuf;
    param.inLen = inLen;
    param.outBuf = outBuf;
    param.outLen = outLen;
    return CodeConvert(&param);
}

STATIC INT32 GB2312Demo(VOID)
{
    CHAR utf8Buf[] = "\xE6\xAD\xA3\xE5\x9C\xA8\xE5\xAE\x89\xE8\xA3\x85";
    CHAR gb2312Buf[BUF_LEN] = {0};
    CHAR out[OUT_LEN] = {0};
    INT32 ret;

    ret = UTF8ToG2312(utf8Buf, strlen(utf8Buf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("unicode -> gb2312 : %s -> %s\n", utf8Buf, out);

    ret = memcpy_s(gb2312Buf, BUF_LEN, out, strlen(out));
    if (ret != EOK) {
        return ret;
    }
    ret = memset_s(out, OUT_LEN, 0, OUT_LEN);
    if (ret != EOK) {
        return ret;
    }

    ret = G2312ToUTF8(gb2312Buf, strlen(gb2312Buf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("gb2312  -> unicode: %s -> %s\n", gb2312Buf, out);
    return ICONV_OK;
}

STATIC INT32 GBKDemo(VOID)
{
    CHAR utf8Buf[] = "\xE6\xAD\xA3\xE5\x9C\xA8\xE5\xAE\x89\xE8\xA3\x85";
    CHAR gbkBuf[BUF_LEN] = {0};
    CHAR out[OUT_LEN] = {0};
    INT32 ret;

    ret = UTF8ToGBK(utf8Buf, strlen(utf8Buf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("unicode -> gdk    : %s -> %s\n", utf8Buf, out);

    ret = memcpy_s(gbkBuf, BUF_LEN, out, strlen(out));
    if (ret != EOK) {
        return ret;
    }
    ret = memset_s(out, OUT_LEN, 0, OUT_LEN);
    if (ret != EOK) {
        return ret;
    }

    ret = GBKToUTF8(gbkBuf, strlen(gbkBuf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("gdk     -> unicode: %s -> %s\n", gbkBuf, out);
    return ICONV_OK;
}

STATIC INT32 GB18030Demo(VOID)
{
    CHAR utf8Buf[] = "\xE6\xAD\xA3\xE5\x9C\xA8\xE5\xAE\x89\xE8\xA3\x85";
    CHAR gb18030Buf[BUF_LEN] = {0};
    CHAR out[OUT_LEN] = {0};
    INT32 ret;

    ret = UTF8ToGB18030(utf8Buf, strlen(utf8Buf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("unicode -> gb18030: %s -> %s\n", utf8Buf, out);

    ret = memcpy_s(gb18030Buf, BUF_LEN, out, strlen(out));
    if (ret != EOK) {
        return ret;
    }
    ret = memset_s(out, OUT_LEN, 0, OUT_LEN);
    if (ret != EOK) {
        return ret;
    }

    ret = GB18030ToUTF8(gb18030Buf, strlen(gb18030Buf), out, OUT_LEN);
    if (ret != ICONV_OK) {
        return ret;
    }
    printf("gb18030 -> unicode: %s -> %s\n", gb18030Buf, out);
    return ICONV_OK;
}

STATIC UINT32 DemoTaskEntry(VOID)
{
    INT32 ret;

    printf("Iconv demo task start to run.\n");
    ret = GB2312Demo();
    if (ret != ICONV_OK) {
        printf("GB2312Demo fail.\n");
    }

    ret = GBKDemo();
    if (ret != ICONV_OK) {
        printf("GBKDemo fail.\n");
    }

    ret = GB18030Demo();
    if (ret != ICONV_OK) {
        printf("GB18030Demo fail.\n");
    }
    printf("Iconv demo task finished.\n");
    return (UINT32)ret;
}

VOID IconvDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = ICONV_TASK_STACK_SIZE;
    taskInitParam.pcName = "IconvDemoTask";
    taskInitParam.usTaskPrio = ICONV_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create iconv demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
