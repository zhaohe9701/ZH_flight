//
// Created by zhaohe on 2023/7/30.
//
#include "ac_action.h"

ActionManager::ActionManager()
{
    _handle = xEventGroupCreate();
}
ActionGroup ActionManager::wait(const ActionGroup action, AC_RELATION relation, bool clear)
{
    BaseType_t clear_on_exit;
    if (true == clear)
    {
        clear_on_exit = pdTRUE;
    } else
    {
        clear_on_exit = pdFALSE;
    }
    if (AC_AND == relation)
    {
        return xEventGroupWaitBits(_handle, action, clear_on_exit, pdTRUE, AC_FOREVER);
    } else
    {
        return xEventGroupWaitBits(_handle, action, clear_on_exit, pdFALSE, AC_FOREVER);
    }
}

ActionGroup ActionManager::get()
{
    if (IS_IN_IRQ())
    {
        return xEventGroupGetBitsFromISR(_handle);
    } else
    {
        return xEventGroupGetBits(_handle);
    }
}

AC_RET ActionManager::clear(const ActionGroup action)
{
    if (IS_IN_IRQ())
    {
        if (pdPASS != xEventGroupClearBitsFromISR(_handle, action))
        {
            return AC_ERROR;
        }
    } else
    {
        xEventGroupClearBits(_handle, action);
    }
    return AC_OK;
}

AC_RET ActionManager::give(const ActionGroup action)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdPASS != xEventGroupSetBitsFromISR(_handle, action, &yield))
        {
            return AC_ERROR;
        }
    } else
    {
        xEventGroupSetBits(_handle, action);
    }
    return AC_OK;
}

ActionManager::~ActionManager()
{
    vEventGroupDelete(_handle);
}