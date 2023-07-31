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
#include "ac_action.h"
#include "config.h"
#include "data_manager.h"

typedef uint8_t EventSet[EVENT_NUM];

struct Event
{
    uint16_t name = 0;
    uint8_t value = 0;
};

class EventGroup
{
public:
    void set(const EventSet event);
    void set(Event &event);
    bool operator==(EventGroup &events);
    bool operator!=(EventGroup &events);
private:
    EventSet _event = {0};
};

class State
{
    struct ST
    {
        State* state = nullptr;
        EventGroup* events = nullptr;
    };
public:
    State *trans(EventGroup &events);
    AC_RET addNextState(State *state, EventGroup *events);
    void setAction(ActionGroup &action);
    ActionGroup getAction();
private:
    AcList<ST> _st_list;
    ActionGroup _action;
};

class StateMachine
{
public:
    StateMachine();
    AC_RET setCurrentState(State *state);
    AC_RET run();
private:
    State *_current_state = nullptr;
    EventGroup _current_event;
    DataManager<Event> *_event_manager = nullptr;
    ActionManager *_action_manager = nullptr;
};

#endif
