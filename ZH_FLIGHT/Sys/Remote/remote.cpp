/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-02 16:55:53
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-06 03:20:28
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "remote.h"


Remote::Remote(RemoteInterface *interface)
{
    _interface = interface;
}


void Remote::Init(RemoteParam *param)
{
    _x_channel = param->x_channel;
    _y_channel = param->y_channel;
    _z_channel = param->z_channel;
    _patterm_channel = param->patterm_channel;
    _attitude_scope = param->attitude_scope;
    _palstance_scope = param->palstance_scope;
}

void Remote::GetCommand(Command &command)
{

    command.attitude.Set(_interface->GetChannelFloatData(X_CHANNEL) * _attitude_scope,
                         _interface->GetChannelFloatData(Y_CHANNEL) * _attitude_scope,
                         _interface->GetChannelFloatData(Z_CHANNEL) * _attitude_scope);

    command.throttle = _interface->GetChannelFloatData(H_CHANNEL) * _throttle_scope;

    command.patterm = _interface->GetChannelSwitchData(PATTERN_CHANNEL);
}