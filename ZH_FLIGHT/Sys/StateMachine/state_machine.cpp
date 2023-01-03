/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-11 15:04:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-03 23:55:58
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "state_machine.h"
#include <string.h>

AC_RET StateMachine::TransToNextState(Condition condition)
{
    _current_state = state[_current_state].GetNextState(condition);
    if (_current_state == ERROR_STATE)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

StateList StateMachine::GetCurrentState()
{
    return _current_state;
}

ActionList StateMachine::GetAction()
{
    return (ActionList)_current_state;
}

bool StateMap::IsMatch(Condition trans_condition)
{
    Condition tmp;
    tmp = trans_condition & _positive_condition;
    if (tmp != _positive_condition)
    {
        return false;
    }

    tmp = ~trans_condition;
    tmp = tmp & _negative_condition;
    if (tmp != _negative_condition)
    {
        return false;
    }
    return true;
}

void StateMap::AddPositiveCondition(Condition condition)
{
    _positive_condition = condition;
}

void StateMap::AddNegativeCondition(Condition condition)
{
    _negative_condition = condition;
}

StateList StateMap::GetMatchState()
{
    return _state;
}

State::State()
{
    _neighbor_head.next = &_neighbor_head;
    _neighbor_head.prev = &_neighbor_head;
}

void State::AddNeighborState(StateList neighbor_state, Condition positive, Condition negative)
{
    StateMap *state = new StateMap();
    state->AddPositiveCondition(positive);
    state->AddNegativeCondition(negative);
    state->next = _neighbor_head.next;
    _neighbor_head.next->prev = state;
    _neighbor_head.next = state;
    state->prev = &_neighbor_head;
}

StateList State::GetNextState(Condition condition)
{
    StateMap *state = nullptr;

    state = _neighbor_head.next;
    while (state != &_neighbor_head)
    {
        if (state->IsMatch(condition))
        {
            return state->GetMatchState();
        }
        state = state->next;
    }
    return ERROR_STATE;
}

State::~State()
{
    StateMap *state = nullptr;
    state = _neighbor_head.next;
    while (state != &_neighbor_head)
    {
        StateMap *tmp = state;
        state = state->next;
        delete tmp;
    }
}
