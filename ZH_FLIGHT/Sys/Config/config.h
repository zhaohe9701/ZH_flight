/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 00:18:30
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdint.h>
#include "main.h"
/*------------------------配置区------------------------*/
#define MPU6000
#define MAGNETOMETER
#define D_SHOT

#define MOTOR_OUTPUT_MAX 100.0f
#define MOTOR_OUTPUT_MIN 0.0f
#define MOTOR_NUM 4

#define MOTOR_1_TIM                 htim3
#define MOTOR_1_CHANNEL         TIM_CHANNEL_3

#define MOTOR_2_TIM                 htim3
#define MOTOR_2_CHANNEL         TIM_CHANNEL_4

#define MOTOR_3_TIM                 htim8
#define MOTOR_3_CHANNEL         TIM_CHANNEL_4

#define MOTOR_4_TIM                 htim8
#define MOTOR_4_CHANNEL         TIM_CHANNEL_3


#define MAIN_CYCLE_FREQ 500.0f

#define PWM_NUM 4
/*------------------------------------------------------*/


#ifdef D_SHOT
    #define THROTTLE_MAP(value) ((uint16_t)((value) * 20.47f))
#endif

#endif