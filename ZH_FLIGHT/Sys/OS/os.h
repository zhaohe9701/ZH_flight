/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 00:05:07
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:31:23
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __OS_H__
#define __OS_H__

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "semphr.h"

typedef osMutexId_t Mutex;
typedef osSemaphoreId_t Semaphore;
typedef osThreadId_t Thread;
typedef osMessageQueueId_t Queue;

#define AcLock(id) osMutexAcquire(id, osWaitForever)
#define AcUnLock(id) osMutexRelease(id)
#define AcWaitSemaphore(id) osSemaphoreAcquire(id, osWaitForever)
#define AcReleaseSemaphore(id) osSemaphoreRelease(id)
#define AcSignalSet(dec, value) osThreadFlagsSet(dec, value);
#define AcSignalWait(value) osThreadFlagsWait(value, osFlagsWaitAll, osWaitForever);

#endif
