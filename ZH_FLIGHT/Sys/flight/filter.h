/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-12 15:30:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-15 23:47:44
 * @FilePath: \ZH_FLIGHT\Sys\Flight\filter.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __FILTER_H__
#define __FLITER_H__

#define IIR_SHIFT   8

class LowPassFilter
{
public:
	void Init(float sample_freq, float cutoff_freq);
	float Apply(float sample);
	float Reset(float sample);

private:
    float _a1 = 0.0f;
	float _a2 = 0.0f;
	float _b0 = 0.0f;
	float _b1 = 0.0f;
	float _b2 = 0.0f;
	float _delay_element_1 = 0.0f;
	float _delay_element_2 = 0.0f;
	void SetCutoffFreq(float sample_freq, float cutoff_freq);
};

#endif
