//
// Created by zhaohe on 2023/5/20.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::TestTask(void *argument)
{
    for(;;)
    {
        aircraft->Test();
        osDelay(10);
    }
}
