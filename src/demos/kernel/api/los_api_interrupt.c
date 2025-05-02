/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Interrupt Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_api_interrupt.h"
#include "los_hwi.h"
#ifdef LOSCFG_PLATFORM_STM32F429IGTX
#include "stm32f4xx_hal.h"
#endif
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_ARCH_CSKY_V2
#define SOFT_IRQ_NUM    31
#elif defined  LOSCFG_ARCH_RISCV_RV32IMC
#define SOFT_IRQ_NUM    3
#elif defined (LOSCFG_FAMILY_RASPBERRY)
#define SOFT_IRQ_NUM    RASPBERRY_IRQ
#elif defined (LOSCFG_PLATFORM_ESP32) || (LOSCFG_PLATFORM_ESP8266) || (LOSCFG_PLATFORM_ESP32_QEMU)
#define SOFT_IRQ_NUM    7
#else
#define SOFT_IRQ_NUM    39
#endif

#define BUTTON_IRQ_NUM  (EXTI0_IRQn + 16)

#ifdef LOSCFG_PLATFORM_STM32F429IGTX
STATIC VOID Exti0Init(VOID)
{
    /* add your IRQ init code here */
    GPIO_InitTypeDef GPIO_InitStructure;

    __GPIOA_CLK_ENABLE();

    GPIO_InitStructure.Pin  = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    return;
}

STATIC VOID ButtonIrqHandler(VOID)
{
    printf("Button IRQ test successfully.\n");
    LOS_HwiClear(BUTTON_IRQ_NUM);
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    InspectStatusSetById(LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_SUCCESS);
    return;
}
#endif

STATIC VOID UserIrqHandler(VOID)
{
    printf("User IRQ test successfully.\n");
    LOS_HwiClear(SOFT_IRQ_NUM);
    InspectStatusSetById(LOS_INSPECT_INTERRUPT, LOS_INSPECT_STU_SUCCESS);
    return;
}

UINT32 InterruptDemo(VOID)
{
    UINTPTR intSave = LOS_IntLock();

    printf("Kernel interrupt demo start to run.\n");
    /* software interrupt test */
    LOS_HwiCreate(SOFT_IRQ_NUM, 0, 0, UserIrqHandler, 0);
    LOS_HwiEnable(SOFT_IRQ_NUM);
    LOS_IntRestore(intSave);
    LOS_HwiTrigger(SOFT_IRQ_NUM);

    /* hardware interrupt test */
#ifdef LOSCFG_PLATFORM_STM32F429IGTX
    Exti0Init();
    LOS_HwiCreate(BUTTON_IRQ_NUM, 0, 0, ButtonIrqHandler, 0); // 16: cortex-m irq shift
    LOS_HwiEnable(BUTTON_IRQ_NUM);
    LOS_IntRestore(intSave);
#endif

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
