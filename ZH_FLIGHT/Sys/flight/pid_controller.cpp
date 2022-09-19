/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-18 23:38:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-20 00:00:26
 * @FilePath: \ZH_FLIGHT\Sys\Flight\pid_controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "pid_controller.h"

void PidController::Init(ControllerParam &controller_param, uint8_t type)
{
    _iner_pid.Init(controller_param.param[0],
                   controller_param.param[1],
                   controller_param.param[2],
                   controller_param.param[3],
                   controller_param.param[4],
                   controller_param.param[5]);

    _outer_pid.Init(controller_param.param[6],
                    controller_param.param[7],
                    controller_param.param[8],
                    controller_param.param[9],
                    controller_param.param[10],
                    controller_param.param[11]);
    _type = type;
    
#ifdef DIFFERENTIAL_AHEAD
    _iner_pid.EnableDiffAhead();
#endif

#ifdef DIFFERENTIAL_FILER
    _iner_pid.EnableDiffFilter(DIFFERENTIAL_FILTER_SAMPLE_FREQ, DIFFERENTIAL_FILTER_CUTOFF_FREQ);
#endif   
}

void PidController::Update(AircraftState &actual_state, ExpectState &expect_state, ControlOutput &control_output)
{
    if (_type == PITCH_CONTROLLER)
    {
        expect_state.palstance.x = _outer_pid.Update(actual_state.euler.PITCH, expect_state.euler.PITCH);
        control_output.attitude_out.PITCH = _iner_pid.Update(actual_state.palstance.x, expect_state.palstance.x);
        
    }
    else if (_type == ROLL_CONTROLLER)
    {
        expect_state.palstance.y = _outer_pid.Update(actual_state.euler.ROLL, expect_state.euler.ROLL);
        control_output.attitude_out.ROLL = _iner_pid.Update(actual_state.palstance.y, expect_state.palstance.y);
        
    }
    else if (_type == YAW_CONTROLLER)
    {
        if (actual_state.euler.YAW - expect_state.euler.YAW >= 180.0f)
        {
            expect_state.euler.YAW += 180.0f;
        }
        if (actual_state.euler.YAW - expect_state.euler.YAW <= -180.0f)
        {
            expect_state.euler.YAW -= 180.0f;
        }
        expect_state.palstance.z = _outer_pid.Update(actual_state.euler.YAW, expect_state.euler.YAW);
        control_output.attitude_out.YAW = _iner_pid.Update(actual_state.palstance.z, expect_state.palstance.z);
    }
}
