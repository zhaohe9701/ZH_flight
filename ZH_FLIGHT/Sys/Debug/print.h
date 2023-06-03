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
#include "data_manager.h"
#include <cstdint>

#define MAX_PRINT_LENGTH MAX_MESSAGE_LENGTH

class Printer
{
public:
    explicit Printer(DataManager<Message> *manager);
    void setDecPort(uint8_t port);
    void print(const char *format, ...);
    void error(const char *format, ...);
    void info(const char *format, ...);
    void transmit(const char *buf, uint32_t len);
private:
    uint8_t _port = 0x00;
    DataManager<Message> *_manager = nullptr;
};

#endif