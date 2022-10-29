/*
 * @Description: 

 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:50:24
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-30 04:04:47
 * @FilePath: \H7B0d:\Git\ZH_flight\ZH_FLIGHT\Sys\Driver\z_usb.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __Z_USB_H__
#define __Z_USB_H__

#include "main.h"
#include "message_interface.h"
#include "config.h"

#define USB_MAX_RECEIVE 64

class Usb : virtual public MessageInterface
{
public:
    void Init(uint16_t receive_length_in) override;
    void Receive() override;
    uint8_t Send(uint8_t *data, uint16_t length) override;
    static void UsbHandle(uint8_t *buf);
    ~Usb();
    static uint8_t *receive_buf;
    static Message message;
    uint16_t receive_length;
};

#endif
