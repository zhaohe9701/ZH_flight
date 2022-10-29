/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:51:45
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-30 04:07:25
 * @FilePath: \H7B0d:\Git\ZH_flight\ZH_FLIGHT\Sys\Driver\z_usb.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_usb.h"
#include "os.h"
#include <string.h>
#include "usbd_cdc_if.h"

extern GlobalVar g_glob_var;

extern "C"
{
void UsbIRQHandler(uint8_t *buf)
{
    Usb::UsbHandle(buf);
}
}

uint8_t* Usb::receive_buf = nullptr;

void Usb::UsbHandle(uint8_t *buf)
{
    static uint16_t ptr = 0;
    memcpy(receive_buf + ptr, buf, USB_MAX_RECEIVE);
    ptr += USB_MAX_RECEIVE;
    if (buf[USB_MAX_RECEIVE - 1] == 0)
    {
        ptr = 0;
        xQueueSendFromISR(g_glob_var.queue.message_queue, &message, RETURN_IMMEDIATELY);
    }
}



void Usb::Init(uint16_t receive_length_in)
{
    if (nullptr == Usb::receive_buf)
    {
        Usb::receive_buf = new uint8_t[receive_length_in];
    }
    receive_length = receive_length_in;
    message.length = receive_length_in;
    message.data = new uint8_t[receive_length_in];
}

uint8_t Usb::Send(uint8_t *data, uint16_t length)
{
    return CDC_Transmit_FS(data, length);
}

Usb::~Usb()
{
    if (nullptr != receive_buf)
    {
        delete[] receive_buf;
    }
    if (nullptr != message.data)
    {
        delete[] message.data;
    }
}