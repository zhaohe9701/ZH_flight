#include "attitude_control.h"


AttitudeControl::AttitudeControl(AttitudeControllerInterface *interface)
{
    _interface = interface;
}

void AttitudeControl::Init(ControlParam *param)
{
    _interface->Init();
    _interface->Init(param);
}

void AttitudeControl::Update(const AircraftState &actual_state, ExpectState &expect_state)
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

float AttitudeControl::_Limit(float value)
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