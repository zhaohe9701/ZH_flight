/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:34:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:30:18
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "print.h"
#include "ac_queue.h"
#include "message.h"
#include "stm32h7xx.h"
#include "type.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ERROR   "[ERROR]:"
#define INFO    "[INFO]:"
#define ERROR_LEN   8
#define INFO_LEN    7
Printer::Printer(AcQueue<Message> *queue)
{
    _queue = queue;
}

void Printer::SetInterfaceMark(uint8_t mark)
{
    _mark = mark;
}

void Printer::Print(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data + 1, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.data[0] = _mark;
    _queue->Push(&message);
}

void Printer::Error(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data + 1 + ERROR_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.data[0] = _mark;
    memcpy(message.data + 1, ERROR, ERROR_LEN);
    _queue->Push(&message);
}

void Printer::Info(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data + 1 + INFO_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.data[0] = _mark;
    memcpy(message.data + 1, INFO, INFO_LEN);
    _queue->Push(&message);
}