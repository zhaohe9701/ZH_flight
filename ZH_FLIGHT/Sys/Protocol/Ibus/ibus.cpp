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

#define IBUS_HEAD 0x20

MessageHead IbusParser::getHead()
{
    return IBUS_HEAD;
}

AC_RET IbusParser::parseMessage(uint8_t *buf, uint32_t len)
{
    RemoteData data;

    if (buf[0] != IBUS_HEAD || buf[1] != 0x40 || len != IBUS_LENGTH)
    {
        return AC_ERROR;
    }
    _channel_data[0]  = ((uint16_t)(buf[3]  & 0x0F) << 8) | buf[2];
    _channel_data[1]  = ((uint16_t)(buf[5]  & 0x0F) << 8) | buf[4];
    _channel_data[2]  = ((uint16_t)(buf[7]  & 0x0F) << 8) | buf[6];
    _channel_data[3]  = ((uint16_t)(buf[9]  & 0x0F) << 8) | buf[8];
    _channel_data[4]  = ((uint16_t)(buf[11] & 0x0F) << 8) | buf[10];
    _channel_data[5]  = ((uint16_t)(buf[13] & 0x0F) << 8) | buf[12];
    _channel_data[6]  = ((uint16_t)(buf[15] & 0x0F) << 8) | buf[14];
    _channel_data[7]  = ((uint16_t)(buf[17] & 0x0F) << 8) | buf[16];
    _channel_data[8]  = ((uint16_t)(buf[19] & 0x0F) << 8) | buf[18];
    _channel_data[9]  = ((uint16_t)(buf[21] & 0x0F) << 8) | buf[20];
    _channel_data[10] = ((uint16_t)(buf[23] & 0x0F) << 8) | buf[22];
    _channel_data[11] = ((uint16_t)(buf[25] & 0x0F) << 8) | buf[24];

    _channel_data[12] = ((uint16_t)buf[27] << 8) | buf[26];
    _channel_data[13] = ((uint16_t)buf[29] << 8) | buf[28];

    _channel_data[14] = ((uint16_t)(buf[3]  & 0xF0) >> 4) | (uint16_t)(buf[5]  & 0xF0) | ((uint16_t)(buf[7]  & 0xF0) << 4);
    _channel_data[15] = ((uint16_t)(buf[9]  & 0xF0) >> 4) | (uint16_t)(buf[11] & 0xF0) | ((uint16_t)(buf[13] & 0xF0) << 4);
    _channel_data[16] = ((uint16_t)(buf[15] & 0xF0) >> 4) | (uint16_t)(buf[17] & 0xF0) | ((uint16_t)(buf[19] & 0xF0) << 4);
    _channel_data[17] = ((uint16_t)(buf[21] & 0xF0) >> 4) | (uint16_t)(buf[23] & 0xF0) | ((uint16_t)(buf[25] & 0xF0) << 4);

    memcpy(data.channel, _channel_data, sizeof(uint16_t) * TOTAL_CHANNEL_NUM);
    _manager->update(&data);
    return AC_OK;
}

void IbusParser::setDataManager(void *manager)
{
    _manager = (DataManager<RemoteData>*)manager;
}

