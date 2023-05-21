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
#include <string.h>

AC_RET StateMachine::TransToNextState(Condition condition)
{
    _current_state = state[_current_state].GetNextState(condition);
    if (_current_state == AS_ERROR)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

StateGroup StateMachine::GetCurrentState()
{
    return _current_state;
}

ActionGroup StateMachine::GetAction()
{
    return (ActionGroup)_current_state;
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

StateGroup StateMap::GetMatchState()
{
    return _state;
}

void State::AddNeighborState(StateGroup neighbor_state, Condition positive, Condition negative)
{
    StateMap *state = new StateMap();
    state->AddPositiveCondition(positive);
    state->AddNegativeCondition(negative);
    _neighbor.PushBack(state);
}

StateGroup State::GetNextState(Condition condition)
{
    for(AcListNode<StateMap*> *node = _neighbor.Begin(); node != _neighbor.End(); node = node->GetNext())
    {
        if (node->data->IsMatch(condition))
        {
            return node->data->GetMatchState();
        }
    }
    return AS_ERROR;
}

State::~State()
{
    for(AcListNode<StateMap*> *node = _neighbor.Begin(); node != _neighbor.End(); node = node->GetNext())
    {
        delete node->data;
    }
}
