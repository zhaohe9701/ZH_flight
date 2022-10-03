/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-03 02:16:08
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "ibus.h"
#include <string.h>

uint8_t Ibus::Parse(uint16_t *channel_data)
{
    if (_raw_data[0] == 0x20 && _raw_data[1] == 0x40)
    {
        channel_data[0] = ((uint16_t)(_raw_data[3] & 0x0F) << 8) | _raw_data[2];
        channel_data[1] = ((uint16_t)(_raw_data[5] & 0x0F) << 8) | _raw_data[4];
        channel_data[2] = ((uint16_t)(_raw_data[7] & 0x0F) << 8) | _raw_data[6];
        channel_data[3] = ((uint16_t)(_raw_data[9] & 0x0F) << 8) | _raw_data[8];
        channel_data[4] = ((uint16_t)(_raw_data[11] & 0x0F) << 8) | _raw_data[10];
        channel_data[5] = ((uint16_t)(_raw_data[13] & 0x0F) << 8) | _raw_data[12];
        channel_data[6] = ((uint16_t)(_raw_data[15] & 0x0F) << 8) | _raw_data[14];
        channel_data[7] = ((uint16_t)(_raw_data[17] & 0x0F) << 8) | _raw_data[16];
        channel_data[8] = ((uint16_t)(_raw_data[19] & 0x0F) << 8) | _raw_data[18];
        channel_data[9] = ((uint16_t)(_raw_data[21] & 0x0F) << 8) | _raw_data[20];
        channel_data[10] = ((uint16_t)(_raw_data[23] & 0x0F) << 8) | _raw_data[22];
        channel_data[11] = ((uint16_t)(_raw_data[25] & 0x0F) << 8) | _raw_data[24];

        channel_data[12] = ((uint16_t)_raw_data[27] << 8) | _raw_data[26];
        channel_data[13] = ((uint16_t)_raw_data[29] << 8) | _raw_data[28];
    
        channel_data[14] = ((uint16_t)(_raw_data[3] & 0xF0) >> 4) | (uint16_t)(_raw_data[5] & 0xF0) | ((uint16_t)(_raw_data[7] & 0xF0) << 4);
        channel_data[15] = ((uint16_t)(_raw_data[9] & 0xF0) >> 4) | (uint16_t)(_raw_data[11] & 0xF0) | ((uint16_t)(_raw_data[13] & 0xF0) << 4);
        channel_data[16] = ((uint16_t)(_raw_data[15] & 0xF0) >> 4) | (uint16_t)(_raw_data[17] & 0xF0) | ((uint16_t)(_raw_data[19] & 0xF0) << 4);
        channel_data[17] = ((uint16_t)(_raw_data[21] & 0xF0) >> 4) | (uint16_t)(_raw_data[23] & 0xF0) | ((uint16_t)(_raw_data[25] & 0xF0) << 4);
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
}