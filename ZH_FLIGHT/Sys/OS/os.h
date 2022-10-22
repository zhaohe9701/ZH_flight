/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 00:05:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 03:06:14
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __OS_H__
#define __OS_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define RETURN_IMMEDIATELY 0


struct Queue
{
    QueueHandle_t message_queue;
};

struct Semaphore
{
    SemaphoreHandle_t message_semaphore;
};

struct GlobalVar
{
    Queue queue;
    Semaphore semaphore;
};

class Os
{
public:
    static void Init();
};


#endif
