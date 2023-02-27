/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-21 23:28:46
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:16:38
 * @FilePath: \ZH_FLIGHT\Sys\MessageServer\message_server.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __MESSAGE_SERVER_H__
#define __MESSAGE_SERVER_H__


#include "config.h"
#include "type.h"
#include "communicate_interface.h"
#include "message_parser.h"
class MessageReceiveServer
{
public:
    void SetParser(MessageReceiveParser *interface, uint8_t ind);
    AC_RET RunReceiveService();
    void PraseMessage();
private:
    MessageReceiveParser *_parser[MESSAGE_TYPE_NUM] = {nullptr};
    MessageHead _head_list[MESSAGE_TYPE_NUM] = {0x00};
    Byte *_buf = nullptr;
    uint32_t length = 0;
};


class MessageTransmitServer
{
public:
    void AddTransmitter(CommunicateInterface *interface);
    void RunTransmitService();
private:
    CommunicateInterface *_interface[MESSAGE_TTANSMIT_NUM] = {nullptr};
    static uint8_t _interface_ind; 
};

#endif
