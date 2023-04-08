/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:10:20
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-08 18:26:09
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm42688.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __ICM42688_H__
#define __ICM42688_H__

#include "imu.h"

class Icm42688 : virtual public Imu
{
public:
    Icm42688(SensorInterface *interface);
    void Init() override;
    uint8_t GetId() override;
    float GetTemperature() override;
    void GetGyroData(ImuData &sensor_data) override;
    void GetAccData(ImuData &sensor_data) override;

private:
    void _ImuWriteRag(uint8_t bank, uint8_t address, uint8_t value);
    void _ImuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf);
    SensorInterface *_interface = nullptr;

    //void _GetGyroRawData(int16_t *gx, int16_t *gy, int16_t *gz);
    //void _GetAccRawData(int16_t *ax, int16_t *ay, int16_t *az);

    float _acc_sensitivity = 0.0f;
    float _gyro_sensitivity = 0.0f;
    int16_t _bias_acc_x = 0;
    int16_t _bias_acc_y = 0;
    int16_t _bias_acc_z = 0;
    int16_t _bias_gyro_x = 0;
    int16_t _bias_gyro_y = 0;
    int16_t _bias_gyro_z = 0;
    uint8_t _last_bank = 0xFF;
};


#endif