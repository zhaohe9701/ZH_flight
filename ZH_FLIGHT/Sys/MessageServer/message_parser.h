/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-10 23:11:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-14 23:12:04
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_parser.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__

#include "type.h"

class MessageParser
{
public:
    virtual AC_RET ParseMessage(Byte *message, uint32_t length) = 0;
    virtual void SetDownStream(void *carrier) = 0;
    virtual void HandOut() = 0;
    //virtual void PackMessage() = 0;
};

#endif