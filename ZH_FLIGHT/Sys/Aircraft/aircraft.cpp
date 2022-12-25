/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:58:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 23:46:15
 * @FilePath: \ZH_FLIGHT\Sys\Aircraft\aircraft.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include "main.h"

#include "sensor_interface.h"
#include "z_spi.h"
#include "imu.h"

#include "icm20602.h"
#include "baro.h"
#include "z_iic.h"
#include "ms5611.h"

#include "motor_interface.h"
#include "motor_protocol_interface.h"
#include "dshot.h"
#include "z_pwm.h"

#include "aircraft.h"
#include "mahony.h"

#include "attitude_controller_interface.h"
#include "pid_attitude_controller.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim8;

Aircraft::Aircraft()
{
    /*创建组件*/
    _sensor = new Sensor();
    _motors = new Motor[MOTOR_NUM]();
    _actual_state = new ActualState();
    _expect_state = new ExpectState();
    _attitude_controller = new AttitudeController();
    _attitude_solver = new Mahony();
    _attitude_control_param = new ControlParam();
}

AC_RET Aircraft::Init()
{
    /*加载惯性测量单元*/
#ifdef IMU1
    SensorInterface *imu_interface1 = new IMU1_INTERFACE(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    Imu *imu1 = new IMU1(imu_interface1);
    _sensor->AddImu(imu1);
#endif /*#ifdef IMU1*/

#ifdef IMU2
    SensorInterface *imu_interface2 = new IMU2_INTERFACE(&IMU1_INTERFACE_OBJ, IMU2_CS_PORT, IMU2_CS_PIN);
    Imu *imu2 = new IMU2(imu_interface2);
    _sensor->AddImu(imu2);
#endif /*#ifdef IMU2*/

    /*加载气压计*/
    SensorInterface* baro_interface = new Iic(&hi2c1, 0xEE);
    Baro *baro = new Ms5611(baro_interface);
    _sensor->AddBaro(baro);
    /*传感器初始化*/
    _sensor->Init();

    /*电机控制接口*/
    Pwm *motor_interface = nullptr;
    MotorProtocolInterface *motor_protocol_interface = nullptr;
    /*1号电机*/
    motor_interface = new Pwm(&MOTOR_1_TIM, MOTOR_1_CHANNEL);
    motor_protocol_interface = new Dshot(motor_interface);
    _motors[0].SetProtocol(motor_protocol_interface);
    /*2号电机*/
    motor_interface = new Pwm(&MOTOR_2_TIM, MOTOR_2_CHANNEL);
    motor_protocol_interface = new Dshot(motor_interface);
    _motors[1].SetProtocol(motor_protocol_interface);
    /*3号电机*/
    motor_interface = new Pwm(&MOTOR_3_TIM, MOTOR_3_CHANNEL);
    motor_protocol_interface = new Dshot(motor_interface);
    _motors[2].SetProtocol(motor_protocol_interface);
    /*4号电机*/
    motor_interface = new Pwm(&MOTOR_4_TIM, MOTOR_4_CHANNEL);
    motor_protocol_interface = new Dshot(motor_interface);
    _motors[3].SetProtocol(motor_protocol_interface);

    /*实际姿态和期望姿态初始化*/
    _actual_state->Init();
    _expect_state->Init();

    /*姿态控制器*/   
    AttitudeControllerInterface *attitude_controller_interface = new PidAttitudeController();
    _attitude_controller->SetMethod(attitude_controller_interface);
    _attitude_control_param->Init(ATTITUDE_CONTROLLER_PARAM_NUM);
    // _attitude_control_param->GetParam(attitude_pid_param);
    _attitude_controller->Init(_attitude_control_param);

    return AC_OK;
}

AC_RET Aircraft::UpdateAttitude()
{
    _sensor->imu->GetAccData(_imu_data);
    _sensor->imu->GetGyroData(_imu_data);

    _attitude_solver->Update(*_actual_state, _imu_data);

    return AC_OK;
}

Aircraft::~Aircraft()
{
    delete _sensor;
    delete[] _motors;
    delete _actual_state;
    delete _expect_state;
    delete _attitude_controller;
    delete _attitude_control_param;
    delete _attitude_solver;
}