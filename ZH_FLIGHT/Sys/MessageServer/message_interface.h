/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:50:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-29 00:05:04
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_INTERFACE_H__
#define __MESSAGE_INTERFACE_H__

#include "type.h"
#include "config.h"
#include <stdint.h>
#include "message.h"

class MessageInterface
{
public:
    virtual AC_RET Transmit(uint8_t *data, uint16_t length) = 0;
    virtual bool MatchMark(uint8_t mark) = 0;
};

#endif