/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:23:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-19 23:00:19
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ATTITUDE_CONTROLLER_H__
#define __ATTITUDE_CONTROLLER_H__
#include "state.h"
#include "type.h"
#include "control_param.h"
#include <stdint.h>

// #define PID 101
// #define PID_PARAM_NUM 12
// class ControlOutput
// {
// public:
//     Vec3<float> attitude_out;
//     Vec4<float> motor_out;
//     float throttle = 0.0f;
//     void Allocate();
//     void SetThrottle(float value);
// private:
//     float _Limit(float value);
// };

// class ControllerParam 
// {
// public:
//     float *param = nullptr;
//     int size = 0;
//     ControllerParam(uint8_t type);
//     ~ControllerParam();
// private:
//     void _AllocParamMem(uint8_t n);
// };

// class AttitudeControllerParam
// {
// public:
//     AttitudeControllerParam(uint8_t type);
//     ControllerParam *pitch = nullptr;
//     ControllerParam *roll = nullptr;
//     ControllerParam *yaw = nullptr;
//     ~AttitudeControllerParam();
// };


class Controll
class AttitudeControllerInterface
{
public:
    virtual void Init(const ControlParam *param);
    virtual void Init();
    virtual void Update(const ControlInput &control_input, ControlOutput &control_output) = 0;
};

#endif
