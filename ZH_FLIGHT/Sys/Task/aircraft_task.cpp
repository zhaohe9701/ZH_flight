/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-12-19 23:45:38
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:17:22
 * @FilePath: \ZH_FLIGHT\Sys\Task\aircraft_task.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include <stdio.h>
#include "aircraft_state.h"
#include "baro.h"
#include "icm20689.h"
#include "communicate_interface.h"
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
void AttitudeSolveTaskInterface(void *argument)
{
    StaticTask::AttitudeSolveTask();
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
void TransmitDataTaskInterface(void *argument)
{
    StaticTask::TransmitDataTask();
}
void TestTaskInterface(void *argument)
{
    StaticTask::TestTask();
}

osThreadId_t ledTaskHandle;
osThreadId_t testTaskHandle;
osThreadId_t transmitDataTaskHandle;

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
        AcWaitSemaphore(system_var.IMU1_SEMAPHORE);
        /*姿态更新*/
        aircraft->UpdateAttitude();
        AcReleaseSemaphore(system_var.ACTUAL_STATE_SEMAPHORE);
    }
}

void StaticTask::ControlTask(void)
{
    for (;;)
    {
        AcWaitSemaphore(system_var.ACTUAL_STATE_SEMAPHORE);
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

// static const osMessageQueueAttr_t transmitDataQueue_attributes = {
//   .name = "transmitDataQueue"
// };
void StaticTask::TransmitDataTask(void)
{
    for (;;)
    {
        message_transmit_server->RunTransmitService();
        //osDelay(1000);
    }
}

void StaticTask::TestTask(void)
{
    // system_var.IMU1_SEMAPHORE = osSemaphoreNew(1, 1, NULL);

    // SensorInterface *interface = new Spi(&IMU1_INTERFACE_OBJ, IMU1_CS_PORT, IMU1_CS_PIN);
    // Imu *imu = new Icm20689(interface);
    Printer printer;
    printer.SetInterfaceMark(0x01);
    SensorInterface *interface1 = new Iic(&BARO_INTERFACE_OBJ, BARO_ADDRESS);
    Baro *baro = new Ms5611(interface1);

    // imu->Init();
    baro->Init();
    // ImuData imu_data;
    for(;;)
    {
        // osSemaphoreAcquire(system_var.IMU1_SEMAPHORE, osWaitForever);
        // imu->GetGyroData(imu_data);
        // imu->GetAccData(imu_data);
        // printer.Print("%d %d %d\r\n", (int)imu_data.acc.x, (int)imu_data.acc.y, (int)imu_data.acc.z);
        float height = 0.0f;
        height = baro->GetAltitude();
        // UsbPrintf("%d\r\n", (int)height);
        printer.Print("%d\r\n", (int)height);
        // printer.Print("OK\r\n");
        // osDelay(10);
    }
}

void DynamicTask::StartTask(void)
{


    system_var.IMU1_SEMAPHORE = osSemaphoreNew(1, 1, NULL);
    system_var.ACTUAL_STATE_SEMAPHORE = osSemaphoreNew(1, 1, NULL);
    system_var.ACTUAL_STATE_MUTEX = osMutexNew(NULL);
    system_var.EXPECT_STATE_MUTEX = osMutexNew(NULL);
    system_var.TRANSMIT_MESSAGE_QUEUE = osMessageQueueNew(3, sizeof(Message), NULL);
    system_var.RECEIVE_MESSAGE_QUEUE = osMessageQueueNew(3, sizeof(Message), NULL);
    /*创建事件服务器*/
    // event_server = new EventServer();
    // event_server->SetInformThread(stateMachineTaskHandle, STATE_MACHINE_SIGNAL);
    /*创建消息接收服务器*/
    // message_receive_server = new MessageReceiveServer();
    // MessageReceiveParser *ibus_parser = new IbusParser();
    // message_receive_server->SetParser(ibus_parser, 0);
    /*创建消息发送服务器*/
    message_transmit_server = new MessageTransmitServer();
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
    // aircraft = new Aircraft();
    /*初始化飞行器*/
    // aircraft->Init();
    /*置位初始化完成事件*/
    // event_server->SetEvent(INIT_OVER_EVENT);

    const osThreadAttr_t testTask_attributes = {
    .name = "testTask",
    .stack_size = 512 * 4,
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
    
    testTaskHandle = osThreadNew(TestTaskInterface, NULL, &testTask_attributes);
    ledTaskHandle = osThreadNew(LightTaskInterface, NULL, &ledTask_attributes);
    transmitDataTaskHandle = osThreadNew(TransmitDataTaskInterface, NULL, &transmitDataTask_attributes);
    // for (;;)
    // {
    //     osDelay(10);
    // }
    osThreadExit();
}
