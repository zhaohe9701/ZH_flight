/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-22 23:58:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:35:38
 * @FilePath: \ZH_FLIGHT\Sys\Aircraft\aircraft.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include "actuator_data.h"
#include "aircraft_state.h"
#include "sys.h"

#include "print.h"
#include "sensor_interface.h"
#include "type.h"
#include "z_spi.h"
#include "imu.h"
#include "mag.h"

#include "icm20602.h"
#include "icm20689.h"
#include "icm20948.h"
#include "icm42688.h"
#include "baro.h"
#include "z_iic.h"
#include "ms5611.h"

#include "motor_interface.h"
#include "motor_protocol_interface.h"
#include "Dshot/dshot.h"
#include "z_pwm.h"

#include "aircraft.h"
#include "mahony.h"

#include "attitude_controller.h"

#include "global_var.h"

#include "data_manager.h"

#include "Mavlink/Core/common/mavlink.h"
#include "Mavlink/Core/mavlink_types.h"



extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim8;

Aircraft::Aircraft()
{
    /*创建组件*/
    _sensor = new Sensor();
    // _motors = new Motor[MOTOR_NUM]();
    _led = new Led(LED_GPIO_Port, LED_Pin);
    _attitude_controller = new DefaultAttitudeController();
    _attitude_solver = new Mahony();
    _printer = new Printer(message_transmit_server->getMessageManager());
}

AC_RET Aircraft::init()
{
    /*加载惯性测量单元*/
#ifdef IMU1
    SensorInterface *imu_interface1 = new IMU1_INTERFACE(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    Imu *imu1 = new IMU1(imu_interface1);
    _sensor->addImu(imu1);
    _sensor->init();
#endif /*#ifdef IMU1*/

#ifdef IMU2
    SensorInterface *imu_interface2 = new IMU2_INTERFACE(&IMU1_INTERFACE_OBJ, IMU2_CS_PORT, IMU2_CS_PIN);
    Imu *imu2 = new IMU2(imu_interface2);
    _sensor->AddImu(imu2);
#endif /*#ifdef IMU2*/

    /*加载气压计*/
    SensorInterface* baro_interface = new Iic(&hi2c1, 0xEE);
    Baro *baro = new Ms5611(baro_interface);
    _sensor->addBaro(baro);
    /*传感器初始化*/
    _sensor->init();

    _printer->setDecPort(0x01);

    _index = createIndex();
    // /*电机控制接口*/
    // Pwm *motor_interface = nullptr;
    // MotorProtocolInterface *motor_protocol_interface = nullptr;
    // /*1号电机*/
    // motor_interface = new Pwm(&MOTOR_1_TIM, MOTOR_1_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[0].setProtocol(motor_protocol_interface);
    // /*2号电机*/
    // motor_interface = new Pwm(&MOTOR_2_TIM, MOTOR_2_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[1].setProtocol(motor_protocol_interface);
    // /*3号电机*/
    // motor_interface = new Pwm(&MOTOR_3_TIM, MOTOR_3_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[2].setProtocol(motor_protocol_interface);
    // /*4号电机*/
    // motor_interface = new Pwm(&MOTOR_4_TIM, MOTOR_4_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[3].setProtocol(motor_protocol_interface);
    // /*姿态控制器*/   
    // AttitudeControllerInterface *attitude_controller_interface = new PidAttitudeController();
    // _attitude_controller->setMethod(attitude_controller_interface);
    // _attitude_control_param->Init(ATTITUDE_CONTROLLER_PARAM_NUM);
    // // _attitude_control_param->getParam(attitude_pid_param);
    // _attitude_controller->Init(_attitude_control_param);
    return AC_OK;
}


AC_RET Aircraft::getAccAndGyro()
{
    ImuData imu_data;
    _sensor->imu->getAccData(imu_data);
    _sensor->imu->getGyroData(imu_data);
    // uint8_t id = _sensor->imu->GetId();
    // UsbPrintf("0x%x\r\n", id);
    // UsbPrintf("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
    _imu_data_manager.update(&imu_data);
    //_printer->info("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
    return AC_OK;
}

AC_RET Aircraft::getAltitude()
{
    BaroData baro_data;
    _sensor->baro->getAltitude(baro_data);
    _baro_data_manager.update(&baro_data);
    return AC_OK;
}

AC_RET Aircraft::updateAttitude()
{
    ActualAttitudeState actual_state;
    SensorData sensor_data;
    ImuData imu_data;
    MagData mag_data;

    _imu_data_manager.copy(&imu_data);
    // _actual_state_manager.Copy(&actual_state);

    sensor_data.acc = imu_data.acc;
    sensor_data.gyr = imu_data.gyr;
    sensor_data.mag = mag_data.mag;

    _attitude_solver->update(actual_state, sensor_data);

    _actual_state_manager.update(&actual_state);
    return AC_OK;
}



AC_RET Aircraft::controlAttitude()
{
    AttitudeState state;
    ActuatorData expect_actuator = {{0}};



    _actual_state_manager.copy(&state.actual);
    _expect_state_manager.copy(&state.expect);
    _attitude_controller->update(state, expect_actuator);
    _expect_actuator_manager.update(&expect_actuator);
    return AC_OK;
}



AC_RET Aircraft::controlAltitude()
{


    return AC_OK;
}

AC_RET Aircraft::controlMotor()
{
    ActuatorData expect_actuator;


    _expect_actuator_manager.copy(&expect_actuator);
    for (int i = 0; i < MOTOR_NUM; ++i)
    {
        _motors[i].setSpeed(expect_actuator.motor[i]);
    }
    return AC_OK;
}

AC_RET Aircraft::test()
{
    uint8_t *buf = nullptr;
    mavlink_message_t msg_t;
    int len = mavlink_msg_mission_count_pack(255, 1, &msg_t, 1, 0, 5, MAV_MISSION_TYPE_MISSION);
    buf = new uint8_t[len];
    mavlink_msg_to_send_buffer(buf, &msg_t);
    // _printer->transmit(buf, len);
    // _printer->info("test run\n");
    return AC_OK;
}

Aircraft::~Aircraft()
{
    delete _sensor;
    delete[] _motors;
    delete _attitude_controller;
    delete _attitude_solver;
}

JsonTree *Aircraft::createIndex()
{
    JsonTree *root = new JsonTree();
    JsonTree *node = nullptr;

    root->addData(nullptr, AC_STRUCT, "aircraft");

    node = _sensor->createIndex();
    root->addChild(node);

    node = _led->createIndex();
    root->addChild(node);

    return root;
}

JsonTree *Aircraft::getIndex()
{
    return _index;
}

AC_RET Aircraft::controlLight()
{
    _led->run();
    return AC_OK;
}
