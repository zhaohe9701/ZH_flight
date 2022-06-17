#include "pid.h"
#include "main.h"
static float Limit(float limit, float value)
{
    if(value > limit)
    {
        return limit;
    }
    else if(value < -limit)
    {
        return -limit;
    }
    else
    {
        return value;
    }
}


void Pid_Init()
{
    extern BicycloPidParam pitch_param;
    extern BicycloPidParam roll_param;
    extern BicycloPidParam yaw_param;

    pitch_param.iner.kp = 0.08f;
    pitch_param.iner.ki = 0.0f;
    pitch_param.iner.kd = 0.12f;
    pitch_param.iner.limit_i = 10.0f;
    pitch_param.iner.limit_out = 20.0f;
    pitch_param.iner.last_error = 0.0f;
    pitch_param.iner.integral = 0.0f;

    pitch_param.outer.kp = 8.0f;
    pitch_param.outer.ki = 0.0f;
    pitch_param.outer.kd = 0.0f;
    pitch_param.outer.limit_i = 10.0f;
    pitch_param.outer.limit_out = 200.0f;
    pitch_param.outer.last_error = 0.0f;
    pitch_param.outer.integral = 0.0f;

    roll_param.iner.kp = 0.08f;
    roll_param.iner.ki = 0.0f;
    roll_param.iner.kd = 0.12f;
    roll_param.iner.limit_i = 10.0f;
    roll_param.iner.limit_out = 20.0f;
    roll_param.iner.last_error = 0.0f;
    roll_param.iner.integral = 0.0f;

    roll_param.outer.kp = 8.0f;
    roll_param.outer.ki = 0.0f;
    roll_param.outer.kd = 0.0f;
    roll_param.outer.limit_i = 10.0f;
    roll_param.outer.limit_out = 200.0f;
    roll_param.outer.last_error = 0.0f;
    roll_param.outer.integral = 0.0f;

    yaw_param.iner.kp = 0.08f;
    yaw_param.iner.ki = 0.0f;
    yaw_param.iner.kd = 0.1f;
    yaw_param.iner.limit_i = 10.0f;
    yaw_param.iner.limit_out = 20.0f;
    yaw_param.iner.last_error = 0.0f;
    yaw_param.iner.integral = 0.0f;

    yaw_param.outer.kp = 6.0f;
    yaw_param.outer.ki = 0.0f;
    yaw_param.outer.kd = 0.0f;
    yaw_param.outer.limit_i = 10.0f;
    yaw_param.outer.limit_out = 200.0f;
    yaw_param.outer.last_error = 0.0f;
    yaw_param.outer.integral = 0.0f;
}

uint8_t Update_Pid_Param(union ParamTrans *param)
{
    extern BicycloPidParam pitch_param;
    extern BicycloPidParam roll_param;
    extern BicycloPidParam yaw_param;
    if(param->chr[0] == PID_PARAM && param->chr[1] == PITCH_PARAM)
    {
        pitch_param.iner.kp = param->flo[1];
        pitch_param.iner.ki = param->flo[2];
        pitch_param.iner.kd = param->flo[3];
        pitch_param.iner.limit_i = param->flo[4];
        pitch_param.iner.limit_out = param->flo[5];

        pitch_param.outer.kp = param->flo[6];
        pitch_param.outer.ki = param->flo[7];
        pitch_param.outer.kd = param->flo[8];
        pitch_param.outer.limit_i = param->flo[9];
        pitch_param.outer.limit_out = param->flo[10];
    }
    else if(param->chr[0] == PID_PARAM && param->chr[1] == ROLL_PARAM)
    {
        roll_param.iner.kp = param->flo[1];
        roll_param.iner.ki = param->flo[2];
        roll_param.iner.kd = param->flo[3];
        roll_param.iner.limit_i = param->flo[4];
        roll_param.iner.limit_out = param->flo[5];

        roll_param.outer.kp = param->flo[6];
        roll_param.outer.ki = param->flo[7];
        roll_param.outer.kd = param->flo[8];
        roll_param.outer.limit_i = param->flo[9];
        roll_param.outer.limit_out = param->flo[10];
    }
    else if(param->chr[0] == PID_PARAM && param->chr[1] == YAW_PARAM)
    {
        yaw_param.iner.kp = param->flo[1];
        yaw_param.iner.ki = param->flo[2];
        yaw_param.iner.kd = param->flo[3];
        yaw_param.iner.limit_i = param->flo[4];
        yaw_param.iner.limit_out = param->flo[5];

        yaw_param.outer.kp = param->flo[6];
        yaw_param.outer.ki = param->flo[7];
        yaw_param.outer.kd = param->flo[8];
        yaw_param.outer.limit_i = param->flo[9];
        yaw_param.outer.limit_out = param->flo[10];
    }
    else
    {
        return 1;
    }
    //usb_printf("%.2f\r\n", yaw_param.outer.limit_out);
    return 0;
}


float Pid_Controler(PidParam *pid_param, float error)
{
    float P = error * pid_param->kp;
    float D = (error - pid_param->last_error) * pid_param->kd;
    pid_param->integral = Limit(pid_param->limit_i, pid_param->integral + error);
    float I = pid_param->integral * pid_param->ki;
    pid_param->last_error = error;
    return Limit(pid_param->limit_out, P+I+D);
}

float Pitch_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pid_param)
{
    expect_state->pitch_rate = Pid_Controler(&(pid_param->outer), expect_state->pitch - actual_state->pitch);
    return Pid_Controler(&(pid_param->iner), expect_state->pitch_rate - actual_state->pitch_rate);
}

float Roll_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pid_param)
{
    expect_state->roll_rate = Pid_Controler(&(pid_param->outer), expect_state->roll - actual_state->roll);
    return Pid_Controler(&(pid_param->iner), expect_state->roll_rate - actual_state->roll_rate);
}

float Yaw_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pid_param)
{
    float error = 0.0f;

    if(expect_state->yaw - actual_state->yaw > 180.0f)
    {
        error = 360.0f - (expect_state->yaw - actual_state->yaw);
    }

    else if(expect_state->yaw - actual_state->yaw <  -180.0f)
    {
        error = 360.0f + (expect_state->yaw - actual_state->yaw);
    }
    else
    {
        error = expect_state->yaw - actual_state->yaw;
    }

    expect_state->yaw_rate = Pid_Controler(&(pid_param->outer), error);
    return Pid_Controler(&(pid_param->iner), expect_state->yaw_rate - actual_state->yaw_rate);
}