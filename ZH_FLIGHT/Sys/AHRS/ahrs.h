/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:37:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-09 23:00:34
 * @FilePath: \ZH_FLIGHT\Sys\AHRS\ahrs.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AHRS_H__
#define __AHRS_H__

#include "state.h"
#include "sensor.h"
#include "config.h"

class AttitudeAlgorithm
{
public:
    virtual void Update(AircraftState &actual_state, ImuData &imu_data) = 0;
    virtual void TransformBodyToEarth(float &x, float &y, float &z) = 0;
    virtual void TransformEarthToBody(float &x, float &y, float &z) = 0;
};

#endif
