/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 22:31:04
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:27:48
 * @FilePath: \ZH_FLIGHT\Sys\Motor\motor_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MOTOR_INTERFACE_H__
#define __MOTOR_INTERFACE_H__

#include <stdint.h>
class MotorInterface
{
public:
    virtual void output(uint16_t *data, uint16_t length) = 0;
    virtual void stop() = 0;
    virtual ~MotorInterface() = 0;
};


#endif
