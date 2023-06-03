#include "aircraft_task.h"
#include "aircraft.h"
#include "sys.h"

void AircraftTask::attitudeSolveTask(void *argument)
{
    uint32_t previous_wake_time = 0;
    for (;;)
    {
        previous_wake_time = osKernelGetTickCount();
        /*姿态更新*/
        aircraft->updateAttitude();
        osDelayUntil(previous_wake_time + ATTITUDE_SOLVE_DELAY_TIME);
    }
}