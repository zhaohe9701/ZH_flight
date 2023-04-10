/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-08 23:40:52
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 00:01:36
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */

#include <stdio.h>
#include "Command/command.h"
#include "ac_queue.h"
#include "ac_semaphore.h"
#include "aircraft_state.h"
#include "baro.h"
#include "icm20689.h"
#include "communicate_interface.h"
#include "message.h"
#include "message_parser.h"
#include "ms5611.h"
#include "imu.h"
#include "main.h"
#include "aircraft_task.h"
#include "config.h"
#include "cmsis_os2.h"
#include "aircraft.h"
#include "global_var.h"
#include "print.h"
#include "sensor_interface.h"
#include "state_machine.h"
#include "event_server.h"
#include "message_server.h"
#include "ibus.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "communicate_interface.h"
#include "z_iic.h"
#include "z_spi.h"
#include "z_usb.h"

/****************对外暴露任务接口****************/
extern "C" void ImuTaskInterface(void *argument);
extern "C" void BaroTaskInterface(void *argument);
extern "C" void StartTaskInterface(void *argument);
extern "C" void AttitudeSolveTaskInterface(void *argument);
extern "C" void ControlTaskInterface(void *argument);
extern "C" void StateMachineTaskInterface(void *argument);
extern "C" void LightTaskInterface(void *argument);
extern "C" void ReceiveDataTaskInterface(void *argument);
extern "C" void TransmitDataTaskInterface(void *argument);
extern "C" void TestTaskInterface(void *argument); 
/*********************************************/
void StartTaskInterface(void *argument)
{
    DynamicTask::StartTask();
}

void ControlTaskInterface(void *argument)
{
    StaticTask::ControlTask();
}
void StateMachineTaskInterface(void *argument)
{
    StaticTask::StateMachineTask();
}
void LightTaskInterface(void *argument)
{
    StaticTask::LightTask();
}
void ReceiveDataTaskInterface(void *argument)
{
    StaticTask::ReceiceDataTask();
}

void TestTaskInterface(void *argument)
{
    StaticTask::TestTask();
}

osThreadId_t ledTaskHandle;
osThreadId_t testTaskHandle;
osThreadId_t transmitDataTaskHandle;
osThreadId_t receiveDataTaskHandle;

AcSemaphore *imu_sem = nullptr;
Aircraft *aircraft = nullptr;
StateMachine *state_machine = nullptr;
EventServer *event_server = nullptr;
MessageReceiveServer *message_receive_server = nullptr;
MessageTransmitServer *message_transmit_server = nullptr;
extern GlobalVar system_var;
extern Thread stateMachineTaskHandle;

void StaticTask::ControlTask(void)
{
    for (;;)
    {
        AcWaitSemaphore(system_var.ACTUAL_STATE_SEMAPHORE);
        /*获得当前期望状态与实际状态*/
        //aircraft->GetStateForControl();
        /*高度（油门）控制*/
        //aircraft->ControlAltitudeByDirect();
        //aircraft->ControlAltitudeBySensor();
        /*姿态控制*/
        aircraft->ControlAttitude();
    }
}

void StaticTask::StateMachineTask(void)
{
    Condition condition;
    ActionGroup action;

    for (;;)
    {
        /*等待事件触发*/
        AcSignalWait(STATE_MACHINE_SIGNAL);
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
        previous_wake_time = osKernelGetTickCount();
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelayUntil(previous_wake_time + LIGHT_CONTROL_DELAY_TIME);
    }
}

void StaticTask::ReceiceDataTask(void)
{
    for (;;)
    {
        message_receive_server->RunReceiveService();
    }
}

void StaticTask::TestTask(void)
{
    for(;;)
    {
        aircraft->Test();
        osDelay(1);
    }
}

void DynamicTask::StartTask(void)
{
    imu_sem = new AcSemaphore();
    /*创建事件服务器*/
    // event_server = new EventServer();
    // event_server->SetInformThread(stateMachineTaskHandle, STATE_MACHINE_SIGNAL);
    /*创建消息接收服务器*/
    AcQueue<Message> *receive_queue = new AcQueue<Message>(3);
    message_receive_server = new MessageReceiveServer(receive_queue);
    MessageReceiveParser *ibus_parser = new IbusParser();
    message_receive_server->AddParser(ibus_parser);
    MessageReceiveParser *command_parser = new CommandParser();
    message_receive_server->AddParser(command_parser);
    /*创建消息发送服务器*/
    AcQueue<Message> *transmit_queue = new AcQueue<Message>(3);
    message_transmit_server = new MessageTransmitServer(transmit_queue);
    CommunicateInterface *interface = new Usb(0x01);
    message_transmit_server->AddTransmitter(interface);
    /*创建状态机*/
    // state_machine = new StateMachine();
    // state_machine->state[AS_INITIALIZE].AddNeighborState(AS_STANDBY, INIT_OVER_EVENT, NULL_EVENT);
    /*添加状态转换关系*/
    // state_machine->state[AS_STANDBY].AddNeighborState(AS_SETTING, SETTING_EVENT, UNLOCK_EVENT | CALIBRATION_EVENT);
    // state_machine->state[AS_STANDBY].AddNeighborState(AS_CALIBRATION, CALIBRATION_EVENT, UNLOCK_EVENT | SETTING_EVENT);
    // state_machine->state[AS_STANDBY].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT | ZERO_THROTTLE_EVENT, SETTING_EVENT | CALIBRATION_EVENT | AS_ALTITUDE | AS_AUTO);

    // state_machine->state[AS_MANUAL].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    // state_machine->state[AS_MANUAL].AddNeighborState(AS_ALTITUDE, ALTITUDE_EVENT | UNLOCK_EVENT, NULL_EVENT);
    // state_machine->state[AS_MANUAL].AddNeighborState(AS_AUTO, AUTO_EVENT | UNLOCK_EVENT, NULL_EVENT);

    // state_machine->state[AS_ALTITUDE].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    // state_machine->state[AS_ALTITUDE].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT, NULL_EVENT);

    // state_machine->state[AS_AUTO].AddNeighborState(AS_STANDBY, NULL_EVENT, UNLOCK_EVENT);
    // state_machine->state[AS_MANUAL].AddNeighborState(AS_MANUAL, MANUAL_EVENT | UNLOCK_EVENT, NULL_EVENT);

    // state_machine->state[AS_SETTING].AddNeighborState(AS_STANDBY, NULL_EVENT, SETTING_EVENT | UNLOCK_EVENT);
    // state_machine->state[AS_CALIBRATION].AddNeighborState(AS_STANDBY, NULL_EVENT, CALIBRATION_EVENT | UNLOCK_EVENT);
    /*创建飞行器对象*/
    aircraft = new Aircraft();
    /*初始化飞行器*/
    aircraft->Init();
    /*置位初始化完成事件*/
    // event_server->SetEvent(INIT_OVER_EVENT);

    const osThreadAttr_t testTask_attributes = {
    .name = "testTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    const osThreadAttr_t imuTask_attributes = {
    .name = "imuTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    const osThreadAttr_t baroTask_attributes = {
    .name = "baroTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    const osThreadAttr_t magTask_attributes = {
    .name = "magTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityBelowNormal ,
    };
    const osThreadAttr_t attitudeSolveTask_attributes = {
    .name = "attitudeSolveTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    const osThreadAttr_t ledTask_attributes = {
    .name = "ledTask",
    .stack_size = 64 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };

    const osThreadAttr_t transmitDataTask_attributes = {
    .name = "transmitDataTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    const osThreadAttr_t receiveDataTask_attributes = {
    .name = "receiveDataTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t) osPriorityNormal ,
    };
    testTaskHandle = osThreadNew(ImuTaskInterface, NULL, &imuTask_attributes);
    testTaskHandle = osThreadNew(BaroTaskInterface, NULL, &baroTask_attributes);
    // testTaskHandle = osThreadNew(ImuTaskInterface, NULL, &magTask_attributes);
    ledTaskHandle = osThreadNew(AttitudeSolveTaskInterface, NULL, &attitudeSolveTask_attributes);
    testTaskHandle = osThreadNew(TestTaskInterface, NULL, &testTask_attributes);
    ledTaskHandle = osThreadNew(LightTaskInterface, NULL, &ledTask_attributes);
    transmitDataTaskHandle = osThreadNew(TransmitDataTaskInterface, NULL, &transmitDataTask_attributes);
    receiveDataTaskHandle = osThreadNew(ReceiveDataTaskInterface, NULL, &receiveDataTask_attributes);
    // for (;;)
    // {
    //     osDelay(10);
    // }
    osThreadExit();
}
