/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:31:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-23 23:17:30
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_H__
#define __AIRCRAFT_TASK_H__

class StaticTask
{
public:
    static void ImuTask(void);
    static void AttitudeSolveTask(void);
    static void ControlTask(void);
    static void StateMachineTask(void);
    static void LightTask(void);
    static void ReceiceDataTask(void);
    static void TransmitDataTask(void);
    static void TestTask(void); 
};



class DynamicTask
{
public:
    static void StartTask(void);
    static void ResetTask(void);
    static void ParamTask(void);
    static void CalibrateTask(void);
};

#define ATTITUDE_SLOVE_DELAY_TIME (1000 / ATTITUDE_SLOVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)

#endif
