/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 22:36:59
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-20 23:25:28
 * @FilePath: \ZH_FLIGHT\Sys\AircraftState\aircraft_state.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "aircraft_state.h"


void State::Init()
{
    euler.Set(0.0f, 0.0f, 0.0f);
    palstance.Set(0.0f, 0.0f, 0.0f);
    velocity.Set(0.0f, 0.0f, 0.0f);
    gps_coor.Set(0.0f, 0.0f, 0.0f);
    relative_pos.Set(0.0f, 0.0f, 0.0f);
    acceleration.Set(0.0f, 0.0f, 0.0f);
    quarter.Set(0.0f, 0.0f, 0.0f, 1.0f);
}