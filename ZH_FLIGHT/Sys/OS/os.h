/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 00:05:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-02 00:01:37
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __OS_H__
#define __OS_H__

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "semphr.h"
inline int is_isr()
{
    uint32_t ipsr;
    __asm volatile ("mrs %0, IPSR" : "=r" (ipsr) );
    return (ipsr != 0);
}



typedef osMutexId_t Mutex;
typedef osSemaphoreId_t Semaphore;
typedef osThreadId_t Thread;
typedef osMessageQueueId_t Queue;

typedef TaskHandle_t ThreadHandle;
typedef QueueHandle_t QueueHandle;
typedef SemaphoreHandle_t SemaphoreHandle;
typedef TaskFunction_t AcFunction;
typedef UBaseType_t AcPriority;

#define AC_FOREVER 0xFFFFFFFF

#define IS_IN_IRQ() (is_isr() != 0)

#define acSignalSet(dec, value) osThreadFlagsSet(dec, value);
#define acSignalWait(value) osThreadFlagsWait(value, osFlagsWaitAll, osWaitForever);

#endif
