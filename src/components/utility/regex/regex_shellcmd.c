/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tiny-regx-c Shell Command Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-22
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

#include "re.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct regex_t RegexT;
extern void re_print(RegexT *pattern);

static void Usage(void)
{
    printf("\nUsage: regex [options]\n");
    printf("Options:\n");
    printf("  h, --help      print this help\n");
    printf("  <PATTERN> <TEXT>\n\n");

    printf("  eg: regex \\D+ Liteos-regex-test-97\n");
    printf("      regex \\D Liteos-regex-test-97\n");
    printf("      regex \\d Liteos-regex-test-97\n");
}

static int TinyRegexShellCmd(int argc, char **argv)
{
    RegexT *reCompile = NULL;
    if ((argc == 1) && (((strlen("-h") == strlen(argv[0])) && (strncmp("-h", argv[0], strlen("-h")) == 0)) ||
        ((strlen("--help") == strlen(argv[0])) && (strncmp("--help", argv[0], strlen("--help")) == 0)))) {
        Usage();
        return 0;
    }

    int length;
    if (argc == 2) { // Determine whether the input parameter is 2.
        int m = re_match(argv[0], argv[1], &length);
        if (m != -1) {
            printf("regex run success [%d].\n", length);
            return 0;
        }
        reCompile = (re_t)re_compile(argv[0]);
        if (reCompile != NULL) {
            re_print(reCompile);
        }
        printf("pattern '%s' matched '%s' unexpectedly, matched %i chars. \n", argv[0], argv[1], length);
    } else {
        printf("Input error, please execute 'regex -h' to view help.");
    }
    return -2; // -2, Shell error return code.
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(regexShellcmd, CMD_TYPE_EX, "regex", XARGS, (CMD_CBK_FUNC)TinyRegexShellCmd);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
