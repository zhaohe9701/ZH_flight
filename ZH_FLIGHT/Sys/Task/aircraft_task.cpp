/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:45:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 03:36:37
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include "baro.h"
#include "icm20689.h"
#include "message_interface.h"
#include "ms5611.h"
#include "imu.h"
#include "main.h"
#include "aircraft_task.h"
#include "config.h"
#include "cmsis_os.h"
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

#include "z_iic.h"
#include "z_spi.h"
#include "z_usb.h"

/****************对外暴露任务接口****************/
extern "C" void AttitudeSolveTaskInterface(void const *argument);
extern "C" void ControlTaskInterface(void const *argument);
extern "C" void StateMachineTaskInterface(void const *argument);
extern "C" void LightTaskInterface(void const *argument);
extern "C" void ReceiveDataTaskInterface(void const *argument);
extern "C" void TransmitDataTaskInterface(void const *argument);
extern "C" void TestTaskInterface(void const *argument); 
/*********************************************/

void AttitudeSolveTaskInterface(void const *argument)
{
    StaticTask::AttitudeSolveTask();
}
void ControlTaskInterface(void const *argument)
{
    StaticTask::ControlTask();
}
void StateMachineTaskInterface(void const *argument)
{
    StaticTask::StateMachineTask();
}
void LightTaskInterface(void const *argument)
{
    StaticTask::LightTask();
}
void ReceiveDataTaskInterface(void const *argument)
{
    StaticTask::ReceiceDataTask();
}
void TransmitDataTaskInterface(void const *argument)
{
    StaticTask::TransmitDataTask();
}
void TestTaskInterface(void const *argument)
{
    StaticTask::TestTask();
}

Aircraft *aircraft = nullptr;
StateMachine *state_machine = nullptr;
EventServer *event_server = nullptr;
MessageReceiveServer *message_receive_server = nullptr;
MessageTransmitServer *message_transmit_server = nullptr;
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
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelayUntil(&previous_wake_time, LIGHT_CONTROL_DELAY_TIME);
    }
}

void StaticTask::ReceiceDataTask(void)
{
    for (;;)
    {
        message_receive_server->RunReceiveService();
    }
}

void StaticTask::TransmitDataTask(void)
{
    // osMessageQDef(TransmitMessageQueue, MESSAGE_QUEUE_MAX_LENGTH, uint32_t);
    // system_var.TRANSMIT_MESSAGE_QUEUE = osMessageCreate(osMessageQ(TransmitMessageQueue), NULL);
    message_transmit_server = new MessageTransmitServer();
    MessageInterface *interface = new Usb();
    interface->Init(128, 0x01);
    message_transmit_server->AddTransmitter(interface);
    for (;;)
    {
        UsbPrintf("OK1\r\n");
        message_transmit_server->RunTransmitService();
    }
}

void StaticTask::TestTask(void)
{
    // osSemaphoreDef(Imu1Semaphore);
    // system_var.IMU1_SEMAPHORE = osSemaphoreCreate(osSemaphore(Imu1Semaphore), 1);

    // SensorInterface *interface = new Spi(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    // Imu *imu = new Icm20689(interface);
    Printer printer;
    printer.SetInterfaceMark(0x01);
    HAL_Delay(10);
    SensorInterface *interface1 = new Iic(&BARO_INTERFACE_OBJ, BARO_ADDRESS);
    Baro *baro = new Ms5611(interface1);

    // imu->Init();
    baro->Init();
    // ImuData imu_data;
    for(;;)
    {
        // osSemaphoreWait(system_var.IMU1_SEMAPHORE, osWaitForever);
        // imu->GetGyroData(imu_data);
        // imu->GetAccData(imu_data);
        // UsbPrintf("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
        float height = 0.0f;
        height = baro->GetAltitude();
        //printer.Print("%d\r\n", (int)height);
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
    osMessageQDef(ReceiveMessageQueue, MESSAGE_QUEUE_MAX_LENGTH, void*);
    system_var.RECEIVE_MESSAGE_QUEUE = osMessageCreate(osMessageQ(ReceiveMessageQueue), NULL);
    osMessageQDef(TransmitMessageQueue, MESSAGE_QUEUE_MAX_LENGTH, void*);
    system_var.TRANSMIT_MESSAGE_QUEUE = osMessageCreate(osMessageQ(TransmitMessageQueue), NULL);
    /*创建事件服务器*/
    event_server = new EventServer();
    event_server->SetInformThread(stateMachineTaskHandle, STATE_MACHINE_SIGNAL);
    /*创建消息服务器*/
    message_receive_server = new MessageReceiveServer();
    MessageReceiveParser *ibus_parser = new IbusParser();
    message_receive_server->SetParser(ibus_parser, 0);
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
