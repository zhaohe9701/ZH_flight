/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-29 00:03:09
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:13:29
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "config.h"
#include <stdint.h>

class Message
{
public:
    uint8_t port = 0x00;
    uint8_t data[MAX_MESSAGE_LENGTH] = {0};
    uint16_t length = 0;
};

#endif