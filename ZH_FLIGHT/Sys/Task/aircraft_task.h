/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:31:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 22:53:43
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_H__
#define __AIRCRAFT_TASK_H__

class AircraftTask
{
public:
    static void ImuTask(void *argument);
    static void BaroTask(void *argument);
    static void AttitudeSolveTask(void *argument);
    static void ControlTask(void *argument);
    static void StateMachineTask(void *argument);
    static void LightTask(void *argument);
    static void ReceiveDataTask(void *argument);
    static void TransmitDataTask(void *argument);
    static void CommandTask(void *argument);
    static void TestTask(void *argument);
    static void StartTask(void *argument);
    static void DefaultTask(void *argument);
};

#define ATTITUDE_SOLVE_DELAY_TIME (1000 / ATTITUDE_SOLVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)
#define BARO_UPDATE_DELAY_TIME (1000 / BARO_UPDATE_CYCLE_FREQ)
#endif
