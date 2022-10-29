/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 23:47:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 23:57:04
 * @FilePath: \ZH_FLIGHT\Sys\global_var.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define RETURN_IMMEDIATELY 0

typedef struct _Queue
{
    QueueHandle_t message_queue;
} Queue;

typedef struct _Semaphore
{
    SemaphoreHandle_t usb_semaphore;
} Semaphore;



typedef struct _GlobalVar
{
    Queue queue;
    Semaphore semaphore;
} GlobalVar;