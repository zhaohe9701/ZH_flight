/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-08 22:48:51
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-08 23:19:20
 * @FilePath: \ZH_FLIGHT\Sys\EventServer\event_server.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __EVENT_SERVER_H__
#define __EVENT_SERVER_H__

#include "state_machine.h"
#include "config.h"

class EventServer
{
private:
    Condition _current_condition = NULL_EVENT;
    Thread _state_machine_thread;
    uint32_t _inform_value;
public:
    Condition GetCurrentCondition();
    void SetInformThread(Thread thread, uint32_t value);
    void SetEvent(EventList event);
    void ClearEvent(EventList event);
};

#endif
