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
#include "netif/ethernet.h"
#include "lwip/ethnip.h"

int CmdNipNeigh(int argc, const char **argv)
{
    if (argc == 0) {
        show_table_nip();
    } else if (strcmp(argv[0], "add") == 0){
        if (argc != 4) {
            printf("Wrong number of parameters.\n");
            return -1;
        }
        nip_addr_t *ipaddr = NULL;
        ipaddr = malloc(sizeof(*ipaddr));
        int aton_err = nipaddr_aton2(argv[1], argv[2], ipaddr);
        if (aton_err < 0) {
            printf("Address format error.\n");
            return -1;
        }
        struct eth_addr ethaddr;
        if (strlen(argv[3]) != 12) {
            printf("MAC address error.\n");
            return -1;
        }
        for (int i = 0; i < 6; i++){
            int a = hex_digit_value(argv[3][i * 2]);
            int b = hex_digit_value(argv[3][i * 2 + 1]);
            if ((a == -1) || (b == -1)) {
                printf("MAC address error.\n");
                return -1;
            }
            ethaddr.addr[i] = (uint8_t)(a * 16 + b);
        }
        int err = add_table_nip(*ipaddr, ethaddr);
        if (err == -1) {
            printf("The number has reached the upper limit.\n");
        }
        if (err == -2) {
            printf("IP already exists.\n");
        }
        return err;
    } else if (strcmp(argv[0], "del") == 0) {
        if (argc != 3) {
            printf("Wrong number of parameters.\n");
            return -1;
        }
        nip_addr_t *ipaddr;
        ipaddr = malloc(sizeof(*ipaddr));
        int aton_err = nipaddr_aton2(argv[1], argv[2], ipaddr);
        if (aton_err < 0) {
            printf("Address format error.\n");
            return -1;
        }
        int err = del_table_nip(*ipaddr);
        if (err == -1) {
            printf("IP does not exist.\n");
        }
        return err;
    } else if (strcmp(argv[0], "set") == 0) {
        if (argc != 4) {
            printf("Wrong number of parameters.\n");
            return -1;
        }
        nip_addr_t *ipaddr = NULL;
        ipaddr = malloc(sizeof(*ipaddr));
        int aton_err = nipaddr_aton2(argv[1], argv[2], ipaddr);
        if (aton_err < 0) {
           return aton_err;
        }
        struct eth_addr ethaddr;
        if (strlen(argv[3]) != 12) {
            printf("MAC address error.\n");
            return -1;
        }
        for (int i = 0; i < 6; i++) {
            int a = hex_digit_value(argv[3][i * 2]);
            int b = hex_digit_value(argv[3][i * 2 + 1]);
            if (a == -1 || b == -1) {
                printf("MAC address error.\n");
                return -1;
            }
            ethaddr.addr[i] = (uint8_t)(a * 16 + b);
            printf("%2x ", ethaddr.addr[i]);
        }
        printf("\n");
        int err = set_table_nip(*ipaddr, ethaddr);
        if (err == -1) {
            printf("IP does not exist.\n");
        }
        return err;
    } else {
        printf("No command.\n");
        return -1;
    }
    return -1;
}
#endif /* LOSCFG_LWIP_NIP */
