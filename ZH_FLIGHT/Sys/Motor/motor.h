/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-26 22:53:58
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-26 23:42:07
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdint.h>

class Motor
{
public:
    virtual void Init() = 0;
    virtual void EnableMotor() = 0;
    virtual void SetMotor(uint16_t *value) = 0;
};

#endif
