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
#include <cstring>
#include <cstdio>
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

AC_RET Type::MatchType(char *type_str, AC_DATA_TYPE type)
{
    switch (type)
    {
        case AC_UINT8:
            strncpy(type_str, "uint8", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT16:
            strncpy(type_str, "uint16", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT32:
            strncpy(type_str, "uint32", TYPE_BUF_LEN - 1);
            break;
        case AC_INT8:
            strncpy(type_str, "int8", TYPE_BUF_LEN - 1);
            break;
        case AC_INT16:
            strncpy(type_str, "int16", TYPE_BUF_LEN - 1);
            break;
        case AC_INT32:
            strncpy(type_str, "int32", TYPE_BUF_LEN - 1);
            break;
        case AC_FLOAT:
            strncpy(type_str, "float", TYPE_BUF_LEN - 1);
            break;
        case AC_DOUBLE:
            strncpy(type_str, "double", TYPE_BUF_LEN - 1);
            break;
        case AC_STRING:
            strncpy(type_str, "string", TYPE_BUF_LEN - 1);
            break;
        case AC_SWITCH:
            strncpy(type_str, "switch", TYPE_BUF_LEN - 1);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}

AC_RET Type::TransDataToStr(char *data_buf, void *data, AC_DATA_TYPE type, uint16_t index)
{
    switch (type)
    {
        case AC_UINT8:
            snprintf(data_buf, DATA_BUF_LEN, "%u", ((uint8_t *) data)[index]);
            break;
        case AC_UINT16:
            snprintf(data_buf, DATA_BUF_LEN, "%u", ((uint16_t *) data)[index]);
            break;
        case AC_UINT32:
            snprintf(data_buf, DATA_BUF_LEN, "%lu", ((uint32_t *) data)[index]);
            break;
        case AC_INT8:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int8_t *) data)[index]);
            break;
        case AC_INT16:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int16_t *) data)[index]);
            break;
        case AC_INT32:
            snprintf(data_buf, DATA_BUF_LEN, "%ld", ((int32_t *) data)[index]);
            break;
        case AC_FLOAT:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((float *) data)[index]);
            break;
        case AC_DOUBLE:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((double *)data)[index]);
            break;
        case AC_SWITCH:
            if (AC_OFF == ((AcSwitch *)data)[index])
            {
                strcpy(data_buf, "off");
            }
            else
            {
                strcpy(data_buf, "on");
            }
            break;
        case AC_STRING:
            if (index != 0)
            {
                return AC_ERROR;
            }
            snprintf(data_buf, DATA_BUF_LEN, "%s", (char*)data);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}