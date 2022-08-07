/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-08-07 22:30:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-08-07 22:40:29
 * @FilePath: \ZH_FLIGHT\Sys\flight\state.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __STATE_H__
#define __STATE_H__

#define PATCH x
#define ROLL y
#define YAW z

#include "type.h"



class State
{
public:
    Vec3<float> euler;
    Vec3<float> vel;
    Vec3<float> gps_coor;
    Vec3<float> relative_pos;
};



#endif

