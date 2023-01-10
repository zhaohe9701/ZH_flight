/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:28:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-11 00:04:17
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_server.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_SERVER_H__
#define __MESSAGE_SERVER_H__


#include "config.h"
#include "type.h"
#include "message_parse_interface.h"
class MessageServer
{
public:
    void SetParser(MessageParseInterface *interface, uint8_t ind);
    AC_RET GetMessage(Byte *buf, uint32_t length);
    void PraseMessage();
private:
    MessageParseInterface *_parser[MESSAGE_TYPE_NUM] = {nullptr};
    MessageHead _head_list[MESSAGE_TYPE_NUM] = {0x00};
    Byte *_buf = nullptr;
    uint32_t length = 0;
};




#endif
