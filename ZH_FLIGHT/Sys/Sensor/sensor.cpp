/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-03 02:34:04
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-23 00:06:26
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sensor.h"

void Sensor::AddImu(Imu *imu)
{
    _imu = imu;
}

void Sensor::AddBaro(Baro *baro)
{
    _baro = baro;
}

void Sensor::Init()
{
    _imu->Init();
    _baro->Init();
}