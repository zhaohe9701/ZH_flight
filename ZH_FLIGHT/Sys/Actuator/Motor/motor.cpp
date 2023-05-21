/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:54:15
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 00:49:02
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "motor.h"


void Motor::SetProtocol(MotorProtocolInterface *interface)
{
    _interface = interface;
}

void Motor::EnableMotor()
{
	float value = 0.0f;
    _interface->SetMotor(value);
}

void Motor::SetSpeed(float value)
{
    _interface->SetMotor(value);
}