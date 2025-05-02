/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ifconfig Shell Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-20
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

#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/netifapi.h"
#include "lwip/api.h"

#ifdef LOSCFG_SHELL
#include "shcmd.h"
#include "shell.h"
#endif /* LOSCFG_SHELL */

#define TYPE_VAL_ARGS                   2

#define CODE_SIZE_CONVERSION            1024
#define CODE_SIZE_CONVERSION_F          1024.0

int PrintNetif(struct netif *netif, char *printBuf, unsigned int bufLen);
void LwipIfconfigShowInternal(void *arg);
void LwipIfconfigInternal(void *arg);
void LwipPrintsize(size_t size);
static void LwipIfconfigUsage(const char *cmd);
struct netif *g_locNetif = NULL;

#define IFCONFIG_OPTION_SET_IP          (1)
#define IFCONFIG_OPTION_SET_NETMASK     (1 << 1)
#define IFCONFIG_OPTION_SET_GW          (1 << 2)
#define IFCONFIG_OPTION_SET_UP          (1 << 4)
#define IFCONFIG_OPTION_SET_DOWN        (1 << 5)
#define IFCONFIG_OPTION_SET_MTU         (1 << 6)
#define IFCONFIG_OPTION_DEL_IP          (1 << 7)

#define PRINT_BUF_LEN                   1024

#define ERR_IFCONFIG_STRING_PUT(ret, str)                                                   \
    do {                                                                                    \
        (ret) = snprintf_s(ifconfigCmd->cbPrintBuf + ifconfigCmd->printLen,                 \
            PRINT_BUF_LEN - ifconfigCmd->printLen,                                          \
            ((PRINT_BUF_LEN - ifconfigCmd->printLen) - 1),                                  \
            "%s\t",                                                                         \
            (str));                                                                         \
        if (((ret) > 0) && ((unsigned int)(ret) < (PRINT_BUF_LEN - ifconfigCmd->printLen))) \
            ifconfigCmd->printLen += (unsigned int)(ret);                                   \
    } while (0)

struct IfconfigOption {
    char iface[NETIF_NAMESIZE];
    unsigned int option;
    ip_addr_t ipAddr;
    ip_addr_t netmask;
    ip_addr_t gw;
    u16_t mtu;
    /* when using telnet, print to the telnet socket will result in system  */
    /* deadlock. So we cahe the prinf data to a buf, and when the tcpip     */
    /* callback returns, then print the data out to the telnet socket       */
    sys_sem_t cb_completed;
    char cbPrintBuf[PRINT_BUF_LEN];
    unsigned int printLen;
};

int PrintNetif(struct netif *netif, char *printBuf, unsigned int bufLen)
{
    int i, ret;
    char *tmp = printBuf;

    if (netif == NULL) {
        return -1;
    }
    if (bufLen > PRINT_BUF_LEN) {
        printf("Buflen is too large.\n");
        return -1;
    }
    if (sizeof(netif->name) + sizeof(netif->num) >= bufLen) {
        return -1;
    }
    if (netif->link_layer_type == LOOPBACK_IF) {
        ret = snprintf_s(tmp, bufLen, (bufLen - 1), "%s\t", netif->name);
    } else {
        ret = snprintf_s(tmp, bufLen, (bufLen - 1), "%s%u\t", netif->name, netif->num);
    }
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;
#if LWIP_IPV4
    ret = snprintf_s(tmp, bufLen, (bufLen - 1), "ip:%s ", ipaddr_ntoa(&netif->ip_addr));
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;

    ret = snprintf_s(tmp, bufLen, (bufLen - 1), "netmask:%s ", ipaddr_ntoa(&netif->netmask));
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;

    ret = snprintf_s(tmp, bufLen, (bufLen - 1), "gateway:%s\n", ipaddr_ntoa(&netif->gw));
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;
#endif

    ret = snprintf_s(tmp, bufLen, (bufLen - 1), "%s", "\tHWaddr:");
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;

    if (bufLen <= netif->hwaddr_len) {
        return -1;
    }
    for (i = 0; i < netif->hwaddr_len - 1; i++) {
        ret = snprintf_s(tmp, bufLen, (bufLen - 1), "%02x:", netif->hwaddr[i]);
        if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
            return (int)(tmp - printBuf);
        }
        tmp += ret;
        bufLen -= (unsigned int)ret;
    }

    ret = snprintf_s(tmp, bufLen, (bufLen - 1), "%02x", netif->hwaddr[i]);
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;

    ret = snprintf_s(
        tmp, bufLen, (bufLen - 1), " MTU:%d %s", netif->mtu, netif->flags & NETIF_FLAG_UP ? "Running" : "Stop");
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;
    bufLen -= (unsigned int)ret;

    if (netif_default == netif && netif_is_up(netif)) {
        ret = snprintf_s(tmp, bufLen, (bufLen - 1), " %s", "Default");
        if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
            return (int)(tmp - printBuf);
        }
        tmp += ret;
        bufLen -= (unsigned int)ret;
    }

    ret = snprintf_s(tmp, bufLen, (bufLen - 1), " %s\n", netif->flags & NETIF_FLAG_LINK_UP ? "Link UP" : "Link Down");
    if ((ret <= 0) || ((unsigned int)ret >= bufLen)) {
        return (int)(tmp - printBuf);
    }
    tmp += ret;

    return (int)(tmp - printBuf);
}

void LwipIfconfigShowInternal(void *arg)
{
    struct netif *netif = NULL;
    struct IfconfigOption *ifconfigCmd = (struct IfconfigOption *)arg;
    int ret;

    if (ifconfigCmd == NULL) {
        printf("Ifconfig cmd is null.\n");
        return;
    }
    if (netif_list == NULL) {
        ret = snprintf_s(ifconfigCmd->cbPrintBuf,
            PRINT_BUF_LEN - ifconfigCmd->printLen,
            ((PRINT_BUF_LEN - ifconfigCmd->printLen) - 1),
            "%s",
            "Device not init.\n");
        if ((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfigCmd->printLen))) {
            ifconfigCmd->printLen += (unsigned int)ret;
        }
        sys_sem_signal(&ifconfigCmd->cb_completed);
        return;
    }

    if (ifconfigCmd->iface[0] == '\0') {
        /* display all netif */
        for (netif = netif_list; netif != NULL; netif = netif->next) {
            ret = PrintNetif(
                netif, ifconfigCmd->cbPrintBuf + ifconfigCmd->printLen, PRINT_BUF_LEN - ifconfigCmd->printLen);
            ifconfigCmd->printLen += (unsigned int)ret;
        }
    } else {
        netif = netif_find(ifconfigCmd->iface);
        if (netif == NULL) {
            ret = snprintf_s(ifconfigCmd->cbPrintBuf + ifconfigCmd->printLen,
                (PRINT_BUF_LEN - ifconfigCmd->printLen),
                ((PRINT_BUF_LEN - ifconfigCmd->printLen) - 1),
                "%s",
                "Device not found.\n");
            if ((ret > 0) && ((unsigned int)ret < (PRINT_BUF_LEN - ifconfigCmd->printLen))) {
                ifconfigCmd->printLen += (unsigned int)ret;
            }

            sys_sem_signal(&ifconfigCmd->cb_completed);
            return;
        }

        ret = PrintNetif(
            netif, ifconfigCmd->cbPrintBuf + ifconfigCmd->printLen, PRINT_BUF_LEN - ifconfigCmd->printLen);
        ifconfigCmd->printLen += (unsigned int)ret;
    }
    sys_sem_signal(&ifconfigCmd->cb_completed);
}


static void LwipIfconfigInternalLink(struct netif *netif, struct IfconfigOption *ifconfigCmd)
{
    if ((netif == NULL) || (ifconfigCmd == NULL)) {
        return;
    }

    if (ifconfigCmd->option & IFCONFIG_OPTION_SET_UP) {
        netif_set_up(netif);
        sys_sem_signal(&ifconfigCmd->cb_completed);
    } else if (ifconfigCmd->option & IFCONFIG_OPTION_SET_DOWN) {
        netif_set_down(netif);
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }
}

static void LwipIfconfigSetIp(struct netif *netif, struct IfconfigOption *ifconfigCmd)
{
    int ret;
    ip_addr_t ipAddr;

    if ((netif == NULL) || (ifconfigCmd == NULL)) {
        return;
    }

    ip_addr_set_val(ipAddr, (ifconfigCmd->ipAddr));
    /* check the address is not multicast/broadcast/0/loopback */
    if (ip_addr_ismulticast_val(ipAddr) || ip_addr_isbroadcast_val(ipAddr, netif) ||
        ip_addr_isany(&ipAddr) || ip_addr_isloopback(&ipAddr)) {
        ERR_IFCONFIG_STRING_PUT(ret, "Don't set ip as a multicast/broadcast/0/loopback address.\n");
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }

    /* reset gateway if new and previous ipaddr not in same net */
    if (!ip_addr_netcmp_val(ipAddr, netif->ip_addr, ip_2_ip4(&netif->netmask))) {
        ip_addr_set_zero(&netif->gw);
        if (netif == netif_default) {
            (void)netif_set_default(NULL);
        }
    }

    /* lwip disallow two netif sit in same net at the same time */
    g_locNetif = netif_list;
    while (g_locNetif != NULL) {
        if (g_locNetif == netif) {
            g_locNetif = g_locNetif->next;
            continue;
        }
        if (IP_IS_V4_VAL(ifconfigCmd->ipAddr) && ip_addr_cmp_zoneless(&netif->netmask, &g_locNetif->netmask) &&
            ip_addr_netcmp_val(g_locNetif->ip_addr, ipAddr, ip_2_ip4(&netif->netmask))) {
            ERR_IFCONFIG_STRING_PUT(ret, "Duplicate network.\n");
            sys_sem_signal(&ifconfigCmd->cb_completed);
        }
        g_locNetif = g_locNetif->next;
    }

#if LWIP_DHCP
    if (netif_dhcp_data(netif) && netif_dhcp_data(netif)->state != DHCP_STATE_OFF) {
        (void)netif_dhcp_off(netif);
    }
#endif
    netif_set_ipaddr(netif, ip_2_ip4(&ipAddr));
}
static void LwipIfconfigSetMask(struct netif *netif, struct IfconfigOption *ifconfigCmd)
{
    int ret;
    ip_addr_t netmask;

    ip_addr_set_val(netmask, (ifconfigCmd->netmask));
    /* check data valid */
    if (!ip_addr_netmask_valid(ip_2_ip4(&netmask))) {
        ERR_IFCONFIG_STRING_PUT(ret, "Ifconfig: netmask is invalid.\n");
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }

#if LWIP_DHCP
    if (netif_dhcp_data(netif) && netif_dhcp_data(netif)->state != DHCP_STATE_OFF) {
        (void)netif_dhcp_off(netif);
    }
#endif
    if (netif_ip4_netmask(netif)->addr != ip_2_ip4(&netmask)->addr) {
        /* lwip disallow two netif sit in same net at the same time */
        g_locNetif = netif_list;
        while (g_locNetif != NULL) {
            if (g_locNetif == netif) {
                g_locNetif = g_locNetif->next;
                continue;
            }
            if (ip_addr_cmp_zoneless(&g_locNetif->netmask, &netmask) &&
                ip_addr_netcmp(&g_locNetif->ip_addr, &netif->ip_addr, ip_2_ip4(&netmask))) {
                ERR_IFCONFIG_STRING_PUT(ret, "Duplicate network.\n");
                sys_sem_signal(&ifconfigCmd->cb_completed);
            }
            g_locNetif = g_locNetif->next;
        }
        netif_set_netmask(netif, ip_2_ip4(&netmask));
        /* check if gateway still reachable */
        if (!ip_addr_netcmp(&netif->gw, &netif->ip_addr, ip_2_ip4(&netmask))) {
            ip_addr_set_zero(&(netif->gw));
            if (netif == netif_default) {
                (void)netif_set_default(NULL);
            }
        }
    }
}

static void LwipIfconfigSetGw(struct netif *netif, struct IfconfigOption *ifconfigCmd)
{
    int ret;
    ip_addr_t gw;

    if ((netif == NULL) || (ifconfigCmd == NULL)) {
        return;
    }

    ip_addr_set_val(gw, ifconfigCmd->gw);

    /* check the address multicast/0/loopback */
    if (ip_addr_ismulticast_val(gw) || ip_addr_isbroadcast_val(gw, netif) || ip_addr_isany(&gw) ||
        ip_addr_isloopback(&gw)) {
        ERR_IFCONFIG_STRING_PUT(ret, "Don't set gateway as a multicast/broadcast/0/loopback address.\n");
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }

    /* check if reachable */
    if (!ip_addr_netcmp_val(gw, netif->ip_addr, ip_2_ip4(&netif->netmask))) {
        ERR_IFCONFIG_STRING_PUT(ret, "The address is unreachable.\n");
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }

    if (netif_default != netif) {
        ip_addr_set_zero(&netif->gw);
        (void)netif_set_default(netif);
    }

#if LWIP_DHCP
    if (netif_dhcp_data(netif) && netif_dhcp_data(netif)->state != DHCP_STATE_OFF) {
        (void)netif_dhcp_off(netif);
    }
#endif
    netif_set_gw(netif, ip_2_ip4(&gw));
}

static void LwipIfconfigInternalNet(struct netif *netif, struct IfconfigOption *ifconfigCmd)
{
    if ((netif == NULL) || (ifconfigCmd == NULL)) {
        return;
    }

    netif_set_down(netif);
    if (ifconfigCmd->option & IFCONFIG_OPTION_SET_IP) {
        LwipIfconfigSetIp(netif, ifconfigCmd);
    }

    if (ifconfigCmd->option & IFCONFIG_OPTION_SET_NETMASK) {
        LwipIfconfigSetMask(netif, ifconfigCmd);
    }
}


void LwipIfconfigInternal(void *arg)
{
    struct IfconfigOption *ifconfigCmd = NULL;
    struct netif *netif = NULL;

    int ret;
    unsigned short mtu;

    ifconfigCmd = (struct IfconfigOption *)arg;
    if (ifconfigCmd == NULL) {
        printf("Ifconfig option is null.\n");
        return;
    }
    netif = netif_find(ifconfigCmd->iface);
    if (netif == NULL) {
        ERR_IFCONFIG_STRING_PUT(ret, "Device not found.\n");
        sys_sem_signal(&ifconfigCmd->cb_completed);
    }

    if ((ifconfigCmd->option & IFCONFIG_OPTION_SET_UP) || (ifconfigCmd->option & IFCONFIG_OPTION_SET_DOWN)) {
        LwipIfconfigInternalLink(netif, ifconfigCmd);
    }

    if ((ifconfigCmd->option & IFCONFIG_OPTION_SET_IP) || (ifconfigCmd->option & IFCONFIG_OPTION_SET_NETMASK)) {
        netif_set_down(netif);
        LwipIfconfigInternalNet(netif, ifconfigCmd);
    }

    if ((ifconfigCmd->option & IFCONFIG_OPTION_SET_IP) || (ifconfigCmd->option & IFCONFIG_OPTION_SET_NETMASK)) {
        netif_set_up(netif);
    }

    if (ifconfigCmd->option & IFCONFIG_OPTION_SET_GW) {
        LwipIfconfigSetGw(netif, ifconfigCmd);
    }

    if (ifconfigCmd->option & IFCONFIG_OPTION_SET_MTU) {
        mtu = ifconfigCmd->mtu;
        if (netif_set_mtu_api(netif, mtu) != ERR_OK) {
            ERR_IFCONFIG_STRING_PUT(ret, "Invalid arguments:mtu should be in [68, 1500].\n");
        }
    }
    sys_sem_signal(&ifconfigCmd->cb_completed);
}

void LwipPrintsize(size_t size)
{
    static const char *sizes[] = {"B", "KB", "MB", "GB"};
    size_t divis = 0;
    size_t rem = 0;

    while ((size >= CODE_SIZE_CONVERSION) && (divis < ((sizeof(sizes) / sizeof(char *)) - 1))) {
        rem = (size % CODE_SIZE_CONVERSION);
        divis++;
        size /= CODE_SIZE_CONVERSION;
    }

    printf("(%.1f %s).\r\n", (float)size + (float)rem / CODE_SIZE_CONVERSION_F, sizes[divis]); /*  */
}

static void LwipIfconfigUsage(const char *cmd)
{
    printf("Usage:"
           "\n%s [-h] "
           "\nifconfig <interface>"
           "\n[<ipaddr>] [netmask <mask>] [gateway <gw>]"
           "\n[mtu <size>]"
           "\n[up|down]...\n",
        cmd);
}

u32_t LwipIfconfig(int argc, const char **argv)
{
    int i;
    static struct IfconfigOption ifconfigCmd;
    err_t ret;

#if LWIP_ARP

    struct netif *netiftmp = NULL;
#endif /* LWIP_ARP */
    if (argv == NULL) {
        return 1;
    }

    if (argc) {
        if (strcmp("-h", argv[0]) == 0) {
            LwipIfconfigUsage("ifconfig");
            return 0;
        }
    }

    (void)memset_s(&ifconfigCmd, sizeof(ifconfigCmd), 0, sizeof(ifconfigCmd));
    if (sys_sem_new(&ifconfigCmd.cb_completed, 0) != ERR_OK) {
        printf("%s: sys_sem_new fail.\n", __FUNCTION__);
        return 1;
    }

    i = 0;
    /* Get the interface */
    if (argc > 0) {
        if (strlen(argv[i]) < NETIF_NAMESIZE) {
            if (strncpy_s(ifconfigCmd.iface, NETIF_NAMESIZE, argv[i], (strlen(argv[i]))) != EOK) {
                sys_sem_free(&ifconfigCmd.cb_completed);
                printf("Ifconfig : strncpy_s error.\n");
                return 1;
            }
            ifconfigCmd.iface[NETIF_NAMESIZE - 1] = '\0';
        } else {
            sys_sem_free(&ifconfigCmd.cb_completed);
            printf("Ifconfig : interface name is too big.\n");
            return 1;
        }
        i++;
        argc--;
        if (argc == 0) {
            /* no more arguments, show the interface state. */
            ret = tcpip_callback(LwipIfconfigShowInternal, &ifconfigCmd);
            if (ret != ERR_OK) {
                sys_sem_free(&ifconfigCmd.cb_completed);
                printf("Ifconfig : internal error, l:%d err:%d.\n", __LINE__, ret);
                return 1;
            }
            (void)sys_arch_sem_wait(&ifconfigCmd.cb_completed, 0);
            sys_sem_free(&ifconfigCmd.cb_completed);
            ifconfigCmd.cbPrintBuf[PRINT_BUF_LEN - 1] = '\0';
            printf("%s\n", ifconfigCmd.cbPrintBuf);
            return 0;
        }
    } else {
        /* no more arguments, show all the interface state. */
        ret = tcpip_callback(LwipIfconfigShowInternal, &ifconfigCmd);
        if (ret != ERR_OK) {
            sys_sem_free(&ifconfigCmd.cb_completed);
            printf("Ifconfig : internal error, l:%d err:%d.\n", __LINE__, ret);
            return 1;
        }
        (void)sys_arch_sem_wait(&ifconfigCmd.cb_completed, 0);
        sys_sem_free(&ifconfigCmd.cb_completed);
        ifconfigCmd.cbPrintBuf[PRINT_BUF_LEN - 1] = '\0';
        printf("%s\n", ifconfigCmd.cbPrintBuf);

        return 0;
    }

    /* ifup/ifdown */
    if (strcmp("up", argv[i]) == 0) {
        ifconfigCmd.option |= IFCONFIG_OPTION_SET_UP;
        /* setup the interface, other arguments is ignored. */
        ret = tcpip_callback(LwipIfconfigInternal, &ifconfigCmd);
        if (ret != ERR_OK) {
            sys_sem_free(&ifconfigCmd.cb_completed);
            printf("Ifconfig : internal error, l:%d err:%d.\n", __LINE__, ret);
            return 1;
        }
        (void)sys_arch_sem_wait(&ifconfigCmd.cb_completed, 0);
        sys_sem_free(&ifconfigCmd.cb_completed);
        ifconfigCmd.cbPrintBuf[PRINT_BUF_LEN - 1] = '\0';
        printf("%s\n", ifconfigCmd.cbPrintBuf);
        return 0;
    } else if (strcmp("down", argv[i]) == 0) {
        ifconfigCmd.option |= IFCONFIG_OPTION_SET_DOWN;
        /* setdown the interface, other arguments is ignored. */
        ret = tcpip_callback(LwipIfconfigInternal, &ifconfigCmd);
        if (ret != ERR_OK) {
            sys_sem_free(&ifconfigCmd.cb_completed);
            printf("Ifconfig : internal error, l:%d err:%d.\n", __LINE__, ret);
            return 1;
        }
        (void)sys_arch_sem_wait(&ifconfigCmd.cb_completed, 0);
        sys_sem_free(&ifconfigCmd.cb_completed);
        ifconfigCmd.cbPrintBuf[PRINT_BUF_LEN - 1] = '\0';
        printf("%s\n", ifconfigCmd.cbPrintBuf);
        return 0;
    }
    /* check if set the ip address. */
#if LWIP_ARP
    netiftmp = netifapi_netif_find_by_name(ifconfigCmd.iface);
    if (netiftmp == NULL) {
        sys_sem_free(&ifconfigCmd.cb_completed);
        printf("Ifconfig : Interface %s not found.\n", ifconfigCmd.iface);
        return 1;
    }
#endif /* LWIP_ARP */
    if (!strcmp(argv[i], "inet") || ip4addr_aton(argv[i], ip_2_ip4(&ifconfigCmd.ipAddr))) {
        if (!strcmp(argv[i], "inet")) {
            if (argc <= 1) {
                sys_sem_free(&ifconfigCmd.cb_completed);
                LwipIfconfigUsage("ifconfig");
                return 1;
            }

            if (!ip4addr_aton(argv[i + 1], ip_2_ip4(&ifconfigCmd.ipAddr))) {
                sys_sem_free(&ifconfigCmd.cb_completed);
                printf("Ifconfig : Invalid IPv4 Address.\n");
                return 1;
            }
            argc--;
            i++;
        }
        IP_SET_TYPE_VAL((ifconfigCmd.ipAddr), IPADDR_TYPE_V4);
#if LWIP_ARP
        if (!ip_addr_cmp_zoneless(&ifconfigCmd.ipAddr, &netiftmp->ip_addr)) {
            ifconfigCmd.option |= IFCONFIG_OPTION_SET_IP;
        }
#else
        ifconfigCmd.option |= IFCONFIG_OPTION_SET_IP;
#endif /* LWIP_ARP */
        argc--;
        i++;
    }

    if (ifconfigCmd.option & IFCONFIG_OPTION_DEL_IP) {
        if (argc != 0) {
            sys_sem_free(&ifconfigCmd.cb_completed);
            LwipIfconfigUsage("ifconfig");
            return 1;
        }
    }

    while (argc > 0) {
        if (strcmp("netmask", argv[i]) == 0 && (argc > 1) && (ipaddr_addr(argv[i + 1]) != IPADDR_NONE)) {
            /* if set netmask */
            ip_addr_set_ip4_u32_val((ifconfigCmd.netmask), ipaddr_addr(argv[i + 1]));
            ifconfigCmd.option |= IFCONFIG_OPTION_SET_NETMASK;
            i += TYPE_VAL_ARGS;
            argc -= TYPE_VAL_ARGS;
        } else if (strcmp("gateway", argv[i]) == 0 && (argc > 1) && (ipaddr_addr(argv[i + 1]) != IPADDR_NONE)) {
            /* if set gateway */
            ip_addr_set_ip4_u32_val((ifconfigCmd.gw), ipaddr_addr(argv[i + 1]));
            ifconfigCmd.option |= IFCONFIG_OPTION_SET_GW;
            i += TYPE_VAL_ARGS;
            argc -= TYPE_VAL_ARGS;
        } else if (!strcmp("mtu", argv[i]) && (argc > 1)) {
            /* if set mtu */
            if ((atoi(argv[i + 1]) < 0) || (atoi(argv[i + 1]) > 0xFFFF)) {
                sys_sem_free(&ifconfigCmd.cb_completed);
                printf("\nifconfig: Invalid argument for mtu.\n");
                LwipIfconfigUsage("ifconfig");
                return 1;
            }

            ifconfigCmd.mtu = (u16_t)(atoi(argv[i + 1]));
            ifconfigCmd.option |= IFCONFIG_OPTION_SET_MTU;
            i += TYPE_VAL_ARGS;
            argc -= TYPE_VAL_ARGS;
        } else {
            sys_sem_free(&ifconfigCmd.cb_completed);
            LwipIfconfigUsage("ifconfig");
            return 1;
        }
    }

    ret = tcpip_callback(LwipIfconfigInternal, &ifconfigCmd);
    if (ret != ERR_OK) {
        sys_sem_free(&ifconfigCmd.cb_completed);
        printf("%s : tcpip_callback failed in line %d : errnu %d.\n", __FUNCTION__, __LINE__, ret);
        return 1;
    }
    (void)sys_arch_sem_wait(&ifconfigCmd.cb_completed, 0);
    ifconfigCmd.cbPrintBuf[PRINT_BUF_LEN - 1] = '\0';
    printf("%s\n", ifconfigCmd.cbPrintBuf);

    sys_sem_free(&ifconfigCmd.cb_completed);
    return 0;
}

#ifdef LOSCFG_SHELL
SHELLCMD_ENTRY(ifconfig_shellcmd, CMD_TYPE_EX, "ifconfig", XARGS, (CmdCallBackFunc)LwipIfconfig);
#endif /* LOSCFG_SHELL */
