/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-27 23:39:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:57:10
 * @FilePath: \ZH_FLIGHT\Sys\Data\data_manager.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "data_manager.h"
#include "cmsis_os2.h"

template <class T>
DataManager<T>::DataManager()
{
    _handler = osMessageQueueNew(1, sizeof(T), NULL);
}

template <class T>
void DataManager<T>::Put(T *data)
{
    osStatus_t ret = osOK;
    ret = osMessageQueuePut(_handler, data, 0U, 0U);
    if (osErrorResource == ret)
    {
        T *temp_data = nullptr;
        osMessageQueueGet(_handler, temp_data, NULL, osWaitForever);
        osMessageQueuePut(_handler, data, 0U, 0U);
    }
}

template <class T>
void DataManager<T>::Get(T *data)
{
    osMessageQueueGet(_handler, data, NULL, osWaitForever);
}

template <class T>
void DataManager<T>::GetWithoutDelete(T *data)
{
    osMessageQueueGet(_handler, data, NULL, osWaitForever);
    osMessageQueuePut(_handler, data, 0U, 0U);
}


template <class T>
DataManager<T>::~DataManager()
{
    osMessageQueueDelete(_handler);
}