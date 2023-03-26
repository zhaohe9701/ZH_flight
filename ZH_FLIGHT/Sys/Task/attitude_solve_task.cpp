#include "aircraft_task.h"
#include "aircraft.h"
#include "main.h"

extern Aircraft *aircraft;

extern "C" void AttitudeSolveTaskInterface(void *argument);

void AttitudeSolveTaskInterface(void *argument)
{
    StaticTask::AttitudeSolveTask();
}

void StaticTask::AttitudeSolveTask(void)
{
    uint32_t previous_wake_time = 0;
    for (;;)
    {
        previous_wake_time = osKernelGetTickCount();
        /*姿态更新*/
        aircraft->UpdateAttitude();
        osDelayUntil(previous_wake_time + ATTITUDE_SLOVE_DELAY_TIME);
    }
}