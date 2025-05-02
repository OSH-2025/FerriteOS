/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Fastlz Shell Command Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-05-07
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

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#endif

#include "fastlz_shellcmd.h"
#include "fastlz.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HEADER_BLOCK_SIZE      4
#define FAATLZ_HEADER_SIZE     8
#define INPUT_BUFF_SIZE        2048

/* The output buffer needs to be larger than the input buffer, 128 is the minimum value */
#define OUTPUT_BUFF_SIZE       (INPUT_BUFF_SIZE + 128 + ((INPUT_BUFF_SIZE) * 5 / 100))

STATIC CHAR g_fastlzHeader[FAATLZ_HEADER_SIZE] = {9, 'F', 'a', 's', 't', 'L', 'Z', 7};

typedef struct {
    INT32 outFileLen;
    INT32 rSize;
    INT32 chunkSize;
    INT32 inFileSize;
    INT32 inFd;
    INT32 outFd;
    CHAR *inBuff;
    CHAR *outBuff;
    CHAR header[HEADER_BLOCK_SIZE];
} FastlzInfo;

STATIC VOID ClearUp(FastlzInfo *fastlz)
{
    if (fastlz->inFd > 0) {
        close(fastlz->inFd);
    }
    if (fastlz->outFd > 0) {
        close(fastlz->outFd);
    }
    if (fastlz->inBuff != NULL) {
        free(fastlz->inBuff);
    }
    if (fastlz->outBuff != NULL) {
        free(fastlz->outBuff);
    }
    if (fastlz != NULL) {
        free(fastlz);
    }
}

STATIC INT32 WriteFastlzHeader(INT32 fd)
{
    return write(fd, g_fastlzHeader, FAATLZ_HEADER_SIZE);
}

STATIC INT32 CompareFastlzHeader(INT32 fd)
{
    INT32 i;
    INT32 ret;
    CHAR buff[FAATLZ_HEADER_SIZE] = {0};

    lseek(fd, 0, SEEK_SET);
    ret = read(fd, buff, FAATLZ_HEADER_SIZE);
    if (ret < FAATLZ_HEADER_SIZE) {
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    for (i = 0; i < FAATLZ_HEADER_SIZE; i++) {
        if (buff[i] != g_fastlzHeader[i]) {
            return -1;
        }
    }
    return 0;
}

STATIC INT32 GetInFileSize(INT32 inFd, const CHAR *inFile, const CHAR *flagStr)
{
    INT32 inFileSize = 0;

    if (memcmp("FastlzCompressDemo", flagStr, strlen(flagStr)) == 0) {
        if (CompareFastlzHeader(inFd) == 0) {
            printf("File %s is already a fastlz compressed file.\n", inFile);
            return -1;
        }
        lseek(inFd, 0, SEEK_SET);
        inFileSize = lseek(inFd, 0, SEEK_END);
        lseek(inFd, 0, SEEK_SET);
        printf("The size of the %s file before compression is %d bytes.\n", inFile, inFileSize);
        if (inFileSize < 32) {      /* 32: The file size is less than 32 bytes */
            printf("Too small, don't bother to compress.\n");
            return -1;
        }
    } else if (memcmp("FastlzDcompressDemo", flagStr, strlen(flagStr)) == 0) {
        if (CompareFastlzHeader(inFd) != 0) {
            printf("File %s is not a fastlz compressed file.\n", inFile);
            return -1;
        }
        lseek(inFd, 0, SEEK_SET);
        inFileSize = lseek(inFd, 0, SEEK_END);
        lseek(inFd, FAATLZ_HEADER_SIZE, SEEK_SET);
        printf("The size of the %s file before decompression is %d bytes.\n", inFile, inFileSize);
    }
    return inFileSize;
}

STATIC INT32 FastlzInit(FastlzInfo *fastlz, const CHAR *inFile,
                        const CHAR *outFile, const CHAR *flagStr)
{
    if ((inFile == NULL) || (outFile == NULL)) {
        printf("Parameter error.\n");
        return -1;
    }

    fastlz->inFd = open(inFile, O_RDONLY);
    if (fastlz->inFd < 0) {
        printf("Open file %s failed.\n", inFile);
        return -1;
    }

    fastlz->outFd = open(outFile, O_RDONLY);
    if (fastlz->outFd > 0) {
        printf("File %s already exists.\n", outFile);
        return -1;
    }

    fastlz->inFileSize = GetInFileSize(fastlz->inFd, inFile, flagStr);
    if (fastlz->inFileSize < 0) {
        return -1;
    }

    fastlz->outFd = open(outFile, O_CREAT | O_WRONLY | O_TRUNC, 0664); /* 0664: File Permissions */
    if (fastlz->outFd < 0) {
        printf("Open file %s failed.\n", outFile);
        return -1;
    }

    return 0;
}

INT32 FastlzCompress(const CHAR *inFile, const CHAR *outFile, INT32 compressLevel)
{
    INT32 i;
    INT32 ret;
    FastlzInfo *fastlz = NULL;

    fastlz = (FastlzInfo *)malloc(sizeof(FastlzInfo));
    if (fastlz == NULL) {
        return -1;
    }
    (VOID)memset_s(fastlz, sizeof(FastlzInfo), 0, sizeof(FastlzInfo));

    ret = FastlzInit(fastlz, inFile, outFile, "FastlzCompressDemo");
    if (ret < 0) {
        ClearUp(fastlz);
        return -1;
    }

    fastlz->inBuff = (CHAR *)malloc(INPUT_BUFF_SIZE);
    if (fastlz->inBuff == NULL) {
        ClearUp(fastlz);
        return -1;
    }
    fastlz->outBuff = (CHAR *)malloc(OUTPUT_BUFF_SIZE);
    if (fastlz->outBuff == NULL) {
        ClearUp(fastlz);
        return -1;
    }

    fastlz->outFileLen = WriteFastlzHeader(fastlz->outFd);

    for (i = 0; i < fastlz->inFileSize; i += fastlz->rSize) {
        (VOID)memset_s(fastlz->inBuff, INPUT_BUFF_SIZE, 0, INPUT_BUFF_SIZE);
        (VOID)memset_s(fastlz->outBuff, INPUT_BUFF_SIZE, 0, INPUT_BUFF_SIZE);
        (VOID)memset_s(fastlz->header, HEADER_BLOCK_SIZE, 0, HEADER_BLOCK_SIZE);

        fastlz->rSize = read(fastlz->inFd, fastlz->inBuff, INPUT_BUFF_SIZE);
        if (fastlz->rSize == 0) {
            break;
        }

        fastlz->chunkSize = fastlz_compress_level(compressLevel, fastlz->inBuff, fastlz->rSize, fastlz->outBuff);
        if (fastlz->chunkSize < 0) {
            ClearUp(fastlz);
            return -1;
        }

        fastlz->header[0] = fastlz->chunkSize & 0xFF;
        fastlz->header[1] = (fastlz->chunkSize >> 8) & 0xFF;    /* 8: shift right by 8 bits */
        fastlz->header[2] = (fastlz->chunkSize >> 16) & 0xFF;   /* 2: second byte, 16: shift right by 16 bits */
        fastlz->header[3] = (fastlz->chunkSize >> 24) & 0xFF;   /* 3: third byte, 24: shift right by 24 bits */

        ret = write(fastlz->outFd, fastlz->header, HEADER_BLOCK_SIZE);
        fastlz->outFileLen += ret;
        ret = write(fastlz->outFd, fastlz->outBuff, fastlz->chunkSize);
        fastlz->outFileLen += ret;
        printf(".");
    }

    printf("\nThe size of the %s file after compression is %d bytes, compression ratio is %d%%.\n", inFile,
           fastlz->outFileLen, ((fastlz->outFileLen * 100) / fastlz->inFileSize));  /* 100: Convert to Percentage */

    ClearUp(fastlz);
    return 0;
}

INT32 FastlzDcompress(const CHAR *inFile, const CHAR *outFile)
{
    INT32 i;
    INT32 ret;
    FastlzInfo *fastlz = NULL;

    fastlz = (FastlzInfo *)malloc(sizeof(FastlzInfo));
    if (fastlz == NULL) {
        return -1;
    }
    (VOID)memset_s(fastlz, sizeof(FastlzInfo), 0, sizeof(FastlzInfo));

    ret = FastlzInit(fastlz, inFile, outFile, "FastlzDcompressDemo");
    if (ret < 0) {
        ClearUp(fastlz);
        return -1;
    }

    fastlz->inBuff = (CHAR *)malloc(OUTPUT_BUFF_SIZE);
    if (fastlz->inBuff == NULL) {
        ClearUp(fastlz);
        return -1;
    }
    fastlz->outBuff = (CHAR *)malloc(INPUT_BUFF_SIZE);
    if (fastlz->outBuff == NULL) {
        ClearUp(fastlz);
        return -1;
    }

    for (i = 0; i < (fastlz->inFileSize - FAATLZ_HEADER_SIZE); i += fastlz->rSize + HEADER_BLOCK_SIZE) {
        (VOID)memset_s(fastlz->inBuff, INPUT_BUFF_SIZE, 0, INPUT_BUFF_SIZE);
        (VOID)memset_s(fastlz->outBuff, INPUT_BUFF_SIZE, 0, INPUT_BUFF_SIZE);
        (VOID)memset_s(fastlz->header, HEADER_BLOCK_SIZE, 0, HEADER_BLOCK_SIZE);

        read(fastlz->inFd, fastlz->header, HEADER_BLOCK_SIZE);

        fastlz->chunkSize = (fastlz->header[0] + (fastlz->header[1] << 8) /* shift left by 8 bits */
                + (fastlz->header[2] << 16)                 /* 2: third byte, 16: shift left by 16 bits */
                + (fastlz->header[3] << 24)) & 0xFFFFFFFF;  /* 3: second byte, 24: shift left by 24 bits */

        fastlz->rSize = read(fastlz->inFd, fastlz->inBuff, fastlz->chunkSize);
        if (fastlz->rSize == 0) {
            break;
        }

        ret = fastlz_decompress(fastlz->inBuff, fastlz->rSize, fastlz->outBuff, INPUT_BUFF_SIZE);
        if (ret < 0) {
            ClearUp(fastlz);
            return -1;
        }
        ret = write(fastlz->outFd, fastlz->outBuff, ret);
        fastlz->outFileLen += ret;
        printf(".");
    }

    printf("\nThe size of the %s file after decompression is %d bytes.\n", inFile, fastlz->outFileLen);

    ClearUp(fastlz);
    return 0;
}

STATIC INT32 Usage(VOID)
{
    printf("Usage: fastlz [options] input-file  output-file\n");
    printf("Options:\n");
    printf("  -c -1    compress faster\n");
    printf("  -c -2    compress better\n");
    printf("  -d       decompress\n");
    return 0;
}

STATIC INT32 FastlzShellCmd(INT32 argc, CHAR **argv)
{
    INT32 i;
    INT32 ret = 0;
    CHAR *flagStr = NULL;
    CHAR *inputFile = NULL;
    CHAR *outputFile = NULL;
    INT32 compressLevel;

    if (argc < 3 || argc > 4) {     /* 3/4: number of parameters */
        Usage();
        return 0;
    }

    compressLevel = 2;              /* 2: compress better */
    for (i = 0; i < argc; i++) {
        if ((memcmp("-c", argv[i], strlen(argv[i])) == 0)
            || (memcmp("-d", argv[i], strlen(argv[i])) == 0)) {
            flagStr = argv[i];
            continue;
        }

        if (memcmp("-1", argv[i], strlen(argv[i])) == 0) {
            compressLevel = 1;      /* 1: compress faster */
            continue;
        }

        if (memcmp("-2", argv[i], strlen(argv[i])) == 0) {
            compressLevel = 2;      /* 2: compress better */
            continue;
        }

        if (inputFile == NULL) {
            inputFile = argv[i];
            continue;
        }

        if (outputFile == NULL) {
            outputFile = argv[i];
            continue;
        }
        Usage();
        return -1;
    }

    if ((inputFile == NULL) || (outputFile == NULL)) {
        Usage();
        return -1;
    }

    if (memcmp("-c", flagStr, strlen(flagStr)) == 0) {
        ret = FastlzCompress(inputFile, outputFile, compressLevel);
    } else if (memcmp("-d", flagStr, strlen(flagStr)) == 0) {
        ret = FastlzDcompress(inputFile, outputFile);
    }

    return ret;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(fastlz_shellcmd, CMD_TYPE_EX, "fastlz", XARGS, (CMD_CBK_FUNC)FastlzShellCmd);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
