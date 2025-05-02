/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: NewIP Task Implementation
 * Author: Huawei LiteOS Team
 * Create: 2022-02-17
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

#if LOSCFG_LWIP_NIP
#include "sys_init.h"
#include "demo_entry.h"
#include "nip_bupt.h"
#include "shell.h"
#include "print.h"

VOID NewipCmdTask(VOID)
{
    osCmdReg(CMD_TYPE_EX, "udpclient", 2, (CMD_CBK_FUNC)CmdUdpClient);
    osCmdReg(CMD_TYPE_EX, "udpserver", 1, (CMD_CBK_FUNC)CmdUdpserver);
    osCmdReg(CMD_TYPE_EX, "nip_neigh", 4, (CMD_CBK_FUNC)CmdNipNeigh);
    osCmdReg(CMD_TYPE_EX, "nip_addr", 2, (CMD_CBK_FUNC)CmdNipAddr);
    osCmdReg(CMD_TYPE_EX, "mac", 2, (CMD_CBK_FUNC)CmdMac);
    osCmdReg(CMD_TYPE_EX, "kill", 1, (CMD_CBK_FUNC)CmdKill);
    osCmdReg(CMD_TYPE_EX, "nip_gw", 2, (CMD_CBK_FUNC)CmdNipGw);
    osCmdReg(CMD_TYPE_EX, "print", 0, (CMD_CBK_FUNC)CmdPrint);
}
#endif /* LOSCFG_LWIP_NIP */
