/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: System Init Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-29
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

#include "sys_init.h"
#include "los_sys.h"
#include "hal_rng.h"

#ifdef LOSCFG_COMPONENTS_NET_LWIP
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "netif/etharp.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"

#ifdef LOSCFG_COMPONENTS_SECURITY_MBEDTLS
#include "mbedtls/ssl.h"
#endif

#include "eth.h"
#endif

#ifdef LOSCFG_COMPONENTS_NET_LWIP
struct netif g_netif;
// Set the actual ip address, eg: 192.168.3.122
uint8_t ipAddr[4] = {192, 168, 3, 122};
// Set the actual netmask address, eg: 255.255.255.0
uint8_t netmaskAddr[4] = {255, 255, 255, 0};
// Set the actual gateway address, eg: 192.168.3.1
uint8_t gatewayAddr[4] = {192, 168, 3, 1};

#if LWIP_IPV4 && LWIP_IPV6
ip_addr_t ipaddr;
ip_addr_t netmask;
ip_addr_t gw;
#elif LWIP_IPV6
#else
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
#endif

void net_init(void)
{
    struct ethernet_api ethAapi;

    ethAapi = EthInterface();
#if defined(LWIP_DHCP) && (LWIP_DHCP == 1)
    err_t result;

    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;

    tcpip_init(NULL, NULL);
    (void)ethernetif_api_register(&ethAapi);

    printf("lwip_init OK!!\n");
    (void)netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);
    netif_set_default(&g_netif);
    if (netif_is_link_up(&g_netif)) {
        netif_set_up(&g_netif);
    } else {
        netif_set_down(&g_netif);
    }

    result = dhcp_start(&g_netif);
    if (result != ERR_OK) {
        printf("dhcp start error!...\n result = %d\n", result);
    }
    printf("dhcp start...\n");
#else /* LWIP_DHCP */
    /* IP addresses initialization */

#if LWIP_IPV4 && LWIP_IPV6
    IP_ADDR4(&ipaddr, ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
    IP_ADDR4(&netmask, netmaskAddr[0], netmaskAddr[1], netmaskAddr[2], netmaskAddr[3]);
    IP_ADDR4(&gw, gatewayAddr[0], gatewayAddr[1], gatewayAddr[2], gatewayAddr[3]);
#elif LWIP_IPV6
#else
    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
    IP4_ADDR(&netmask, netmaskAddr[0], netmaskAddr[1], netmaskAddr[2], netmaskAddr[3]);
    IP4_ADDR(&gw, gatewayAddr[0], gatewayAddr[1], gatewayAddr[2], gatewayAddr[3]);
#endif

    /* Initilialize the LwIP stack without RTOS */
    tcpip_init(NULL, NULL);
    printf("lwip test init ok.\n");

    (void)ethernetif_api_register(&ethAapi);
    /* Add the network interface (IPv4/IPv6) without RTOS */
#if LWIP_IPV4 && LWIP_IPV6
    (void)netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);
#elif LWIP_IPV6
    (void)netif_add(&g_netif, NULL, ethernetif_init, tcpip_input);
    netif_create_ip6_linklocal_address(&g_netif, 1);
    {
        ip6_addr_t ip6;
        err_t ret;
        s8_t idx;
        ip6_addr_t ipv6_gw;

        if (inet_pton(AF_INET6, "2000::2", &ip6) <= 0) {
            printf("set source ip6 failed \n");
            return;
        }
        ret = netif_add_ip6_address(&g_netif, &ip6, &idx);
        if (ret != 0) {
            printf("netif_add_ip6_address failed,ret %d\n", ret);
            return;
        }

        if (inet_pton(AF_INET6, "2000::1", &ipv6_gw) <= 0) {
            printf("inet_pton failed\n");
            return;
        }
        set_lwip_ipv6_default_gw(&g_netif, &ipv6_gw);
    }
#else
    (void)netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input);
#endif
    /* Registers the default network interface */
    netif_set_default(&g_netif);
    if (netif_is_link_up(&g_netif)) {
        /* When the netif is fully configured this function must be called */
        g_netif.flags |= NETIF_FLAG_LINK_UP;
        netif_set_up(&g_netif);
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&g_netif);
    }
#endif /* LWIP_DHCP */
}
#endif


#ifdef LOSCFG_COMPONENTS_NET_LWIP
void hieth_hw_init(void)
{
    (void)LOS_HwiCreate(ETH_IRQn + 16, 1, 0, ETH_IRQHandler, 0); // 16: cortex-m irq num shift
}
#endif

uint32_t HAL_GetTick(void) {
    return (UINT32)LOS_TickCountGet();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char const *file, int line) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    (void)file;
    (void)line;
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef rccOscInitStruct = {0};
    RCC_ClkInitTypeDef rccClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef periphClkInitStruct = {0};

    /** Configure LSE Drive Capability
    */
    HAL_PWR_EnableBkUpAccess();
    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
#ifdef CLK_SOURCE_HSE
    rccOscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rccOscInitStruct.HSEState = RCC_HSE_BYPASS;
    rccOscInitStruct.PLL.PLLState = RCC_PLL_ON;
    rccOscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rccOscInitStruct.PLL.PLLM = 4; // 4, Division factor for PLL VCO iput clock.
    rccOscInitStruct.PLL.PLLN = 216; // 216, Multiplication factor for PLL VCO output clock.
    rccOscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    rccOscInitStruct.PLL.PLLQ = 9; // 9, Division factor for
    rccOscInitStruct.PLL.PLLR = 2;
#else
    rccOscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    rccOscInitStruct.HSIState = RCC_HSI_ON;
    rccOscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    rccOscInitStruct.PLL.PLLState = RCC_PLL_ON;
    rccOscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    rccOscInitStruct.PLL.PLLM = 8;
    rccOscInitStruct.PLL.PLLN = 216;
    rccOscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    rccOscInitStruct.PLL.PLLQ = 9;
    rccOscInitStruct.PLL.PLLR = 2;
#endif

    if (HAL_RCC_OscConfig(&rccOscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Activate the Over-Drive mode
    */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    rccClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rccClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    rccClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    rccClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    rccClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&rccClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
        Error_Handler();
    }
    periphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_I2C1
                                               | RCC_PERIPHCLK_CLK48;
    periphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    periphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    periphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&periphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Enables the Clock Security System
    */
    HAL_RCC_EnableCSS();
}

void HAL_MspInit(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/*
 * atiny_adapter user interface
 */
void atiny_usleep(unsigned long usec)
{
    delayus((uint32_t)usec);
}

int atiny_random(void *output, size_t len)
{
    return HalRngGenerateBuffer(output, len);
}

void atiny_reboot(void)
{
    HAL_NVIC_SystemReset();
}
