/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:48:48
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-22 21:21:55
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
    void init(float kp, float ki, float kd, float out_limit, float int_limit, float dt);
    float update(float actual, float except);
    void enableDiffFilter(float sample_freq, float cutoff_freq);
    void disableDiffFilter();
    void enableDiffAhead();
    void disableDiffAhead();
    void reset();

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

    FirstOrderLPF _diff_filter;
    
    float _limit(float value, float limit);
};

#endif
