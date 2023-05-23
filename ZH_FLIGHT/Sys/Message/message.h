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

#define END_INDEX 0

class Message
{
public:
    uint8_t src_port = 0x00;                /* 源地址 */
    uint8_t dec_port = 0x00;                /* 目的地址 */
    uint8_t ind = 0;                        /* 消息包索引，从1开始，0代表最后一个包 */
    uint8_t data[MAX_MESSAGE_LENGTH] = {0}; /* 消息体 */
    uint16_t length = 0;                    /* 消息体长度 */
    uint16_t seq = 0x0000;                  /* 消息序号 */
};

#endif