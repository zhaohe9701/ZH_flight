/*
 * @Description: 
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2023-03-26 22:14:49
 * @LastEditors: zhaohe
 * @LastEditTime: 2023-03-26 22:27:52
 * @FilePath: \ZH_FLIGHT\Sys\OS\ac_semaphore.h
 * Copyright (C) 2023 zhaohe. All rights reserved.
 */
#ifndef __AC_SEMAPHORE_H__
#define __AC_SEMAPHORE_H__

#include "cmsis_os2.h"
#include "type.h"

class AcSemaphore
{
private:
    osSemaphoreId_t _handle = nullptr;
public:
    AcSemaphore();
    AC_RET Get();
    AC_RET Give();
};

#endif
