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
#include "default_attitude_controller.h"
#include "ahrs.h"
#include "state_machine.h"
#include "data_manager.h"
#include "actuator_data.h"
#include "remote_data.h"
#include "print.h"
#include "Light/light.h"

class Aircraft
{
private:
    /*传感器*/
    Sensor *_sensor = nullptr;
    /*电机*/
    Motor *_motors = nullptr;
    /*打印机*/
    Printer *_printer = nullptr;
    /*LED*/
    Led *_led = nullptr;

    DataManager<ImuData> _imu_data_manager;
    DataManager<BaroData> _baro_data_manager;
    DataManager<RemoteData> _remote_data_manager;
    DataManager<ActualAttitudeState> _actual_state_manager;
    DataManager<ExpectAttitudeState> _expect_state_manager;
    DataManager<ActuatorData> _expect_actuator_manager;
    /*姿态控制器*/
    AttitudeController *_attitude_controller = nullptr;
    /*姿态解算器*/
    AttitudeSolver *_attitude_solver = nullptr;
    /*当前动作*/

    JsonTree *_index = nullptr;
public:

    Aircraft();
    AC_RET init();
    JsonTree *createIndex();
    JsonTree *getIndex();
    AC_RET getAccAndGyro();
    AC_RET getAltitude();
    AC_RET getMagnetic();
    AC_RET updateAttitude();
    AC_RET controlAttitude();
    AC_RET controlAltitude();
    AC_RET controlMotor();
    AC_RET controlLight();
    AC_RET test();
    ~Aircraft();

};



#endif
