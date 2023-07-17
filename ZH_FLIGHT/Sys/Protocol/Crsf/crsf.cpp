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
#include "default_attitude_controller.h"
#include "rtwtypes.h"
#include "type.h"
#include <string.h>

#define getBit(x, n) (((x) & ((unsigned long long)1 << (sizeof(x) * 8 - 1 - (n)))) != 0)
#define setBit(x, v, n) ((x) | ((v) << (n)))

AC_RET CrsfParser::_decode(const uint8_t *bin, uint16_t *axis, int bit_num, int axis_num)
{
    for (int i = 0; i < bit_num * axis_num; ++i)
    {
        int bi = i / 8;
        int di = i / bit_num;
        int b_offset = i % 8;
        int d_offset = i % bit_num;
        axis[di] = axis[di] | (getBit(bin[bi], b_offset) << (bit_num - d_offset - 1));
    }
    return AC_OK;
}

MessageHead CrsfParser::getHead()
{
    return CRSF_HEAD;
}

AC_RET CrsfParser::parseMessage(uint8_t *buf, uint32_t len)
{
    RemoteData data;
    if (buf[0] != CRSF_HEAD || buf[2] != CRSF_PAYLOAD || buf[1] >len - 2)
    {
        return AC_ERROR;
    }
    uint16_t axis[CRSF_CHANNEL_NUM] = {0};
    _decode(buf + 3, axis, CRSF_BIT_LEN, CRSF_CHANNEL_NUM);

    memcpy(data.channel, axis, sizeof(uint16_t) * TOTAL_CHANNEL_NUM);
    _manager->update(&data);
    return AC_OK;
}

void CrsfParser::setDataManager(void *manager)
{
    _manager = (DataManager<RemoteData>*)manager;
}
