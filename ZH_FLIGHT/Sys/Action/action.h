/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-13 22:55:26
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-12-19 23:38:07
 * @FilePath: \ZH_FLIGHT\Sys\Action\action.h
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#ifndef __ACTION_H__
#define __ACTION_H__

class Action
{
public:
    static void Initialize();
    static void Standby();
    static void Wait();
    static void ParamService();
    static void Calibration();
    static void Manual();
    static void Auto();
};


#endif