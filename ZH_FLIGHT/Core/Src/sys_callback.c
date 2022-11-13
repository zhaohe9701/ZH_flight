/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 23:43:52
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-13 13:46:12
 * @FilePath: \H7B0\Core\Src\sys_callback.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sys_callback.h"
#include "main.h"
#include "cmsis_os.h"

extern osSemaphoreId SensorSemaphoreHandle;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GYRO_EXIT_Pin)
    {
        if (NULL != SensorSemaphoreHandle)
        {
            osSemaphoreRelease(SensorSemaphoreHandle);
        }
    }

}