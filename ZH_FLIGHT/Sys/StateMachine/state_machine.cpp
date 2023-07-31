/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-11 15:04:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-24 00:16:04
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "state_machine.h"
#include "ac_list.h"

void EventGroup::set(const EventSet event)
{
    for (int i = 0; i < EVENT_NUM; ++i)
    {
        _event[i] = event[i];
    }
}

void EventGroup::set(Event &event)
{
    _event[event.name] = event.value;
}

bool EventGroup::operator==(EventGroup &events)
{
    for (int i = 0; i < EVENT_NUM; ++i)
    {
        if (_event[i] != events._event[i])
        {
            return false;
        }
    }
    return true;
}

bool EventGroup::operator!=(EventGroup &events)
{
    for (int i = 0; i < EVENT_NUM; ++i)
    {
        if (_event[i] != events._event[i])
        {
            return true;
        }
    }
    return false;
}

State *State::trans(EventGroup &events)
{
    for (AcListNode<ST> *st = _st_list.begin(); st != _st_list.end(); st = st->getNext())
    {
        if (*(st->data.events) == events)
        {
            return st->data.state;
        }
    }
    return nullptr;
}

AC_RET State::addNextState(State *state, EventGroup *events)
{
    if (nullptr == state || nullptr == events)
    {
        return AC_ERROR;
    }
    ST st;
    st.state = state;
    st.events = events;
    _st_list.pushBack(st);
    return AC_OK;
}

void State::setAction(ActionGroup &action)
{
    _action = action;
}

ActionGroup State::getAction()
{
    return _action;
}

StateMachine::StateMachine()
{
    _event_manager = new DataManager<Event>(EVENT_QUEUE_LEN);
    _action_manager = new ActionManager();
}

AC_RET StateMachine::setCurrentState(State *state)
{
    if (nullptr == state)
    {
        return AC_ERROR;
    }
    _current_state = state;
    return AC_OK;
}

AC_RET StateMachine::run()
{
    Event event;
    ActionGroup action = 0;
    State *temp_state = nullptr;

    if (AC_OK != _event_manager->pop(&event))
    {
        return AC_ERROR;
    }
    _current_event.set(event);
    temp_state = _current_state->trans(_current_event);
    if (nullptr == temp_state)
    {
        return AC_ERROR;
    }
    _current_state = temp_state;
    action = _current_state->getAction();
    _action_manager->give(action);
    return AC_OK;
}