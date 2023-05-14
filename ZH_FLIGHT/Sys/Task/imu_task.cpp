/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-21 23:33:51
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 22:58:33
 * @FilePath: \ZH_FLIGHT\Sys\Task\imu_task.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "aircraft.h"
#include "aircraft_task.h"
#include "cmsis_os2.h"
#include "main.h"
#include "sys.h"

extern "C" void ImuTaskInterface(void *argument);

void ImuTaskInterface(void *argument)
{
    StaticTask::ImuTask();
}

void StaticTask::ImuTask(void)
{
    for (;;)
    {
        imu_sem->Get();
        aircraft->GetAccAndGyro();
        // osDelay(1);
    }
}
    