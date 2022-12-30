/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-30 00:43:18
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>
#include "type.h"

#define EVENT_NUM       6
#define STATE_NUM       7
#define EVENT_ON        1
#define EVENT_OFF       0
#define EVENT_IGNORE    2

/*状态集合定义*/
enum StateList
{
    INITIALIZE_STATE = 0,
    STANDBY_STATE,
    WAIT_STATE,
    PARAM_SERVICE_STATE,
    CALIBRATION_STATE,
    MANUAL_STATE,
    ALTITUDE_STATE,
    AUTO_STATE,
};
 
 /*动作集合定义*/
 enum ActionList
{
    INITIALIZE_ACTION    = 0b00000001,
    STANDBY_ACTION       = 0b00000010,
    WAIT_ACTION          = 0b00000100,
    PARAM_SERVICE_ACTION = 0b00001000,
    CALIBRATION_ACTION   = 0b00010000,
    MANUAL_ACTION        = 0b00100000,
    ALTITUDE_ACTION      = 0b01000000,
    AUTO_ACTION          = 0b10000000,
};

/*事件集合定义*/
enum EventList
{
    UNLOCK_EVENT,
    SET_EVENT,
    CALIBRATION_EVENT,
    MANUAL_EVENT,
    ALTITUDE_EVENT,
    AUTO_EVENT,
    ZERO_THROTTLE_EVENT,
};

class StateMap
{
public:

private:

};

class StateMachine
{
public:
    AC_RET TransToNextState(ActionList action);
    StateList GetCurrentState();
    AC_RET ExcuteAction();
private:
    StateList _current_state;

};

#endif
