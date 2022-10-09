/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-18 23:38:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-04 01:57:01
 * @FilePath: \ZH_FLIGHT\Sys\Flight\pid_controller.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __PID_CONTROLLER_H__
#define __PID_CONTROLLER_H__

#include "controller.h"
#include "pid_core.h"

#define DIFFERENTIAL_FILTER_SAMPLE_FREQ 500
#define DIFFERENTIAL_FILTER_CUTOFF_FREQ 100

#define PITCH_CONTROLLER 1
#define ROLL_CONTROLLER 2
#define YAW_CONTROLLER 3
class PidController
{
private:
    PidCore _iner_pid;
    PidCore _outer_pid;
    uint8_t _type = 0;

public:
    void Init(ControllerParam *controller_param, uint8_t type);
    void Update(AircraftState &actual_state, ExpectState &expect_state, ControlOutput &control_output);

    
};


class PidAttitudeController : virtual public AttitudeController
{
public:
    void Init(AttitudeControllerParam *param) override;
    void Update(AircraftState &actual_state, ExpectState &expect_state, ControlOutput &control_output) override;
    PidController _pitch;
    PidController _roll;
    PidController _yaw;
};

#endif
