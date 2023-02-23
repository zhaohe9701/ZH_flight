/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-17 22:31:44
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-29 01:38:42
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task_interface.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_INTERFACE_H__
#define __AIRCRAFT_TASK_INTERFACE_H__

void StartTaskInterface(void *argument);
void AttitudeSolveTaskInterface(void *argument);
void ControlTaskInterface(void *argument);
void StateMachineTaskInterface(void *argument);
void LightTaskInterface(void *argument);
void ReceiveDataTaskInterface(void *argument);
void TransmitDataTaskInterface(void *argument);
void TestTaskInterface(void *argument); 

#endif