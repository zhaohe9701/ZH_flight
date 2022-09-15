/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:23:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-08-20 14:48:32
 * @FilePath: \ZH_FLIGHT\Sys\Flight\controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include "state.h"
#include "type.h"

class ControlOutput
{
public:
    Vec3<float> attitude_out;
    Vec4<float> motor_out;
    void Allocate();
};

class ControlParam
{
public:

};

class Controller
{
public:
    virtual void Init(ControlParam &ControlParam) = 0;
    virtual void Update(State &actual_state, State &expect_state, ControlOutput &control_output) = 0;
};




#endif
