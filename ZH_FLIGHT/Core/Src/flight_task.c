/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 13:40:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-18 00:43:51
 * @FilePath: \ZH_FLIGHT\Core\Src\flight_task.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "flight_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

#include "aircraft_task_interface.h"

osThreadId ledTaskHandle;
osThreadId sensorTaskHandle;
osThreadId flightControlTaskHandle;
osThreadId stateMachineTaskHandle;



void StartDefaultTask(void const *argument)
{
    //MX_USB_DEVICE_Init();
    osThreadDef(ledTask, LightTaskInterface, osPriorityNormal, 0, 128);
    ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

    osThreadDef(testTask, TestTaskInterface, osPriorityNormal, 0, 128);
    sensorTaskHandle = osThreadCreate(osThread(testTask), NULL);

    // osThreadDef(baroAndMagTask, BaroAndMagTask, osPriorityBelowNormal, 0, 128);
    // sensorTaskHandle = osThreadCreate(osThread(baroAndMagTask), NULL);
    // osThreadDef(flightControlTask, FlightControlTask, osPriorityBelowNormal, 0, 128);
    // flightControlTaskHandle = osThreadCreate(osThread(flightControlTask), NULL);

    for (;;)
    {
        osDelay(10);
    }
    /* USER CODE END StartDefaultTask */
}

