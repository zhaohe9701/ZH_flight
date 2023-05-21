#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::TransmitDataTask(void *argument)
{
    for (;;)
    {
        message_transmit_server->RunTransmitService();
    }
}