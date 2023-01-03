/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-03 23:51:20
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
    ERROR_STATE = -1,
    INITIALIZE_STATE,
    STANDBY_STATE,
    WAIT_STATE,
    PARAM_SERVICE_STATE,
    CALIBRATION_STATE,
    MANUAL_STATE,
    ALTITUDE_STATE,
    AUTO_STATE,
};

/*动作集合定义*/
typedef StateList ActionList;

/*事件集合定义*/
enum EventList
{
    UNLOCK_EVENT = 0b00000001,
    SET_EVENT = 0b00000010,
    CALIBRATION_EVENT = 0b00000100,
    MANUAL_EVENT = 0b00001000,
    ALTITUDE_EVENT = 0b00010000,
    AUTO_EVENT = 0b00100000,
    ZERO_THROTTLE_EVENT = 0b01000000,
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
