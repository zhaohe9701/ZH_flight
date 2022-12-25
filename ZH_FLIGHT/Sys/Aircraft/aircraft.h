/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:42:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:16:37
 * @FilePath: \ZH_FLIGHT\Sys\Aircraft\aircraft.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_H__
#define __AIRCRAFT_H__

#include "type.h"
#include "sensor.h"
#include "motor.h"
#include "aircraft_state.h"
#include "config.h"
#include "attitude_controller.h"
#include "control_param.h"
#include "ahrs.h"

class Aircraft
{
private:
    Sensor *_sensor = nullptr;
    Motor *_motors = nullptr;

    ActualState *_actual_state = nullptr;
    ExpectState *_expect_state = nullptr;

    AttitudeController *_attitude_controller = nullptr;
    ControlParam *_attitude_control_param = nullptr;
    AttitudeSolver *_attitude_solver = nullptr;

    ImuData _imu_data;
public:
    Aircraft();
    AC_RET Init();
    AC_RET UpdateAttitude();
    ~Aircraft();

};



#endif
