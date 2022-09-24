/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-25 02:04:02
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-25 02:26:47
 * @FilePath: \ZH_FLIGHT\Sys\Driver\pwm.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __PWM_H__
#define __PWM_H__
#include "main.h"
void PwmCallbackHandle(TIM_HandleTypeDef *htim);
class Pwm
{
    friend void PwmCallbackHandle(TIM_HandleTypeDef *htim);
public:
    Pwm(TIM_HandleTypeDef *htim, unsigned int channel, uint16_t length);
    void Output(uint16_t *data);
    void Stop();
private:

};


#endif