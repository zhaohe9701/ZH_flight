/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:23:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-04 01:52:53
 * @FilePath: \ZH_FLIGHT\Sys\Flight\controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "state.h"
#include "type.h"
#include <stdint.h>

#define PID 101
#define PID_PARAM_NUM 12
class ControlOutput
{
public:
    Vec3<float> attitude_out;
    Vec4<float> motor_out;
    float throttle = 0.0f;
    void Allocate();
    void SetThrottle(float value);
private:
    float _Limit(float value);
};

class ControllerParam 
{
public:
    float *param = nullptr;
    int size = 0;
    ControllerParam(uint8_t type);
    ~ControllerParam();
private:
    void _AllocParamMem(uint8_t n);
};

class AttitudeControllerParam
{
public:
    AttitudeControllerParam(uint8_t type);
    ControllerParam *pitch = nullptr;
    ControllerParam *roll = nullptr;
    ControllerParam *yaw = nullptr;
    ~AttitudeControllerParam();
};

class AttitudeController
{
public:
    virtual void Init(AttitudeControllerParam *param) = 0;
    virtual void Update(AircraftState &actual_state, ExpectState &expect_state, ControlOutput &control_output) = 0;
};

#endif
