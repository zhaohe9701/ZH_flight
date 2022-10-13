/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-10-13 22:55:26
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-10-13 23:00:21
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
    static void Wite();
    static void ParamService();
    static void Calibration();
    static void Manual();
    static void Auto();
};


#endif