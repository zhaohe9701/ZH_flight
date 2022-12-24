/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 21:59:47
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-23 00:05:59
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "imu.h"
#include "baro.h"
class SensorData
{
public:
    ImuData imu;
};

class Sensor
{
public:
    void Init();
    void AddImu(Imu *imu);
    void AddBaro(Baro *baro);
private:
    Imu *_imu = nullptr;
    Baro *_baro = nullptr;
};

#endif
