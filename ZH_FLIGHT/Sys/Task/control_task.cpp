//
// Created by zhaohe on 2023/5/20.
//
#include "aircraft_task.h"
#include "sys.h"

void AircraftTask::ControlTask(void *argument)
{
    for (;;)
    {
        /*获得当前期望状态与实际状态*/
        //aircraft->GetStateForControl();
        /*高度（油门）控制*/
        //aircraft->ControlAltitudeByDirect();
        //aircraft->ControlAltitudeBySensor();
        /*姿态控制*/
        aircraft->ControlAttitude();
    }
}