/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:42:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:16:01
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
#include "data_manager.h"
#include "actuator_data.h"
#include "remote_data.h"
#include "print.h"
class Aircraft
{
private:
    /*传感器*/
    Sensor *_sensor = nullptr;
    /*电机*/
    Motor *_motors = nullptr;
    /*打印机*/
    Printer *_printer = nullptr;
    DataManager<ImuData> _imu_data_manager;
    DataManager<BaroData> _baro_data_manager;
    DataManager<RemoteData> _remote_data_manager;
    DataManager<ActualState> _actual_state_manager;
    DataManager<ExpectState> _expect_state_manager;
    DataManager<ActuatorData> _expect_actuator_manager;
    /*姿态控制器*/
    AttitudeController *_attitude_controller = nullptr;
    /*控制参数*/
    ControlParam *_attitude_control_param = nullptr;
    /*姿态解算器*/
    AttitudeSolver *_attitude_solver = nullptr;
    /*当前动作*/
    volatile ActionGroup _current_action = AS_INITIALIZE;

    AcTreeNode *_index = nullptr;
public:

    Aircraft();
    AC_RET Init();
    AcTreeNode *CreateIndex();
    AcTreeNode *GetIndex();
    AC_RET SetAction(ActionGroup action);
    AC_RET GetAccAndGyro();
    AC_RET GetAltitude();
    AC_RET GetMagnetic();
    AC_RET UpdateAttitude();
    AC_RET ControlAttitude();
    AC_RET ControlAltitude();
    AC_RET ControlMotor();
    AC_RET Test();
    ~Aircraft();

};



#endif
