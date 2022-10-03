/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-04 02:41:20
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "type.h"
#include "remote_interface.h"
#include "config.h"
class RemoteParam
{
    uint8_t pitch_channel = PITCH_CHANNEL;
    uint8_t roll_channel = ROLL_CHANNEL;
    uint8_t yaw_channel = YAW_CHANNEL;
    uint8_t patterm_channel = PATTERN_CHANNEL;
};
class Command
{
    Vec3<float> attitude;
    float throttle;
    uint8_t patterm;
};

class Remote
{
public:
    Remote(RemoteInterface *interface);
    void Init(RemoteParam *param);
    void GetCommand(Command &command);
private:
    RemoteInterface *_interface = nullptr;
    uint8_t _pitch_channel = PITCH_CHANNEL;
    uint8_t _roll_channel = ROLL_CHANNEL;
    uint8_t _yaw_channel = YAW_CHANNEL;
    uint8_t _patterm_channel = PATTERN_CHANNEL;
};
#endif