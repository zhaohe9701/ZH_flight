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

#include <string.h>
#include "config.h"
#include "message_parser.h"
#include "type.h"
#include "message_server.h"
#include "sys.h"

uint8_t MessageReceiveServer::_interface_ind = 0;
uint8_t MessageTransmitServer::_interface_ind = 0;

MessageReceiveServer::MessageReceiveServer(uint8_t len)
{
    _manager = new MessageManager(len);
}

void MessageReceiveServer::addParser(MessageReceiveParser *interface)
{
    _parser[_interface_ind] = interface;
    if (_interface_ind > MESSAGE_TYPE_NUM)
    {
        return;
    }
    _parser[_interface_ind] = interface;
    _interface_ind++;
}

AC_RET MessageReceiveServer::runReceiveService()
{
    MessageHead head = 0;
    Message message;
    message.buf = _buffer;
    message.len = RECEIVE_SERVER_BUF_LEN;
    memset(_buffer, 0, 1024);
    _manager->receive(message);
    head = _buffer[0];
    debug_printer->info("HEAD:%x\n", head);
    for (int i = 0; i < _interface_ind; ++i)
    {
        if (head == _parser[i]->getHead())
        {
            _parser[i]->parseMessage(_buffer, message.len);
            return AC_OK;
        }
    }
    return AC_ERROR;
}

MessageManager *MessageReceiveServer::getMessageManager()
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
    _manager = new MessageManager(len);
}

void MessageTransmitServer::addTransmitter(CommunicateInterface *interface)
{
    if (_interface_ind > MESSAGE_TRANSMIT_NUM)
    {
        return;
    }
    _interface[_interface_ind] = interface;
    _interface_ind++;
}

void MessageTransmitServer::runTransmitService()
{
    Message message;

    message.buf = _buffer;
    message.len = TRANSMIT_SERVER_BUF_LEN;
    _manager->receive(message);
    uint8_t port = message.dec_port;
    for (int i = 0; i < _interface_ind; ++i)
    {
        if (_interface[i]->matchPort(port))
        {
            uint16_t try_times = 0;
            while (try_times < 1000 && AC_OK != _interface[i]->transmit(_buffer, message.len))
            {
                try_times++;
            }
            return;
        }
    }
}

MessageManager *MessageTransmitServer::getMessageManager()
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