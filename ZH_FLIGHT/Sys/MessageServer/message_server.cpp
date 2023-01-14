/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-14 23:13:07
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "message_server.h"
#include <string.h>
void MessageServer::SetParser(MessageParser *interface, uint8_t ind)
{
    _parser[ind] = interface;
}

AC_RET MessageServer::GetMessage(Message *message)
{
    _buf = new Byte[message->length];
    if (_buf == nullptr || length == 0)
    {
        return AC_ERROR;
    }
    memcpy(_buf, message->data, length);
    return AC_OK;
}

void MessageServer::PraseMessage()
{
    MessageHead head = 0;

    head = _buf[0];
    for (int i = 0; i < MESSAGE_TYPE_NUM; ++i)
    {
        if (head == _head_list[i])
        {
            _parser[i]->ParseMessage(_buf, length);
            _parser[i]->HandOut();
            return;
        }
    }
}
