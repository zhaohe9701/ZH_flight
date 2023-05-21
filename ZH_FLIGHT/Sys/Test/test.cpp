/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-05 02:20:44
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-26 01:27:31
 * @FilePath: \ZH_FLIGHT\Sys\Test\test.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "test.h"
#include "cmsis_os2.h"
#include "main.h"

#include "sensor_interface.h"
#include "z_spi.h"
#include "imu.h"

#include "icm20602.h"
#include "baro.h"
#include "z_iic.h"
#include "ms5611.h"

#include "motor_interface.h"
#include "motor.h"
#include "dshot.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern osSemaphoreId SensorSemaphoreHandle;
extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef MOTOR_1_TIM;
extern TIM_HandleTypeDef MOTOR_3_TIM;

Imu *imu;
Baro *baro;
Motor *motor;

extern "C"
{

void test()
{
    SensorInterface *sensor_interface = new Spi(&hspi1, GYRO_CS_GPIO_Port, GYRO_CS_Pin);
    Imu *imu = new Icm20602(sensor_interface);
    uint8_t id1 = imu->GetId();
    HAL_Delay(10);
    imu->Init();
    uint8_t id2 = imu->GetId();
    UsbPrintf("ID:0x%x 0x%x\n", id1, id2);
}

}

extern "C"
{

void InitializeFight()
{
    SensorInterface *imu_interface = new Spi(&hspi1, GYRO_CS_GPIO_Port, GYRO_CS_Pin);
    imu = new Icm20602(imu_interface);
    imu->Init();
    SensorInterface* baro_interface = new Iic(&hi2c1, 0xEE);
    baro = new Ms5611(baro_interface);
    //baro->Init();
    MotorInterface *motor_interface = new Dshot();
    motor = new Motor(motor_interface);
    //motor->EnableMotor();
}

}

extern "C"
{

void ReadImu()
{
    for(;;)
    {
        ImuData data;
        osSemaphoreWait(SensorSemaphoreHandle, osWaitForever);
        //UsbPrintf("ID\r\n");
        //uint8_t id = imu->GetId();
        //UsbPrintf("ID:0x%x\n", id);
        //int temp = 0;

        //temp = imu->GetTemperature();
        imu->GetGyroData(data);
        //UsbPrintf("gyro %d %d %d\r\n", (int)data.gyr.x, (int)data.gyr.y, (int)data.gyr.z);
        //UsbPrintf("gyro:%d\n", data.gyr.x);
        //osDelay(1);
    }
    
}

}

extern "C"
{

void ReadBaro()
{
    for(;;)
    {
        float altitude = 0.0f;
        altitude = baro->GetAltitude();
        UsbPrintf("%d\r\n", (int)altitude);
    }
    
}

}


extern "C"
{

void FlightControl()
{
    for(;;)
    {
        float value[4] = {10.0f, 20.0f, 30.0f, 40.0f};

        motor->SetSpeed(value);
        

        osDelay(10);
    }
    
}

}