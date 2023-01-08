/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 23:47:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-08 23:47:22
 * @FilePath: \ZH_FLIGHT\Sys\global_var.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "cmsis_os.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"

typedef struct _GlobalVar
{
    osMessageQId queue[QUEUE_NUM];
    osSemaphoreId semaphore[SEMAPHORE_NUM];
    osMutexId mutex[MUTEX_NUM];
} GlobalVar;