/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:23:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-25 02:09:57
 * @FilePath: \ZH_FLIGHT\Sys\Control\AttitudeControl\attitude_controller.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */


#include "attitude_controller.h"

/**
 * @description: 
 * @param {AttitudeControllerInterface} *interface
 * @return {*}
 */
void AttitudeController::SetMethod(AttitudeControllerInterface *interface)
{
    _interface = interface;
}

/**
 * @description: 
 * @param {ControlParam} *param
 * @return {*}
 */
void AttitudeController::Init(ControlParam *param)
{
    _interface->Init();
    _interface->Init(param);
}

/**
 * @description: 
 * @param {AircraftState} &actual_state
 * @param {ExpectState} &expect_state
 * @return {*}
 */
void AttitudeController::Update(const ActualState &actual_state, ExpectState &expect_state)
{
    AttitudeControlInput input;
    AttitudeControlOutput output;

    input.actual_pitch = actual_state.euler.PITCH;
    input.actual_roll = actual_state.euler.ROLL;
    input.actual_yaw = actual_state.euler.YAW;

    input.expect_pitch = expect_state.euler.PITCH;
    input.expect_roll = expect_state.euler.ROLL;
    input.expect_yaw = expect_state.euler.YAW;

    input.actual_palstance_x = actual_state.palstance.x;
    input.actual_palstance_y = actual_state.palstance.y;
    input.actual_palstance_z = actual_state.palstance.z;

    _interface->Update(input, output);

    expect_state.motor[0] = output.motor1;
    expect_state.motor[1] = output.motor2;
    expect_state.motor[2] = output.motor3;
    expect_state.motor[3] = output.motor4;


}

/**
 * @description: 
 * @param {float} value
 * @return {*}
 */
float AttitudeController::_Limit(float value)
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