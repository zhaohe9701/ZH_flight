/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-11-06 23:43:52
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-24 23:11:32
 * @FilePath: \ZH_FLIGHT\Core\Src\sys_callback.c
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "sys_callback.h"
#include "main.h"
#include "cmsis_os.h"

extern osSemaphoreId SensorSemaphoreHandle;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == SPI1_EXIT_Pin)
    {
        if (NULL != SensorSemaphoreHandle)
        {
            osSemaphoreRelease(SensorSemaphoreHandle);
        }
    }

}