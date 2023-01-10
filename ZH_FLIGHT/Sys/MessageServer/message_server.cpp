/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-11 00:04:02
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "message_server.h"
#include <string.h>
void MessageServer::SetParser(MessageParseInterface *interface, uint8_t ind)
{
    _parser[ind] = interface;
}

AC_RET MessageServer::GetMessage(Byte *buf, uint32_t length)
{
    _buf = new Byte[length];
    if (_buf == nullptr || length == 0)
    {
        return AC_ERROR;
    }
    memcpy(_buf, buf, length);
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
            _parser[i]->ParseMessage(_buf + 1, length - 1);
            _parser[i]->HandOut();
            return;
        }
    }
}
