/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:50:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 03:05:10
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_INTERFACE_H__
#define __MESSAGE_INTERFACE_H__

#include "type.h"
#include <stdint.h>
struct Message
{
    Byte *data = nullptr;
    uint8_t finish = 0;
    uint32_t length = 0;
};

class MessageInterface
{
public:
    virtual void Init(uint16_t receive_length_in, uint8_t mark) = 0;
    virtual void Receive() = 0;
    virtual AC_RET Transmit(uint8_t *data, uint16_t length) = 0;
    virtual bool MatchMark(uint8_t mark) = 0;
};

#endif