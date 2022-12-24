/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:42:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-24 23:18:41
 * @FilePath: \ZH_FLIGHT\Sys\Aircraft\aircraft.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_H__
#define __AIRCRAFT_H__

#include "type.h"
#include "sensor.h"
#include "motor.h"
#include "config.h"
class Aircraft
{
private:
    Sensor _sensor;
    Motor _motors[MOTOR_NUM];
public:
    AC_RET Init();

};



#endif
