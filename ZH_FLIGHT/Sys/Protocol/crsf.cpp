/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 23:22:56
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-13 00:51:27
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\crsf.cpp
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#include "crsf.h"
#include "attitude_controller.h"
#include "rtwtypes.h"
#include "type.h"
#include <string.h>

#pragma pack(1)
struct _ChannelFormat
{
    unsigned ch0    : 11;
    unsigned ch1    : 11;
    unsigned ch2    : 11;
    unsigned ch3    : 11;
    unsigned ch4    : 11;
    unsigned ch5    : 11;
    unsigned ch6    : 11;
    unsigned ch7    : 11;
    unsigned ch8    : 11;
    unsigned ch9    : 11;
    unsigned ch10   : 11;
    unsigned ch11   : 11;
    unsigned ch12   : 11;
    unsigned ch13   : 11;
    unsigned ch14   : 11;
    unsigned ch15   : 11;
};
#pragma pack()

MessageHead CrsfParser::getHead()
{
    return CRSF_HEAD;
}

AC_RET CrsfParser::parseMessage(Message &message)
{
    _ChannelFormat format = {0};
    RemoteData data;
    if (message.data[0] != CRSF_HEAD || message.data[2] != CRSF_PAYLOAD || message.data[3] > message.length - 2)
    {
        return AC_ERROR;
    }
    memcpy(&format, message.data, sizeof(_ChannelFormat));

    _channel_data[0]    = format.ch0;
    _channel_data[1]    = format.ch1;
    _channel_data[2]    = format.ch2;
    _channel_data[3]    = format.ch3;
    _channel_data[4]    = format.ch4;
    _channel_data[5]    = format.ch5;
    _channel_data[6]    = format.ch6;
    _channel_data[7]    = format.ch7;
    _channel_data[8]    = format.ch8;
    _channel_data[9]    = format.ch9;
    _channel_data[10]   = format.ch10;
    _channel_data[11]   = format.ch11;
    _channel_data[12]   = format.ch12;
    _channel_data[13]   = format.ch13;
    _channel_data[14]   = format.ch14;
    _channel_data[15]   = format.ch15;

    memcpy(data.channel, _channel_data, sizeof(uint16_t) * TOTAL_CHANNEL_NUM);
    _manager->update(&data);
    return AC_OK;
}

void CrsfParser::setDataManager(void *manager)
{
    _manager = (DataManager<RemoteData>*)manager;
}
