/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 13:40:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-09 00:01:26
 * @FilePath: \ZH_FLIGHT\Core\Src\flight_task.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "flight_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

osThreadId ledTaskHandle;
osThreadId sensorTaskHandle;
osThreadId flightControlTaskHandle;
osThreadId stateMachineTaskHandle;

void LedTask(void const *argument);
void ImuTask(void const *argument);
void BaroAndMagTask(void const *argument);
void FlightControlTask(void const *argument);

void StartDefaultTask(void const *argument)
{
    //MX_USB_DEVICE_Init();
    osThreadDef(ledTask, LedTask, osPriorityBelowNormal, 0, 128);
    ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

    // osThreadDef(imuTask, ImuTask, osPriorityNormal, 0, 128);
    // sensorTaskHandle = osThreadCreate(osThread(imuTask), NULL);

    // osThreadDef(baroAndMagTask, BaroAndMagTask, osPriorityBelowNormal, 0, 128);
    // sensorTaskHandle = osThreadCreate(osThread(baroAndMagTask), NULL);
    osThreadDef(flightControlTask, FlightControlTask, osPriorityBelowNormal, 0, 128);
    flightControlTaskHandle = osThreadCreate(osThread(flightControlTask), NULL);

    for (;;)
    {
        osDelay(10);
    }
    /* USER CODE END StartDefaultTask */
}

void LedTask(void const *argument)
{
    for (;;)
    {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        osDelay(1000);
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        osDelay(1000);
    }
}

void ImuTask(void const *argument)
{
    //HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

void BaroAndMagTask(void const *argument)
{

}

void FlightControlTask(void const *argument)
{

}