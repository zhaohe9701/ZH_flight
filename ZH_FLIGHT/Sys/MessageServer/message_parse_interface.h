/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-10 23:11:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-11 00:03:30
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_parse_interface.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_PARSE_INTERFACE_H__
#define __MESSAGE_PARSE_INTERFACE_H__

#include "type.h"

class MessageParseInterface
{
public:
virtual void ParseMessage(Byte *message, uint32_t length) = 0;
virtual void SetCarrier(void *carrier) = 0;
virtual void HandOut() = 0;
};

#endif