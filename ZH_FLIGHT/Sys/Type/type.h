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
    AC_ROOT,
    AC_STRUCT,
    AC_ARRAY,
    AC_UINT8,
    AC_UINT16,
    AC_UINT32,
    AC_INT8,
    AC_INT16,
    AC_INT32,
    AC_FLOAT,
    AC_DOUBLE,
    AC_STRING,
    AC_NULL,
    AC_SWITCH

};

enum AC_RELATION
{
    AC_AND,
    AC_OR
};
#define PARAM_NAME_LEN  32
#define TYPE_BUF_LEN 16
#define DATA_BUF_LEN 64

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

class Type
{
public:
    static AC_RET transTypeToStr(char *type_buf, AC_DATA_TYPE type);
    static AC_RET transDataToStr(char *data_buf, void *data, AC_DATA_TYPE type);
    static AC_RET transStrToType(char *type_buf, AC_DATA_TYPE &type);
    static AC_RET transStrToData(char *data_buf, void *data, AC_DATA_TYPE type);
};

typedef uint32_t Condition;
typedef uint8_t MessageHead;
typedef uint8_t Byte;
typedef uint8_t AcSwitch;

#define AC_ON 1
#define AC_OFF 0

#endif
