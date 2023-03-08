/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-27 23:39:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-02 00:37:14
 * @FilePath: \ZH_FLIGHT\Sys\Data\data_manager.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "data_manager.h"
#include "cmsis_os2.h"
#include "os.h"

template <class T>
DataManager<T>::DataManager()
{
    T init;
    _handler = osMessageQueueNew(1, sizeof(T), NULL);
    Push(&init);
}

template <class T>
void DataManager<T>::Push(T *data)
{
    osMessageQueuePut(_handler, data, NULL, osWaitForever);
}

template <class T>
void DataManager<T>::Pop(T *data)
{
    osMessageQueueGet(_handler, data, NULL, osWaitForever);
}

template <class T>
void DataManager<T>::Copy(T *data)
{
    osMessageQueuePeek(_handler, data, NULL, osWaitForever);
}

template <class T>
void DataManager<T>::Update(T *data)
{
    osMessageQueueOverwrite(_handler, data);
}

template <class T>
DataManager<T>::~DataManager()
{
    osMessageQueueDelete(_handler);
}