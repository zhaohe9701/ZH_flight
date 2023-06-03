#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::transmitDataTask(void *argument)
{
    for (;;)
    {
        message_transmit_server->runTransmitService();
    }
}