/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Gyro
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

#include "gyro.h"
#include "los_printf.h"
#include "los_swtmr.h"
#include "protocol.h"
#include "sensor_manager.h"
#include "mpu6050.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define SENSOR_DATA_LEN  3
#define DEFAULT_INTERVAL 5000
#define INVALID_TIMER_ID 0xFFFF

STATIC INT32 g_demoGyroSensorData[SENSOR_DATA_LEN];
STATIC INT16 g_demoAccel[SENSOR_DATA_LEN];
STATIC INT16 g_demoGyro[SENSOR_DATA_LEN];
STATIC INT16 g_demoTempDate = 0;

STATIC Mpu6050Priv g_gyroPriv = {
    .gyroTimerId = INVALID_TIMER_ID,
};
STATIC INT32 g_gyroPeriod;

STATIC VOID GypoTimerFunc(VOID const * arg)
{
    SensorSample((SensorType *)arg);
}

STATIC INT32 GyroInit(SensorType *sensor)
{
    (VOID)(sensor);
    printf("Gypo init.\n");
    MpuSensorInit();
    if (MpuSensorGetId() == LOS_OK) { // 1 read ok
        printf("Read gypo id successflly.\n");
    } else {
        printf("Read gypo id failed.\n");
        return LOS_NOK;
    }
    return LOS_OK;
}

STATIC INT32 GyroOpen(SensorType *sensor, OpenParam *para)
{
    UINT32 ret;
    (VOID)(para);

    Mpu6050Priv *gyro = (Mpu6050Priv *)sensor->priv;

    if ((sensor->sensorStat == SENSOR_STAT_OPEN) && (sensor->interval == g_gyroPeriod)) {
        return LOS_OK;
    }

    if (gyro->gyroTimerId != INVALID_TIMER_ID) {
        ret = LOS_SwtmrDelete(gyro->gyroTimerId);
        gyro->gyroTimerId = INVALID_TIMER_ID;
        if (ret != LOS_OK) {
            printf("Delete software timer failed.\n");
            return LOS_NOK;
        }
    }

    // creat a timer, first parameter is ticks.
    ret = LOS_SwtmrCreate(sensor->interval, LOS_SWTMR_MODE_PERIOD, (SWTMR_PROC_FUNC)GypoTimerFunc, &gyro->gyroTimerId,
        (UINT32)sensor);
    if (ret != LOS_OK) {
        printf("Create software timer failed.\n");
        return LOS_NOK;
    }

    ret = LOS_SwtmrStart(gyro->gyroTimerId);
    if (ret != LOS_OK) {
        printf("Start software timer failed.\n");
    }

    g_gyroPeriod = sensor->interval;

    printf("Gyro on.\n");
    return LOS_OK;
}

STATIC INT32 GyroClose(SensorType *sensor)
{
    UINT32 ret;

    if (sensor->sensorStat == SENSOR_STAT_CLOSE) {
        printf("Gyro has been closed.\n");
        return LOS_OK;
    }

    Mpu6050Priv *gyro = (Mpu6050Priv *)sensor->priv;
    g_gyroPeriod = 0;

    ret = LOS_SwtmrStop(gyro->gyroTimerId);
    if (ret != LOS_OK) {
        printf("Gyro stop software timer failed.\n");
        return LOS_NOK;
    }

    printf("Gyro off.\n");
    return LOS_OK;
}

STATIC INT32 GyroReadData(SensorType *sensor)
{
    printf("Read data\n");
    MpuSensorGetData(g_demoAccel, MPU6050_AACEL_DATA_TYPE);
    printf("Acc:  %8d%8d%8d\n", g_demoAccel[0], g_demoAccel[1], g_demoAccel[2]); // 0: x, 1: y, 2: z

    MpuSensorGetData(g_demoGyro, MPU6050_GYRO_DATA_TYPE);
    printf("Gyro: %8d%8d%8d\n", g_demoGyro[0], g_demoGyro[1], g_demoGyro[2]); // 0: x, 1: y, 2: z

    MpuSensorGetData(&g_demoTempDate, MPU6050_TEMP_DATA_TYPE);
    printf("Temp: %8d\n", g_demoTempDate); // temperature °C

    INT32 *data = (INT32 *)sensor->sensorData;
    data[0] = g_demoGyro[0]; // 0: x
    data[1] = g_demoGyro[1]; // 1: y
    data[2] = g_demoGyro[2]; // 2: z

    return LOS_OK;
}

STATIC struct SensorOperation g_gyroOps = {
    .Init = GyroInit,
    .Open = GyroOpen,
    .Close = GyroClose,
    .ReadData = GyroReadData,
};

STATIC SensorType g_sensorGyro = {
    .sensorOp = &g_gyroOps,
    .sensorData = &g_demoGyroSensorData,
    .sensorDataLen = sizeof(g_demoGyroSensorData),
    .priv = &g_gyroPriv,
    .tag = TAG_GYRO,
    .cmd = CMD_GYRO_DATA_REQ,
    .interval = DEFAULT_INTERVAL,
    .slaveAddr = MPU6050_ADDRESS,
};

VOID GyroSensorRegister(VOID)
{
    SensorRegister(&g_sensorGyro);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
