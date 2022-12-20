/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-11-22 22:56:55
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "type.h"
#include "remote_interface.h"
#include "config.h"

#define CHANNEL_NUM 5
class RemoteParam
{
public:
    uint8_t x_channel = X_CHANNEL;
    uint8_t y_channel = Y_CHANNEL;
    uint8_t z_channel = Z_CHANNEL;
    uint8_t h_channel = H_CHANNEL;
    uint8_t patterm_channel = PATTERN_CHANNEL;
    float attitude_scope = REMOTE_ATTITUDE_SCOPE;
    float position_scope = REMOTE_POSITION_SCOPE;
    float throttle_scope = REMOTE_THROTTLE_SCOPE;
    float palstance_scope = REMOTE_PALSTANCE_SCOPE;
};
class Command
{
public:
    Vec3 attitude;
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
    uint8_t _x_channel = X_CHANNEL;
    uint8_t _y_channel = Y_CHANNEL;
    uint8_t _z_channel = Z_CHANNEL;
    uint8_t _h_channel = H_CHANNEL;
    uint8_t _patterm_channel = PATTERN_CHANNEL;
    float _attitude_scope = REMOTE_ATTITUDE_SCOPE;
    float _position_scope = REMOTE_POSITION_SCOPE;
    float _throttle_scope = REMOTE_THROTTLE_SCOPE;
    float _palstance_scope = REMOTE_PALSTANCE_SCOPE;
};
#endif