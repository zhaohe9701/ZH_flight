/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 19:32:29
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 19:41:06
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote_channel.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __REMOTE_CHANNEL_H__
#define __REMOTE_CHANNEL_H__

#include "ac_tree.h"
#include "config.h"
#include "type.h"

class RemoteChannelMapper
{
private:
    uint8_t _channel_map[TOTAL_CHANNEL_NUM];
public:
    RemoteChannelMapper();
    AcTreeNode* ParamRegister();
    ~RemoteChannelMapper();
};

#endif
