/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-25 22:53:22
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-25 23:50:42
 * @FilePath: \ZH_FLIGHT\Sys\Driver\pwm.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "pwm.h"

uint16_t Pwm::_PwmChannelToMark(uint16_t channel)
{
    uint16_t mark = 0x00U;
    switch (channel)
    {
    case 0x00U:
        mark = 0x01U;
        break;
    case 0x04U:
        mark = 0x02U;
        break;
    case 0x08U:
        mark = 0x04U;
        break;
    case 0x0CU:
        mark = 0x08U;
        break;
    default:
        break;
    }
    return mark;
}

Pwm* Pwm::controllers[PWM_NUM] = {nullptr};

void PwmCallbackHandle(TIM_HandleTypeDef *htim)
{
    for (int i = 0; i < PWM_NUM; ++i)
    {
        if (Pwm::controllers[i] != nullptr)
        {
            if (Pwm::controllers[i]->IsMe(htim))
            {
                Pwm::controllers[i]->Stop();
                break;
            }
        }
    }
}

extern "C"
{
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    PwmCallbackHandle(htim);
}
}

Pwm::Pwm(TIM_HandleTypeDef *htim, uint16_t channel)
{
    _htim = htim;
    _channel = channel;
    
}

void Pwm::Output(uint16_t *data, uint16_t length)
{
    HAL_TIM_PWM_Start_DMA(_htim, _channel, (uint32_t *)data, length);
}

void Pwm::Stop()
{
    HAL_TIM_PWM_Stop_DMA(_htim, _channel);
}

bool Pwm::IsMe(TIM_HandleTypeDef *htim)
{
    if (htim == _htim && htim->Channel == _PwmChannelToMark(_channel))
    {
        return true;
    }
    else
    {
        return false;
    }
}