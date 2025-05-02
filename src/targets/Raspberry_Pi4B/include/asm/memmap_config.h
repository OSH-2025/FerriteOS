/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Memory Map Config HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-07-27
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

#define AUX_BASE                (PERIPHERAL_BASE_PHYS + 0x215000)
#define AUX_ENABLES             (AUX_BASE + 0x4)
#define AUX_MU_IO_REG           (AUX_BASE + 0x40)
#define AUX_MU_IER_REG          (AUX_BASE + 0x44)
#define AUX_MU_IIR_REG          (AUX_BASE + 0x48)
#define AUX_MU_LCR_REG          (AUX_BASE + 0x4c)
#define AUX_MU_MCR_REG          (AUX_BASE + 0x50)
#define AUX_MU_LSR_REG          (AUX_BASE + 0x54)
#define AUX_MU_MSR_REG          (AUX_BASE + 0x58)
#define AUX_MU_SCRATCH          (AUX_BASE + 0x5c)
#define AUX_MU_CNTL_REG         (AUX_BASE + 0x60)
#define AUX_MU_BAUD_REG         (AUX_BASE + 0x68)

#define GPFSEL0                 GPIO_REG_BASE
#define GPSET0                  (GPIO_REG_BASE + 0x1C)
#define GPCLR0                  (GPIO_REG_BASE + 0x28)
#define GPPUPPDN0               (GPIO_REG_BASE + 0xE4)

#define TIMER_LOAD              0x0
#define TIMER_VALUE             0x4
#define TIMER_CONTROL           0x8
#define TIMER_INT_CLR           0xc
#define TIMER_RIS               0x10
#define TIMER_MIS               0x14
#define TIMER_BGLOAD            0x18

#define PULL_NONE               0b00
#define PULL_UP                 0b01
#define PULL_DOWN               0b10

/* vs_server.bin should be loaded at SYS_MEM_BASE */
#define DDR_MEM_ADDR            (0x0)
#define DDR_MEM_SIZE            (0x100000000UL)
#define SYS_MEM_BASE            (0x80000)
#define TEXT_OFFSET             (0x80000)
#define MMZ_MEM_BASE            (0x60000000)
#define MMZ_MEM_LEN             (0x20000000)
#define SHIFT_PGD               (30)
#define SHIFT_PMD               (21)
#define SHIFT_PTE               (12)

/* map all of 0-1GB into kernel space in one shot */
#define PERIPHERAL_BASE_PHYS    (0xFE000000)
#define PERIPHERAL_BASE_SIZE    (0x40000000UL)
#define ARCH_ARM64              (0)

/* not use mmu, use physical addr */
#define PERIPHERAL_BASE_VIRT    (PERIPHERAL_BASE_PHYS)

/* individual peripherals in this mapping */
#define CPUPRIV_BASE_VIRT        (PERIPHERAL_BASE_VIRT + 0x08000000)
#define CPUPRIV_BASE_PHYS        (PERIPHERAL_BASE_PHYS + 0x08000000)
#define CPUPRIV_SIZE             (0x00020000)
#define UART_REG_BASE            (PERIPHERAL_BASE_VIRT +  0x201000)
#define UART_SIZE                (0x00001000)
#define RTC_BASE                 (PERIPHERAL_BASE_VIRT + 0x09010000)
#define RTC_SIZE                 (0x00001000)
#define FW_CFG_BASE              (PERIPHERAL_BASE_VIRT + 0x09020000)
#define FW_CFG_SIZE              (0x00001000)
#define NUM_VIRTIO_TRANSPORTS    (32)
#define VIRTIO_BASE              (PERIPHERAL_BASE_VIRT + 0xa000000)
#define VIRTIO_SIZE              (NUM_VIRTIO_TRANSPORTS * 0x200)
#define GPIO_REG_BASE            (PERIPHERAL_BASE_VIRT + 0x200000)
/* Low Peripheral mode is enabled */
#define GIC_BASE_ADDR            (0xff840000)

#define GICD_OFFSET              (0x1000)  /* interrupt distributor offset */
#define GICC_OFFSET              (0x2000)  /* CPU interface register offset */

#define TIMER_TICK_REG_BASE      (0xff800040)

#define FIRST_SECTION_TABLE_LEN  (0x1000 * 9)
#define SECOND_PAGE_TABLE_OS_LEN (0x1000 * 8)
#define SECOND_PAGE_TABLE_APP_LEN (0x100000 * 8)

#define OS_SYS_MEM_ADDR        ((void *)(&__heap_start))
#define OS_SYS_MEM_SIZE        (UINT32)(((UINTPTR)&__LOS_HEAP_ADDR_END__ \
                                        - (UINTPTR)OS_SYS_MEM_ADDR + (64 - 1)) & ~(64 - 1))

extern UINT32 __LOS_HEAP_ADDR_START__;
extern UINT32 __LOS_HEAP_ADDR_END__;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _MEMMAP_CONFIG_H */
