/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-10 23:11:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-23 17:16:15
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_parser.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__

#include "message_interface.h"
#include "type.h"
#include <stdint.h>

class MessageReceiveParser
{
public:
    virtual AC_RET ParseMessage(Byte *message, uint32_t length) = 0;
    virtual void SetDes(void *carrier) = 0;
    virtual void HandOut() = 0;
};

class MessageTransmitParser
{
public:
    virtual AC_RET ParseMessage(void *message) = 0;
    virtual void SetInterfaceMark(uint8_t mark) = 0;
    virtual void HandOut() = 0;
};
#endif