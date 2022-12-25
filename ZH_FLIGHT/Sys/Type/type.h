/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:03:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:32:24
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __TYPE_H__
#define __TYPE_H__

enum AC_RET
{
    AC_OK = 0,
    AC_ERROR = 1
};

class Vec3
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    void Set(float xi, float yi, float zi);
    void Clear();
};

class Vec4
{
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
    void Set(float xi, float yi, float zi, float wi);
    void Clear();
};

#endif
