//
// Created by zhaohe on 2023/6/18.
//

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "os.h"
#include <stdint.h>

class Message
{
public:
    uint8_t *buf = nullptr;
    uint32_t len = 0;
    uint8_t src_port = 0x00;
    uint8_t dec_port = 0x00;
    uint32_t timeout = AC_FOREVER;
};

#endif
