#include "aircraft_task.h"
#include "sys.h"

extern "C" void TransmitDataTaskInterface(void *argument);

void TransmitDataTaskInterface(void *argument)
{
    StaticTask::TransmitDataTask();
}

void StaticTask::TransmitDataTask(void)
{
    for (;;)
    {
        message_transmit_server->RunTransmitService();
    }
}