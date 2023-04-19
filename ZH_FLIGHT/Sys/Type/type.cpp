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
#include <cstdlib>
#include "type.h"
#include "sys.h"

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

AC_RET Type::TransTypeToStr(char *type_buf, AC_DATA_TYPE type)
{
    switch (type)
    {
        case AC_UINT8:
            strncpy(type_buf, "uint8", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT16:
            strncpy(type_buf, "uint16", TYPE_BUF_LEN - 1);
            break;
        case AC_UINT32:
            strncpy(type_buf, "uint32", TYPE_BUF_LEN - 1);
            break;
        case AC_INT8:
            strncpy(type_buf, "int8", TYPE_BUF_LEN - 1);
            break;
        case AC_INT16:
            strncpy(type_buf, "int16", TYPE_BUF_LEN - 1);
            break;
        case AC_INT32:
            strncpy(type_buf, "int32", TYPE_BUF_LEN - 1);
            break;
        case AC_FLOAT:
            strncpy(type_buf, "float", TYPE_BUF_LEN - 1);
            break;
        case AC_DOUBLE:
            strncpy(type_buf, "double", TYPE_BUF_LEN - 1);
            break;
        case AC_STRING:
            strncpy(type_buf, "string", TYPE_BUF_LEN - 1);
            break;
        case AC_SWITCH:
            strncpy(type_buf, "switch", TYPE_BUF_LEN - 1);
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
            snprintf(data_buf, DATA_BUF_LEN, "%u", ((uint32_t *) data)[index]);
            break;
        case AC_INT8:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int8_t *) data)[index]);
            break;
        case AC_INT16:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int16_t *) data)[index]);
            break;
        case AC_INT32:
            snprintf(data_buf, DATA_BUF_LEN, "%d", ((int32_t *) data)[index]);
            break;
        case AC_FLOAT:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((float *) data)[index]);
            break;
        case AC_DOUBLE:
            snprintf(data_buf, DATA_BUF_LEN, "%f", ((double *)data)[index]);
            break;
        case AC_SWITCH:
            snprintf(data_buf, DATA_BUF_LEN, "0x%x", ((uint8_t *) data)[index]);
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

AC_RET Type::TransStrToType(char *type_buf, AC_DATA_TYPE &type)
{
    if (0 == strncmp("uint8", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT8;
    } else if (0 == strncmp("uint16", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT16;
    } else if (0 == strncmp("uint32", type_buf, TYPE_BUF_LEN))
    {
        type = AC_UINT32;
    } else if (0 == strncmp("int8", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT8;
    } else if (0 == strncmp("int16", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT16;
    } else if (0 == strncmp("int32", type_buf, TYPE_BUF_LEN))
    {
        type = AC_INT32;
    } else if (0 == strncmp("float", type_buf, TYPE_BUF_LEN))
    {
        type = AC_FLOAT;
    } else if (0 == strncmp("double", type_buf, TYPE_BUF_LEN))
    {
        type = AC_DOUBLE;
    } else if (0 == strncmp("string", type_buf, TYPE_BUF_LEN))
    {
        type = AC_STRING;
    } else if (0 == strncmp("switch", type_buf, TYPE_BUF_LEN))
    {
        type = AC_SWITCH;
    }
    else
    {
        return AC_ERROR;
    }

    return AC_OK;
}

AC_RET Type::TransStrToData(char *data_buf, void *data, AC_DATA_TYPE type, uint16_t index)
{
    char **end = nullptr;
    debug_printer->Info("data: %s\n", data_buf);
    switch (type)
    {
        case AC_UINT8:
            ((uint8_t *) data)[index] = strtoul(data_buf, end, 10);
            break;
        case AC_UINT16:
            ((uint16_t *) data)[index] = strtoul(data_buf, end, 10);
            break;
        case AC_UINT32:
            ((uint32_t *) data)[index] = strtoul(data_buf, end, 10);
            break;
        case AC_INT8:
            ((int8_t *) data)[index] = strtol(data_buf, end, 10);
            break;
        case AC_INT16:
            ((int16_t *) data)[index] = strtol(data_buf, end, 10);
            break;
        case AC_INT32:
            ((int32_t *) data)[index] = strtol(data_buf, end, 10);
            break;
        case AC_FLOAT:
            ((float *) data)[index] = strtof(data_buf, end);
            break;
        case AC_DOUBLE:
            ((double *) data)[index] = strtod(data_buf, nullptr);
            break;
        case AC_SWITCH:
            ((uint8_t *) data)[index] = strtoul(data_buf + 2, nullptr, 16);
            break;
        case AC_STRING:
            if (index != 0)
            {
                return AC_ERROR;
            }
            sscanf(data_buf, "%s", (char*)data);
            break;
        default:
            return AC_ERROR;
    }
    return AC_OK;
}
