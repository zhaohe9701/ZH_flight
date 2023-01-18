/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:31:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-15 17:32:49
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_H__
#define __AIRCRAFT_TASK_H__

class StaticTask
{
public:
    static void AttitudeSolveTask(void);
    static void ControlTask(void);
    static void StateMachineTask(void);
    static void LightTask(void);
    static void ReceiceDataTask(void);
    static void SendDataTask(void);
    static void TestTask(void); 
};



class DynamicTask
{
    static void StartTask(void);
    static void ResetTask(void);
    static void ParamTask(void);
    static void CalibrateTask(void);
};



#endif
