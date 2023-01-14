/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:14:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-15 02:45:00
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20689.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "icm20689.h"

#define READ 0x80
#define WRITE 0x7f

/**********REGISTER MAP*************/
#define SELF_TEST_X_GYRO        0x00
#define SELF_TEST_Y_GYRO        0x01
#define SELF_TEST_Z_GYRO        0x02
#define SELF_TEST_X_ACCEL       0x0D
#define SELF_TEST_Y_ACCEL       0x0E
#define SELF_TEST_Z_ACCEL       0x0F

#define XG_OFFS_USRH            0x13            
#define XG_OFFS_USRL            0x14
#define YG_OFFS_USRH            0x15
#define YG_OFFS_USRL            0x16
#define ZG_OFFS_USRH            0x17
#define ZG_OFFS_USRL            0x18

#define SMPLRT_DIV              0x19
#define CONFIG                  0x1A
#define GYRO_CONFIG             0x1B
#define ACCEL_CONFIG_1          0x1C
#define ACCEL_CONFIG_2          0x1D
#define LP_NODE_CFG             0x1E
#define ACCEL_WOM_THR           0x1F
#define FIFO_EN                 0x23
#define FSYNC_INT               0x36
#define INT_PIN_CFG             0x37
#define INT_ENABLE              0x38
#define DMP_INT_STATUS          0x39
#define INT_STATUS              0x3A

#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40

#define TEMP_OUT_H              0x41
#define TEMP_OUT_L              0x42

#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48

#define SIGNAL_PATH_RESET       0x68
#define ACCEL_INTEL_CTRL        0x69
#define USER_CTRL               0x6A

#define PWR_MGMT_1              0x6B
#define PWR_MGMT_2              0x6C

#define FIFO_COUNTH             0x72
#define FIFO_COUNTL             0x73
#define FIFO_R_W                0x74

#define WHO_AM_I                0x75

#define XA_OFFSET_H             0x77
#define XA_OFFSET_L             0x78
#define YA_OFFSET_H             0x7A
#define YA_OFFSET_L             0x7B
#define ZA_OFFSET_H             0x7D
#define ZA_OFFSET_L             0x7E
/***********************************/


void Icm20689::_ImuWriteRag(uint8_t address, uint8_t value)
{
    _interface->WriteReg(address & WRITE, value);
}

void Icm20689::_ImuReadRag(uint8_t address, uint8_t length, uint8_t *buf)
{
    _interface->ReadBytes(address | READ, length, buf);
}

Icm20689::Icm20689(SensorInterface *interface)
{
    _interface = interface;
}
void Icm20689::Init()
{
    /*复位芯片*/
    _ImuWriteRag(PWR_MGMT_1, 0x81);
    HAL_Delay(100);
    /*配置陀螺仪*/
    _ImuWriteRag(GYRO_CONFIG, 0x03 << 3);
    HAL_Delay(100);
    /*配置加速度计*/
    _ImuWriteRag(ACCEL_CONFIG_1, 0x03 << 3);
    HAL_Delay(100);
    /*陀螺仪滤波176Hz*/
    _ImuWriteRag(CONFIG, 0x01);
    HAL_Delay(100);
    /*采样频率1000Hz*/
    _ImuWriteRag(SMPLRT_DIV, 0x00);
    HAL_Delay(100);
    /*设置中断低电平脉冲*/
    _ImuWriteRag(INT_PIN_CFG, 0x10);
    HAL_Delay(100);
    /*使能数据就绪中断*/
    _ImuWriteRag(INT_ENABLE, 0x01);
    HAL_Delay(100);
    /*使能陀螺仪加速度计*/
    _ImuWriteRag(PWR_MGMT_2, 0x00);
    HAL_Delay(100);

    _gyro_sensitivity = 0.061035;
    _acc_sensitivity = 0.48828;

}

uint8_t Icm20689::GetId()
{
    uint8_t id = 0;
    _ImuReadRag(WHO_AM_I, 1, &id);
    return id;
}

float Icm20689::GetTemperature()
{
    uint8_t buf[2];
    int16_t raw;
    float temp;
    _ImuReadRag(TEMP_OUT_H, 2, buf);
    raw = ((int16_t)buf[0] << 8) | buf[1];
    temp = 25.0f + (float)raw / 326.8f;
    return temp;
}

void Icm20689::GetGyroData(ImuData &sensor_data)
{
    uint8_t buf[6];
    uint16_t gx_raw, gy_raw, gz_raw;

    _ImuReadRag(GYRO_XOUT_H, 6, buf);
    
    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];

    sensor_data.gyr.x = (float)((int16_t)(gx_raw)-_bias_gyro_x) * _gyro_sensitivity;
    sensor_data.gyr.y = (float)((int16_t)(gy_raw)-_bias_gyro_y) * _gyro_sensitivity;
    sensor_data.gyr.z = (float)((int16_t)(gz_raw)-_bias_gyro_z) * _gyro_sensitivity;
}

void Icm20689::GetAccData(ImuData &sensor_data)
{
    uint8_t buf[6];
    uint16_t ax_raw, ay_raw, az_raw;

    _ImuReadRag(ACCEL_XOUT_H, 6, buf);
    
    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    sensor_data.acc.x = (float)((int16_t)(ax_raw) - _bias_acc_x) * _acc_sensitivity;
	sensor_data.acc.y = (float)((int16_t)(ay_raw) - _bias_acc_y) * _acc_sensitivity;
	sensor_data.acc.z = (float)((int16_t)(az_raw) - _bias_acc_z) * _acc_sensitivity;
}