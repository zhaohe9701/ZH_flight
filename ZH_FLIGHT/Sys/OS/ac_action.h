//
// Created by zhaohe on 2023/7/30.
//

#ifndef __AC_ACTION_H__
#define __AC_ACTION_H__
#include "os.h"
#include "type.h"

typedef EventBits_t ActionGroup;

class ActionManager
{
public:
    ActionManager();
    ActionGroup wait(const ActionGroup action, AC_RELATION relation = AC_OR, bool clear = false);
    ActionGroup get();
    AC_RET clear(const ActionGroup action);
    AC_RET give(const ActionGroup action);
    ~ActionManager();
private:
    EventGroupHandle_t _handle;
};
#endif //ZH_FLIGHT_H7_AC_ACTION_H
