/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 21:17:21
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ac_queue.h"
#include "message.h"
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "global_var.h"
#include "main.h"
#include "message_parser.h"
#include "type.h"
#include "message_server.h"

extern GlobalVar system_var;

uint8_t MessageTransmitServer::_interface_ind = 0;

MessageReceiveServer::MessageReceiveServer(AcQueue<Message> *queue)
{
    _queue = queue;
}

void MessageReceiveServer::AddParser(MessageReceiveParser *interface)
{
    _parser[_interface_ind] = interface;
    if (_interface_ind > MESSAGE_TYPE_NUM)
    {
        return;
    }
    _parser[_interface_ind] = interface;
    _interface_ind++;
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
            _parser[i]->Publish();
            delete [] _buf;
            return;
        }
    }
}

AcQueue<Message> *MessageReceiveServer::GetQueueHandle()
{
    return _queue;
}

MessageReceiveServer::~MessageReceiveServer()
{
    if (nullptr == _queue)
    {
        delete _queue;
    }

    for (int i = 0; i < MESSAGE_TYPE_NUM; ++i)
    {
        if (nullptr != _parser[i])
        {
            delete _parser[i];
        }
    }
}

MessageTransmitServer::MessageTransmitServer(AcQueue<Message> *queue)
{
    _queue = queue;
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
    _queue->Pop(&message);
    uint8_t mark = message.data[0];
    for(int i = 0; i < _interface_ind; ++i)
    {
        if (true == _interface[i]->MatchMark(mark))
        {
            AC_RET ret = _interface[i]->Transmit(message.data + 1, message.length);
            if (AC_OK != ret)
            {
                _queue->Push(&message);
            }
            return;
        }
    }
}

AcQueue<Message> *MessageTransmitServer::GetQueueHandle()
{
    return _queue;
}

MessageTransmitServer::~MessageTransmitServer()
{
    if (nullptr != _queue)
    {
        delete _queue;
    }
    for (int i = 0; i < MESSAGE_TTANSMIT_NUM; ++i)
    {
        if (nullptr != _interface[i])
        {
            delete _interface[i];
        }
    }
};