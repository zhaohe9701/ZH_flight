/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-18 23:38:17
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 00:27:01
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\pid_attitude_controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "pid_attitude_controller.h"

void PidController::init()
{  
#ifdef DIFFERENTIAL_AHEAD
    _iner_pid.EnableDiffAhead();
#endif

#ifdef DIFFERENTIAL_FILER
    _iner_pid.EnableDiffFilter(DIFFERENTIAL_FILTER_SAMPLE_FREQ, DIFFERENTIAL_FILTER_CUTOFF_FREQ);
#endif   
}

float  PidController::update(float expect_attitude, float actual_attitude, float actual_speed)
{
    float expect_speed = 0.0f;
    float output = 0.0f;

    if (expect_attitude - actual_attitude >= 180.0f)
    {
        expect_attitude -= 180.0f;
    }
    if (expect_attitude - actual_attitude <= -180.0f)
    {
        expect_attitude += 180.0f;
    }
    expect_speed = outer_pid.update(actual_attitude, expect_attitude);
    output = iner_pid.update(actual_speed, expect_speed);
    return output;
}



void PidAttitudeController::init(ControlParam *param)
{
    pitch.init();
    roll.init();
    yaw.init();
    pitch.iner_pid.init(param->param[0], param->param[1], param->param[2], param->param[3], param->param[4], param->param[5]);
    pitch.outer_pid.init(param->param[6], param->param[7], param->param[8], param->param[9], param->param[10], param->param[11]);

    roll.iner_pid.init(param->param[12], param->param[13], param->param[14], param->param[15], param->param[16], param->param[17]);
    roll.outer_pid.init(param->param[18], param->param[19], param->param[20], param->param[21], param->param[22], param->param[23]);

    yaw.iner_pid.init(param->param[24], param->param[25], param->param[26], param->param[27], param->param[28], param->param[29]);
    yaw.outer_pid.init(param->param[30], param->param[31], param->param[32], param->param[33], param->param[34], param->param[35]);
}

void PidAttitudeController::update(const AttitudeControlInput &control_input, AttitudeControlOutput &control_output)
{
    float pitch_out = pitch.update(control_input.expect_pitch, control_input.actual_pitch, control_input.actual_palstance_x);
    float roll_out = roll.update(control_input.expect_roll, control_input.actual_roll, control_input.actual_palstance_y);
    float yaw_out = yaw.update(control_input.expect_yaw, control_input.actual_yaw, control_input.actual_palstance_z);

    control_output.motor1 = pitch_out - roll_out + yaw_out;
    control_output.motor2 = pitch_out - roll_out - yaw_out;
    control_output.motor3 = pitch_out + roll_out - yaw_out;
    control_output.motor4 = pitch_out + roll_out + yaw_out;
}