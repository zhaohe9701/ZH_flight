/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:53:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 23:59:56
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>
#include "config.h"
#include "motor_interface.h"
class Motor
{
public:
    Motor(MotorInterface *interface);
    void EnableMotor();
    void SetSpeed(float *value);
private:
    MotorInterface *_interface;
};

#endif
