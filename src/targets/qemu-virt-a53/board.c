/* ----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * Description: Board Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
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

#if LOSCFG_DRIVERS_UART
#include "uart.h"
#include "hisoc/uart.h"
#include "linux/device.h"
#include "linux/platform_device.h"
#include "linux/module.h"
#include "virtual_serial.h"
#include "console.h"
#endif
#include "mmu.h"
#include "los_memory.h"
#include "mmu_pri.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

VOID BoardConfig(VOID)
{
    g_sys_mem_addr_end = 0x52000000;
}

/* 3 : 2^3 = 8, 8 Bytes per item; 12 : 2^12 = 4K, second page granule size, address length of each item mapping */
VOID OsSecPageInit(VOID)
{
    OsSysSecPteInit(SYS_MEM_BASE + TEXT_OFFSET,
        ALIGN((UINTPTR)&__ram_data_start - SYS_MEM_BASE - TEXT_OFFSET, MMU_2M), MMU_PTE_CACHE_RW_FLAGS);

    OsAppSecPteInit(MMZ_MEM_BASE, MMZ_MEM_LEN, MMU_PTE_NONCACHE_RW_XN_FLAGS);
}

 /*
 *   device:
 *       access without cache;
 *       not support out-of-order execution;
 *       not support Instruction pre read;
 *       not support non-aligned access
 *   normal:
 *       access cacheability(cache or non-cache);
 *       support out-of-order execution;
 *       support Instruction pre read;
 *       support non-aligned access
 */
VOID MmuSectionMap(VOID)
{
    UINT64 flag;
     /* device */
    flag = MMU_INITIAL_MAP_DEVICE | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsInit(flag, 0, DDR_MEM_ADDR + DDR_MEM_SIZE);
     /* normal cache */
    flag = MMU_PTE_CACHE_RW_FLAGS | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsSet(flag, SYS_MEM_BASE, g_sys_mem_addr_end - 1);
     /* normal non-cache */
    flag = MMU_PTE_NONCACHE_RW_FLAGS | MMU_PTE_L012_DESCRIPTOR_BLOCK;
    OsBlockMapsSet(flag, DDR_MEM_ADDR, SYS_MEM_BASE);
    OsBlockMapsSet(flag, g_sys_mem_addr_end, (DDR_MEM_ADDR + DDR_MEM_SIZE) - 1);
     /*
     * set mmu descriptor of (0~2M) invalid, check the illegal access to NULL pointer in code.
     * Access to NULL pointer and mem (0 ~ 2M) will trigger excption immediately
     */
    flag = MMU_PTE_CACHE_RW_FLAGS | MMU_PTE_DESCRIPTOR_INVALID;
    OsBlockMapsSet(flag, 0, (MMU_2M - 1));
}

#if LOSCFG_DRIVERS_UART
static struct uart_driver_data uart0_pl011_driver = {
    .num             = 0,
    .baudrate        = 115200,
    .attr.fifo_rx_en = 1,
    .attr.fifo_tx_en = 1,
    .flags           = UART_FLG_RD_BLOCK,
};

static struct resource uart0_pl011_resources[] = {
    {
        .start  = UART_REG_BASE,
        .end    = UART_REG_BASE + 0x48,
        .flags  = IORESOURCE_MEM,
    },
    {
        .start  = NUM_HAL_INTERRUPT_UART,
        .end    = NUM_HAL_INTERRUPT_UART,
        .flags  = IORESOURCE_IRQ,
    },
};
struct platform_device uart0_pl011_device = {
    .name       = "uart-pl011",
    .id = -1,
    .dev = {
        .driver_data = &uart0_pl011_driver,
    },
    .resource   = uart0_pl011_resources,
    .num_resources  = ARRAY_SIZE(uart0_pl011_resources),
};

int machine_init(void)
{
    PRINTK("machine_init\n");
    (void)platform_device_register(&uart0_pl011_device);

    return 0;
}

#ifdef LOSCFG_DRIVERS_BASE
arch_initcall(machine_init);
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
