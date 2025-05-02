/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Jerry Main Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-19
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
#include <stdlib.h>
#include <assert.h>

#include "unistd.h"
#include "fcntl.h"
#include "securec.h"

#include "jerry_main.h"
#include "jerryscript.h"
#include "jerryscript-ext/handler.h"
#include "jerryscript-port-default.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static void Usage(void)
{
    printf("\nUsage: jerry [options]\n");
    printf("Options:\n");
    printf("  -h, --help      print this help\n");
    printf("  -v, --version   print tool and library version\n");
    printf("  FILE            input JS file\n");
}

static void *contextAlloc(size_t size, void *pCbData)
{
    (void)pCbData;
    return malloc(size);
}

jerry_value_t JerryExit(const jerry_value_t function_obj, const jerry_value_t this_val,
                        const jerry_value_t args_p[], const jerry_length_t args_count)
{
    printf("exit functions are not supported.\n");
    return 0;
}

jerry_value_t JerryAssert(const jerry_value_t function_obj, const jerry_value_t this_val,
                          const jerry_value_t args_p[], const jerry_length_t args_count)
{
    printf("assert functions are not supported.\n");
    return 0;
}

static int ReadJsFile(const char *fileName, char **script)
{
    int fd, fileSize, readLen;

    if (fileName == NULL || script == NULL) {
        return -1;
    }

    fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        printf("Open file %s failed.\n", fileName);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);
    fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    *script = (char *)malloc(fileSize + 1);
    if (*script == NULL) {
        close(fd);
        return -1;
    }
    (void)memset_s(*script, fileSize, 0, fileSize);

    readLen = read(fd, *script, fileSize);
    if (readLen < 0) {
        printf("Read %s failed.\n", fileName);
        free(*script);
        close(fd);
        return -1;
    }

    close(fd);
    return readLen;
}

static int JerryEntry(int argc, char *argv[])
{
    char *script = NULL;
    int sourceSize;
    jerry_value_t retValue;
    const char *filePath = argv[1];
    uint32_t parseOpts = JERRY_PARSE_NO_OPTS;

#ifdef JERRY_GLOBAL_HEAP_SIZE
    jerry_context_t *pContext = jerry_create_context(JERRY_GLOBAL_HEAP_SIZE * 1024, contextAlloc, NULL); // 1024: 1M
    jerry_port_default_set_current_context(pContext);
#endif

    jerry_init(JERRY_INIT_EMPTY);   /* Initialize engine */
    jerryx_handler_register_global((const jerry_char_t *)"print", jerryx_handler_print);
    jerryx_handler_register_global((const jerry_char_t *)"assert", JerryAssert);
    jerryx_handler_register_global((const jerry_char_t *)"exit", JerryExit);

    retValue = jerry_create_undefined();
    sourceSize = ReadJsFile((const char *)filePath, &script);
    if ((script == NULL) || (sourceSize < 0)) {
        printf("Source file load error.\n");
        jerry_cleanup();
        return -1;
    }

    retValue = jerry_parse((jerry_char_t *)filePath, strlen(filePath), (const jerry_char_t*)script,
                           sourceSize, parseOpts);
    jerry_port_release_source((uint8_t *)script);
    if (!jerry_value_is_error(retValue)) {
        jerry_value_t funcVal = retValue;
        retValue = jerry_run(funcVal);
        jerry_release_value(funcVal);
    }

    if (jerry_value_is_error(retValue)) {
        printf("Script error!");
        jerry_cleanup();
        return -1;
    }

    retValue = jerry_run_all_enqueued_jobs();
    if (jerry_value_is_error(retValue)) {
        jerry_cleanup();
        return -1;
    }

    jerry_release_value(retValue);
    jerry_cleanup();
#ifdef JERRY_GLOBAL_HEAP_SIZE
    free(pContext);
#endif
    return 0;
}

int JerryMain(int argc, char *argv[])
{
    int ret;
    int i = 1;          /* argv[1] */
    if (argc != 2) {    /* 2: Number of parameters */
        Usage();
        return -1;
    }

    if (((strlen("-h") == strlen(argv[i])) && (strncmp("-h", argv[i], strlen("-h")) == 0)) ||
        ((strlen("--help") == strlen(argv[i])) && (strncmp("--help", argv[i], strlen("--help")) == 0))) {
        Usage();
        ret = 0;
    } else if (((strlen("-v") == strlen(argv[i])) && (strncmp("-v", argv[i], strlen("-v")) == 0)) ||
                ((strlen("--version") == strlen(argv[i])) &&
                (strncmp("--version", argv[i], strlen("--version")) == 0))) {
        printf ("Version: %d.%d.%d%s\n", JERRY_API_MAJOR_VERSION, JERRY_API_MINOR_VERSION,
                JERRY_API_PATCH_VERSION, JERRY_COMMIT_HASH);
        ret = 0;
    } else {
        ret = JerryEntry(argc, argv);
    }

    if (ret < 0) {
        Usage();
    }
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
