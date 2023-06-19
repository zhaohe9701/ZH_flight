/*
 * @Description: 

 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:50:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-02-27 22:59:56
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_usb.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_USB_H__
#define __Z_USB_H__

#include "main.h"
#include "communicate_interface.h"
#include "config.h"
#include "type.h"
#include <stdint.h>

class Usb : virtual public CommunicateInterface
{
public:
    Usb(uint8_t mark);
    AC_RET transmit(uint8_t *data, uint16_t length) override;
    bool matchPort(uint8_t mark) override;
    
    uint8_t _mark;
};

#endif
