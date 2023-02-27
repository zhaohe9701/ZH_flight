/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:44:27
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-25 02:05:47
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\sensor_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __SENSOR_INTERFACE_H__
#define __SENSOR_INTERFACE_H__

#include "main.h"

class SensorInterface
{
public:
    SensorInterface(){};
    virtual void ReadBytes(uint8_t address, uint8_t len, uint8_t *dataBuf) = 0;
    virtual void WriteReg(uint8_t address, uint8_t value) = 0;
    virtual void WriteRegs(uint8_t address, uint8_t len, uint8_t *value) = 0;
    virtual ~SensorInterface(){}; 
};

#endif