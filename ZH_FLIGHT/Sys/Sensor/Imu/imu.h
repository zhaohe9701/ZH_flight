/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-07-07 23:41:39
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-04-10 23:04:02
 * @FilePath: \ZH_FLIGHT\Sys\Sensor\Imu\imu.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */

#ifndef __IMU_H__
#define __IMU_H__

#include "ac_tree.h"
#include "sensor_interface.h"
#include "type.h"
class ImuData
{
public:
    Vec3 acc;
    Vec3 gyr;
};
class Imu
{
public:
    virtual void Init() = 0;
    virtual uint8_t GetId() = 0;
    virtual float GetTemperature() = 0;
    virtual void GetGyroData(ImuData &data) = 0;
    virtual void GetAccData(ImuData &data) = 0;
    virtual AcTreeNode *CreateIndex();
private:
    AcMark _get_acc_mark = AC_OFF;
    AcMark _get_gyr_mark = AC_OFF;
    AcMark _get_temp_mark = AC_OFF;
    AcMark _get_id_mark = AC_OFF;
};

#endif