/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:10:20
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 22:45:38
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20689.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __ICM20689_H__
#define __ICM20689_H__

#include "imu.h"

class Icm20689 : virtual public Imu
{
public:
    Icm20689(SensorInterface *interface);
    void init() override;
    uint8_t getId() override;
    float getTemperature() override;
    void getGyroData(ImuData &data) override;
    void getAccData(ImuData &data) override;
private:
    void _imuWriteRag(uint8_t address, uint8_t value);
    void _imuReadRag(uint8_t address, uint8_t length, uint8_t *buf);
    SensorInterface *_interface = nullptr;

    //void _getGyroRawData(int16_t *gx, int16_t *gy, int16_t *gz);
    //void _getAccRawData(int16_t *ax, int16_t *ay, int16_t *az);

    float _acc_sensitivity = 0.0f;
    float _gyro_sensitivity = 0.0f;
    int16_t _bias_acc_x = 0;
    int16_t _bias_acc_y = 0;
    int16_t _bias_acc_z = 0;
    int16_t _bias_gyro_x = 0;
    int16_t _bias_gyro_y = 0;
    int16_t _bias_gyro_z = 0;
};


#endif