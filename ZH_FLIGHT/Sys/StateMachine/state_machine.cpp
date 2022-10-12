/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-11 15:04:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-12 23:33:22
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "state_machine.h"
#include <string.h>

template <typename T>
RunningState<T>::RunningState(uint8_t my_state)
{
    _my_state = my_state;
}

template <typename T>
void RunningState<T>::SetObject(T *object)
{
    _object = object;
}

template <typename T>
void RunningState<T>::SetEventStateMap(uint8_t state, uint8_t *event)
{
    memcpy(_map[_reachable_state_num].event, event, EVENT_NUM);
    _map[_reachable_state_num].state = state;
    _reachable_state_num++;
}

template <typename T>
void RunningState<T>::SetActionNum(uint8_t num)
{
    _action_num = num;
    _action_set = new void *[num];
}

template <typename T>
void RunningState<T>::AddAction(void (T::*func)(), uint8_t order)
{
    if (order >= _action_num)
    {
        return;
    }
    _action_set[order] = func;
}

template <typename T>
uint8_t RunningState<T>::GetNextState(uint8_t *event)
{
    for (int i = 0; i < STATE_NUM; ++i)
    {
        if (_MatchEvent(_map[i].event, event))
        {
            return _map[i].state;
        }
    }
    return _my_state;
}

template <typename T>
void RunningState<T>::ExecuteAction()
{
    for (int i = 0; i < _action_num; ++i)
    {
        (_object->*_action_set[i])();
    }
}

template <typename T>
uint8_t RunningState<T>::GetMyState()
{
    return _my_state;
}

template <typename T>
RunningState<T>::~RunningState()
{
    if (_action_set != nullptr)
    {
        delete[] _action_set;
    }
}

template <typename T>
bool RunningState<T>::_MatchEvent(uint8_t *source, uint8_t *event)
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

template <typename T>
void StateMachine<T>::AddState(RunningState<T> *state)
{
    if (_state_index >= STATE_NUM)
    {
        return;
    }
    _state_set[_state_index] = state;
    _state_index++;
}

template <typename T>
void StateMachine<T>::SetInitialState(RunningState<T> *state)
{
    _current_state = state;
}

template <typename T>
void StateMachine<T>::TransferState(uint8_t *event)
{  
    uint8_t next_state = _current_state->GetNextState(event);
    _current_state = _state_set[next_state];
}

template <typename T>
void StateMachine<T>::Run()
{
    _current_state->ExecuteAction();
}