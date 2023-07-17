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
#ifndef __DEFAULT_ATTITUDE_CONTROLLER_H__
#define __DEFAULT_ATTITUDE_CONTROLLER_H__

#include "attitude_controller.h"
#include "pid_core.h"

class PidController
{
public:
    PidCore iner_pid;
    PidCore outer_pid;
    void init();
    float update(float expect_attitude, float actual_attitude, float actual_speed);
};

class DefaultAttitudeController : virtual public AttitudeController
{
public:
    void init() override;
    void update(AttitudeState &state, ActuatorData &actuator_value) override;
private:
    PidController pitch;
    PidController roll;
    PidController yaw;
    float _limit(float value);
};

#endif