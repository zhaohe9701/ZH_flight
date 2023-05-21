//
// Created by zhaohe on 2023/4/16.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::ReceiveDataTask(void *argument)
{
    for (;;)
    {
        message_receive_server->RunReceiveService();
    }
}
