/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 19:36:40
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 23:40:41
 * @FilePath: \H7B0\Sys\Sensor\Baro\ms5611.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MS5611_H__
#define __MS5611_H__

#include "sensor_interface.h"
#include "baro.h"

class Ms5611 : virtual public Baro
{
public:
    Ms5611(SensorInterface *interface);
    void Init() override;
    float GetTemperature() override;
    float GetPressure() override;
    float GetAltitude() override;
    ~Ms5611();
private:
    SensorInterface *_interface;
    uint16_t _calculation[8] = {0};
    double _d_t = 0.0f;
    float _temperature = 0.0f;
    float _pressure = 0.0f;
    void _BaroWriteRag(uint8_t address, uint8_t length, uint8_t *value);
    void _BaroReadRag(uint8_t address, uint8_t length, uint8_t *buf);
};

#endif
