//
// Created by zhaohe on 2023/4/16.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::receiveDataTask(void *argument)
{
    for (;;)
    {
        message_receive_server->runReceiveService();
    }
}
