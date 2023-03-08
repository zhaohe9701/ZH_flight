/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:30:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-02 00:23:50
 * @FilePath: \ZH_FLIGHT\Sys\AircraftState\aircraft_state.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_STATE_H__
#define __AIRCRAFT_STATE_H__
#include "type.h"
#include "os.h"
#include "config.h"
#include "cmsis_os2.h"
#define PITCH x
#define ROLL y
#define YAW z

#define MOTOR1 x
#define MOTOR2 y
#define MOTOR3 z
#define MOTOR4 w

enum Pattern
{
    MANUAL_PATTERN,
    ALTITUDE_PATTERN,
    AUTO_PATTERN,
};

enum FlyLock
{
    FLY_LOCK,
    FLY_UNLOCK,
};
class ActualState
{
public:
    Vec3 euler;
    Vec3 palstance;
    Vec3 velocity;
    Vec3 gps_coor;
    Vec3 relative_pos;
    Vec3 acceleration;
    Vec4 quarter;
};

class ExpectState
{
public:
    Vec3 euler;
    Vec3 palstance;
    Vec3 velocity;
    Vec3 gps_coor;
    Vec3 relative_pos;
    Vec3 acceleration;
    Vec4 quarter;

    Pattern pattern = MANUAL_PATTERN;
    FlyLock locker = FLY_LOCK;
    float throttle = 0.0f;
};



#endif

