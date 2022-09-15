/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:08:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-08-20 14:37:39
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "type.h"

template <typename T>
void Vec3<T>::Set(T xi, T yi, T zi)
{
    x = xi;
    y = yi;
    z = zi;
}

template <typename T>
void Vec3<T>::Clear()
{
    x = 0;
    y = 0;
    z = 0;
}

template <typename T>
void Vec4<T>::Set(T xi, T yi, T zi, T wi)
{
    x = xi;
    y = yi;
    z = zi;
    w = wi;
}

template <typename T>
void Vec4<T>::Clear()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}