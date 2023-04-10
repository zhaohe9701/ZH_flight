/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:08:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 23:30:07
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "type.h"

void Vec3::Set(float xi, float yi, float zi)
{
    x = xi;
    y = yi;
    z = zi;
}


void Vec3::Clear()
{
    x = 0;
    y = 0;
    z = 0;
}

Vec3& Vec3::operator=(Vec3 &src)
{
    x = src.x;
    y = src.y;
    z = src.z;
    return *this;
}

void Vec4::Set(float xi, float yi, float zi, float wi)
{
    x = xi;
    y = yi;
    z = zi;
    w = wi;
}

void Vec4::Clear()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}