/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:30:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-24 22:41:37
 * @FilePath: \ZH_FLIGHT\Sys\Flight\state.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_H__
#define __STATE_H__
#include "type.h"

#define PITCH x
#define ROLL y
#define YAW z

#define MOTOR1 x
#define MOTOR2 y
#define MOTOR3 z
#define MOTOR4 w

class State
{
public:
    Vec3<float> euler;
    Vec3<float> palstance;
    Vec3<float> velocity;
    Vec3<float> gps_coor;
    Vec3<float> relative_pos;
    Vec3<float> acceleration;
    Vec4<float> quarter;
    void Init();
};

class AircraftState : public State
{
public:
};

class ExpectState : public State
{
public:
    float throttle = 0.0f;
};


#endif

