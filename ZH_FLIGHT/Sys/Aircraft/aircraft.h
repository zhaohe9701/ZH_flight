/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:42:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-09 00:39:03
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
#include "state_machine.h"
class Aircraft
{
private:
    /*传感器*/
    Sensor *_sensor = nullptr;
    /*电机*/
    Motor *_motors = nullptr;
    /*全局实际状态*/
    ActualState *_actual_state = nullptr;
    ActualState *_actual_state_for_attitude_solve = nullptr;
    ActualState *_actual_state_for_attitude_control = nullptr;
    /*全局期望状态*/
    ExpectState *_expect_state = nullptr;
    ExpectState *_expect_state_for_control = nullptr;
    ExpectState *_expect_state_for_remote = nullptr;
    /*姿态控制器*/
    AttitudeController *_attitude_controller = nullptr;
    /*控制参数*/
    ControlParam *_attitude_control_param = nullptr;
    /*姿态解算器*/
    AttitudeSolver *_attitude_solver = nullptr;
    /*IMU数据*/
    ImuData _imu_data;
    /*当前动作*/
    ActionList _current_action = AS_INITIALIZE;
public:
    Aircraft();
    AC_RET Init();
    AC_RET SetAction(ActionList action);
    AC_RET UpdateAttitude();
    AC_RET GetStateForControl();
    AC_RET ControlAttitude();
    AC_RET ControlAltitudeByDirect();
    AC_RET ControlAltitudeBySensor();
    AC_RET ControlMotor();
    ~Aircraft();

};



#endif
