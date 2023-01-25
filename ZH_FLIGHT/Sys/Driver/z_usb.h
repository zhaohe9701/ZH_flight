/*
 * @Description: 

 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:50:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 03:24:41
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_usb.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_USB_H__
#define __Z_USB_H__

#include "main.h"
#include "message_interface.h"
#include "config.h"
#include <stdint.h>

#define USB_MAX_RECEIVE 64

class Usb : virtual public MessageInterface
{
public:
    void Init(uint16_t receive_length_in, uint8_t mark) override;
    void Receive() override;
    AC_RET Transmit(uint8_t *data, uint16_t length) override;
    bool MatchMark(uint8_t mark) override;
    ~Usb();

    static void UsbHandle(uint8_t *buf);
    
    uint16_t receive_length;
    uint8_t _mark;
};

#endif
