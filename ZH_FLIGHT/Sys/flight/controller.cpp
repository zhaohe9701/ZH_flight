/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:40:35
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-22 23:25:26
 * @FilePath: \ZH_FLIGHT\Sys\Flight\controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "controller.h"
#include "config.h"

void ControlOutput::SetThrottle(float value)
{
    throttle = value;
}

void ControlOutput::Allocate()
{
    motor_out.MOTOR1 = THROTTLE_MAP(_Limit((throttle + attitude_out.PITCH - attitude_out.ROLL + attitude_out.YAW)));
    motor_out.MOTOR2 = THROTTLE_MAP(_Limit((throttle - attitude_out.PITCH - attitude_out.ROLL - attitude_out.YAW)));
    motor_out.MOTOR3 = THROTTLE_MAP(_Limit((throttle + attitude_out.PITCH + attitude_out.ROLL - attitude_out.YAW)));
    motor_out.MOTOR4 = THROTTLE_MAP(_Limit((throttle - attitude_out.PITCH + attitude_out.ROLL + attitude_out.YAW)));
}

float ControlOutput::_Limit(float value)
{
    if(value > MOTOR_OUTPUT_MAX)
    {
        return MOTOR_OUTPUT_MAX;
    }
    else if(value < MOTOR_OUTPUT_MIN)
    {
        return MOTOR_OUTPUT_MIN;
    }
    else
    {
        return value;
    }
}

ControllerParam::ControllerParam(uint8_t type)
{
    switch (type)
    {
    case PID:
        _AllocParamMem(PID_PARAM_NUM);
        break;
    
    default:
        break;
    }
}

ControllerParam::~ControllerParam()
{
    if (param != nullptr)
    {
        delete param;
    }
}
void ControllerParam::_AllocParamMem(uint8_t n)
{
    param = new float[n];
    size = n;
}