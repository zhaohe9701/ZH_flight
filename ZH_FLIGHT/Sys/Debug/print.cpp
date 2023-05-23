/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:34:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:17:21
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

Printer::Printer(DataManager<Message> *manager)
{
    _manager = manager;
}

void Printer::SetDecPort(uint8_t port)
{
    _port = port;
}

void Printer::Print(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.dec_port = _port;
    _manager->Push(&message);
}

void Printer::Error(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data + ERROR_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.length += ERROR_LEN;
    message.dec_port = _port;
    memcpy(message.data, ERROR, ERROR_LEN);
    _manager->Push(&message);
}

void Printer::Info(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.length = vsnprintf((char *)message.data + INFO_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.length += INFO_LEN;
    message.dec_port = _port;
    memcpy(message.data, INFO, INFO_LEN);
    _manager->Push(&message);
}

void Printer::Transmit(const char *buf, uint32_t len)
{
    Message message;
    uint32_t ptr = 0;

    message.dec_port = _port;
    while (ptr < len)
    {
        if (len - ptr >= MAX_MESSAGE_LENGTH)
        {
            message.length = MAX_MESSAGE_LENGTH;
            memcpy(message.data, buf + ptr, MAX_MESSAGE_LENGTH);
            _manager->Push(&message);
        }
        else
        {
            message.length = len - ptr;
            memcpy(message.data, buf + ptr, len - ptr);
            _manager->Push(&message);
        }
        ptr += MAX_MESSAGE_LENGTH;
    }
}
