/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-19 22:45:00
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-19 22:59:39
 * @FilePath: \ZH_FLIGHT\Sys\Control\control_param.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONTROL_PARAM_H__
#define __CONTROL_PARAM_H__

#include <stdint.h>

class ControlParam
{
public:
    void Init(uint8_t size);
    void GetParam(uint8_t *data);
    void Destroy();

    float *param = nullptr;
    uint8_t _size = 0;

    ~ControlParam();
};

#endif