/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:31:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 23:35:45
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_H__
#define __AIRCRAFT_TASK_H__

class StaticTask
{
public:
    static void AttitudeTask(void);
    static void ControlTask(void);
    static void PositionTask(void);
    static void LightTask(void);
    static void DataLinkTask(void);    
};



class DynamicTask
{
    static void StartTask(void);
    static void ResetTask(void);
    static void ParamTask(void);
    static void CalibrateTask(void);
};



#endif
