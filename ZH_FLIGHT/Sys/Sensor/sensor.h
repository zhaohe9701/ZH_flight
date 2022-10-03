/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 21:59:47
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-04 02:14:47
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "imu.h"
class SensorData
{
public:
    ImuData imu;
};

class Sensor
{
public:
    void Init();

    void AddImu(Imu *imu_init);

    Imu *imu = nullptr;
};

#endif
