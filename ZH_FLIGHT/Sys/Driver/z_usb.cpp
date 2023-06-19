/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-23 22:51:45
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-27 00:15:05
 * @FilePath: \ZH_FLIGHT\Sys\Driver\z_usb.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "z_usb.h"
#include "aircraft_state.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "main.h"
#include "type.h"
#include "usbd_cdc_if.h"

Usb::Usb(uint8_t mark)
{
    _mark = mark;
}

AC_RET Usb::transmit(uint8_t *data, uint16_t length)
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

bool Usb::matchPort(uint8_t mark)
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