/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:40:35
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-19 23:38:18
 * @FilePath: \ZH_FLIGHT\Sys\Flight\controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "controller.h"


void ControlOutput::Allocate()
{
    
}


ControllerParam::ControllerParam(uint8_t type)
{
    switch (type)
    {
    case PID:
        _AllocParamMem(PID_PARAM_NUM);
        break;
    
    default:
        break;
    }
}

ControllerParam::~ControllerParam()
{
    if (param != nullptr)
    {
        delete param;
    }
}
void ControllerParam::_AllocParamMem(uint8_t n)
{
    param = new float[n];
    size = n;
}