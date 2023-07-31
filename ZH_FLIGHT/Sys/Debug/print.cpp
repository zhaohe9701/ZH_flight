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
#include "data.h"
#include "stm32h7xx.h"
#include "type.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define ERROR   "$[ERROR]:"
#define INFO    "$[INFO]:"
#define ERROR_LEN   9
#define INFO_LEN    8

Printer::Printer(MessageManager *manager)
{
    _manager = manager;
}

void Printer::setDecPort(uint8_t port)
{
    _port = port;
}

void Printer::print(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.len = vsnprintf(_buffer + 1, MAX_PRINT_LENGTH, (char *)format, args) + 1;
    va_end(args);
    _buffer[0] = '$';
    message.buf = (uint8_t *)_buffer;
    message.dec_port = _port;
    _manager->transmit(message);
}

void Printer::error(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.len = vsnprintf(_buffer + ERROR_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.len += ERROR_LEN;
    memcpy(_buffer, ERROR, ERROR_LEN);
    message.buf = (uint8_t *)_buffer;
    message.dec_port = _port;
    _manager->transmit(message);
}

void Printer::info(const char *format, ...)
{
    Message message;
    va_list args;
    va_start(args, format);
    message.len = vsnprintf(_buffer + INFO_LEN, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    message.len += INFO_LEN;
    memcpy(_buffer, INFO, INFO_LEN);
    message.buf = (uint8_t *)_buffer;
    message.dec_port = _port;
    _manager->transmit(message);
}

void Printer::transmit(uint8_t *buf, uint32_t len)
{
    Message message;
    message.len = len;
    message.buf = (uint8_t *)buf;
    message.dec_port = _port;
    _manager->transmit(message);
}
