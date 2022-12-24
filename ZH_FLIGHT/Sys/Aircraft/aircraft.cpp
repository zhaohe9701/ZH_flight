/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:58:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 00:56:45
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

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim8;

AC_RET Aircraft::Init()
{
    /*加载惯性测量单元*/
#ifdef IMU1
    SensorInterface *imu_interface1 = new IMU1_INTERFACE(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    Imu *imu1 = new IMU1(imu_interface1);
    _sensor.AddImu(imu1);
#endif /*#ifdef IMU1*/

#ifdef IMU2
    SensorInterface *imu_interface2 = new IMU2_INTERFACE(&IMU1_INTERFACE_OBJ, IMU2_CS_PORT, IMU2_CS_PIN);
    Imu *imu2 = new IMU2(imu_interface2);
    _sensor.AddImu(imu2);
#endif /*#ifdef IMU2*/

    /*加载气压计*/
    SensorInterface* baro_interface = new Iic(&hi2c1, 0xEE);
    Baro *baro = new Ms5611(baro_interface);
    _sensor.AddBaro(baro);
    /*传感器初始化*/
    _sensor.Init();

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

    return AC_OK;
}