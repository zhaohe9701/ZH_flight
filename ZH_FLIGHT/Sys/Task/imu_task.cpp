#include "aircraft.h"
#include "aircraft_task.h"
#include "global_var.h"
#include "main.h"

extern "C" void ImuTaskInterface(void *argument);

extern Aircraft *aircraft;
extern GlobalVar system_var;

void ImuTaskInterface(void *argument)
{
    StaticTask::ImuTask();
}

void StaticTask::ImuTask(void)
{
    for (;;)
    {
        osSemaphoreAcquire(system_var.IMU1_SEMAPHORE, osWaitForever);
        aircraft->GetAccAndGyro();
    }
}
    