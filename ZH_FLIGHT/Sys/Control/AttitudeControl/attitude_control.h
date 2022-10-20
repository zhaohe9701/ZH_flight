/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-20 23:12:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 23:53:34
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_control.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ATTITUDE_CONTROL_H__
#define __ATTITUDE_CONTROL_H__

#include "attitude_controller_interface.h"
#include "aircraft_state.h"
class AttitudeControl
{
public:
    AttitudeControl(AttitudeControllerInterface *interface);
    void Init(ControlParam *param);
    void Update(const AircraftState &actual_state, ExpectState &ExpectState);
private:
    AttitudeControllerInterface *_interface = nullptr;
    float _Limit(float value);
};

#endif