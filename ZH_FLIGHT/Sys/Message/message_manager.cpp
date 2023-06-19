//
// Created by zhaohe on 2023/5/25.
//

#include <cstring>
#include "message_manager.h"

AC_RET MessageManager::transmit(Message &message)
{
    uint32_t ptr = 0;
    uint8_t ind = 0;
    uint16_t seq = ++_src_seq;
    uint8_t pkg_num = message.len / MAX_MESSAGE_LENGTH;
    if (message.len % MAX_MESSAGE_LENGTH != 0)
    {
        pkg_num++;
    }
    while (ptr < message.len)
    {
        MessageData message_data;
        if (message.len - ptr > MAX_MESSAGE_LENGTH)
        {
            memcpy(message_data.data, message.buf + ptr, MAX_MESSAGE_LENGTH);
            message_data.src_port = message.src_port;
            message_data.dec_port = message.dec_port;
            message_data.length = MAX_MESSAGE_LENGTH;
            message_data.ind = ind;
            message_data.seq = seq;
            message_data.pkg_num = pkg_num;
            if (AC_OK != _manager->push(&message_data, message.timeout))
            {
                return AC_ERROR;
            }
            ind++;
            ptr += MAX_MESSAGE_LENGTH;
        } else
        {
            memcpy(message_data.data, message.buf + ptr, message.len - ptr);
            message_data.src_port = message.src_port;
            message_data.dec_port = message.dec_port;
            message_data.length = message.len - ptr;
            message_data.ind = ind;
            message_data.seq = seq;
            message_data.pkg_num = pkg_num;
            if (AC_OK != _manager->push(&message_data, AC_FOREVER))
            {
                return AC_ERROR;
            }
            ptr = message.len;
        }
    }
    return AC_OK;
}

AC_RET MessageManager::receive(Message &message)
{
    MessageData message_data;
    uint32_t tmp_len = 0;
    uint32_t count = 0;
    uint16_t seq = 0;
    uint8_t not_received_pkg_num = 0;
    if (AC_OK != _manager->pop(&message_data, message.timeout))
    {
        return AC_ERROR;
    }
    if (message.len < (uint32_t)message_data.ind * MAX_MESSAGE_LENGTH + (uint32_t)message_data.length)
    {
        message.len = 0;
        return AC_ERROR;
    }
    if (message_data.ind == message_data.pkg_num - 1)
    {
        tmp_len = message_data.ind * MAX_MESSAGE_LENGTH + message_data.length;
    }
    seq = message_data.seq;
    memcpy(message.buf + message_data.ind * MAX_MESSAGE_LENGTH, message_data.data, message_data.length);
    not_received_pkg_num = --message_data.pkg_num;
    while (not_received_pkg_num > 0)
    {
        if (AC_OK != _manager->pop(&message_data, message.timeout))
        {
            return AC_ERROR;
        }
        if (message_data.seq != seq)  /* 不是同一条消息，放回队列*/
        {
            count++;
            if (count >= _que_len)  /* 连续放回次数超过队列长度，抛弃，防止死锁 */
            {
                memset(message.buf, 0, message.len);
                return AC_ERROR;
            }
            if (AC_OK != _manager->push(&message_data, message.timeout))
            {
                return AC_ERROR;
            }
        } else
        {
            count = 0;
            if ((uint32_t)message.len < (uint32_t)message_data.ind * MAX_MESSAGE_LENGTH + (uint32_t)message_data.length)
            {
                message.len = 0;
                return AC_ERROR;
            }
            if (message_data.ind == message_data.pkg_num - 1)
            {
                tmp_len = message_data.ind * MAX_MESSAGE_LENGTH + message_data.length;
            }
            memcpy(message.buf + message_data.ind * MAX_MESSAGE_LENGTH, message_data.data, message_data.length);
            not_received_pkg_num--;
        }
    }
    message.len = tmp_len;
    return AC_OK;
}

MessageManager::MessageManager(uint32_t len)
{
    _manager = new DataManager<MessageData>(len);
    _que_len = len;
}

MessageManager::~MessageManager()
{
    delete _manager;
}
