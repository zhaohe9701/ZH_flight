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
#include "data.h"
#include "type.h"
#include "data_manager.h"
#include "message_manager.h"
#include <stdint.h>

#define MAX_PRINT_LENGTH 1024

class Printer
{
public:
    explicit Printer(MessageManager *manager);
    void setDecPort(uint8_t port);
    void print(const char *format, ...);
    void error(const char *format, ...);
    void info(const char *format, ...);
    void transmit(char *buf, uint32_t len);
private:
    uint8_t _port = 0x00;
    MessageManager *_manager = nullptr;
    char _buffer[MAX_PRINT_LENGTH] = {0};
};

#endif