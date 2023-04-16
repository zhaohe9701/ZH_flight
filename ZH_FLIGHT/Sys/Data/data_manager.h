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

template <class T>
class DataManager
{
public:
    explicit DataManager(uint32_t len = 1);
    void Push(T *data);
    void Pop(T *data);
    void Update(T *data);
    void Copy(T *data);
    ~DataManager();
private:
    Queue _handler;
};

template <class T>
DataManager<T>::DataManager(uint32_t len)
{
    T init;
    _handler = osMessageQueueNew(1, sizeof(T), NULL);
    Push(&init);
}

template <class T>
void DataManager<T>::Push(T *data)
{
    osMessageQueuePut(_handler, data, 0, osWaitForever);
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

#endif