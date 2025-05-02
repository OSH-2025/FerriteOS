/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interrupt Config HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-08-04
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

#ifndef _ASM_INTERRUPT_CONFIG_H
#define _ASM_INTERRUPT_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ARM_TIMER_INI               0
#define AUX_INT                     29
#define I2C_SPI_SLV_INT             43
#define PWA0_INT                    45
#define PWA1_INT                    46
#define CNTPS_IRQ                   65
#define CNTPNS_IRQ                  66
#define CNTHP_IRQ                   67
#define CNTV_IRQ                    68
#define MAILBOX0_IRQ                69
#define MAILBOX1_IRQ                70
#define MAILBOX2_IRQ                71
#define MAILBOX3_IRQ                72
#ifdef LOSCFG_KERNEL_SMP
#define OS_TICK_INT_NUM             CNTPNS_IRQ
#else
#define OS_TICK_INT_NUM             ARM_TIMER_INI
#endif

#define RASPBERRY_IRQ               MAILBOX3_IRQ
#define NUM_HAL_INTERRUPT_UART      AUX_INT

#define IRQ_REG_BASE                ((INTERRUPTS_INFO *)(0x3F000000 + 0xB200))
#define SYSTEMTIMER_REG_BASE        ((SYSTEMTIMER_INFO *)(0x3F000000 + 0x3000))
#define ARMTIMER_REG_BASE           ((ARMTIMER_INFO *)(0x3F000000 + 0xB000 + 0x400))
#define CORE_MAILBOX_REG_BASE       ((MAILBOXES_INFO *)(0x40000000 + 0x40))

typedef struct TagInterruptsInfo {
    volatile UINT32 basicPending;       /* IRQ basic pending */
    volatile UINT32 pending[2];         /* IRQ pending x */
    volatile UINT32 FIQControl;         /* FIQ control */
    volatile UINT32 enableIRQs[2];      /* Enable IRQs */
    volatile UINT32 enableBasicIRQs;    /* Enable Basic IRQs */
    volatile UINT32 disableIRQs[2];     /* Disable IRQs x */
    volatile UINT32 disableBasicIRQs;   /* Disable Basic IRQs */
} INTERRUPTS_INFO;

typedef struct TagArmTimerInfo {
    volatile UINT32 load;
    volatile UINT32 value;
    volatile UINT32 control;
    volatile UINT32 IRQClear;
    volatile UINT32 rawIRQ;
    volatile UINT32 maskedIRQ;
    volatile UINT32 reload;
    volatile UINT32 preDivider;
    volatile UINT32 freeRunningCounter;
} ARMTIMER_INFO;

typedef struct TagSystemTimerInfo {
    volatile UINT32 CS;         /* System Timer Control/Status  */
    volatile UINT32 CLO;        /* System Timer Counter Lower 32 bits */
    volatile UINT32 CHI;        /* System Timer Counter Higher 32 bits */
    volatile UINT32 C[4];       /* System Timer Compare x */
} SYSTEMTIMER_INFO;

typedef struct TagMailboxesInfo {
    volatile UINT32 CoreTimeIRQ[4]; /* Core timers interrupts */
    volatile UINT32 IRQControl[4];  /* Core x timers Interrupt control */
    volatile UINT32 IRQSource[4];   /* Core x IRQ Source */
    volatile UINT32 FIQSource[4];   /* Core x FIQ Source */
    volatile UINT32 writeSet[16];   /* Core x Mailbox x write-set */
    volatile UINT32 readClear[16];  /* Core x Mailbox x read & write-high-to-clear */
} MAILBOXES_INFO;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ASM_INTERRUPT_CONFIG_H */

