//
// Created by zhaohe on 2023/4/16.
//
#include "aircraft_task.h"
#include "sys.h"

extern "C" void ReceiveDataTaskInterface(void *argument);

void ReceiveDataTaskInterface(void *argument)
{
    StaticTask::ReceiveDataTask();
}

void StaticTask::ReceiveDataTask(void)
{
    for (;;)
    {
        message_receive_server->RunReceiveService();
    }
}
