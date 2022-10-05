/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-06 03:18:02
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ibus.h"
#include <string.h>

uint8_t Ibus::_Parse()
{
    if (_raw_data[0] == 0x20 && _raw_data[1] == 0x40)
    {
        _channel_data[0] = ((uint16_t)(_raw_data[3] & 0x0F) << 8) | _raw_data[2];
        _channel_data[1] = ((uint16_t)(_raw_data[5] & 0x0F) << 8) | _raw_data[4];
        _channel_data[2] = ((uint16_t)(_raw_data[7] & 0x0F) << 8) | _raw_data[6];
        _channel_data[3] = ((uint16_t)(_raw_data[9] & 0x0F) << 8) | _raw_data[8];
        _channel_data[4] = ((uint16_t)(_raw_data[11] & 0x0F) << 8) | _raw_data[10];
        _channel_data[5] = ((uint16_t)(_raw_data[13] & 0x0F) << 8) | _raw_data[12];
        _channel_data[6] = ((uint16_t)(_raw_data[15] & 0x0F) << 8) | _raw_data[14];
        _channel_data[7] = ((uint16_t)(_raw_data[17] & 0x0F) << 8) | _raw_data[16];
        _channel_data[8] = ((uint16_t)(_raw_data[19] & 0x0F) << 8) | _raw_data[18];
        _channel_data[9] = ((uint16_t)(_raw_data[21] & 0x0F) << 8) | _raw_data[20];
        _channel_data[10] = ((uint16_t)(_raw_data[23] & 0x0F) << 8) | _raw_data[22];
        _channel_data[11] = ((uint16_t)(_raw_data[25] & 0x0F) << 8) | _raw_data[24];

        _channel_data[12] = ((uint16_t)_raw_data[27] << 8) | _raw_data[26];
        _channel_data[13] = ((uint16_t)_raw_data[29] << 8) | _raw_data[28];
    
        _channel_data[14] = ((uint16_t)(_raw_data[3] & 0xF0) >> 4) | (uint16_t)(_raw_data[5] & 0xF0) | ((uint16_t)(_raw_data[7] & 0xF0) << 4);
        _channel_data[15] = ((uint16_t)(_raw_data[9] & 0xF0) >> 4) | (uint16_t)(_raw_data[11] & 0xF0) | ((uint16_t)(_raw_data[13] & 0xF0) << 4);
        _channel_data[16] = ((uint16_t)(_raw_data[15] & 0xF0) >> 4) | (uint16_t)(_raw_data[17] & 0xF0) | ((uint16_t)(_raw_data[19] & 0xF0) << 4);
        _channel_data[17] = ((uint16_t)(_raw_data[21] & 0xF0) >> 4) | (uint16_t)(_raw_data[23] & 0xF0) | ((uint16_t)(_raw_data[25] & 0xF0) << 4);
        return REMOTE_OK;
    }
    else
    {
        return REMOTE_ERROR;
    }
}

void Ibus::Receive(uint8_t *raw_data)
{
    memcpy(_raw_data, raw_data, IBUS_LENGTH);
    _Parse();
}

float Ibus::GetChannelFloatData(uint8_t channel)
{
    return IBUS_TRANS_CHANNEL_FROM_INT_TO_FLOAT(_channel_data[channel]);
}

uint16_t Ibus::GetChannelUintData(uint8_t channel)
{
    return _channel_data[channel];
}

int8_t Ibus::GetChannelSwitchData(uint8_t channel)
{
    int8_t switch_data = -1;
    switch (_channel_data[channel])
    {
    case IBUS_SWITCH_0:
        switch_data = 0;
        break;
    case IBUS_SWITCH_1:
        switch_data = 1;
        break;
    case IBUS_SWITCH_2:
        switch_data = 2;
        break;
    default:
        break;
    }
    return switch_data;
}