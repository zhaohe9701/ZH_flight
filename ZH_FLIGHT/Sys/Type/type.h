/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:03:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:41:09
 * @FilePath: \ZH_FLIGHT\Sys\Type\type.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdint.h>

enum AC_RET
{
    AC_OK = 0,
    AC_ERROR = 1
};

enum AC_DATA_TYPE
{
    AC_UINT8    =   1,
    AC_UINT16   =   2,
    AC_UINT32   =   3,
    AC_INT8     =   4,
    AC_INT16    =   5,
    AC_INT32    =   6,
    AC_FLOAT    =   7,
    AC_DOUBLE   =   8,
    AC_STRING   =   9,
    AC_NULL     =   10,
    AC_MARK     =   11
};

#define PARAM_NAME_LEN  32
class Vec3
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    void Set(float xi, float yi, float zi);
    void Clear();
    Vec3& operator=(Vec3 &src);
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

typedef uint32_t Condition;
typedef uint8_t MessageHead;
typedef uint8_t Byte;
typedef uint8_t AcMark;

#define AC_ON 1
#define AC_OFF 0

#endif
