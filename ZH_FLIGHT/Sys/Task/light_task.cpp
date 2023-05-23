//
// Created by zhaohe on 2023/5/20.
//
#include "aircraft_task.h"
#include "sys.h"
#include "Light/light.h"

void AircraftTask::LightTask(void *argument)
{
    uint32_t previous_wake_time = 0;
    for (;;)
    {
        previous_wake_time = osKernelGetTickCount();
        aircraft->ControlLight();
        osDelayUntil(previous_wake_time + LIGHT_CONTROL_DELAY_TIME);
    }
}

