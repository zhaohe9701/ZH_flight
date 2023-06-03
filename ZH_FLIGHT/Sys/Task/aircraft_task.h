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
    static void imuTask(void *argument);
    static void baroTask(void *argument);
    static void attitudeSolveTask(void *argument);
    static void controlTask(void *argument);
    static void stateMachineTask(void *argument);
    static void lightTask(void *argument);
    static void receiveDataTask(void *argument);
    static void transmitDataTask(void *argument);
    static void commandTask(void *argument);
    static void testTask(void *argument);
    static void startTask(void *argument);
    static void defaultTask(void *argument);
};

#define ATTITUDE_SOLVE_DELAY_TIME (1000 / ATTITUDE_SOLVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)
#define BARO_UPDATE_DELAY_TIME (1000 / BARO_UPDATE_CYCLE_FREQ)
#endif
