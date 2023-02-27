/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:18:22
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "message.h"
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "global_var.h"
#include "main.h"
#include "type.h"
#include "message_server.h"

extern GlobalVar system_var;

uint8_t MessageTransmitServer::_interface_ind = 0;

void MessageReceiveServer::SetParser(MessageReceiveParser *interface, uint8_t ind)
{
    _parser[ind] = interface;
}

AC_RET MessageReceiveServer::RunReceiveService()
{
    Message *message = nullptr;
    osMessageQueueGet(system_var.RECEIVE_MESSAGE_QUEUE, message, NULL, osWaitForever);
    _buf = new Byte[message->length];
    if (_buf == nullptr || length == 0)
    {
        return AC_ERROR;
    }
    memcpy(_buf, message->data, length);
    return AC_OK;
}

void MessageReceiveServer::PraseMessage()
{
    MessageHead head = 0;

    head = _buf[0];
    for (int i = 0; i < MESSAGE_TYPE_NUM; ++i)
    {
        if (head == _head_list[i])
        {
            _parser[i]->ParseMessage(_buf, length);
            _parser[i]->HandOut();
            delete [] _buf;
            return;
        }
    }
}


void MessageTransmitServer::AddTransmitter(CommunicateInterface *interface)
{
    if (_interface_ind > MESSAGE_TTANSMIT_NUM)
    {
        return;
    }
    _interface[_interface_ind] = interface;
    _interface_ind++;
}

void MessageTransmitServer::RunTransmitService()
{
    Message message;
    osMessageQueueGet(system_var.TRANSMIT_MESSAGE_QUEUE, &message, NULL, osWaitForever);
    uint8_t mark = message.data[0];
    for(int i = 0; i < _interface_ind; ++i)
    {
        if (true == _interface[i]->MatchMark(mark))
        {
            AC_RET ret = _interface[i]->Transmit(message.data + 1, message.length);
            if (AC_OK != ret)
            {
                osMessageQueuePut(system_var.TRANSMIT_MESSAGE_QUEUE, &message, 0U, 0);
            }
            return;
        }
    }
}