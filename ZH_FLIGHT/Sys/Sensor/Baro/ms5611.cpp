/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-13 19:37:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:50:47
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Baro\ms5611.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ms5611.h"
#include "cmsis_os2.h"
#include <math.h>

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
    _BaroWriteRag(RESET, 0, nullptr);
    HAL_Delay(100);
    for (int i = 0; i < 8; ++i)
    {
        uint8_t buf[2] = {0};
        _BaroReadRag(PROM_READ + i * 2, 2, buf);
        _calculation[i] = buf[0] << 8 | buf[1];
    }
    if (_CheckCRC() == false)
    {
        UsbPrintf("MS5611 Init Fail\r\n");
    }
    HAL_Delay(1000);
}
void Ms5611::GetTemperature(BaroData& data)
{
    uint8_t buf[3] = {0};
    uint32_t temperature_raw = 0;

    _BaroWriteRag(OSR_4096_D2, 0, nullptr);
    osDelay(10);
    _BaroReadRag(ADC_READ, 3, buf);
    temperature_raw = buf[0] << 16 | buf[1] << 8 | buf[2];

    _dt = (int64_t)temperature_raw - ((uint64_t)_calculation[5] * 256);
    _temperature = 2000 + ((_dt * (int64_t)_calculation[6]) >> 23);

    data.temperature = (float)_temperature / 100.0f;
}

void Ms5611::GetPressure(BaroData& data)
{
    uint8_t buf[3] = {0};
    uint32_t pressure_raw = 0;
    int64_t delt = 0;
    int64_t off = ((int64_t)_calculation[2] << 16) + (((int64_t)_calculation[4] * _dt) >> 7);
    int64_t sens = ((int64_t)_calculation[1] << 15) + (((int64_t)_calculation[3] * _dt) >> 8);

    _BaroWriteRag(OSR_4096_D1, 0, nullptr);
    osDelay(10);
    _BaroReadRag(ADC_READ, 3, buf);
    pressure_raw = buf[0] << 16 | buf[1] << 8 | buf[2];

    if (_temperature < 2000)
    {
        delt = _temperature - 2000;
        delt = 5 * delt * delt;
        off -= delt >> 1;
        sens -= delt >> 2;
        if (_temperature < -1500)
        {
            delt = _temperature + 1500;
            delt = delt * delt;
            off -= 7 * delt;
            sens -= (11 * delt) >> 1;
        }
        _temperature -= ((_dt * _dt) >> 31);
    }

    _pressure = ((((int64_t)pressure_raw * sens) >> 21) - off) >> 15;

    data.pressure = (float)_pressure;
}

void Ms5611::GetAltitude(BaroData& data)
{
    GetTemperature(data);
    GetPressure(data);
    float altitude = (1.0f - powf(_pressure / 101325.0f, 0.190295f)) * 4433000.0f;
    data.altitude = altitude;
}

bool Ms5611::_CheckCRC()
{
    int32_t i, j;
    uint32_t res = 0;
    uint8_t crc = _calculation[7] & 0xF;
    _calculation[7] &= 0xFF00;

    bool blankEeprom = true;

    for (i = 0; i < 16; i++)
    {
        if (_calculation[i >> 1])
        {
            blankEeprom = false;
        }
        if (i & 1)
            res ^= ((_calculation[i >> 1]) & 0x00FF);
        else
            res ^= (_calculation[i >> 1] >> 8);
        for (j = 8; j > 0; j--)
        {
            if (res & 0x8000)
                res ^= 0x1800;
            res <<= 1;
        }
    }
    _calculation[7] |= crc;
    if (!blankEeprom && crc == ((res >> 12) & 0xF))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void Ms5611::_BaroWriteRag(uint8_t address, uint8_t length, uint8_t *value)
{
    _interface->WriteRegs(address, length, value);
}

void Ms5611::_BaroReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->ReadBytes(address, length, buf);
}