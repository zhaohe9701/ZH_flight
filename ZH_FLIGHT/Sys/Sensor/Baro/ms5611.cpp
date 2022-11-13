/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 19:37:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-14 00:03:27
 * @FilePath: \H7B0\Sys\Sensor\Baro\ms5611.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ms5611.h"
#include "cmsis_os.h"

#define MS5611_ADDR 0xEE // MS5611的地址
#define RESET 0x1E       // cmd 复位

#define ADC_READ 0x00
#define OSR_256_D1 0x40
#define OSR_512_D1 0x42
#define OSR_1024_D1 0x44
#define OSR_2048_D1 0x46
#define OSR_4096_D1 0x48

#define OSR_256_D2 0x50
#define OSR_512_D2 0x52
#define OSR_1024_D2 0x54
#define OSR_2048_D2 0x56
#define OSR_4096_D2 0x58
#define PROM_READ 0xA0 // Prom read command

Ms5611::Ms5611(SensorInterface *interface)
{
    _interface = interface;
}
void Ms5611::Init()
{
    _BaroWriteRag(MS5611_ADDR, 0, nullptr);
    HAL_Delay(100);
    for (int i = 0; i < 8; ++i)
    {
        uint8_t buf[2] = {0};
        _BaroReadRag(PROM_READ + i * 2, 2, buf);
        _calculation[i] = (uint16_t)buf[0] << 8 | (uint16_t)buf[1];
    }
}
float Ms5611::GetTemperature()
{
    uint8_t buf[3] = {0};
    uint32_t temperature_raw = 0;
    uint32_t ca = 0;

    _BaroWriteRag(OSR_4096_D2, 0, nullptr);
    osDelay(10);
    _BaroReadRag(ADC_READ, 3, buf);
    temperature_raw = (uint32_t)buf[0] << 16 | (uint32_t)buf[1] << 8 | (uint32_t)buf[2];
    ca = ((uint32_t)_calculation[5]) << 8;
    if (temperature_raw > ca)
    {
        _d_t = (double)(temperature_raw - ca);
    }
    else
    {
        _d_t = -(double)(ca - temperature_raw);
    }
    _temperature = 2000.0f + _d_t * (float)((uint32_t)_calculation[6]) / 8388608.0f;
    return _temperature;
}

float Ms5611::GetPressure()
{
    uint8_t buf[3] = {0};
    uint32_t pressure_raw = 0;
    double T2 = 0.0f;
    double Aux = 0.0f;
    double OFF2 = 0.0;
    double SENS2 = 0.0;
    double OFF = (uint32_t)(_calculation[2] << 16) + ((uint32_t)_calculation[4] * _d_t) / 128;
    double SENS = (uint32_t)(_calculation[1] << 15) + ((uint32_t)_calculation[3] * _d_t) / 256;

    _BaroWriteRag(OSR_4096_D1, 0, nullptr);
    osDelay(10);
    _BaroReadRag(ADC_READ, 3, buf);
    pressure_raw = (uint32_t)buf[0] << 16 | (uint32_t)buf[1] << 8 | (uint32_t)buf[2];

    if (_temperature < 2000.0)
    {
        T2 = (_d_t * _d_t) / 2147483648.0;
        Aux = (_temperature - 2000.0) * (_temperature - 2000.0);
        OFF2 = 2.5 * Aux;
        SENS2 = 1.25 * Aux;
        if (_temperature < -1500)
        {
            Aux = (_temperature + 1500.0) * (_temperature + 1500.0);
            OFF2 = OFF2 + 7 * Aux;
            SENS2 = SENS + (float)(5.5) * Aux;
        }
    }
    else //(Temperature > 2000)
    {
        T2 = 0.0;
        OFF2 = 0.0;
        SENS2 = 0.0;
    }

    _temperature -= T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    _pressure = ((double)pressure_raw * SENS / 2097152.0 - OFF) / 32768.0;
    _pressure = _pressure / 1000000.0;

    return _pressure;
}

float Ms5611::GetAltitude()
{
    GetTemperature();
    GetPressure();

    float altitude = 4433000.0f * (1 - powf((((float)_pressure) / 1013.25f), 0.190295f));

    return altitude;
}

void Ms5611::_BaroWriteRag(uint8_t address, uint8_t length, uint8_t *value)
{
    _interface->WriteRegs(address, length, value);
}

void Ms5611::_BaroReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->ReadBytes(address, length, buf);
}