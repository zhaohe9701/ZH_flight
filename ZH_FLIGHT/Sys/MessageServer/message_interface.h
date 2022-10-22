/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:50:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-23 02:51:04
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_INTERFACE_H__
#define __MESSAGE_INTERFACE_H__

#include <stdint.h>

struct Message
{
    uint8_t *data = nullptr;
    uint16_t length = 0;
};

class MessageInterface
{
public:
    virtual void Init(uint16_t receive_length_in) = 0;
    virtual void Receive() = 0;
    virtual void Send(uint8_t *data, uint16_t length) = 0;
};

#endif