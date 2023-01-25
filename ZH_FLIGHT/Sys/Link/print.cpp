/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:34:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 02:11:30
 * @FilePath: \ZH_FLIGHT\Sys\Link\print.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "print.h"
#include "message_interface.h"
#include "type.h"
#include <stdarg.h>
#include <stdio.h>
#include "global_var.h"

extern GlobalVar system_var;

Printer::Printer()
{
    _message.data = new Byte[MAX_PRINT_LENGTH];
}

void Printer::SetInterfaceMark(uint8_t mark)
{
    _mark = mark;
}

void Printer::Print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    _message.length = vsnprintf((char *)_message.data + 1, MAX_PRINT_LENGTH, (char *)format, args);
    va_end(args);
    _message.data[0] = _mark;
    osMessagePut(system_var.TRANSMIT_MESSAGE_QUEUE, (uint32_t)(&_message), 0);
}

Printer::~Printer()
{
    delete [] _message.data;
}
