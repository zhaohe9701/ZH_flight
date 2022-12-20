/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:08:18
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-22 22:55:52
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