/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:47:30
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 01:15:07
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_server.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ac_queue.h"
#include "message.h"
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "main.h"
#include "message_parser.h"
#include "type.h"
#include "message_server.h"
#include "sys.h"

uint8_t MessageReceiveServer::_interface_ind = 0;
uint8_t MessageTransmitServer::_interface_ind = 0;

MessageReceiveServer::MessageReceiveServer(uint8_t len)
{
    _manager = new DataManager<Message>(len);
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
    Message message;
    MessageHead head = 0;

    _manager->Pop(&message);
    head = message.data[0];
    for (int i = 0; i < _interface_ind; ++i)
    {
        if (head == _parser[i]->GetHead())
        {
            _parser[i]->ParseMessage(message);
            return AC_OK;
        }
    }
    return AC_ERROR;
}

DataManager<Message> *MessageReceiveServer::GetMessageManager()
{
    return _manager;
}

MessageReceiveServer::~MessageReceiveServer()
{

    delete _manager;

    for (int i = 0; i < MESSAGE_TYPE_NUM; ++i)
    {
        delete _parser[i];
    }
}



MessageTransmitServer::MessageTransmitServer(uint8_t len)
{
    _manager = new DataManager<Message>(len);
}

void MessageTransmitServer::AddTransmitter(CommunicateInterface *interface)
{
    if (_interface_ind > MESSAGE_TRANSMIT_NUM)
    {
        return;
    }
    _interface[_interface_ind] = interface;
    _interface_ind++;
}

void MessageTransmitServer::RunTransmitService()
{
    Message message;
    _manager->Pop(&message);
    uint8_t mark = message.dec_port;
    for (int i = 0; i < _interface_ind; ++i)
    {
        if (_interface[i]->MatchMark(mark))
        {
            uint8_t try_times = 0;
            while (try_times < 100 && AC_OK != _interface[i]->Transmit(message.data, message.length))
            {
                try_times++;
            }
            return;
        }
    }
}

DataManager<Message> *MessageTransmitServer::GetMessageManager()
{
    return _manager;
}

MessageTransmitServer::~MessageTransmitServer()
{
    delete _manager;

    for (int i = 0; i < MESSAGE_TRANSMIT_NUM; ++i)
    {
        delete _interface[i];
    }
};