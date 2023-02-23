/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-08 22:53:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-26 01:39:45
 * @FilePath: \ZH_FLIGHT\Sys\EventServer\event_server.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "event_server.h"

Condition EventServer::GetCurrentCondition()
{
    return _current_condition;
}

void EventServer::SetInformThread(Thread thread, uint32_t value)
{
    _state_machine_thread = thread;
    _inform_value = value;
}



void EventServer::SetEvent(EventList event)
{
    _current_condition = _current_condition | event;
    AcSignalSet(_state_machine_thread, _inform_value);
}

void EventServer::ClearEvent(EventList event)
{
    _current_condition = _current_condition & (~event);
    AcSignalSet(_state_machine_thread, _inform_value);
}
