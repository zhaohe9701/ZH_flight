/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 21:11:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:35:47
 * @FilePath: \ZH_FLIGHT\Sys\sys.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __SYS_H__
#define __SYS_H__

#include "aircraft.h"
#include "message_server.h"
#include "ac_semaphore.h"
#include "command_server.h"
#include "event_server.h"

extern Aircraft *aircraft;
extern MessageTransmitServer *message_transmit_server;
extern MessageReceiveServer *message_receive_server;
extern CommandServer *command_server;
extern AcSemaphore *imu_sem;
extern Printer *debug_printer;
extern EventServer *event_server;
extern StateMachine *state_machine;
#endif
