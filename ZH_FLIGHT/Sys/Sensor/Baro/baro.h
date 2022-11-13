/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 16:36:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 23:11:16
 * @FilePath: \H7B0\Sys\Sensor\Baro\baro.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __BARO_H__
#define __BARO_H__

#include <stdint.h>

class BaroData
{
public:

};

class Baro
{
public:
    virtual void Init() = 0;
    virtual float GetTemperature() = 0;
    virtual float GetPressure() = 0;
    virtual float GetAltitude() = 0;
};

#endif