/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-18 23:38:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 00:14:24
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\pid_attitude_controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __PID_ATTITUDE_CONTROLLER_H__
#define __PID_ATTITUDE_CONTROLLER_H__

#include "attitude_controller.h"
#include "pid_core.h"

#define DIFFERENTIAL_FILTER_SAMPLE_FREQ 500
#define DIFFERENTIAL_FILTER_CUTOFF_FREQ 100

#define PITCH_CONTROLLER 1
#define ROLL_CONTROLLER 2
#define YAW_CONTROLLER 3
class PidController
{
public:
    PidCore iner_pid;
    PidCore outer_pid;
    void Init();
    float Update(float expect_attitude, float actual_attitude, float actual_speed);

    
};


class PidAttitudeController : virtual public AttitudeControllerInterface
{
public:
    void Init(ControlParam *param);
    void Update(const AttitudeControlInput &control_input, AttitudeControlOutput &control_output) override;
    PidController pitch;
    PidController roll;
    PidController yaw;
};

#endif
