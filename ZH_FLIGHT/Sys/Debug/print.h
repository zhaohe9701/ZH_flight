/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:31:55
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:18:47
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __PRINT_H__
#define __PRINT_H__

#include "ac_queue.h"
#include "message.h"
#include "type.h"
#include <stdint.h>

#define MAX_PRINT_LENGTH 128

class Printer
{
public:
    Printer(AcQueue<Message> *queue);
    void SetInterfaceMark(uint8_t mark);
    void Print(const char *format, ...);
    void Error(const char *format, ...);
    void Info(const char *format, ...);
private:
    uint8_t _mark;
    AcQueue<Message> *_queue = nullptr;
};

#endif