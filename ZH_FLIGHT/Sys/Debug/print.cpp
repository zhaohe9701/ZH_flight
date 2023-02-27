/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:34:50
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:15:04
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "print.h"
#include "message.h"
#include "type.h"
#include <stdarg.h>
#include <stdio.h>
#include "global_var.h"

extern GlobalVar system_var;



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
    osMessageQueuePut(system_var.TRANSMIT_MESSAGE_QUEUE, &message, 0U, 0);
}
