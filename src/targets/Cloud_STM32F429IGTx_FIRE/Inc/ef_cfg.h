/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: EasyFlash Cfg HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-07-10
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

#ifndef _EF_CFG_H
#define _EF_CFG_H

#ifdef LOSCFG_COMPONENTS_EASYFLASH
/* using ENV function */
#define EF_USING_ENV

#ifdef EF_USING_ENV

/**
 * ENV version number defined by user.
 * Please change it when your firmware add a new ENV to default_env_set.
 */
#define EF_ENV_VER_NUM      1.0
#endif /* EF_USING_ENV */

/* the minimum size of flash erasure */
#define EF_ERASE_MIN_SIZE 4096

/* backup area start address */
#define EF_START_ADDR 0

/* the user setting size of ENV, must be word alignment */
#define ENV_USER_SETTING_SIZE (4 * EF_ERASE_MIN_SIZE)

/* ENV area total bytes size in normal mode. */
#define ENV_AREA_SIZE (4 * EF_ERASE_MIN_SIZE)

/* saved log area size */
#define LOG_AREA_SIZE (2 * EF_ERASE_MIN_SIZE)

#endif /* LOSCFG_COMPONENTS_EASYFLASH */
#endif /* _EF_CFG_H */
