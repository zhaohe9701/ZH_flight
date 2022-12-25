/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 23:43:52
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:35:19
 * @FilePath: \ZH_FLIGHT\Core\Src\sys_callback.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sys_callback.h"
#include "main.h"
#include "cmsis_os.h"
#include "global_var.h"
#include "config.h"

extern GlobalVar system_var;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == IMU1_EXIT_PIN)
    {
        if (NULL != system_var.IMU1_SEMAPHORE)
        {
            osSemaphoreRelease(system_var.IMU1_SEMAPHORE);
        }
    }
}