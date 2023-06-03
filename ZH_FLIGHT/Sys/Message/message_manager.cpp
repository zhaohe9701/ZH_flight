//
// Created by zhaohe on 2023/5/25.
//

#include <cstring>
#include "message_manager.h"

AC_RET MessageManager::transmit(char *buf, uint32_t len, uint8_t src_port, uint8_t dec_port, uint32_t timeout)
{
    uint32_t ptr = 0;
    uint8_t ind = 0;
    uint16_t seq = ++_src_seq;
    while (ptr < len)
    {
        Message message;
        if (len - ptr > MAX_MESSAGE_LENGTH)
        {
            memcpy(message.data, buf + ptr, MAX_MESSAGE_LENGTH);
            message.src_port = src_port;
            message.dec_port = dec_port;
            message.length = MAX_MESSAGE_LENGTH;
            message.ind = ind;
            message.seq = seq;
            if (AC_OK != _manager->push(&message, timeout))
            {
                return AC_ERROR;
            }
            ind++;
            ptr += MAX_MESSAGE_LENGTH;
        } else
        {
            memcpy(message.data, buf + ptr, len - ptr);
            message.src_port = src_port;
            message.dec_port = dec_port;
            message.length = len - ptr;
            message.ind = ind;
            message.seq = seq;
            if (AC_OK != _manager->push(&message, timeout))
            {
                return AC_ERROR;
            }
            ptr = len;
        }
    }
    return AC_OK;
}

AC_RET MessageManager::receive(char *buf, uint32_t &len, uint8_t src_port, uint8_t dec_port, uint32_t timeout)
{
    Message message;
    uint32_t tmp_len = 0;
    uint32_t count = 0;
    uint16_t seq = 0;
    uint8_t not_received_pkg_num = 0;
    if (AC_OK != _manager->pop(&message, timeout))
    {
        return AC_ERROR;
    }
    if (len < (uint32_t)message.ind * MAX_MESSAGE_LENGTH + (uint32_t)message.length)
    {
        len = 0;
        return AC_ERROR;
    }
    if (message.ind == message.pkg_num - 1)
    {
        tmp_len = message.ind * MAX_MESSAGE_LENGTH + message.length;
    }
    seq = message.seq;
    memcpy(buf + message.ind * MAX_MESSAGE_LENGTH, message.data, message.length);
    not_received_pkg_num = --message.pkg_num;
    while (not_received_pkg_num > 0)
    {
        if (AC_OK != _manager->pop(&message, timeout))
        {
            return AC_ERROR;
        }
        if (message.seq != seq)  /* 不是同一条消息，放回队列*/
        {
            count++;
            if (count >= _que_len)  /* 连续放回次数超过队列长度，抛弃，防止死锁 */
            {
                memset(buf, 0, len);
                return AC_ERROR;
            }
            if (AC_OK != _manager->push(&message, timeout))
            {
                return AC_ERROR;
            }
        } else
        {
            count = 0;
            if ((uint32_t)len < (uint32_t)message.ind * MAX_MESSAGE_LENGTH + (uint32_t)message.length)
            {
                len = 0;
                return AC_ERROR;
            }
            if (message.ind == message.pkg_num - 1)
            {
                tmp_len = message.ind * MAX_MESSAGE_LENGTH + message.length;
            }
            memcpy(buf + message.ind * MAX_MESSAGE_LENGTH, message.data, message.length);
            not_received_pkg_num--;
        }
    }
    len = tmp_len;
    return AC_OK;
}

MessageManager::MessageManager(uint32_t len)
{
    _manager = new DataManager<Message>(len);
    _que_len = len;
}
