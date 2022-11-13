/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-05 02:20:44
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 19:20:35
 * @FilePath: \H7B0\Sys\Test\test.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "test.h"
#include "sensor_interface.h"
#include "z_spi.h"
#include "main.h"
#include "imu.h"
#include "icm20602.h"
#include "cmsis_os.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern osSemaphoreId SensorSemaphoreHandle;
Imu *imu;

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
    SensorInterface *sensor_interface = new Spi(&hspi1, GYRO_CS_GPIO_Port, GYRO_CS_Pin);
    imu = new Icm20602(sensor_interface);
    imu->Init();
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