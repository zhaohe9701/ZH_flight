/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 23:47:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-26 00:08:20
 * @FilePath: \ZH_FLIGHT\Sys\global_var.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"

typedef struct _GlobalVar
{
    QueueHandle_t queue[QUEUE_NUM];
    SemaphoreHandle_t semaphore[SEMAPHORE_NUM];
} GlobalVar;