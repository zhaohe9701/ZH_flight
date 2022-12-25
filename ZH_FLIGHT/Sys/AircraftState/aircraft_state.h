/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:30:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 01:33:15
 * @FilePath: \ZH_FLIGHT\Sys\AircraftState\aircraft_state.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_STATE_H__
#define __AIRCRAFT_STATE_H__
#include "type.h"
#include "config.h"
#define PITCH x
#define ROLL y
#define YAW z

#define MOTOR1 x
#define MOTOR2 y
#define MOTOR3 z
#define MOTOR4 w

class AircraftState
{
public:
    Vec3 euler;
    Vec3 palstance;
    Vec3 velocity;
    Vec3 gps_coor;
    Vec3 relative_pos;
    Vec3 acceleration;
    Vec4 quarter;
    void Init();
};

class ActualState : public AircraftState
{
public:
};

class ExpectState : public AircraftState
{
public:
    float throttle = 0.0f;
    float motor[MOTOR_NUM] = {0.0f};
};


#endif

