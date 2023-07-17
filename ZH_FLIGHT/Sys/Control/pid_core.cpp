/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-20 14:56:25
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-18 23:38:00
 * @FilePath: \ZH_FLIGHT\Sys\Flight\pid_core.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "pid_core.h"
void PidCore::init(float kp, float ki, float kd, float out_limit, float int_limit, float dt)
{
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _out_limit = out_limit;
    _int_limit = int_limit;
    _dt = dt;
}

float PidCore::update(float actual, float except)
{
    float error = 0.0f;
    float p = 0.0f;
    float i = 0.0f;
    float d = 0.0f;
    float output = 0.0f;

    error = except - actual;

    // 积分限幅
    _integral = _limit(_integral + error * _dt, _int_limit);

    // 比例
    p = _kp * error;

    // 微分
    
    // 微分先行
    if (_diff_ahead_fw == ON)
    {
        d = _kd * (actual - _pre_actual) / _dt;
    }
    else
    {
        d = _kd * (error - _pre_error) / _dt;
    }

    // 不完全微分
    if (_diff_filter_fw == ON)
    {
        d = _diff_filter.apply(d);
    }

    // 积分
    i = _ki * _integral;

    _pre_error = error;
    _pre_actual = actual;

    output = p + i + d;
    return output;
}

void PidCore::enableDiffFilter(float sample_freq, float cutoff_freq)
{
    _diff_filter_fw = ON;
    _diff_filter.init(sample_freq, cutoff_freq);
}

void PidCore::disableDiffFilter()
{
    _diff_filter_fw = OFF;
}

void PidCore::enableDiffAhead()
{
    _diff_ahead_fw = ON;
}

void PidCore::disableDiffAhead()
{
    _diff_ahead_fw = OFF;
}

float PidCore::_limit(float value, float limit)
{
    if (value > limit)
    {
        return limit;
    }
    if (value < -limit)
    {
        return -limit;
    }
    return value;
}

void PidCore::reset()
{

}
