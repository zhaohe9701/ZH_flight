/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-25 02:04:02
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-24 00:03:14
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_pwm.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __PWM_H__
#define __PWM_H__

#include "config.h"


#include "main.h"


// extern "C"
// {
// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
// }



void PwmCallbackHandle(TIM_HandleTypeDef *htim);


class Pwm
{
public:
    void Init(TIM_HandleTypeDef *htim, uint16_t channel);
    void Output(uint16_t *data, uint16_t length);
    void Stop();
private:
    bool _IsMe(TIM_HandleTypeDef *htim);
    TIM_HandleTypeDef *_htim;
    uint16_t _channel;
    uint16_t _PwmChannelToMark(uint16_t channel);
};



#endif