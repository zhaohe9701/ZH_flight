#ifndef _PID_H__
#define _PID_H__

#include "flight.h"

#define PITCH_PARAM     0xF1
#define ROLL_PARAM      0xF2
#define YAW_PARAM       0xF3
#define PID_PARAM       0x81
typedef struct 
{
    float kp;
    float ki;
    float kd;
    float limit_out;
    float limit_i;
    float last_error;
    float integral;
} PidParam;

typedef struct 
{
    PidParam iner;
    PidParam outer;
} BicycloPidParam;



 


float Pitch_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pitch_pid_param);
float Roll_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pitch_pid_param);
float Yaw_Bicyclo_Pid(State *actual_state, State *expect_state, BicycloPidParam *pitch_pid_param);
void Pid_Init();
uint8_t Update_Pid_Param(union ParamTrans *param);




#endif