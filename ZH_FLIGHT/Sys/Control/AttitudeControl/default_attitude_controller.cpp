/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:23:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-01 00:01:47
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */


#include "default_attitude_controller.h"
#include "control_input.h"
#include "control_output.h"

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


void DefaultAttitudeController::init()
{

}

void DefaultAttitudeController::update(AttitudeState &state, ActuatorData &actuator_value)
{
    float pitch_out = pitch.update(state.expect.euler.PITCH, state.actual.euler.PITCH, state.actual.ang_acc.x);
    float roll_out  =  roll.update(state.expect.euler.ROLL,  state.actual.euler.ROLL,  state.actual.ang_acc.y);
    float yaw_out   =   yaw.update(state.expect.euler.YAW,   state.actual.euler.YAW,   state.actual.ang_acc.z);

    actuator_value.motor[0] = _limit(actuator_value.motor[0] + pitch_out - roll_out + yaw_out);
    actuator_value.motor[1] = _limit(actuator_value.motor[1] + pitch_out - roll_out - yaw_out);
    actuator_value.motor[2] = _limit(actuator_value.motor[2] + pitch_out + roll_out - yaw_out);
    actuator_value.motor[3] = _limit(actuator_value.motor[3] + pitch_out + roll_out + yaw_out);
}

float DefaultAttitudeController::_limit(float value)
{
    if (value > MOTOR_OUTPUT_MAX)
    {
        return MOTOR_OUTPUT_MAX;
    }
    if (value < MOTOR_OUTPUT_MIN)
    {
        return MOTOR_OUTPUT_MIN;
    }

    return value;
}