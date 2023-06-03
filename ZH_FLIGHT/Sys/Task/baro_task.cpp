/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-21 23:33:51
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:23:39
 * @FilePath: \ZH_FLIGHT\Sys\Task\baro_task.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "aircraft.h"
#include "aircraft_task.h"
#include "cmsis_os2.h"
#include "sys.h"


void AircraftTask::baroTask(void *argument)
{
    uint32_t previous_wake_time = 0;
    for (;;)
    {
        previous_wake_time = osKernelGetTickCount();
        aircraft->getAltitude();
        osDelayUntil(previous_wake_time + BARO_UPDATE_DELAY_TIME);
    }
}
    