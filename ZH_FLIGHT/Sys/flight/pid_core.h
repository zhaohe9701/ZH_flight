/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:48:48
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-19 23:20:44
 * @FilePath: \ZH_FLIGHT\Sys\Flight\pid_core.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#ifndef __PID_CORE_H__
#define __PID_CORE_H__

#include "filter.h"
#include <stdint.h>

#define OFF 0
#define ON 1



class PidCore
{
public:
    void Init(float kp, float ki, float kd, float out_limit, float int_limit, float dt);
    float Update(float actual, float except);
    void EnableDiffFilter(float sample_freq, float cutoff_freq);
    void DisableDiffFilter();
    void EnableDiffAhead();
    void DisableDiffAhead();
    void Reset();

private:
    float _kp = 0.0f;
    float _ki = 0.0f;
    float _kd = 0.0f;


    float _pre_error = 0.0f;
    float _pre_actual = 0.0f;
    float _integral = 0.0f;
    float _out_limit = 0.0f;
    float _int_limit = 0.0f;

    float _dt = 0.0f;
    
    uint8_t _diff_filter_fw;
    uint8_t _diff_ahead_fw;

    LowPassFilter _diff_filter;
    
    float _Limit(float value, float limit);
};

#endif
