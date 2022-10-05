/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-06 02:46:47
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdint.h>
#include "main.h"
/*------------------------配置区------------------------*/

/*传感器*/
#define MPU6000
#define MAGNETOMETER


/*电机*/
#define D_SHOT
#define PWM_NUM             4
#define MOTOR_OUTPUT_MAX    100.0f
#define MOTOR_OUTPUT_MIN    0.0f
#define MOTOR_NUM           4

#define MOTOR_1_TIM                 htim3
#define MOTOR_1_CHANNEL         TIM_CHANNEL_3

#define MOTOR_2_TIM                 htim3
#define MOTOR_2_CHANNEL         TIM_CHANNEL_4

#define MOTOR_3_TIM                 htim8
#define MOTOR_3_CHANNEL         TIM_CHANNEL_4

#define MOTOR_4_TIM                 htim8
#define MOTOR_4_CHANNEL         TIM_CHANNEL_3

/*遥控*/
#define X_CHANNEL       0
#define Y_CHANNEL       2
#define Z_CHANNEL       3
#define H_CHANNEL       1
#define PATTERN_CHANNEL 4

#define REMOTE_ATTITUDE_SCOPE  30.0f
#define REMOTE_POSITION_SCOPE  30.0f
#define REMOTE_THROTTLE_SCOPE  100.0f
#define REMOTE_PALSTANCE_SCOPE 60.0f
/*控制*/
#define MAIN_CYCLE_FREQ 500.0f


/*------------------------------------------------------*/


#ifdef D_SHOT
    #define THROTTLE_MAP(value) ((uint16_t)((value) * 20.47f))
#endif

#endif