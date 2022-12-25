/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-20 23:12:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 02:09:43
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ATTITUDE_CONTROLLER_H__
#define __ATTITUDE_CONTROLLER_H__

#include "attitude_controller_interface.h"
#include "aircraft_state.h"
class AttitudeController
{
public:
    void SetMethod(AttitudeControllerInterface *interface);
    void Init(ControlParam *param);
    void Update(const ActualState &actual_state, ExpectState &ExpectState);
private:
    AttitudeControllerInterface *_interface = nullptr;
    float _Limit(float value);
};

#endif