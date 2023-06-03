//
// Created by zhaohe on 2023/5/25.
//

#include "type.h"
#include "data_manager.h"
#include "message.h"

#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__


class MessageManager
{
public:
    explicit MessageManager(uint32_t len = 10);
    AC_RET transmit(char *buf, uint32_t len, uint8_t src_port, uint8_t dec_port, uint32_t timeout = AC_FOREVER);
    AC_RET receive(char *buf, uint32_t &len, uint8_t src_port, uint8_t dec_port, uint32_t timeout = AC_FOREVER);
private:
    DataManager<Message> *_manager = nullptr;
    uint32_t _que_len = 0;
    uint16_t _src_seq = 0;
};

#endif
