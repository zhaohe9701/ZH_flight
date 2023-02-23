/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-24 00:16:26
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>
#include "type.h"
#include "ac_list.h"

#define EVENT_NUM 6
#define STATE_NUM 7

/*状态集合定义*/
enum StateGroup
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
typedef StateGroup ActionGroup;

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
    StateGroup GetMatchState();
    StateMap *next = nullptr;
    StateMap *prev = nullptr;

private:
    StateGroup _state;
    Condition _positive_condition;
    Condition _negative_condition;
};

class State
{
public:
    State(){};
    void AddNeighborState(StateGroup neighbor_state, Condition positive, Condition negative);
    StateGroup GetNextState(Condition _condition);
    ~State();

private:
    AcList<StateMap> _neighbor;
};

class StateMachine
{
public:
    AC_RET TransToNextState(Condition condition);
    StateGroup GetCurrentState();
    ActionGroup GetAction();
    State state[STATE_NUM];
private:
    StateGroup _current_state;
};

#endif
