/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-27 23:06:21
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 23:23:20
 * @FilePath: \ZH_FLIGHT\Sys\Protocol\ibus.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __IBUS_H__
#define __IBUS_H__

#define IBUS_LENGTH 32
#include "remote_interface.h"

class Ibus : virtual public RemoteInterface
{
public:
    uint8_t Parse(uint16_t *channel_data) override;
    void Receive(uint8_t *raw_data) override;
private:
    uint8_t _raw_data[IBUS_LENGTH] = {0};
};


#endif