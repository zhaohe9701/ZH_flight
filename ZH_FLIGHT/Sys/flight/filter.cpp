/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-12 15:40:28
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-12 16:14:12
 * @FilePath: \ZH_FLIGHT\Sys\Flight\filter.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "filter.h"
#include "math_param.h"
#include <math.h>

void LowPassFilter::Init(float sample_freq, float cutoff_freq)
{
    SetCutoffFreq(sample_freq, cutoff_freq);
}

void LowPassFilter::SetCutoffFreq(float sample_freq, float cutoff_freq)
{
    float fr = sample_freq / cutoff_freq;
    float ohm = tanf(PI / fr);
    float c = 1.0f + 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm;
    _b0 = ohm * ohm / c;
    _b1 = 2.0f * _b0;
    _b2 = _b0;
    _a1 = 2.0f * (ohm * ohm - 1.0f) / c;
    _a2 = (1.0f - 2.0f * cosf(PI / 4.0f) * ohm + ohm * ohm) / c;
    _delay_element_1 = 0.0f;
    _delay_element_2 = 0.0f;
}

float LowPassFilter::Apply(float sample)
{
    float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;
	if (!isfinite(delay_element_0)) 
	{
		// don't allow bad values to propigate via the filter
		delay_element_0 = sample;
	}

	float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;

	_delay_element_2 = _delay_element_1;
	_delay_element_1 = delay_element_0;
	return output;
}

float LowPassFilter::Reset(float sample)
{
    float dval = sample / (_b0 + _b1 + _b2);
	_delay_element_1 = dval;
	_delay_element_2 = dval;
	return Apply(sample);
}