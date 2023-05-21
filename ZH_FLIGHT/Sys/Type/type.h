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
    AC_SWITCH,
    AC_STRUCT,
    AC_BASIC_ARRAY,
    AC_STRUCT_ARRAY
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
    static AC_RET TransTypeToStr(char *type_buf, AC_DATA_TYPE type);
    static AC_RET TransDataToStr(char *data_buf, void *data, AC_DATA_TYPE type, uint16_t index);
    static AC_RET TransStrToType(char *type_buf, AC_DATA_TYPE &type);
    static AC_RET TransStrToData(char *data_buf, void *data, AC_DATA_TYPE type, uint16_t index);
};

typedef uint32_t Condition;
typedef uint8_t MessageHead;
typedef uint8_t Byte;
typedef uint8_t AcSwitch;

#define AC_ON 1
#define AC_OFF 0

#endif
