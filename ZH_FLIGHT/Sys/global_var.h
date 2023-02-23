/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 23:47:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-26 01:10:34
 * @FilePath: \ZH_FLIGHT\Sys\global_var.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"

typedef struct _GlobalVar
{
    osMessageQueueId_t queue[QUEUE_NUM];
    osSemaphoreId_t semaphore[SEMAPHORE_NUM];
    osMutexId_t mutex[MUTEX_NUM];
} GlobalVar;