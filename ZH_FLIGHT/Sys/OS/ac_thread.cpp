//
// Created by zhaohe on 2023/5/9.
//

#include "ac_thread.h"

AC_RET AcThread::Init(AcFunction func, const char *name, uint16_t stack, AcPriority prio, void *param)
{
    if (pdPASS == xTaskCreate(func, name, stack, param, prio, &_handle))
    {
        return AC_OK;
    } else
    {
        return AC_ERROR;
    }
}

void AcThread::Kill()
{
    if (nullptr != _handle)
    {
        vTaskDelete(_handle);
    }
}

void AcThread::Suspend()
{
    if (nullptr != _handle)
    {
        vTaskSuspend(_handle);
    }
}

AC_RET AcThread::Resume()
{
    if (nullptr != _handle)
    {
        if (IS_IN_IRQ())
        {
            if (pdPASS != xTaskResumeFromISR(_handle))
            {
                return AC_ERROR;
            } else
            {
                return AC_OK;
            }
        } else
        {
            vTaskResume(_handle);
            return AC_OK;
        }
    }
    return AC_ERROR;
}

void AcThread::KillSelf()
{
    vTaskDelete(nullptr);
}

void ThreadManager::AddThread(AcThread &thread)
{
    _list.PushBack(thread);
}
