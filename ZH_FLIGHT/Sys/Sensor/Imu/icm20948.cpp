/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-14 23:14:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 22:48:02
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\icm20948.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "icm20948.h"
#include "main.h"

#define READ 0x80
#define WRITE 0x7f

/**********REGISTER MAP*************/
#define REG_BANK_SEL					0x7F
#define BANK0                           0 << 4
#define BANK1                           1 << 4
#define BANK2                           2 << 4
#define BANK3                           3 << 4
// USER BANK 0
#define WHO_AM_I						0x00		
#define USER_CTRL					    0x03
#define LP_CONFIG					    0x05
#define PWR_MGMT_1					    0x06
#define PWR_MGMT_2					    0x07
#define INT_PIN_CFG					    0x0F		
#define INT_ENABLE					    0x10
#define INT_ENABLE_1					0x11
#define INT_ENABLE_2					0x12
#define INT_ENABLE_3					0x13
#define I2C_MST_STATUS				    0x17		
#define INT_STATUS					    0x19		
#define INT_STATUS_1					0x1A
#define INT_STATUS_2					0x1B
#define INT_STATUS_3					0x1C
#define DELAY_TIMEH					    0x28
#define DELAY_TIMEL					    0x29
#define ACCEL_XOUT_H					0x2D		
#define ACCEL_XOUT_L					0x2E		
#define ACCEL_YOUT_H					0x2F		
#define ACCEL_YOUT_L					0x30		
#define ACCEL_ZOUT_H					0x31		
#define ACCEL_ZOUT_L					0x32	
#define GYRO_XOUT_H					    0x33	
#define GYRO_XOUT_L					    0x34
#define GYRO_YOUT_H					    0x35
#define GYRO_YOUT_L					    0x36
#define GYRO_ZOUT_H					    0x37
#define GYRO_ZOUT_L					    0x38
#define TEMP_OUT_H					    0x39		
#define TEMP_OUT_L					    0x3A
#define EXT_SLV_SENS_DATA_00			0x3B
#define EXT_SLV_SENS_DATA_01			0x3C
#define EXT_SLV_SENS_DATA_02			0x3D
#define EXT_SLV_SENS_DATA_03			0x3E
#define EXT_SLV_SENS_DATA_04			0x3F
#define EXT_SLV_SENS_DATA_05			0x40
#define EXT_SLV_SENS_DATA_06			0x41
#define EXT_SLV_SENS_DATA_07			0x42
#define EXT_SLV_SENS_DATA_08			0x43
#define EXT_SLV_SENS_DATA_09			0x44
#define EXT_SLV_SENS_DATA_10			0x45
#define EXT_SLV_SENS_DATA_11			0x46
#define EXT_SLV_SENS_DATA_12			0x47
#define EXT_SLV_SENS_DATA_13			0x48
#define EXT_SLV_SENS_DATA_14			0x49
#define EXT_SLV_SENS_DATA_15			0x4A
#define EXT_SLV_SENS_DATA_16			0x4B
#define EXT_SLV_SENS_DATA_17			0x4C
#define EXT_SLV_SENS_DATA_18			0x4D
#define EXT_SLV_SENS_DATA_19			0x4E
#define EXT_SLV_SENS_DATA_20			0x4F
#define EXT_SLV_SENS_DATA_21			0x50
#define EXT_SLV_SENS_DATA_22			0x51
#define EXT_SLV_SENS_DATA_23			0x52
#define FIFO_EN_1					    0x66	
#define FIFO_EN_2					    0x67
#define FIFO_RST						0x68
#define FIFO_MODE					    0x69
#define FIFO_COUNTH					    0X70
#define FIFO_COUNTL					    0X71
#define FIFO_R_W						0x72
#define DATA_RDY_STATUS				    0x74
#define FIFO_CFG						0x76	

// USER BANK 1
#define SELF_TEST_X_GYRO				0x02	
#define SELF_TEST_Y_GYRO				0x03
#define SELF_TEST_Z_GYRO				0x04
#define SELF_TEST_X_ACCEL			    0x0E	
#define SELF_TEST_Y_ACCEL			    0x0F
#define SELF_TEST_Z_ACCEL			    0x10
#define XA_OFFS_H					    0x14	
#define XA_OFFS_L					    0x15
#define YA_OFFS_H					    0x17
#define YA_OFFS_L					    0x18
#define ZA_OFFS_H					    0x1A
#define ZA_OFFS_L					    0x1B
#define TIMEBASE_CORRECTION_PLL		    0x28	

// USER BANK 2
#define GYRO_SMPLRT_DIV				    0x00	
#define GYRO_CONFIG_1				    0x01	
#define GYRO_CONFIG_2				    0x02
#define XG_OFFS_USRH					0x03	
#define XG_OFFS_USRL 				    0x04
#define YG_OFFS_USRH					0x05
#define YG_OFFS_USRL					0x06
#define ZG_OFFS_USRH					0x07
#define ZG_OFFS_USRL					0x08
#define ODR_ALIGN_EN					0x09	
#define ACCEL_SMPLRT_DIV_1			    0x10	
#define ACCEL_SMPLRT_DIV_2			    0x11		
#define ACCEL_INTEL_CTRL				0x12		
#define ACCEL_WOM_THR				    0x13
#define ACCEL_CONFIG					0x14
#define ACCEL_CONFIG_2				    0x15
#define FSYNC_CONFIG					0x52
#define TEMP_CONFIG					    0x53
#define MOD_CTRL_USR					0X54

// USER BANK 3
#define I2C_MST_ODR_CONFIG			    0x00
#define I2C_MST_CTRL					0x01
#define I2C_MST_DELAY_CTRL			    0x02	
#define I2C_SLV0_ADDR				    0x03
#define I2C_SLV0_REG					0x04		
#define I2C_SLV0_CTRL				    0x05
#define I2C_SLV0_DO					    0x06
#define I2C_SLV1_ADDR				    0x07		
#define I2C_SLV1_REG					0x08		
#define I2C_SLV1_CTRL				    0x09
#define I2C_SLV1_DO					    0x0A
#define I2C_SLV2_ADDR				    0x0B		
#define I2C_SLV2_REG					0x0C		
#define I2C_SLV2_CTRL				    0x0D
#define I2C_SLV2_DO					    0x0E
#define I2C_SLV3_ADDR				    0x0F		
#define I2C_SLV3_REG					0x10		
#define I2C_SLV3_CTRL				    0x11
#define I2C_SLV3_DO					    0x12
#define I2C_SLV4_ADDR				    0x13	
#define I2C_SLV4_REG					0x14		
#define I2C_SLV4_CTRL				    0x15
#define I2C_SLV4_DO					    0x16
#define I2C_SLV4_DI					    0x17
	

/* AK09916 Registers */
#define AK09916_ID						0x09
#define MAG_SLAVE_ADDR                  0x0C

#define MAG_WIA2						0x01
#define MAG_ST1							0x10
#define MAG_HXL							0x11
#define MAG_HXH							0x12
#define MAG_HYL							0x13
#define MAG_HYH							0x14
#define MAG_HZL							0x15
#define MAG_HZH							0x16
#define MAG_ST2							0x18
#define MAG_CNTL2						0x31
#define MAG_CNTL3						0x32
#define MAG_TS1							0x33
#define MAG_TS2							0x34
/***********************************/


void Icm20948::_ImuWriteRag(uint8_t bank, uint8_t address, uint8_t value)
{
    if (bank != BANK0)
    {
        _interface->WriteReg(REG_BANK_SEL & WRITE, bank);
        _interface->WriteReg(address & WRITE, value);
        _interface->WriteReg(REG_BANK_SEL & WRITE, BANK0);

    }
    else 
    {
        _interface->WriteReg(address & WRITE, value);
    }
}   

void Icm20948::_ImuReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf)
{
    if (bank != _last_bank)
    {
        _interface->WriteReg(REG_BANK_SEL & WRITE, bank);
        _interface->ReadBytes(address | READ, length, buf);
        _interface->WriteReg(REG_BANK_SEL & WRITE, BANK0);
    }
    else 
    {
        _interface->ReadBytes(address | READ, length, buf);
    }
}

Icm20948::Icm20948(SensorInterface *interface)
{
    _interface = interface;
}
void Icm20948::Init()
{
    /*复位芯片*/
    _ImuWriteRag(BANK0, PWR_MGMT_1, 0x80);
    HAL_Delay(200);
    /*选择时钟源,唤醒芯片*/
    _ImuWriteRag(BANK0, PWR_MGMT_1, 0x01);
    HAL_Delay(50);
    /*禁用iic*/
    _ImuWriteRag(BANK0, USER_CTRL, 0x10);
    HAL_Delay(50);
    /*配置陀螺仪*/
    _ImuWriteRag(BANK2, GYRO_CONFIG_1, 0x07);
    HAL_Delay(50);
    _ImuWriteRag(BANK2, GYRO_SMPLRT_DIV, 0x00);
    HAL_Delay(50);
    /*配置加速度计*/
    _ImuWriteRag(BANK2, ACCEL_CONFIG, 0x07);
    HAL_Delay(50);
    _ImuWriteRag(BANK2, ACCEL_SMPLRT_DIV_1, 0x00);
    HAL_Delay(50);
    _ImuWriteRag(BANK2, ACCEL_SMPLRT_DIV_2, 0x00);
    HAL_Delay(50);
    /*设置中断低电平脉冲*/
    _ImuWriteRag(BANK0, INT_PIN_CFG, 0x10);
    HAL_Delay(50);
    /*使能数据就绪中断*/
    _ImuWriteRag(BANK0, INT_ENABLE, 0x01);
    HAL_Delay(50);
    /*使能陀螺仪加速度计*/
    _ImuWriteRag(BANK0, PWR_MGMT_2, 0x00);
    HAL_Delay(50);

    _gyro_sensitivity = 0.061035;
    _acc_sensitivity = 0.48828;

}

uint8_t Icm20948::GetId()
{
    uint8_t id = 0;
    _ImuReadRag(BANK0, WHO_AM_I, 1, &id);
    return id;
}

float Icm20948::GetTemperature()
{
    uint8_t buf[2];
    int16_t raw;
    float temp;
    _ImuReadRag(BANK0, TEMP_OUT_H, 2, buf);
    raw = ((int16_t)buf[0] << 8) | buf[1];
    temp = 25.0f + (float)raw / 326.8f;
    return temp;
}

void Icm20948::GetGyroData(ImuData &data)
{
    uint8_t buf[6];
    uint16_t gx_raw, gy_raw, gz_raw;

    _ImuReadRag(BANK0, GYRO_XOUT_H, 6, buf);
    
    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    data.gyr.x = (float)((int16_t)(gx_raw)-_bias_gyro_x) * _gyro_sensitivity;
    data.gyr.y = (float)((int16_t)(gy_raw)-_bias_gyro_y) * _gyro_sensitivity;
    data.gyr.z = (float)((int16_t)(gz_raw)-_bias_gyro_z) * _gyro_sensitivity;
}

void Icm20948::GetAccData(ImuData &data)
{
    uint8_t buf[6];
    uint16_t ax_raw, ay_raw, az_raw;

    _ImuReadRag(BANK0, ACCEL_XOUT_H, 6, buf);
    
    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    data.acc.x = (float)((int16_t)(ax_raw) - _bias_acc_x) * _acc_sensitivity;
	data.acc.y = (float)((int16_t)(ay_raw) - _bias_acc_y) * _acc_sensitivity;
	data.acc.z = (float)((int16_t)(az_raw) - _bias_acc_z) * _acc_sensitivity;
}