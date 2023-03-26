/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:28:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 20:58:40
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
class MessageReceiveServer
{
public:
    MessageReceiveServer(AcQueue<Message> *_queue);
    void AddParser(MessageReceiveParser *interface);
    AC_RET RunReceiveService();
    void PraseMessage();
    AcQueue<Message> *GetQueueHandle();
    ~MessageReceiveServer();
private:
    MessageReceiveParser *_parser[MESSAGE_TYPE_NUM] = {nullptr};
    MessageHead _head_list[MESSAGE_TYPE_NUM] = {0x00};
    Byte *_buf = nullptr;
    uint32_t length = 0;
    static uint8_t _interface_ind;
    AcQueue<Message> *_queue = nullptr;
};


class MessageTransmitServer
{
public:
    MessageTransmitServer(AcQueue<Message> *queue);
    void AddTransmitter(CommunicateInterface *interface);
    void RunTransmitService();
    AcQueue<Message> *GetQueueHandle();
    ~MessageTransmitServer();
private:
    CommunicateInterface *_interface[MESSAGE_TTANSMIT_NUM] = {nullptr};
    static uint8_t _interface_ind;
    AcQueue<Message> *_queue = nullptr;
    
};

#endif
