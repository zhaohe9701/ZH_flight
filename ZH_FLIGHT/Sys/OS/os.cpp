/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 00:05:13
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 03:06:18
 * @FilePath: \ZH_FLIGHT\Sys\OS\os.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "os.h"
#include "config.h"
#include "message_interface.h"

GlobalVar g_glob_var;

void Os::Init()
{
    g_glob_var.queue.message_queue = xQueueCreate(MESSAGE_QUEUE_MAX_LENGTH, sizeof(Message));
    g_glob_var.semaphore.message_semaphore = xSemaphoreCreateBinary();
}

