//
// Created by zhaohe on 2023/4/16.
//
#include "aircraft_task.h"
#include "sys.h"

extern "C" void CommandTaskInterface(void *argument);

void CommandTaskInterface(void *argument)
{
    StaticTask::CommandTask();
}

void StaticTask::CommandTask(void)
{
    for (;;)
    {
        command_server->RunCommandService();
    }
}
