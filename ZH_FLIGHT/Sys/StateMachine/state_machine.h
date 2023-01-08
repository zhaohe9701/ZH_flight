/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-08 23:50:16
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>
#include "type.h"

#define EVENT_NUM 6
#define STATE_NUM 8
#define EVENT_ON 1
#define EVENT_OFF 0
#define EVENT_IGNORE 2

/*状态集合定义*/
enum StateList
{
    AS_ERROR = -1,
    AS_INITIALIZE,
    AS_STANDBY,
    AS_SETTING,
    AS_CALIBRATION,
    AS_MANUAL,
    AS_ALTITUDE,
    AS_AUTO,
};

/*动作集合定义*/
typedef StateList ActionList;

/*事件集合定义*/
enum EventList
{
    NULL_EVENT          = 0b00000000,
    INIT_OVER_EVENT     = 0b00000001,
    UNLOCK_EVENT        = 0b00000010,
    SETTING_EVENT       = 0b00000100,
    CALIBRATION_EVENT   = 0b00001000,
    MANUAL_EVENT        = 0b00010000,
    ALTITUDE_EVENT      = 0b00100000,
    AUTO_EVENT          = 0b01000000,
    ZERO_THROTTLE_EVENT = 0b10000000,
};

class StateMap
{
public:
    bool IsMatch(Condition trans_condition);
    void AddPositiveCondition(Condition condition);
    void AddNegativeCondition(Condition condition);
    StateList GetMatchState();
    StateMap *next = nullptr;
    StateMap *prev = nullptr;

private:
    StateList _state;
    Condition _positive_condition;
    Condition _negative_condition;
};

class State
{
public:
    State();
    void AddNeighborState(StateList neighbor_state, Condition positive, Condition negative);
    StateList GetNextState(Condition _condition);
    ~State();

private:
    StateMap _neighbor_head;
};

class StateMachine
{
public:
    AC_RET TransToNextState(Condition condition);
    StateList GetCurrentState();
    ActionList GetAction();
    State state[STATE_NUM];
private:
    StateList _current_state;
};

#endif
