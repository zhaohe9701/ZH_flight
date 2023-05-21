/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:41:27
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:13:09
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\mpu6X00.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "imu.h"
#include "mpu6X00.h"

Mpu6X00::Mpu6X00(SensorInterface *interface)
{
    _interface = interface;
}

void Mpu6X00::Init()
{
    //UsbPrintf("WRITE1\n");
    HAL_Delay(1000);
    _interface->WriteReg(MPU_PWR_MGMT1_REG, 0X80); //复位MPU6000
    //UsbPrintf("WRITE2\n");
    HAL_Delay(1000);
    _interface->WriteReg(MPU_SIGPATH_RST_REG, 0X04 | 0x02 | 0x01);
    HAL_Delay(100);
    _interface->WriteReg(MPU_PWR_MGMT1_REG, 0X03); //复位MPU6000
    //UsbPrintf("WRITE3\n");
    HAL_Delay(1000);
    _interface->WriteReg(MPU_USER_CTRL_REG, 0x10); //屏蔽IIC接口
    HAL_Delay(1000);
    _interface->WriteReg(MPU_PWR_MGMT2_REG, 0X00); //加速度与陀螺仪都工作
    //UsbPrintf("WRITE4\n");
    HAL_Delay(1000);
    _interface->WriteReg(MPU_SAMPLE_RATE_REG, GODR_1000Hz); //设置采样率1000Hz
    //UsbPrintf("WRITE5\n");
    HAL_Delay(1000);
    _interface->WriteReg(MPU_CFG_REG, _GetLpfValue(GODR_1000Hz)); //设置低通滤波器
    HAL_Delay(1000);
    _interface->WriteReg(MPU_GYRO_CFG_REG, GFS_2000DPS); //陀螺仪传感器量程
    HAL_Delay(1000);
    _interface->WriteReg(MPU_ACCEL_CFG_REG, AFS_16G); //加速度传感器量程
    HAL_Delay(1000);
    _gyro_sensitivity = 1.0f / (_GetGyroLsb(GFS_2000DPS));

    _acc_sensitivity = 1.0f / (_GetAccLsb(AFS_16G)) * 1000.0f;

    _acc_lsb = _GetAccLsb(AFS_16G);
    _interface->WriteReg(MPU_INTBP_CFG_REG, 0x90);
    HAL_Delay(1000);
    _interface->WriteReg(MPU_INT_EN_REG, DATA_RDY_EN); //使能数据就绪中断

    //UsbPrintf("IMU INIT SUCCESS\n");
}

uint8_t Mpu6X00::GetId()
{
    uint8_t id;
    //_interface->ReadBytes(MPU_DEVICE_ID_REG, 1, &id);
    _interface->ReadBytes(MPU_ACCEL_CFG_REG, 1, &id);
    
    return id;
}

float Mpu6X00::GetTemperature()
{
    uint8_t buf[2];
    short raw;
    float temp;
    _interface->ReadBytes(MPU_TEMP_OUTH_REG, 2, buf);
    raw = (buf[0] << 8) | buf[1];
    temp = (36.53 + ((double)raw) / 340) * 100;
    return temp / 100.0f;
}

/**
 * @description: Get gyroscope data
 * @param {float} *gx
 * @param {float} *gy
 * @param {float} *gz
 * @return {*}
 */
void Mpu6X00::GetGyroData(ImuData &data)
{
    uint8_t buf[6];
    _interface->ReadBytes(MPU_GYRO_XOUTH_REG, 6, buf);
    uint16_t gx_raw, gy_raw, gz_raw;
    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    //UsbPrintf("gyro %d %d %d\n", gx_raw, gy_raw, gz_raw);
    data.gyr.x = (float)((int16_t)(gx_raw)-_bias_gyro_x) * _gyro_sensitivity;
    data.gyr.y = (float)((int16_t)(gy_raw)-_bias_gyro_y) * _gyro_sensitivity;
    data.gyr.z = (float)((int16_t)(gz_raw)-_bias_gyro_z) * _gyro_sensitivity;
    //UsbPrintf("gyro %d %d %d\n", (uint16_t)data.gyr.x, (uint16_t)data.gyr.y, (uint16_t)data.gyr.z);
}

/**
 * @description: Get accelerometer data
 * @param {float} *ax
 * @param {float} *ay
 * @param {float} *az
 * @return {*}
 */
void Mpu6X00::GetAccData(ImuData &data)
{
    uint8_t buf[6];
    _interface->ReadBytes(MPU_ACCEL_XOUTH_REG, 6, buf);
    uint16_t ax_raw, ay_raw, az_raw;

    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    data.acc.x = (float)((int16_t)(ax_raw) - _bias_acc_x) * _acc_sensitivity;
	data.acc.y = (float)((int16_t)(ay_raw) - _bias_acc_y) * _acc_sensitivity;
	data.acc.z = (float)((int16_t)(az_raw) - _bias_acc_z) * _acc_sensitivity;
}



uint8_t Mpu6X00::_GetLpfValue(const uint8_t frq)
{
    uint8_t lpf = 0x01;
    switch (frq)
    {
    case GODR_1000Hz:
        lpf = 0x01;
        break;
    case GODR_500Hz:
        lpf = 0x01;
        break;
    case GODR_100Hz:
        lpf = 0x02;
        break;
    default:
        break;
    }
    return lpf;
}

float Mpu6X00::_GetAccLsb(uint8_t afs)
{
    float lsb = 4096.0f;
    switch (afs)
    {
    case AFS_2G:
        lsb = 16384.0f;
        break;

    case AFS_4G:
        lsb = 8192.0f;
        break;

    case AFS_8G:
        lsb = 4096.0f;
        break;

    case AFS_16G:
        lsb = 2048.0f;
        break;

    default:
        break;
    }
    return lsb;
}

float Mpu6X00::_GetGyroLsb(uint8_t afs)
{
    float lsb = 16.4f;
    switch (afs)
    {
    case GFS_250DPS:
        lsb = 131.0f;
        break;

    case GFS_500DPS:
        lsb = 65.5f;
        break;

    case GFS_1000DPS:
        lsb = 32.8f;
        break;

    case GFS_2000DPS:
        lsb = 16.4f;
        break;

    default:
        break;
    }
    return lsb;
}

void Mpu6X00::_GetGyroRawData(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[6];
    _interface->ReadBytes(MPU_GYRO_XOUTH_REG, 6, buf);
    uint16_t gx_raw, gy_raw, gz_raw;

    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *gx = (int16_t)(gx_raw);
	*gy = (int16_t)(gy_raw);
	*gz = (int16_t)(gz_raw);
}
void Mpu6X00::_GetAccRawData(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    _interface->ReadBytes(MPU_ACCEL_XOUTH_REG, 6, buf);
    uint16_t ax_raw, ay_raw, az_raw;

    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *ax = (int16_t)(ax_raw);
	*ay = (int16_t)(ay_raw);
	*az = (int16_t)(az_raw);
}