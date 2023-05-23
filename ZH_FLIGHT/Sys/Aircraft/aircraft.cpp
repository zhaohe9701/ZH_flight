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
#include "dshot.h"
#include "z_pwm.h"

#include "aircraft.h"
#include "mahony.h"

#include "attitude_controller_interface.h"
#include "pid_attitude_controller.h"

#include "global_var.h"

#include "data_manager.h"

extern "C"
{
GlobalVar system_var;
}

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
    _attitude_controller = new AttitudeController();
    _attitude_solver = new Mahony();
    _attitude_control_param = new ControlParam();
    _printer = new Printer(message_transmit_server->GetMessageManager());
}

AC_RET Aircraft::Init()
{
    /*加载惯性测量单元*/
#ifdef IMU1
    SensorInterface *imu_interface1 = new IMU1_INTERFACE(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    Imu *imu1 = new IMU1(imu_interface1);
    _sensor->AddImu(imu1);
    _sensor->Init();
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

    _printer->SetDecPort(0x01);

    _index = CreateIndex();
    // /*电机控制接口*/
    // Pwm *motor_interface = nullptr;
    // MotorProtocolInterface *motor_protocol_interface = nullptr;
    // /*1号电机*/
    // motor_interface = new Pwm(&MOTOR_1_TIM, MOTOR_1_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[0].SetProtocol(motor_protocol_interface);
    // /*2号电机*/
    // motor_interface = new Pwm(&MOTOR_2_TIM, MOTOR_2_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[1].SetProtocol(motor_protocol_interface);
    // /*3号电机*/
    // motor_interface = new Pwm(&MOTOR_3_TIM, MOTOR_3_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[2].SetProtocol(motor_protocol_interface);
    // /*4号电机*/
    // motor_interface = new Pwm(&MOTOR_4_TIM, MOTOR_4_CHANNEL);
    // motor_protocol_interface = new Dshot(motor_interface);
    // _motors[3].SetProtocol(motor_protocol_interface);
    // /*姿态控制器*/   
    // AttitudeControllerInterface *attitude_controller_interface = new PidAttitudeController();
    // _attitude_controller->SetMethod(attitude_controller_interface);
    // _attitude_control_param->Init(ATTITUDE_CONTROLLER_PARAM_NUM);
    // // _attitude_control_param->GetParam(attitude_pid_param);
    // _attitude_controller->Init(_attitude_control_param);
    return AC_OK;
}

AC_RET Aircraft::SetAction(ActionGroup action)
{
    _current_action = action;
    return AC_OK;
}

AC_RET Aircraft::GetAccAndGyro()
{
    ImuData imu_data;
    _sensor->imu->GetAccData(imu_data);
    _sensor->imu->GetGyroData(imu_data);
    // uint8_t id = _sensor->imu->GetId();
    // UsbPrintf("0x%x\r\n", id);
    // UsbPrintf("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
    _imu_data_manager.Update(&imu_data);
    // _printer->Info("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
    return AC_OK;
}

AC_RET Aircraft::GetAltitude()
{
    BaroData baro_data;
    _sensor->baro->GetAltitude(baro_data);
    _baro_data_manager.Update(&baro_data);
    return AC_OK;
}

AC_RET Aircraft::UpdateAttitude()
{
    ActualState actual_state;
    SensorData sensor_data;
    ImuData imu_data;
    MagData mag_data;

    _imu_data_manager.Copy(&imu_data);
    // _actual_state_manager.Copy(&actual_state);

    sensor_data.acc = imu_data.acc;
    sensor_data.gyr = imu_data.gyr;
    sensor_data.mag = mag_data.mag;

    _attitude_solver->Update(actual_state, sensor_data);

    _actual_state_manager.Update(&actual_state);
    return AC_OK;
}



AC_RET Aircraft::ControlAttitude()
{
    ActionGroup current_action = _current_action;
    ActualState actual_state;
    ExpectState expect_state;
    ActuatorData expect_actuator = {{0}};

    if (current_action != AS_AUTO     && 
        current_action != AS_MANUAL   && 
        current_action != AS_ALTITUDE)
    {
        return AC_OK;
    }

    _actual_state_manager.Copy(&actual_state);
    _expect_state_manager.Copy(&expect_state);
    _attitude_controller->Update(actual_state, expect_state, expect_actuator);
    _expect_actuator_manager.Update(&expect_actuator);
    return AC_OK;
}



AC_RET Aircraft::ControlAltitude()
{
    ActionGroup current_action = _current_action;
    if (current_action != AS_ALTITUDE)
    {
        return AC_OK;
    }

    return AC_OK;
}

AC_RET Aircraft::ControlMotor()
{
    ActionGroup current_action = _current_action;
    ActuatorData expect_actuator;

    if (current_action != AS_AUTO       &&
        current_action != AS_MANUAL     && 
        current_action != AS_ALTITUDE   &&
        current_action != AS_INITIALIZE)
    {
        return AC_OK;
    }

    _expect_actuator_manager.Copy(&expect_actuator);
    for (int i = 0; i < MOTOR_NUM; ++i)
    {
        _motors[i].SetSpeed(expect_actuator.motor[i]);
    }
    return AC_OK;
}

AC_RET Aircraft::Test()
{
    ActualState state;
    BaroData baro_data;

    _baro_data_manager.Copy(&baro_data);
    _actual_state_manager.Copy(&state);
    // _printer->Info("%.2f %.2f %.2f %.2f\r\n", state.euler.x, state.euler.y, state.euler.z, baro_data.altitude);
    // _printer->Info("%.2f\r\n", baro_data.altitude);
    return AC_OK;
}

Aircraft::~Aircraft()
{
    delete _sensor;
    delete[] _motors;
    delete _attitude_controller;
    delete _attitude_control_param;
    delete _attitude_solver;
}

AcTreeNode *Aircraft::CreateIndex()
{
    AcTreeNode *root = new AcTreeNode();
    AcTreeNode *node = nullptr;

    root->AddData(nullptr, AC_STRUCT, "aircraft", 0);

    node = _sensor->CreateIndex();
    AcTree::AddNode(root, node);

    node = _led->CreateIndex();
    AcTree::AddNode(root, node);

    return root;
}

AcTreeNode *Aircraft::GetIndex()
{
    return _index;
}

AC_RET Aircraft::ControlLight()
{
    _led->Run();
    return AC_OK;
}
