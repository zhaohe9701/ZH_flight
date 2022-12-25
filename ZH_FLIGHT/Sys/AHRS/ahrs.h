/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:37:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:25:57
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
    virtual void Update(ActualState &actual_state, ImuData &imu_data) = 0;
    virtual void TransformBodyToEarth(float &x, float &y, float &z) = 0;
    virtual void TransformEarthToBody(float &x, float &y, float &z) = 0;
    virtual ~AttitudeSolver() = 0;
};

#endif
