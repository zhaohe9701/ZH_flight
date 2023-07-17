#ifndef __CONTROL_INPUT_H__
#define __CONTROL_INPUT_H__

struct ControlInput
{
    float expect_x = 0.0f;
    float expect_y = 0.0f;
    float expect_z = 0.0f;
    float actual_x = 0.0f;
    float actual_y = 0.0f;
    float actual_z = 0.0f;
    float actual_dx = 0.0f;
    float actual_dy = 0.0f;
    float actual_dz = 0.0f;
};
#endif


