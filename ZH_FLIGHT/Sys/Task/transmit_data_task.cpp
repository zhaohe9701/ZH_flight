#include "aircraft_task.h"
#include "message_server.h"

extern "C" void TransmitDataTaskInterface(void *argument);

extern MessageTransmitServer *message_transmit_server;

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