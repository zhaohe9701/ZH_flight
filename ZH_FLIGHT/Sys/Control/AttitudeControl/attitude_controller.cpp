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


#include "attitude_controller.h"

/**
 * @description: 
 * @param {AttitudeControllerInterface} *interface
 * @return {*}
 */
void AttitudeController::setMethod(AttitudeControllerInterface *interface)
{
    _interface = interface;
}

/**
 * @description: 
 * @param {ControlParam} *param
 * @return {*}
 */
void AttitudeController::init(ControlParam *param)
{
    _interface->init();
    _interface->init(param);
}

/**
 * @description: 
 * @param {AircraftState} &actual_state
 * @param {ExpectState} &expect_state
 * @return {*}
 */
void AttitudeController::update(const ActualState &actual_state, ExpectState &expect_state, ActuatorData &actuator_value)
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

    _interface->update(input, output);

    actuator_value.motor[0] = _limit(actuator_value.motor[0] + output.motor1);
    actuator_value.motor[1] = _limit(actuator_value.motor[1] + output.motor2);
    actuator_value.motor[2] = _limit(actuator_value.motor[2] + output.motor3);
    actuator_value.motor[3] = _limit(actuator_value.motor[3] + output.motor4);
}

/**
 * @description: 
 * @param {float} value
 * @return {*}
 */
float AttitudeController::_limit(float value)
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