/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:31:55
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 02:00:28
 * @FilePath: \ZH_FLIGHT\Sys\Link\print.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __PRINT_H__
#define __PRINT_H__

#include "message_interface.h"
#include "type.h"
#include <stdint.h>

#define MAX_PRINT_LENGTH 128

class Printer
{
public:
    Printer();
    void SetInterfaceMark(uint8_t mark);
    void Print(const char *format, ...);
    ~Printer();
private:
    Message _message;
    uint8_t _mark;
};

#endif