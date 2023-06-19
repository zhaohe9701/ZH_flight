/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-01-10 23:11:06
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:52:11
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_parser.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__

#include "communicate_interface.h"
#include "type.h"
#include "data.h"
#include <stdint.h>

class MessageReceiveParser
{
public:
    virtual AC_RET parseMessage(uint8_t *buf, uint32_t len) = 0;
    virtual MessageHead getHead() = 0;
    virtual void setDataManager(void *manager) = 0;
    virtual ~MessageReceiveParser()= default;
};

class MessageTransmitParser
{
public:
    virtual AC_RET parseMessage(void *message) = 0;
    virtual void setInterfaceMark(uint8_t mark) = 0;
    virtual void publish() = 0;
};
#endif