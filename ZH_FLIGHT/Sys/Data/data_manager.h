/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-27 23:27:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-22 00:41:25
 * @FilePath: \ZH_FLIGHT\Sys\Data\data_manager.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "os.h"
#include "type.h"

template <class T>
class DataManager
{
public:
    explicit DataManager(uint32_t len = 1);
    AC_RET Push(T *data, uint32_t timeout = AC_FOREVER);
    AC_RET Pop(T *data, uint32_t timeout = AC_FOREVER);
    AC_RET Update(T *data);
    AC_RET Copy(T *data, uint32_t timeout = AC_FOREVER);
    ~DataManager();
private:
    QueueHandle _handler = nullptr;
};

template <class T>
DataManager<T>::DataManager(uint32_t len)
{
    // T init;
    _handler = xQueueCreate(1, sizeof(T));
    // Push(&init);
}

template <class T>
AC_RET DataManager<T>::Push(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdPASS != xQueueSendToBackFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueSendToBack(_handler, data, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template <class T>
AC_RET DataManager<T>::Pop(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xQueueReceiveFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueReceive(_handler, data, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template <class T>
AC_RET DataManager<T>::Copy(T *data, uint32_t timeout)
{
    if (IS_IN_IRQ())
    {
        if (pdTRUE != xQueuePeekFromISR(_handler, data))
        {
            return AC_ERROR;
        }
    } else
    {
        if (pdTRUE != xQueuePeek(_handler, data, (TickType_t)timeout))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template <class T>
AC_RET DataManager<T>::Update(T *data)
{
    if (IS_IN_IRQ())
    {
        BaseType_t yield = pdFALSE;
        if (pdTRUE != xQueueOverwriteFromISR(_handler, data, &yield))
        {
            return AC_ERROR;
        } else
        {
            portYIELD_FROM_ISR(yield);
        }
    } else
    {
        if (pdTRUE != xQueueOverwrite(_handler, data))
        {
            return AC_ERROR;
        }
    }
    return AC_OK;
}

template <class T>
DataManager<T>::~DataManager()
{
    vQueueDelete(_handler);
}

#endif