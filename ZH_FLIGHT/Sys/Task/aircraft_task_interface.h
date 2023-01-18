/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-17 22:31:44
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-17 22:36:51
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task_interface.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AIRCRAFT_TASK_INTERFACE_H__
#define __AIRCRAFT_TASK_INTERFACE_H__

void AttitudeSolveTaskInterface(void const *argument);
void ControlTaskInterface(void const *argument);
void StateMachineTaskInterface(void const *argument);
void LightTaskInterface(void const *argument);
void ReceiceDataTaskInterface(void const *argument);
void SendDataInterfaceTask(void const *argument);
void TestTaskInterface(void const *argument); 

#endif