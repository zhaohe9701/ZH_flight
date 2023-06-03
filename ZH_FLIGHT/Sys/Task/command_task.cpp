//
// Created by zhaohe on 2023/4/16.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::commandTask(void *argument)
{
    for (;;)
    {
        command_server->runCommandService();
    }
}
