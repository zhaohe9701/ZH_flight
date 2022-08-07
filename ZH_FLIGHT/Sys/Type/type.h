/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:03:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-08-07 22:17:06
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __TYPE_H__
#define __TYPE_H__

template <typename T>
class Vec3
{
public:
    T x = 0;
    T y = 0;
    T z = 0;
    void Set(T xi, T yi, T zi);
    void Clear();
};

#endif
