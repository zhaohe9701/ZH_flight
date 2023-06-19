/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 20:02:09
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:21:10
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_queue.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AC_QUEUE_H__
#define __AC_QUEUE_H__

#include "cmsis_os2.h"
#include "data.h"
#include "type.h"

template <class T>
class AcQueue
{
private:
    osMessageQueueId_t _handle = nullptr;
public:
    AcQueue(uint32_t length);
    AC_RET push(T *message);
    AC_RET pop(T *message);
    ~AcQueue();
};

template <class T>
AcQueue<T>::AcQueue(uint32_t length)
{
    _handle = osMessageQueueNew(length, sizeof(T), NULL);
}

template <class T>
AC_RET AcQueue<T>::push(T *message)
{
    if (osOK != osMessageQueuePut(_handle, message, 0U, 0))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

template <class T>
AC_RET AcQueue<T>::pop(T *message)
{
    if (osOK != osMessageQueueGet(_handle, message, NULL, osWaitForever))
    {
        return AC_ERROR;
    }
    return AC_OK;
}

template <class T>
AcQueue<T>::~AcQueue()
{
    osMessageQueueDelete(_handle);
}

#endif