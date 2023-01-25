/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:51:45
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-25 03:24:04
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_usb.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_usb.h"
#include "os.h"
#include <stdint.h>
#include <string.h>
#include "type.h"
#include "usbd_cdc_if.h"

extern GlobalVar system_var;

extern "C"
{
void UsbIRQHandler(uint8_t *buf)
{
    Usb::UsbHandle(buf);
}
}


void Usb::UsbHandle(uint8_t *buf)
{

}



void Usb::Init(uint16_t receive_length_in, uint8_t mark)
{
    _mark = mark;

}

void Usb::Receive()
{

}

AC_RET Usb::Transmit(uint8_t *data, uint16_t length)
{
    if (USBD_OK == CDC_Transmit_HS(data, length))
    {
        return AC_OK;
    }
    else
    {
        return AC_ERROR;
    }
}

bool Usb::MatchMark(uint8_t mark)
{
    if (_mark == mark)
    {
        return true;
    }
    else
    {
        return false;
    }
}
Usb::~Usb()
{

}