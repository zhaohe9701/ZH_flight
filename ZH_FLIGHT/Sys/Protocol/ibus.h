/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 23:06:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-19 02:07:21
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __IBUS_H__
#define __IBUS_H__

#define IBUS_LENGTH 32
#define IBUS_CHANNEL_NUM 18

#define IBUS_SWITCH_0 1000
#define IBUS_SWITCH_1 1500
#define IBUS_SWITCH_2 2000

#define IBUS_TRANS_CHANNEL_FROM_INT_TO_FLOAT(x) ((float)((x) - 1500) / 1500.0f)
#include "remote_interface.h"

class Ibus : virtual public RemoteInterface
{
public:
    float GetChannelFloatData(uint8_t channel) override;
    uint16_t GetChannelUintData(uint8_t channel) override;
    int8_t GetChannelSwitchData(uint8_t channel) override;
    void Receive(uint8_t *raw_data) override;
private:
    uint8_t _raw_data[IBUS_LENGTH] = {0};
    uint16_t _channel_data[IBUS_CHANNEL_NUM] = {0};
    uint8_t _Parse();
};


#endif