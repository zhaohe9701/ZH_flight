/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:24:01
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ibus.h"
#include "config.h"
#include "data_manager.h"
#include "remote_data.h"
#include <string.h>


AC_RET IbusParser::ParseMessage(Byte *message, uint32_t length)
{
    if (message[0] != 0x20 || message[1] != 0x40 || length != IBUS_LENGTH)
    {
        return AC_ERROR;
    }
    _channel_data[0]  = ((uint16_t)(message[3]  & 0x0F) << 8) | message[2];
    _channel_data[1]  = ((uint16_t)(message[5]  & 0x0F) << 8) | message[4];
    _channel_data[2]  = ((uint16_t)(message[7]  & 0x0F) << 8) | message[6];
    _channel_data[3]  = ((uint16_t)(message[9]  & 0x0F) << 8) | message[8];
    _channel_data[4]  = ((uint16_t)(message[11] & 0x0F) << 8) | message[10];
    _channel_data[5]  = ((uint16_t)(message[13] & 0x0F) << 8) | message[12];
    _channel_data[6]  = ((uint16_t)(message[15] & 0x0F) << 8) | message[14];
    _channel_data[7]  = ((uint16_t)(message[17] & 0x0F) << 8) | message[16];
    _channel_data[8]  = ((uint16_t)(message[19] & 0x0F) << 8) | message[18];
    _channel_data[9]  = ((uint16_t)(message[21] & 0x0F) << 8) | message[20];
    _channel_data[10] = ((uint16_t)(message[23] & 0x0F) << 8) | message[22];
    _channel_data[11] = ((uint16_t)(message[25] & 0x0F) << 8) | message[24];

    _channel_data[12] = ((uint16_t)message[27] << 8) | message[26];
    _channel_data[13] = ((uint16_t)message[29] << 8) | message[28];

    _channel_data[14] = ((uint16_t)(message[3]  & 0xF0) >> 4) | (uint16_t)(message[5]  & 0xF0) | ((uint16_t)(message[7]  & 0xF0) << 4);
    _channel_data[15] = ((uint16_t)(message[9]  & 0xF0) >> 4) | (uint16_t)(message[11] & 0xF0) | ((uint16_t)(message[13] & 0xF0) << 4);
    _channel_data[16] = ((uint16_t)(message[15] & 0xF0) >> 4) | (uint16_t)(message[17] & 0xF0) | ((uint16_t)(message[19] & 0xF0) << 4);
    _channel_data[17] = ((uint16_t)(message[21] & 0xF0) >> 4) | (uint16_t)(message[23] & 0xF0) | ((uint16_t)(message[25] & 0xF0) << 4);

    
    return AC_OK;
}

void IbusParser::SetDataManager(void *manager)
{
    _manager = (DataManager<RemoteData>*)manager;
}

void IbusParser::Publish()
{
    RemoteData data;
    memcpy(data.channel, _channel_data, sizeof(uint16_t) * TOTAL_CHANNEL_NUM);
    _manager->Update(&data);
}
