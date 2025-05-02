/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Bsp Led HeadFile
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

#ifndef _BSP_LED_H
#define _BSP_LED_H

#if LOSCFG_LWIP_NIP

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

// 引脚定义
/*******************************************************/
// R 红色灯
#define LED1_PIN                  GPIO_PIN_10
#define LED1_GPIO_PORT            GPIOH

// G 绿色灯
#define LED2_PIN                  GPIO_PIN_11
#define LED2_GPIO_PORT            GPIOH

// B 蓝色灯
#define LED3_PIN                  GPIO_PIN_12
#define LED3_GPIO_PORT            GPIOH
/************************************************************/


/** 控制LED灯亮灭的宏，
 * LED低电平亮，设置ON=0，OFF=1
 * 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)    HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN, a)
#define LED2(a)    HAL_GPIO_WritePin(LED1_GPIO_PORT,LED2_PIN, a)
#define LED3(a)    HAL_GPIO_WritePin(LED1_GPIO_PORT,LED3_PIN, a)



/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)        HAL_GPIO_WritePin(p, i, GPIO_PIN_SET)   //设置为高电平
#define digitalLo(p,i)        HAL_GPIO_WritePin(p, i, GPIO_PIN_RESET) //输出低电平
#define digitalToggle(p,i)    HAL_GPIO_TogglePin(p, i)                //输出反转状态

/* 定义控制IO的宏 */
#define LED1_TOGGLE           digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF              digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON               digitalLo(LED1_GPIO_PORT,LED1_PIN)

#define LED2_TOGGLE           digitalToggle(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF              digitalHi(LED2_GPIO_PORT,LED2_PIN)
#define LED2_ON               digitalLo(LED2_GPIO_PORT,LED2_PIN)

#define LED3_TOGGLE           digitalToggle(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF              digitalHi(LED3_GPIO_PORT,LED3_PIN)
#define LED3_ON               digitalLo(LED3_GPIO_PORT,LED3_PIN)

/* 基本混色，后面高级用法使用PWM可混出全彩颜色,且效果更好 */

// 红
#define LED_RED    \
    LED1_ON;       \
    LED2_OFF;      \
    LED3_OFF

// 绿
#define LED_GREEN  \
    LED1_OFF;      \
    LED2_ON;       \
    LED3_OFF

// 蓝
#define LED_BLUE   \
    LED1_OFF;      \
    LED2_OFF;      \
    LED3_ON


// 黄(红+绿)
#define LED_YELLOW \
    LED1_ON;       \
    LED2_ON;       \
    LED3_OFF
// 紫(红+蓝)
#define LED_PURPLE \
    LED1_ON;       \
    LED2_OFF;      \
    LED3_ON

// 青(绿+蓝)
#define LED_CYAN   \
    LED1_OFF;      \
    LED2_ON;       \
    LED3_ON

// 白(红+绿+蓝)
#define LED_WHITE  \
    LED1_ON;       \
    LED2_ON;       \
    LED3_ON

// 黑(全部关闭)
#define LED_RGBOFF \
    LED1_OFF;      \
    LED2_OFF;      \
    LED3_OFF

void LED_GPIO_Config(void);

#endif /* LOSCFG_LWIP_NIP */
#endif /* _BSP_LED_H */
