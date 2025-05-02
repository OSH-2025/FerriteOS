/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: NewIP Addres Command Implementation
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
#include "shell.h"
#include "shcmd.h"

#include "lwip/inet.h"
#include "lwip/nip_addr.h"
#include "lwip/netif.h"

int CmdNipGw(int argc, const char **argv)
{
    if (argc == 0) {
        nip_addr_debug_print_val(0x80U, netif_default->nip_gw);
        printf("\n");
    } else if (argc != 2) {
        printf("Wrong number of parameters.");
        return -1;
    } else {
        nip_addr_t *ipaddr;
        ipaddr = malloc(sizeof(*ipaddr));
        int aton_err = nipaddr_aton2(argv[0], argv[1], ipaddr);
        if(aton_err < 0) {
            printf("Address format error.");
            return -1;
        }
        netif_do_set_nipgw(netif_default, ipaddr, NULL);
        return 0;
    }
    return 0;
}
#endif
