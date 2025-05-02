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

#if defined(LOSCFG_SHELL) && defined(LOSCFG_DEBUG_SEMAPHORE)
static UINT32 TestCase(VOID)
{
    int ret;
    const char *argv[20] = {"1", "5", "-1", "0xf", "0", "aaa", "1023", "1024", "", "?", "*@&#$$", "-help", "2m"}; // 20

    dprintf("\r\n **sem, print all sem used \n");
    ret = OsShellCmdSemInfoGet(0, argv);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem 1, print sem 1 message .\n");
    ret = OsShellCmdSemInfoGet(1, argv);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem with two parameters .tips: sem usage. \n");
    ret = OsShellCmdSemInfoGet(2, argv); // argv 2
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem 5 . print sem 5 message. \n");
    ret = OsShellCmdSemInfoGet(1, &argv[1]);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem -1 . tips: sem ID can't access -1 \n");
    ret = OsShellCmdSemInfoGet(1, &argv[2]); // argv:2
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem  0xf . print sem 0xf message. \n");
    ret = OsShellCmdSemInfoGet(1, &argv[3]); // argv:3
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem 0. print sem 0 message.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[4]); // argv:4
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem aaa. tips: sem ID can't access aaa.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[5]); // argv:5
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **semID is OS_SEM_MAX_SUPPORT_NUM . tips:The semphore is not ""in use!\n");
    ret = OsShellCmdSemInfoGet(1, &argv[6]); // argv:6
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **semID is OS_SEM_MAX_SUPPORT_NUM +1. tips:Invalid semphore id!\n");
    ret = OsShellCmdSemInfoGet(1, &argv[7]); // argv:7
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem whith parameter NUll. print sem 0 message\n");
    ret = OsShellCmdSemInfoGet(1, &argv[8]); // argv:8
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    dprintf("\r\n **sem ? .tips: sem ID can't access ?.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[9]); // argv:9
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem *@&#$$ .tips: sem ID can't access *@&#$$.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[10]); // argv:10
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem -help .tips :sem ID can't access -help.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[11]); // argv:11
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    dprintf("\r\n **sem 2m. tips:sem ID can't access 2m.\n");
    ret = OsShellCmdSemInfoGet(1, &argv[12]); // argv:12
    ICUNIT_ASSERT_EQUAL(ret, OS_ERROR, ret);

    return LOS_OK;
}

/**
 * @ingroup TEST_MISC
 * @par TestCase_Number
 * ItShellCmdSem001
 * @par TestCase_TestCase_Type
 * Function test
 * @brief OsShellCmdSemInfoGet API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * Verifying the Functions of the OsShellCmdSemInfoGet API Through Different Parameters.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItShellCmdSem001(VOID)
{
    TEST_ADD_CASE("ItShellCmdSem001", TestCase, TEST_SHELL, TEST_MISC, TEST_LEVEL0, TEST_FUNCTION);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
