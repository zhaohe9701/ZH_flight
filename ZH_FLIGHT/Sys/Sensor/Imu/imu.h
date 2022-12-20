/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:41:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-22 22:57:07
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#ifndef __IMU_H__
#define __IMU_H__

#include "sensor_interface.h"
#include "type.h"
class ImuData
{
public:
    Vec3 acc;
    Vec3 mag;
    Vec3 gyr;
};
class Imu
{
public:
    virtual void Init() = 0;
    virtual uint8_t GetId() = 0;
    virtual float GetTemperature() = 0;
    virtual void GetGyroData(ImuData &sensor_data) = 0;
    virtual void GetAccData(ImuData &sensor_data) = 0;
};

#endif