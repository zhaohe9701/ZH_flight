/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 22:36:59
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-01-05 23:49:19
 * @FilePath: \ZH_FLIGHT\Sys\AircraftState\aircraft_state.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "aircraft_state.h"
#include <string.h>




AC_RET ExpectState::SafeDeepCopyFrom(ExpectState *src)
{
    if (mutex == nullptr)
    {
        return AC_ERROR;
    }
    Mutex *tmp_mutex  = mutex;
    AcLock(*tmp_mutex);
    memcpy(this, src, sizeof(ExpectState));
    AcUnLock(*tmp_mutex);
    mutex = tmp_mutex;
    return AC_OK;
}

AC_RET ExpectState::SafeDeepCopyTo(ExpectState *des)
{
    if (mutex == nullptr)
    {
        return AC_ERROR;
    }
    Mutex *tmp_mutex  = mutex;
    AcLock(*tmp_mutex);
    memcpy(des, this, sizeof(ExpectState));
    AcUnLock(*tmp_mutex);
    mutex = tmp_mutex;
    return AC_OK;
}

AC_RET ActualState::SafeDeepCopyFrom(ActualState *src)
{
    if (mutex == nullptr)
    {
        return AC_ERROR;
    }
    Mutex *tmp_mutex  = mutex;
    AcLock(*tmp_mutex);
    memcpy(this, src, sizeof(ActualState));
    AcUnLock(*tmp_mutex);
    mutex = tmp_mutex;
    return AC_OK;
}

AC_RET ActualState::SafeDeepCopyTo(ActualState *des)
{
    if (mutex == nullptr)
    {
        return AC_ERROR;
    }
    Mutex *tmp_mutex  = mutex;
    AcLock(*tmp_mutex);
    memcpy(des, this, sizeof(ActualState));
    AcUnLock(*tmp_mutex);
    mutex = tmp_mutex;
    return AC_OK;
}
