/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-23 23:19:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-24 00:05:06
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_list.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "ac_list.h"

template <class T>
AcListNode<T> *AcListNode<T>::GetNext()
{
    return _next;
}

template <class T>
AcListNode<T> *AcListNode<T>::GetPrev()
{
    return _prev;
}

template <class T>
AcList<T>::AcList()
{
    _head = new AcListNode<T>();
    _head->_next = _head;
    _head->_prev = _head;
}

template <class T>
void AcList<T>::PushBack(T *data)
{
    AcListNode<T> *node = new AcListNode<T>();
    node->data = data;
    node->_next = _head;
    node->_prev = _head->_prev;
    _head->_prev->_next = node;
    _head->_prev = node;
}

template <class T>
void AcList<T>::PushFront(T *data)
{
    AcListNode<T> *node = new AcListNode<T>();
    node->data = data;
    node->_prev = _head;
    node->_next = _head->_next;
    _head->_next->_prev = node;
    _head->_next = node;
}

template <class T>
AcListNode<T> *AcList<T>::Begin()
{
    return _head->_next;
}

template <class T>
AcListNode<T> *AcList<T>::End()
{
    return _head;
}

template <class T>
AcList<T>::~AcList()
{
    AcListNode<T> *node = _head->GetNext();
    AcListNode<T> *next_node = nullptr;
    while (node != _head)
    {
        next_node = node->GetNext();
        delete node;
        node = next_node;
    }
    delete _head;
}

template<class T>
AcListNode<T> *AcList<T>::Index(int ind)
{
    int i = 0;
    AcListNode<T> *node = _head->GetNext();
    while (node != _head)
    {
        if (i == ind)
        {
            return node;
        }
        if (ind > 0)
        {
            node = node->GetNext();
            i++;
        } else
        {
            node = node->GetPrev();
            i--;
        }
    }
    return nullptr;
}