/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-12 21:14:03
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-12 21:22:05
 * @FilePath: \ZH_FLIGHT\Sys\Remote\remote_data.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __REMOTE_DATA_H__
#define __REMOTE_DATA_H__

#include "config.h"
#include "type.h"

class RemoteData
{
public:
    uint16_t channel[TOTAL_CHANNEL_NUM];
};

#endif