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
#include "print.h"
#include "sensor_interface.h"
#include "state_machine.h"
#include "event_server.h"
#include "message_server.h"
#include "ibus.h"
#include "communicate_interface.h"
#include "z_iic.h"
#include "z_spi.h"
#include "z_usb.h"
#include "command_server.h"
#include "ac_thread.h"



ThreadManager thread_manager;

AcSemaphore *imu_sem = nullptr;
Aircraft *aircraft = nullptr;
StateMachine *state_machine = nullptr;
EventServer *event_server = nullptr;
MessageReceiveServer *message_receive_server = nullptr;
MessageTransmitServer *message_transmit_server = nullptr;
CommandServer *command_server = nullptr;
Printer *debug_printer = nullptr;

void AircraftTask::startTask(void *argument)
{
    imu_sem = new AcSemaphore();
    /*创建事件服务器*/
    // event_server = new EventServer();
    // event_server->SetInformThread(stateMachineTaskHandle, STATE_MACHINE_SIGNAL);

    /*创建消息接收服务器*/
    message_receive_server = new MessageReceiveServer();
    MessageReceiveParser *ibus_parser = new IbusParser();
    message_receive_server->addParser(ibus_parser);
    MessageReceiveParser *command_parser = new CommandParser();
    message_receive_server->addParser(command_parser);

    /*创建消息发送服务器*/
    message_transmit_server = new MessageTransmitServer();
    CommunicateInterface *interface = new Usb(0x01);
    message_transmit_server->addTransmitter(interface);

    /*创建命令服务器*/
    command_server = new CommandServer();
    command_parser->setDataManager(command_server->getManager());

    debug_printer = new Printer(message_transmit_server->getMessageManager());
    debug_printer->setDecPort(0x01);

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
    aircraft->init();
    /*置位初始化完成事件*/
    // event_server->SetEvent(INIT_OVER_EVENT);

    AcThread light_thread;
    AcThread imu_thread;
    AcThread baro_thread;
    AcThread attitude_solve_thread;
    AcThread test_thread;
    AcThread transmit_data_thread;
    AcThread receive_data_thread;
    AcThread command_thread;
    imu_thread.init(AircraftTask::imuTask, "imuTask", 512, 24);
    baro_thread.init(AircraftTask::baroTask, "baroTask", 256, 24);
    attitude_solve_thread.init(AircraftTask::attitudeSolveTask, "attitudeSolveTask", 256, 24);
    light_thread.init(AircraftTask::lightTask, "lightTask", 256, 24);
    transmit_data_thread.init(AircraftTask::transmitDataTask, "transmitDataTask", 256, 24);
    receive_data_thread.init(AircraftTask::receiveDataTask, "receiveDataTask", 256, 24);
    command_thread.init(AircraftTask::commandTask, "commandTask", 1024, 24);
    test_thread.init(AircraftTask::testTask, "testTask", 512, 24);
    thread_manager.AddThread(light_thread);
    thread_manager.AddThread(imu_thread);
    thread_manager.AddThread(baro_thread);
    thread_manager.AddThread(attitude_solve_thread);
    thread_manager.AddThread(test_thread);
    thread_manager.AddThread(transmit_data_thread);
    thread_manager.AddThread(receive_data_thread);
    thread_manager.AddThread(command_thread);
    AcThread::killSelf();
}
