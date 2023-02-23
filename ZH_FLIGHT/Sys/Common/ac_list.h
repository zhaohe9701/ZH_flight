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
    T *data = nullptr;
    AcListNode<T> *Next();
    AcListNode<T> *Prev();
};

template <class T>
class AcList
{
private:
    AcListNode<T> *_head = nullptr;
public:
    AcList();
    void PushBack(T *data);
    void PushFront(T *data);
    AcListNode<T> *Begin();
    AcListNode<T> *End();
    ~AcList();
};
#endif