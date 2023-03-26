/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-03 02:34:04
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-22 23:07:42
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sensor.h"

void Sensor::AddImu(Imu *imu)
{
    this->imu = imu;
}

void Sensor::AddBaro(Baro *baro)
{
    this->baro = baro;
}

void Sensor::Init()
{
    if (nullptr != imu)
    {
        imu->Init();
    }
    if (nullptr != baro)
    {
        baro->Init();
    }
}