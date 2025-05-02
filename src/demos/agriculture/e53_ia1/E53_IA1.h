/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: E53IA1 HeadFile
 * Author: Huawei LiteOS Team
 * Create: 2021-07-20
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

#ifndef _E53_IA1_H
#define _E53_IA1_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef enum {
    OFF = 0,
    ON
} E53IA1Status;

typedef struct {
    float    lux;
    float    humidity;
    float    temperature;
} E53IA1DataType;


#define BH1750_ADDR_WRITE     0x23 << 1
#define BH1750_ADDR_READ      (0x23 << 1) + 1
#define BH1750_INIT           0x01
#define BH1750_START          0x10
#define BH1750_ONE            0x20
#define BH1750_RSET           0x07
#define BH1750_CMDLEN         0x01
#define BH1750_RECVLEN        0x02

#define SHT30_ADDR_WRITE      0x44 << 1
#define SHT30_ADDR_READ       (0x44 << 1) + 1
#define SHT30_INIT            0x22
#define SHT30_TIME            0x20

#define SHT30_CMDLEN          0x02
#define SHT30_RECVLEN         0x06
#define SHT30_CHECKLEN        0x03

#define SHT30_POLYNOMIAL      0x31

#define LIGHT_PIN             GPIO_PIN_0
#define LIGHT_GPIO_PORT       GPIOA

#define MOTER_PIN             GPIO_PIN_8
#define MOTER_GPIO_PORT       GPIOB

#define LIGHT_KEY_IRQ_NUM     EXTI2_IRQn + 16
#define MOTER_KEY_IRQ_NUM     EXTI3_IRQn + 16

VOID E53IA1Init(VOID);
VOID E53IA1Read(E53IA1DataType *data);
VOID E53IA1GpioInit(VOID);
VOID KeyInit(VOID);

VOID SetLightStatus(E53IA1Status status);
VOID SetMoterStatus(E53IA1Status status);

E53IA1Status GetLightStatus(VOID);
E53IA1Status GetMoterStatus(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _E53_IA1_H */

