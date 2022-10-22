/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:23:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 23:27:34
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ATTITUDE_CONTROLLER_H__
#define __ATTITUDE_CONTROLLER_H__
#include "control_param.h"
#include "control_input_type.h"
#include "control_output_type.h"
#include <stdint.h>


class AttitudeControllerInterface
{
public:
    virtual void Init(const ControlParam *param);
    virtual void Init();
    virtual void Update(const AttitudeControlInput &control_input, AttitudeControlOutput &control_output) = 0;
};

#endif