/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: E53IA1 Implementation
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

#include "i2c.h"
#include "gpio.h"
#include "E53_IA1.h"
#include "los_hwi.h"
#include "los_mux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

STATIC E53IA1Status g_lightStatus = OFF;
STATIC E53IA1Status g_moterStatus = OFF;
STATIC UINT32 g_muxLock;

STATIC VOID BH1750WriteCmd(UINT8 data)
{
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    (VOID)I2C1_WriteBuffer(BH1750_ADDR_WRITE, 0, &data, BH1750_CMDLEN);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC VOID SHT30WriteCmd(UINT8 *data)
{
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    (VOID)I2C1_WriteBuffer(SHT30_ADDR_WRITE, 0, data, SHT30_CMDLEN);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC VOID BH1750Init(VOID)
{
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    BH1750WriteCmd(BH1750_INIT);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC VOID BH1750Start(VOID)
{
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    BH1750WriteCmd(BH1750_START);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC VOID BH1750Recv(INT16 *data)
{
    UINT8 rData[BH1750_RECVLEN];
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    I2C1_ReadBuffer(BH1750_ADDR_READ, 0, rData, BH1750_RECVLEN);
    (VOID)LOS_MuxPost(g_muxLock);
    *data = (UINT16)rData[0] << 8 | rData[1];
}

STATIC VOID SHT30Init(VOID)
{
    UINT8 data[SHT30_CMDLEN] = {SHT30_INIT, SHT30_TIME};
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    SHT30WriteCmd(data);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC VOID SHT30Recv(UINT8* data)
{
    // 0xE0, 0x00 : read data cmd
    UINT8 sData[SHT30_CMDLEN] = {0xE0, 0x00};
    (VOID)LOS_MuxPend(g_muxLock, LOS_WAIT_FOREVER);
    SHT30WriteCmd(sData);
    I2C1_ReadBuffer(SHT30_ADDR_READ, 0, data, SHT30_RECVLEN);
    (VOID)LOS_MuxPost(g_muxLock);
}

STATIC UINT8 SHT30CheckCrc(UINT8 *data, UINT8 len, UINT8 checksum)
{
    UINT8 crc = 0xFF;
    UINT8 bit = 0;
    UINT8 i;
    // calculates 8-Bit checksum with given polynomial
    for (i = 0; i < len; ++i) {
        crc ^= (data[i]);
        // 8 : the crc bits
        for (bit = 8; bit > 0; --bit) {
            // 0x80 : The high bit is 1, and XOR is required; otherwise, it is not required
            crc = crc & 0x80 ? (crc << 1) ^ SHT30_POLYNOMIAL : crc << 1;
        }
    }
    if (crc != checksum) {
        return LOS_NOK;
    }
    return LOS_OK;
}

STATIC FLOAT SHT30CalTemperature(UINT16 data)
{

    FLOAT temperature;
    // Calculation formula
    data &= ~0x0003;
    temperature = (175 * (FLOAT)data / 65535 - 45);
    return temperature;
}


STATIC FLOAT SHT30CalHumidity(UINT16 data)
{

    FLOAT humidity;
    // Calculation formula
    data &= ~0x0003;
    humidity = (100 * (FLOAT)data / 65535);
    return humidity;
}

VOID E53IA1Init(VOID)
{
    E53IA1GpioInit();
    BH1750Init();
    SHT30Init();
    BH1750Start();
    (VOID)LOS_MuxCreate(&g_muxLock);
}

VOID E53IA1Read(E53IA1DataType *data)
{
    UINT8 sht30Data[SHT30_RECVLEN] = {0};
    INT16 bh1750Data = 0;
    UINT8 checkData[SHT30_CHECKLEN] = {0};
    UINT16 temperatureData;
    UINT16 humidityData;
    UINT8 ret;

    BH1750Recv(&bh1750Data);
    // Calculation formula
    data->lux = (FLOAT)bh1750Data / 1.2;

    SHT30Recv(sht30Data);
    for (INT32 i = 0; i < SHT30_CHECKLEN; ++i) {
        checkData[i] = sht30Data[i];
    }
    ret = SHT30CheckCrc(checkData, SHT30_CHECKLEN - 1, checkData[SHT30_CHECKLEN - 1]);
    if (ret == LOS_OK) {
        temperatureData = ((UINT16)checkData[0] << 8) | checkData[1];
        data->temperature = SHT30CalTemperature(temperatureData);
    }

    for (INT32 i = 0; i < SHT30_CHECKLEN; ++i) {
        checkData[i] = sht30Data[SHT30_CHECKLEN + i];
    }
    ret = SHT30CheckCrc(checkData, SHT30_CHECKLEN - 1, checkData[SHT30_CHECKLEN -1]);
    if (ret == LOS_OK) {
        humidityData = ((UINT16)checkData[0] << 8) | checkData[1];
        data->humidity = SHT30CalHumidity(humidityData);
    }
}

VOID E53IA1GpioInit(VOID)
{
    GPIO_InitTypeDef GPIOInitStruct;
    GPIOInitStruct.Pin = LIGHT_PIN;
    GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Pull = GPIO_PULLUP;
    GPIOInitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(LIGHT_GPIO_PORT, &GPIOInitStruct);

    GPIOInitStruct.Pin = MOTER_PIN;
    GPIOInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Pull = GPIO_PULLUP;
    GPIOInitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(MOTER_GPIO_PORT, &GPIOInitStruct);
}

VOID SetLightStatus(E53IA1Status status)
{
    if (status == g_lightStatus) {
        return;
    }
    HAL_GPIO_WritePin(LIGHT_GPIO_PORT, LIGHT_PIN, status);
    g_lightStatus = status;
}

VOID SetMoterStatus(E53IA1Status status)
{
    if (status == g_moterStatus) {
        return;
    }
    HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER_PIN, status);
    g_moterStatus = status;
}

E53IA1Status GetLightStatus(VOID)
{
    return g_lightStatus;
}

E53IA1Status GetMoterStatus(VOID)
{
    return g_moterStatus;
}

STATIC VOID LightKeyIrqHandler(VOID)
{
    SetLightStatus((~g_lightStatus) & 0x1);
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_PIN);
}

STATIC VOID MoterKeyIrqHandler(VOID)
{
    SetMoterStatus((~g_moterStatus) & 0x1);
    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_PIN);
}

VOID KeyInit(VOID)
{
    LOS_HwiCreate(LIGHT_KEY_IRQ_NUM, 0, 0, LightKeyIrqHandler, 0);
    LOS_HwiEnable(LIGHT_KEY_IRQ_NUM);
    LOS_HwiCreate(MOTER_KEY_IRQ_NUM, 0, 0, MoterKeyIrqHandler, 0);
    LOS_HwiEnable(MOTER_KEY_IRQ_NUM);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */