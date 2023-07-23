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
#ifndef __DATA_H__
#define __DATA_H__

#include "config.h"
#include <stdint.h>

class MessageData
{
public:
    uint8_t src_port = 0x00;                /* 源地址 */
    uint8_t dec_port = 0x00;                /* 目的地址 */
    uint8_t end_flag = 0x01;                /* 是否为最后一包 */
    uint8_t data[MAX_MESSAGE_LENGTH] = {0}; /* 消息体 */
    uint16_t length = 0;                    /* 消息体长度 */
};

#endif