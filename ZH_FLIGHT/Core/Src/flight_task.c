/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 13:40:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-29 01:34:54
 * @FilePath: \ZH_FLIGHT\Core\Src\flight_task.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "flight_task.h"
#include "main.h"
#include "cmsis_os2.h"

#include "aircraft_task_interface.h"

osThreadId_t startTaskHandle;


const osThreadAttr_t startTask_attributes = {
  .name = "startTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal ,
};



void StartDefaultTask(void *argument)
{
    startTaskHandle = osThreadNew(StartTaskInterface, NULL, &startTask_attributes);

    for (;;)
    {
        osDelay(10);
    }
    /* USER CODE END StartDefaultTask */
}

