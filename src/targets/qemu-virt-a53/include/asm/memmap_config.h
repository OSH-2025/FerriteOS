/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Memory Map Config HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-05-21
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

#ifndef _MEMMAP_CONFIG_H
#define _MEMMAP_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* vs_server.bin should be loaded at SYS_MEM_BASE */
#define DDR_MEM_ADDR            0x40000000
#define DDR_MEM_SIZE            0x100000000ull
#define SYS_MEM_BASE            0x42000000
#define TEXT_OFFSET             0x00000000
#define MMZ_MEM_BASE            0x60000000
#define MMZ_MEM_LEN             0x20000000

/* map all of 0-1GB into kernel space in one shot */
#define PERIPHERAL_BASE_PHYS (0)
#define PERIPHERAL_BASE_SIZE (0x40000000UL)
#define ARCH_ARM64           (0)

/* not use mmu, use physical addr */
#define PERIPHERAL_BASE_VIRT (0x00000000UL)

/* individual peripherals in this mapping */
#define CPUPRIV_BASE_VIRT               (PERIPHERAL_BASE_VIRT + 0x08000000)
#define CPUPRIV_BASE_PHYS               (PERIPHERAL_BASE_PHYS + 0x08000000)
#define CPUPRIV_SIZE                    (0x00020000)
#define UART_REG_BASE                   (PERIPHERAL_BASE_VIRT + 0x09000000)
#define UART_SIZE                       (0x00001000)
#define RTC_BASE                        (PERIPHERAL_BASE_VIRT + 0x09010000)
#define RTC_SIZE                        (0x00001000)
#define FW_CFG_BASE                     (PERIPHERAL_BASE_VIRT + 0x09020000)
#define FW_CFG_SIZE                     (0x00001000)
#define NUM_VIRTIO_TRANSPORTS           32
#define VIRTIO_BASE                     (PERIPHERAL_BASE_VIRT + 0x0a000000)
#define VIRTIO_SIZE                     (NUM_VIRTIO_TRANSPORTS * 0x200)

#define GIC_BASE_ADDR                   CPUPRIV_BASE_VIRT
#ifdef LOSCFG_ARM_GIC_V2
#define GICD_OFFSET                     0x00000  /* interrupt distributor offset */
#define GICC_OFFSET                     0x10000  /* CPU interface register offset */
#else /* GICv3 */
#define GICD_OFFSET                     0x00000  /* interrupt distributor offset */
#define GICR_OFFSET                     0xa0000  /* interrupt redistributor offset */
#define GICR_STRIDE                     0x20000  /* interrupt redistributor stride */
#endif

#define FIRST_SECTION_TABLE_LEN         (0x1000 * 9)
#define SECOND_PAGE_TABLE_OS_LEN        (0x1000 * 8)
#define SECOND_PAGE_TABLE_APP_LEN       (0x100000 * 8)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MEMMAP_CONFIG_H */
