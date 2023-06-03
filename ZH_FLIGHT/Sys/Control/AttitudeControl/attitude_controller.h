/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-20 23:12:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-28 23:55:05
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ATTITUDE_CONTROLLER_H__
#define __ATTITUDE_CONTROLLER_H__

#include "attitude_controller_interface.h"
#include "aircraft_state.h"
#include "actuator_data.h"
class AttitudeController
{
public:
    void setMethod(AttitudeControllerInterface *interface);
    void init(ControlParam *param);
    void update(const ActualState &actual_state, ExpectState &ExpectState, ActuatorData &actuator_value);
private:
    AttitudeControllerInterface *_interface = nullptr;
    float _limit(float value);
};

#endif