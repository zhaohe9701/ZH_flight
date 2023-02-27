/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-23 16:31:55
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:14:33
 * @FilePath: \ZH_FLIGHT\Sys\Debug\print.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __PRINT_H__
#define __PRINT_H__

#include "message.h"
#include "type.h"
#include <stdint.h>

#define MAX_PRINT_LENGTH 128

class Printer
{
public:
    void SetInterfaceMark(uint8_t mark);
    void Print(const char *format, ...);
private:
    uint8_t _mark;
};

#endif