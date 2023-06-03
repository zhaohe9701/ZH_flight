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

AC_RET StateMachine::transToNextState(Condition condition)
{
    _current_state = state[_current_state].getNextState(condition);
    if (_current_state == AS_ERROR)
    {
        return AC_ERROR;
    }
    return AC_OK;
}

StateGroup StateMachine::getCurrentState()
{
    return _current_state;
}

ActionGroup StateMachine::getAction()
{
    return (ActionGroup)_current_state;
}

bool StateMap::isMatch(Condition trans_condition)
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

void StateMap::addPositiveCondition(Condition condition)
{
    _positive_condition = condition;
}

void StateMap::addNegativeCondition(Condition condition)
{
    _negative_condition = condition;
}

StateGroup StateMap::getMatchState()
{
    return _state;
}

void State::addNeighborState(StateGroup neighbor_state, Condition positive, Condition negative)
{
    StateMap *state = new StateMap();
    state->addPositiveCondition(positive);
    state->addNegativeCondition(negative);
    _neighbor.pushBack(state);
}

StateGroup State::getNextState(Condition _condition)
{
    for(AcListNode<StateMap*> *node = _neighbor.begin(); node != _neighbor.end(); node = node->getNext())
    {
        if (node->data->isMatch(_condition))
        {
            return node->data->getMatchState();
        }
    }
    return AS_ERROR;
}

State::~State()
{
    for(AcListNode<StateMap*> *node = _neighbor.begin(); node != _neighbor.end(); node = node->getNext())
    {
        delete node->data;
    }
}
