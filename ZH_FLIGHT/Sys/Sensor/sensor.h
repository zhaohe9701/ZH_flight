/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 21:59:47
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-08-07 22:23:07
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "type.h"

class SensorData
{
public:
    Vec3<float> acc;
    Vec3<float> mag;
    Vec3<float> gyr;
};



#endif
