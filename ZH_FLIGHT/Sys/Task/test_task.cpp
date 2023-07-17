//
// Created by zhaohe on 2023/5/20.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::testTask(void *argument)
{
    for(;;)
    {
        aircraft->test();
        osDelay(1000);
    }
}
