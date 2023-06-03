/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:28:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 23:48:11
 * @FilePath: \ZH_FLIGHT\Sys\Message\message_server.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_SERVER_H__
#define __MESSAGE_SERVER_H__


#include "config.h"
#include "type.h"
#include "communicate_interface.h"
#include "message_parser.h"
#include "ac_queue.h"
#include "data_manager.h"

class MessageReceiveServer
{
public:
    explicit MessageReceiveServer(uint8_t len = MESSAGE_RECEIVE_QUEUE_LEN);
    void addParser(MessageReceiveParser *interface);
    AC_RET runReceiveService();
    DataManager<Message> *getMessageManager();
    ~MessageReceiveServer();
private:
    MessageReceiveParser *_parser[MESSAGE_TYPE_NUM] = {nullptr};
    static uint8_t _interface_ind;
    DataManager<Message> *_manager = nullptr;
};


class MessageTransmitServer
{
public:
    MessageTransmitServer(uint8_t len = MESSAGE_TRANSMIT_QUEUE_LEN);
    void addTransmitter(CommunicateInterface *interface);
    void runTransmitService();
    DataManager<Message> *getMessageManager();
    ~MessageTransmitServer();
private:
    CommunicateInterface *_interface[MESSAGE_TRANSMIT_NUM] = {nullptr};
    static uint8_t _interface_ind;
    DataManager<Message> *_manager = nullptr;
    
};

#endif
