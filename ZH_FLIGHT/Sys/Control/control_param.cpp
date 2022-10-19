/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-19 22:53:19
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-19 22:59:36
 * @FilePath: \ZH_FLIGHT\Sys\Control\control_param.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "control_param.h"
#include <string.h>

void ControlParam::Init(uint8_t size)
{
    _size = size;
    param = new float[size];
}

void ControlParam::GetParam(uint8_t *data)
{
    memcpy(param, data, _size * 4);
}

void ControlParam::Destroy()
{
    if (param != nullptr)
    {
        delete[] param;
    }
}

ControlParam::~ControlParam()
{
    if (param != nullptr)
    {
        delete[] param;
    }    
}