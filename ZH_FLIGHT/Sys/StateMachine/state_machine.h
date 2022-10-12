/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-12 23:21:46
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>

#define EVENT_NUM       6
#define STATE_NUM       5
#define EVENT_ON        1
#define EVENT_OFF       0
#define EVENT_IGNORE    2


class EventStateMap
{
public:
    uint8_t event[EVENT_NUM];
    uint8_t state;
};

template<typename T>
class RunningState
{
public:
    RunningState(uint8_t my_atate);
    void SetEventStateMap(uint8_t state, uint8_t *event);

    void AddAction(void (T::*func)(), uint8_t order);

    void SetActionNum(uint8_t num);

    uint8_t GetNextState(uint8_t *event);

    void SetObject(T *object);

    void ExecuteAction();

    uint8_t GetMyState();
    
    ~RunningState();
private:
    uint8_t _my_state;

    uint8_t _reachable_state_num = 0;
    EventStateMap _map[STATE_NUM];
    
    uint8_t _action_num = 0;
    void * * _action_set = nullptr;
    
    T *_object = nullptr;

    uint8_t *_transfer_event;

    bool _MatchEvent(uint8_t *source, uint8_t *event);
};

template<typename T>
class StateMachine
{
public:
    void AddState(RunningState<T> *state);
    void SetInitialState(RunningState<T> *state);
    void TransferState(uint8_t *event);
    void Run();
private:
    uint8_t _state_index = 0;
    RunningState<T> *_state_set[STATE_NUM];
    RunningState<T> *_current_state;
};



#endif
