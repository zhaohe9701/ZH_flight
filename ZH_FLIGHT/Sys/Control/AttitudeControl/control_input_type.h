//
// File: control_input_type.h
//
// Code generated for Simulink model 'pid_controller'.
//
// Model version                  : 1.37
// Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
// C/C++ source code generated on : Wed Oct 19 00:27:00 2022
//
// Target selection: ert.tlc
// Embedded hardware selection: STMicroelectronics->ST10/Super10
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_control_input_type_h_
#define RTW_HEADER_control_input_type_h_
#include "rtwtypes.h"

struct AttitudeControlInput
{
    real32_T actual_pitch;
    real32_T expect_pitch;
    real32_T actual_palstance_x;
    real32_T actual_roll;
    real32_T expect_roll;
    real32_T actual_palstance_y;
    real32_T actual_yaw;
    real32_T expect_yaw;
    real32_T actual_palstance_z;
};

#endif                                 // RTW_HEADER_control_input_type_h_

//
// File trailer for generated code.
//
// [EOF]
//
