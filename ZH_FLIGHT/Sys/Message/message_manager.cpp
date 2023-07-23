//
// Created by zhaohe on 2023/5/25.
//

#include <cstring>
#include "message_manager.h"

AC_RET MessageManager::transmit(Message &message)
{
    uint32_t ptr = 0;
    _mutex.get();
    while (ptr < message.len)
    {
        MessageData message_data;
        uint32_t temp_len = ((message.len - ptr) < MAX_MESSAGE_LENGTH ? (message.len - ptr) : MAX_MESSAGE_LENGTH);
        memcpy(message_data.data, message.buf + ptr, temp_len);
        message_data.src_port = message.src_port;
        message_data.dec_port = message.dec_port;
        message_data.length = temp_len;
        ptr += temp_len;
        if (ptr < message.len)
        {
            message_data.end_flag = 0x00;
        } else
        {
            message_data.end_flag = 0x01;
        }

        if (AC_OK != _manager->push(&message_data, AC_FOREVER))
        {
            _mutex.give();
            return AC_ERROR;
        }
        _mutex.give();
    }
    return AC_OK;
}

AC_RET MessageManager::receive(Message &message)
{
    MessageData message_data;
    uint32_t ptr = 0;

    do
    {
        if (AC_OK != _manager->pop(&message_data, message.timeout))
        {
            return AC_ERROR;
        }
        if (ptr + message_data.length > message.len)
        {
            return AC_ERROR;
        }
        memcpy(message.buf + ptr, message_data.data, message_data.length);
        ptr += message_data.length;
    } while (0x00 == message_data.end_flag);
    message.src_port = message_data.src_port;
    message.dec_port = message_data.dec_port;
    message.len = ptr;
    return AC_OK;
}

MessageManager::MessageManager(uint32_t len)
{
    _manager = new DataManager<MessageData>(len);
}

MessageManager::~MessageManager()
{
    delete _manager;
}
