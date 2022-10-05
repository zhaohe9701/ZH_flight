/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 23:20:08
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-06 03:17:35
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote_interface.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __REMOTE_INTERFACE_H__
#define __REMOTE_INTERFACE_H__

#include <stdint.h>

#define REMOTE_ERROR -1
#define REMOTE_OK 0


class RemoteInterface
{
public:
    virtual float GetChannelFloatData(uint8_t channel) = 0;
    virtual uint16_t GetChannelUintData(uint8_t channel) = 0;
    virtual int8_t GetChannelSwitchData(uint8_t channel) = 0;
    virtual void Receive(uint8_t *raw_data) = 0;
    virtual void GetCommand() = 0;
};


#endif