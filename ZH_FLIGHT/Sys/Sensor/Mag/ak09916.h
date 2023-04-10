/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-04-09 13:56:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-09 15:15:02
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Mag\ak09916.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AK09916_H__
#define __AK09916_H__

#include "mag.h"
#include "sensor_interface.h"

class Ak09916 : virtual public Mag
{
public:
    void Init();
    uint8_t GetId();
    void GetMagData(MagData &data);
private:
    uint8_t _last_bank = 0xFF;
    SensorInterface *_interface = nullptr;
    void _MasterWriteRag(uint8_t bank, uint8_t address, uint8_t value);
    void _MasterReadRag(uint8_t bank, uint8_t address, uint8_t length, uint8_t *buf);
    void _LoadIicRegParam();

};
#endif