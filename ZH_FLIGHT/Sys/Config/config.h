/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-08 01:24:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-22 23:22:10
 * @FilePath: \ZH_FLIGHT\Sys\Config\config.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdint.h>

/*------------------------配置区------------------------*/
#define MPU6000
#define D_SHOT

#define MOTOR_OUTPUT_MAX 100.0f
#define MOTOR_OUTPUT_MIN 0.0f
/*------------------------------------------------------*/


#ifdef D_SHOT
    #define THROTTLE_MAP(value) ((uint16_t)((value) * 20.47f))
#endif

#endif