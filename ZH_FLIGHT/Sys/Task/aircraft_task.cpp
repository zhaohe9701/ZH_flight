/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:45:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-28 23:26:39
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "aircraft_task.h"
#include "config.h"
#include "cmsis_os.h"
#include "aircraft.h"
#include "global_var.h"

Aircraft *aircraft = nullptr;
extern GlobalVar system_var;


#define ATTITUDE_SLOVE_DELAY_TIME (1000 / ATTITUDE_SLOVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)

void StaticTask::AttitudeSolveTask(void)
{
    for(;;)
    {
        osSemaphoreWait(system_var.IMU1_SEMAPHORE, osWaitForever);
        aircraft->UpdateAttitude();
        osSemaphoreRelease(system_var.ACTUAL_STATE_SEMAPHORE);
    }
}

void StaticTask::ControlTask(void)
{
    for(;;)
    {
        osSemaphoreWait(system_var.ACTUAL_STATE_SEMAPHORE, osWaitForever);
        aircraft->GetStateForControl();
        aircraft->ControlAltitudeByDirect();
        aircraft->ControlAltitudeBySensor();
        aircraft->ControlAttitude();
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

void DynamicTask::StartTask(void)
{

    osSemaphoreDef(Imu1Semaphore);
    system_var.IMU1_SEMAPHORE = osSemaphoreCreate(osSemaphore(Imu1Semaphore), 1);
    osSemaphoreDef(ActualStateSemaphore);
    system_var.ACTUAL_STATE_SEMAPHORE = osSemaphoreCreate(osSemaphore(ActualStateSemaphore), 1);
    osMutexDef(ActualStateMutex);
    system_var.ACTUAL_STATE_MUTEX = osMutexCreate(osMutex(ActualStateMutex));
    osMutexDef(ExpectStateMutex);
    system_var.EXPECT_STATE_MUTEX = osMutexCreate(osMutex(ExpectStateMutex));
    aircraft = new Aircraft();
    aircraft->Init();
}
