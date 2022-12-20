/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-09 23:18:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-19 23:35:38
 * @FilePath: \ZH_FLIGHT\Sys\StateMachine\state_machine.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <stdint.h>

#define EVENT_NUM       6
#define STATE_NUM       7
#define EVENT_ON        1
#define EVENT_OFF       0
#define EVENT_IGNORE    2

/*状态集合定义*/
enum StateList
{
    S_INITIALIZE,
    S_STANDBY,
    S_WITE,
    S_PARAM_SERVICE,
    S_CALIBRATION,
    S_MANUAL,
    S_AUTO
};

/*事件集合定义*/
enum EventList
{
    E_UNLOCK,
    E_PARAM_SERVICE,
    E_CALIBRATION,
    E_MANUAL,
    E_AUTO,
    E_ZERO_THROTTLE
};

class EventStateMap
{
public:
    uint8_t event[EVENT_NUM];
    uint8_t state;
};


class RunningState
{
public:
    RunningState(uint8_t my_state);

    void AddEventStateMap(uint8_t state, uint8_t *event);

    void AddAction(void (*func)());

    uint8_t GetNextState(uint8_t *event);

    void ExecuteAction();

    uint8_t GetMyState();
    
private:
    uint8_t _my_state;

    uint8_t _reachable_state_num = 0;

    EventStateMap _map[STATE_NUM];
    
    void (*_action)() = nullptr;

    uint8_t *_transfer_event;

    bool _MatchEvent(uint8_t *source, uint8_t *event);
};

class StateMachine
{
public:
    void AddState(RunningState *state);
    void SetInitialState(RunningState *state);
    void TransferState(uint8_t *event);
    void Run();
private:
    uint8_t _state_index = 0;
    RunningState *_state_set[STATE_NUM];
    RunningState *_current_state;
};



#endif
