/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-22 00:50:12
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 23:00:42
 * @FilePath: \ZH_FLIGHT\Sys\Communicate\communicate_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __COMMUNICATE_INTERFACE_H__
#define __COMMUNICATE_INTERFACE_H__

#include "type.h"
#include "config.h"
#include <stdint.h>

class CommunicateInterface
{
public:
    virtual AC_RET transmit(uint8_t *data, uint16_t length) = 0;
    virtual bool matchPort(uint8_t mark) = 0;
    virtual ~CommunicateInterface(){}
};

#endif