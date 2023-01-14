/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:45:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-14 23:13:53
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "aircraft_task.h"
#include "config.h"
#include "cmsis_os.h"
#include "aircraft.h"
#include "global_var.h"
#include "state_machine.h"
#include "event_server.h"
#include "message_server.h"
#include "ibus.h"
Aircraft *aircraft = nullptr;
StateMachine *state_machine = nullptr;
EventServer *event_server = nullptr;
MessageServer *message_server = nullptr;

extern GlobalVar system_var;
extern Thread stateMachineTaskHandle;

#define ATTITUDE_SLOVE_DELAY_TIME (1000 / ATTITUDE_SLOVE_CYCLE_FREQ)
#define ATTITUDE_CONTROL_DELAY_TIME (1000 / ATTITUDE_CONTROL_CYCLE_FREQ)
#define POSITION_CONTROL_DELAY_TIME (1000 / POSITION_CONTROL_CYCLE_FREQ)
#define LIGHT_CONTROL_DELAY_TIME (1000 / LIGHT_CONTROL_CYCLE_FREQ)
#define DATA_LINK_DELAY_TIME (1000 / DATA_LINK_CYCLE_FREQ)

void StaticTask::AttitudeSolveTask(void)
{
    for (;;)
    {
        /*等待IMU中断*/
        osSemaphoreWait(system_var.IMU1_SEMAPHORE, osWaitForever);
        /*姿态更新*/
        aircraft->UpdateAttitude();
        osSemaphoreRelease(system_var.ACTUAL_STATE_SEMAPHORE);
    }
}

void StaticTask::ControlTask(void)
{
    for (;;)
    {
        osSemaphoreWait(system_var.ACTUAL_STATE_SEMAPHORE, osWaitForever);
        /*获得当前期望状态与实际状态*/
        aircraft->GetStateForControl();
        /*高度（油门）控制*/
        aircraft->ControlAltitudeByDirect();
        aircraft->ControlAltitudeBySensor();
        /*姿态控制*/
        aircraft->ControlAttitude();
    }
}

void StaticTask::StateMachineTask(void)
{
    Condition condition;
    ActionList action;

    for (;;)
    {
        /*等待事件触发*/
        SignalWait(STATE_MACHINE_SIGNAL);
        /*获取当前事件组合*/
        condition = event_server->GetCurrentCondition();
        /*状态转移*/
        state_machine->TransToNextState(condition);
        /*获得当前状态要采取的动作*/
        action = state_machine->GetAction();
        /*动作交给飞行器执行*/
        aircraft->SetAction(action);
    }
}

void StaticTask::LightTask(void)
{
    uint32_t previous_wake_time = 0;

    for (;;)
    {
        previous_wake_time = osKernelSysTick();
        osDelayUntil(&previous_wake_time, LIGHT_CONTROL_DELAY_TIME);
    }
}

void StaticTask::ReceiceDataTask(void)
{
    Message *receive_message = nullptr;
    osEvent event;
    for (;;)
    {
        event = osMessageGet(system_var.MESSAGE_QUEUE, osWaitForever);
        if (osEventMessage == event.status)
        {
            receive_message = (Message *)event.value.p;
            message_server->GetMessage(receive_message);
        }
    }
}

void DynamicTask::StartTask(void)
{

    osSemaphoreDef(Imu1Semaphore);
    system_var.IMU1_SEMAPHORE = osSemaphoreCreate(osSemaphore(Imu1Semaphore), 1);
    osSemaphoreDef(ActualStateSemaphore);
    system_var.ACTUAL_STATE_SEMAPHORE = osSemaphoreCreate(osSemaphore(ActualStateSemaphore), 1);
    osMutexDef(ActualStateMutex);
    system_var.ACTUAL_STATE_MUTEX = osMutexCreate(osMutex(ActualStateMutex));
    osMutexDef(ExpectStateMutex);
    system_var.EXPECT_STATE_MUTEX = osMutexCreate(osMutex(ExpectStateMutex));
    osMessageQDef(MessageQueue, MESSAGE_QUEUE_MAX_LENGTH, void*);
    system_var.MESSAGE_QUEUE = osMessageCreate(osMessageQ(MessageQueue), NULL);
    /*创建事件服务器*/
    event_server = new EventServer();
    event_server->SetInformThread(stateMachineTaskHandle, STATE_MACHINE_SIGNAL);
    /*创建消息服务器*/
    message_server = new MessageServer();
    MessageParser *ibus_parser = new IbusParser();
    message_server->SetParser(ibus_parser, 0);
    /*创建状态机*/
    state_machine = new StateMachine();
    state_machine->state[AS_INITIALIZE].AddNeighborState(AS_STANDBY, INIT_OVER_EVENT, NULL_EVENT);
    /*添加状态转换关系*/
    state_machine->state[AS_STANDBY].AddNeighborState(AS_SETTING, SETTING_EVENT, UNLOCK_EVENT | CALIBRATION_EVENT);
    state_machine->state[AS_STANDBY].AddNeighborState(AS_CALIBRATION, CALIBRATION_EVENT, UNLOCK_EVENT | SETTING_EVENT);
    state_machine->state[AS_STANDBY].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT | ZERO_THROTTLE_EVENT, SETTING_EVENT | CALIBRATION_EVENT | AS_ALTITUDE | AS_AUTO);

    state_machine->state[AS_MANUAL].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    state_machine->state[AS_MANUAL].AddNeighborState(AS_ALTITUDE, ALTITUDE_EVENT | UNLOCK_EVENT, NULL_EVENT);
    state_machine->state[AS_MANUAL].AddNeighborState(AS_AUTO, AUTO_EVENT | UNLOCK_EVENT, NULL_EVENT);

    state_machine->state[AS_ALTITUDE].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    state_machine->state[AS_ALTITUDE].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT, NULL_EVENT);

    state_machine->state[AS_AUTO].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    state_machine->state[AS_MANUAL].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT, NULL_EVENT);

    state_machine->state[AS_SETTING].AddNeighborState(AS_STANDBY, NULL_EVENT, SETTING_EVENT | UNLOCK_EVENT);
    state_machine->state[AS_CALIBRATION].AddNeighborState(AS_STANDBY, NULL_EVENT, CALIBRATION_EVENT | UNLOCK_EVENT);
    /*创建飞行器对象*/
    aircraft = new Aircraft();
    /*初始化飞行器*/
    aircraft->Init();
    /*置位初始化完成事件*/
    event_server->SetEvent(INIT_OVER_EVENT);
}
