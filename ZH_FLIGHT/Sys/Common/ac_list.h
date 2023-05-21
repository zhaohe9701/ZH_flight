/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-02-23 22:52:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-24 00:05:22
 * @FilePath: \ZH_FLIGHT\Sys\Common\ac_list.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AC_LIST_H__
#define __AC_LIST_H__

template <class T>
class AcList;

template <class T>
class AcListNode
{
friend class AcList<T>;
private:
    AcListNode<T> *_prev = nullptr;
    AcListNode<T> *_next = nullptr;
public:
    T data;
    AcListNode<T> *GetNext();
    AcListNode<T> *GetPrev();
};

template <class T>
class AcList
{
private:
    AcListNode<T> *_head = nullptr;
public:
    AcList();
    void PushBack(T &data);
    void PushFront(T &data);
    AcListNode<T> *Begin();
    AcListNode<T> *End();
    AcListNode<T> *Index(int ind);
    ~AcList();
};

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
void AcList<T>::PushBack(T &data)
{
    AcListNode<T> *node = new AcListNode<T>();
    node->data = data;
    node->_next = _head;
    node->_prev = _head->_prev;
    _head->_prev->_next = node;
    _head->_prev = node;
}

template <class T>
void AcList<T>::PushFront(T &data)
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

#endif