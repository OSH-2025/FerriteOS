/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Misc Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

#include "it_los_misc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_SHELL
static UINT32 TestCase(VOID)
{
    int ret;
    const char *argv[20] = {"-a", "-s", "-t", "-v", "--help", "0", "1",  "-1", "",   "?",  "*@&#$$"}; // 20

    dprintf("\r\n **uname .print kernel name.   \n");
    ret = OsShellCmdUname(0, argv);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    dprintf("\r\n **Two parameters, tips: uname usage.\n");
    ret = OsShellCmdUname(2, argv); // argc:2
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **uname -a, print kernel name,build date,kernel version .\n");
    ret = OsShellCmdUname(1, argv);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **uname -s ,print kernel name. \n");
    ret = OsShellCmdUname(1, &argv[1]);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **uname -t , print kernel build date .\n");
    ret = OsShellCmdUname(1, &argv[2]); // argv:2
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **uname -v ,print kernel version and build date. \n");
    ret = OsShellCmdUname(1, &argv[3]); // argv:3
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **uname --help, print uname usage. \n");
    ret = OsShellCmdUname(1, &argv[4]); // argv:4
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **uname 0, tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[5]); // argv:5
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **uname 1, tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[6]); // argv:6
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **uname -1, tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[7]); // argv:7
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n** uname with parameter NULL, tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[8]); // argv:8
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **uname ? .tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[9]); // argv:9
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **uname @&#$$ . tips:uname usage.\n");
    ret = OsShellCmdUname(1, &argv[10]); // argv:10
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    return LOS_OK;
}
#endif

/**
 * @ingroup TEST_MISC
 * @par TestCase_Number
 * ItShellCmdUname001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief OsShellCmdUname API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Verifying the Functions of the OsShellCmdUname API Through Different Parameters.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItShellCmdUname001(VOID)
{
#ifdef LOSCFG_SHELL
    TEST_ADD_CASE("ItShellCmdUname001", TestCase, TEST_SHELL, TEST_MISC, TEST_LEVEL0, TEST_FUNCTION);
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
