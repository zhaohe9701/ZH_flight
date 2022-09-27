/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:54:15
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 22:49:46
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "motor.h"


Motor::Motor(MotorInterface *interface)
{
    _interface = interface;
    _interface->Init();
}

void Motor::EnableMotor()
{
	uint16_t value[MOTOR_NUM] = {0};
    _interface->SetMotor(value);
}

void Motor::SetSpeed(uint16_t *value)
{
    _interface->SetMotor(value);
}