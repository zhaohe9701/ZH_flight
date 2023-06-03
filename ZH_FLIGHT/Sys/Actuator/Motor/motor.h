/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:53:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 00:46:00
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include "config.h"
#include "motor_protocol_interface.h"
class Motor
{
public:
    void setProtocol(MotorProtocolInterface *interface);
    void enableMotor();
    void setSpeed(float value);
private:
    MotorProtocolInterface *_interface;
};

#endif
