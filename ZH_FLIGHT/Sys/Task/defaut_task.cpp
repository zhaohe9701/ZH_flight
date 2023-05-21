//
// Created by zhaohe on 2023/5/21.
//

#include "aircraft_task.h"
#include "ac_thread.h"

/****************对外暴露任务接口****************/
extern "C" void StartDefaultTask(void *argument);
/*********************************************/

void StartDefaultTask(void *argument)
{
    AircraftTask::DefaultTask(nullptr);
}


void AircraftTask::DefaultTask(void *argument)
{
    AcThread start_thread;
    start_thread.Init(AircraftTask::StartTask, "startTask", 512, 24);
    for (;;)
    {
        osDelay(10);
    }
}