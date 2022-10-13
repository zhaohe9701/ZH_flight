/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-11 15:04:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-13 22:54:47
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "state_machine.h"
#include <string.h>


RunningState::RunningState(uint8_t my_state)
{
    _my_state = my_state;
}


void RunningState::AddEventStateMap(uint8_t state, uint8_t *event)
{
    memcpy(_map[_reachable_state_num].event, event, EVENT_NUM);
    _map[_reachable_state_num].state = state;
    _reachable_state_num++;
}

void RunningState::AddAction(void (*func)())
{
    _action = func;
}

uint8_t RunningState::GetNextState(uint8_t *event)
{
    for (int i = 0; i < _reachable_state_num; ++i)
    {
        if (_MatchEvent(_map[i].event, event))
        {
            return _map[i].state;
        }
    }
    return _my_state;
}

void RunningState::ExecuteAction()
{
    _action();
}

uint8_t RunningState::GetMyState()
{
    return _my_state;
}



bool RunningState::_MatchEvent(uint8_t *source, uint8_t *event)
{
    for (int i = 0; i < EVENT_NUM; ++i)
    {
        if (source[i] != EVENT_IGNORE)
        {
            if (source[i] != event[i])
            {
                return false;
            }
        }
    }
    return true;
}


void StateMachine::AddState(RunningState *state)
{
    if (_state_index >= STATE_NUM)
    {
        return;
    }
    _state_set[_state_index] = state;
    _state_index++;
}

void StateMachine::SetInitialState(RunningState *state)
{
    _current_state = state;
}

void StateMachine::TransferState(uint8_t *event)
{  
    uint8_t next_state = _current_state->GetNextState(event);
    _current_state = _state_set[next_state];
}

void StateMachine::Run()
{
    _current_state->ExecuteAction();
}