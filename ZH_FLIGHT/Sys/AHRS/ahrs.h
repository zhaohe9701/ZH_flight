/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:37:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:23:43
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\ahrs.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AHRS_H__
#define __AHRS_H__

#include "aircraft_state.h"
#include "sensor.h"
#include "config.h"

class AttitudeSolver
{
public:
    virtual void update(ActualAttitudeState &actual_state, SensorData &sensor_data) = 0;
    virtual void transformBodyToEarth(float &x, float &y, float &z) = 0;
    virtual void transformEarthToBody(float &x, float &y, float &z) = 0;
    virtual ~AttitudeSolver(){};
};

#endif
