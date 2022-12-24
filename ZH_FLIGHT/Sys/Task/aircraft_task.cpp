/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:45:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-22 23:50:27
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "aircraft_task.h"
#include "config.h"
#include "cmsis_os.h"

#define ATTITUDE_SLOVE_DELAY_TIME (1000 / ATTITUDE_SLOVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)

void StaticTask::AttitudeTask(void)
{
    uint32_t previous_wake_time = 0;

    for(;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, ATTITUDE_SLOVE_DELAY_TIME);
    }
}

void StaticTask::ControlTask(void)
{
    uint32_t previous_wake_time = 0;

    for(;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, ATTITUDE_CONTROL_DELAY_TIME);
    }
}

void StaticTask::PositionTask(void)
{
    uint32_t previous_wake_time = 0;

    for(;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, POSITION_CONTROL_DELAY_TIME);
    }
}

void StaticTask::LightTask(void)
{
    uint32_t previous_wake_time = 0;

    for(;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, LIGHT_CONTROL_DELAY_TIME);
    }
}

void StaticTask::DataLinkTask(void)
{
    uint32_t previous_wake_time = 0;

    for(;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, DATA_LINK_DELAY_TIME);
    }
}
